/**
 * File:   YdLidarX4.cpp
 * Author: Christian Martín
 */

#include <netinet/in.h>
#include <fcntl.h>

#include "YdLidarX4.h"

namespace YdLidarX4 {

    /**
     * Class destructor
     */
    YdLidarX4Controller::~YdLidarX4Controller() {
    }

    /**
     * YdLidarX3 class constructor:
     * - Crea objeto lidar con los parámetros por defecto
     * - Abre la conexion con el servidor
     */
    YdLidarX4Controller::YdLidarX4Controller() {
    }

    /**
     * 
     * @return 
     */
    bool YdLidarX4Controller::ConnectToServer() {
        // Create IPv4 stream socket
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Could not create socket");
            exit(1);
        }

        struct hostent* he = gethostbyname("127.0.0.1");
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr = *((struct in_addr*) he->h_addr);
        bzero(&(addr.sin_zero), 8);

        // Conectar a servidor
        std::cout << "Connecting to LIDAR server: " << addr.sin_addr.s_addr << std::endl;
        int sock_response = sock_response = connect(sockfd, (struct sockaddr*) &addr, sizeof (struct sockaddr));
        if (sock_response == -1) {
            perror("connect");
            exit(1);
        }
        /*while (sock_response == EAGAIN || sock_response == EWOULDBLOCK) {
            //perror("Could not connect to socket");
            //exit(1);
            sleep(100);
            sock_response = sock_response = connect(sockfd, (struct sockaddr*) &addr, sizeof (struct sockaddr));
        }*/
        std::cout << "Socket connection response: " << sock_response << std::endl;

        // Declarar fields del objeto
        is_connected = false;
        is_scanning = false;

        int* sample;
        sampledata = sample;

        deviceinfo.firmware_version = std::string();
        deviceinfo.hardware_version = std::string();
        deviceinfo.modelnumber = std::string();
        deviceinfo.serial_number = std::string();

