/**
 * File:   YdLidarX4.h
 * Author: chris
 * 
 * This class is a direct re-implementation in C++ of PyLidar3's YdLidarX4 class.
 *   From: https://github.com/lakshmanmallidi/PyLidar3
 *         PyLidar3 is a Python package to get data from Lidar devices from 
 *         various manufacturers.
 *
 * Created on 10 de enero de 2020, 12:33
 */

#ifndef YDLIDARX4_H
#define YDLIDARX4_H

#include <iostream>
#include <string>
#include <cmath>
#include <cstdarg>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>

#include "../nlohmann/json.hpp"
using json = nlohmann::json;

//using namespace std; // Try to avoid "using namespace ..." directives. REASON: https://stackoverflow.com/questions/1452721/why-is-using-namespace-std-considered-bad-practice

//using std::string;

namespace YdLidarX4 {

#define BUFF_SIZE 2048 // TODO: Para que es esta variable?
#define PORT 25138 // TODO: Para que es esta variable?

    /** Lista de comandos posibles para enviar */
    constexpr struct LidarCommands {
        int CMD_CONNECT = 1000;
        int CMD_START_SCANNING = 2000;
        int CMD_GET_DEVICE_INFO = 3000;
        int CMD_DISCONNECT = 4000;
        int CMD_RESET = 5000;
        int CMD_GET_HEALTH_STATUS = 6000;
        int CMD_GET_SAMPLE_DATA = 7000;
        int CMD_STOP_SCANNING = 8000;
    } LidarCommands;

    /** Lista de respuestas Opt: Se pueden añadir mas respuestas dependiendo del tipo de error*/
    constexpr struct LidarResponses {
        int RES_OK = 1001;
        int RES_FAIL = 1002;
        int HEALTH_OK = 2001;
        int HEALTH_BAD = 2002;
    } LidarResponses;

    /** TODO: Documentar*/
    struct DeviceInformation {
        std::string modelnumber;
        std::string firmware_version;
        std::string hardware_version;
        std::string serial_number;
    };

    /**
     * TODO: Documentar clase
     * 
     * This class is a direct re-implementation in C++ of PyLidar3's YdLidarX4 class.
     *   From: https://github.com/lakshmanmallidi/PyLidar3
     *         PyLidar3 is a Python package to get data from Lidar devices from 
     *         various manufacturers.
     */
    class YdLidarX4Controller {
    public:

        /**
         * Default Class constructor
         *
         * TODO: ¿se puede borrar este constructor?
         */
        YdLidarX4Controller();

        /**
         * Class destructor
         */
        virtual ~YdLidarX4Controller();

        /**
         * Connect to the LIDAR server
         * @return 
         */
        bool ConnectToServer();

        /**
         * TODO: Documentar
         * TODO: qué conexión se hace en este metodo?
         */
        bool Connect();

        /**
         * TODO: Documentar
         */
        bool Disconnect();

        /**
         * TODO: Documentar
         */
        bool Reset();

        /**
         * TODO: Documentar
         */
        bool StartScanning();

        /**
         * TODO: Documentar
         */
        bool StopScanning();

        /**
         * TODO: Documentar
         */
        int* GetSampleData();

        /**
         * TODO: Documentar
         */
        DeviceInformation * GetDeviceInfo();

        /**
         * TODO: Documentar
         */
        int GetHealthStatus();

    private:
        bool is_connected; // TODO: documentar
        bool is_scanning; // TODO: Documentar

        int health; // TODO: Documentar
        int* sampledata; // TODO: Documentar

        DeviceInformation deviceinfo; // TODO: Documentar

        int sockfd; // TODO: Documentar

        /**
         * TODO: Documentar
         * 
         * TODO: ¿por qué tienen estos metodos "_" (underscore) al principio?
         */
        bool _send_command(int command);

        /**
         * TODO: Documentar
         */
        bool _get_response();

        /**
         * TODO: Documentar
         */
        std::string _create_command(int command);
    };

} // namespace YdLidarX4

#endif /* YDLIDARX4H */