        return true; // TODO: actually check if connection was successful
    }

    /**
     * Generar comando:
     * Dado un comando devuelve un string JSON bien formado
     * con el comando introducido.
     *
     * Params: Numero de comando, debe pertenecer al struct cmd
     */
    std::string YdLidarX4Controller::_create_command(int command) {
        const char* json = "{\"command\":\"0\"}";
        auto cmd_json = json::parse(json);
        cmd_json["command"] = command;
        std::string cmd_str = cmd_json.dump();
        return cmd_str;
    }

    /**
     * Enviar comando:
     * Dado un comando genera el string JSON correspondiente y lo envia al servidor
     * Params: Numero de comando, debe pertenecer al struct cmd
     */
    bool YdLidarX4Controller::_send_command(int command) {
        std::string command_str = _create_command(command);
        std::cout << "Sending command to YdLidar socket" << command_str << std::endl;
        int sock_response = 0;
        if (sock_response = send(sockfd, command_str.c_str(), command_str.length(), 0) == -1) {
            std::cout << "Error sending command " << command_str.c_str() << std::endl;
            std::cout << "Socket response: " << sock_response << std::endl;
            perror("send");
            return false;
        }
        std::cout << "Socket returned: " << sock_response << std::endl;
        return true;
    }

    /**
     * Recibir respuesta:
     * 1- Obtiene la respuesta del servidor
     * 2- Interpreta y almacena la información en los fields correspondientes
     */
    bool YdLidarX4Controller::_get_response() {

        // 1- Leer respuesta del servidor
        char* buffer = (char*) calloc(BUFF_SIZE, sizeof (char));
        if (read(sockfd, buffer, BUFF_SIZE) == 0) {
            perror("read");
            return false;
        }

        // 2- Parsear respuesta y almacenar
        auto d = json::parse(buffer); // TODO: rename variable 'd' for legibility
        free(buffer);
        if (d["response"].is_number() && d["response"] != LidarResponses.RES_OK) {
            return false;
        }
        if (!d["health"].is_null() && d["health"].is_number()) {
            health = d["health"];
        } //else std::cout << "No hay Health" << std::endl;

        int sampleindex = 0;

        if (!d["sampledata"].is_null() && d["sampledata"].is_array()) {
            // TODO: verificar la documentación de debajo...

            // Create an array and allocate memory to store 360 integer values
            int* newsample;
            newsample = (int*) malloc(360 * sizeof (int));

            // Overwrite sampledata with empty array
            sampledata = newsample;

            // Iterate over 'd' and copy onto 'sampledata'
            for (json::iterator it = d["sampledata"].begin(); it != d["sampledata"].end(); ++it) {
                sampledata[sampleindex] = *it;
                sampleindex++;
            }
        } //else std::cout << "No hay Sampledata" << std::endl;

        if (!d["deviceinfo"].is_null()) {
            deviceinfo.modelnumber = d["deviceinfo"]["model_number"];
            deviceinfo.firmware_version = d["deviceinfo"]["firmware_version"];
            deviceinfo.hardware_version = d["deviceinfo"]["hardware_version"];
            deviceinfo.serial_number = d["deviceinfo"]["serial_number"];
        } //else std::cout << "No hay Deviceinfo" << std::endl;

        return true;
    }

    /**
     * Conectar al Lidar:
     * - Envia un comando de conexión y obtiene la respuesta
     */
    bool YdLidarX4Controller::Connect() {
        // TODO: de dónde sale '_send_command' ? >> Especificar librería
        std::cout << "YdLidarX4Controller::Connect" << std::endl;
        if (!_send_command(LidarCommands.CMD_CONNECT)) {
            return false;
        }
        if (!_get_response()) {
            return false;
        }
        is_connected = true;
        return true;
    }

    /**
     * Iniciar escaneo:
     * Envía comando para iniciar escaneo y obtiene la respuesta
     */
    bool YdLidarX4Controller::StartScanning() {
        if (is_connected) {
            if (!_send_command(LidarCommands.CMD_START_SCANNING)) {
                return false;
            }
            if (!_get_response()) {
                return false;
            }
            is_scanning = true;
            return true;
        } else
            return false;
    }

    /**
     * Obtener informacion del dispositivo:
     * Envia comando para obtener informacion del dispositivo
     *
     * Returns: un apuntador al struct dev
     * TODO: de que tipo es el struc dev? Donde está definido dev?
     */
    DeviceInformation* YdLidarX4Controller::GetDeviceInfo() {
        if (is_connected) {
            if (!_send_command(LidarCommands.CMD_GET_DEVICE_INFO)) {
                return 0;
            }
            if (!_get_response()) {
                return 0;
            }
            //std::cout << "\tModel Number: " << deviceinfo.modelnumber << std::endl;
            //std::cout << "\tFirmware Version: " << deviceinfo.firmware_version << std::endl;
            //std::cout << "\tHardware Version: " << deviceinfo.hardware_version << std::endl;
            //std::cout << "\tSerial Number: " << deviceinfo.serial_number << std::endl;
            return &deviceinfo;
        } else {
            return 0;
        }
    }

    /**
     * Desconectar:
     * Envia un comando para desconectar y finalizar la conexion
     */
    bool YdLidarX4Controller::Disconnect() {
        if (is_connected) {
            if (!_send_command(LidarCommands.CMD_DISCONNECT)) {
                return false;
            }
            if (!_get_response()) {
                return false;
            }
            is_connected = false;
            is_scanning = false;
            close(sockfd);
            return true;
        } else {
            return false;
        }
    }

    /**
     * Reiniciar:
     * Reinicia el dispositivo pero no reinicia la conexion
     */
    bool YdLidarX4Controller::Reset() {
        if (!_send_command(LidarCommands.CMD_RESET)) {
            return false;
        }
        if (!_get_response()) {
            return false;
        }
        is_connected = true;
        is_scanning = false;
        return true;
    }

    /**
     * Obtener estado:
     * Obtiene y devuelve el estado actual del dispositivo
     */
    int YdLidarX4Controller::GetHealthStatus() {
        if (is_connected) {
            if (!_send_command(LidarCommands.CMD_GET_HEALTH_STATUS)) {
                return 0;
            }
            if (!_get_response()) {
                return 0;
            }
            return health;
        } else
            return 0;
    }

    /**
     * Obtener muestra:
     * Obtiene una muestra de datos y devuelve un pointer a un vector de muestras
     * 
     * TODO CRITICAL: when accessing the returning array, DO check that the returned int* is !=0
     *                Otherwise you'll get a SIGSEGV (segmentation fault) error!!!
     */
    int* YdLidarX4Controller::GetSampleData() {
        if (is_connected && is_scanning) {
            if (!_send_command(LidarCommands.CMD_GET_SAMPLE_DATA)) {
                return 0;
            }
            if (!_get_response()) {
                return 0;
            }
            return sampledata;
        } else
            return 0;
    }

    /**
     * Detener escaneo:
     * Envia comando para detener el escaneo y obtiene la respuesta
     */
    bool YdLidarX4Controller::StopScanning() {
        if (is_connected) {
            if (!_send_command(LidarCommands.CMD_STOP_SCANNING)) {
                return false;
            }
            if (!_get_response()) {
                return false;
            }
            is_scanning = false;
            return true;
        } else
            return false;
    }

} // namespace YdLidarX4