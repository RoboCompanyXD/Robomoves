/**
 * File:   YdLidarX4.h
 * Author: chris
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

using namespace std; // https://stackoverflow.com/questions/1452721/why-is-using-namespace-std-considered-bad-practice

//using std::string;

#define BUFF_SIZE 2048 // TODO: Para que es esta variable?
#define PORT 25138 // TODO: Para que es esta variable?

/** Lista de comandos posibles para enviar */
constexpr struct cmd {
    int CMD_CONNECT = 1000;
    int CMD_START_SCANNING = 2000;
    int CMD_GET_DEVICE_INFO = 3000;
    int CMD_DISCONNECT = 4000;
    int CMD_RESET = 5000;
    int CMD_GET_HEALTH_STATUS = 6000;
    int CMD_GET_SAMPLE_DATA = 7000;
    int CMD_STOP_SCANNING = 8000;
} cmd;

/** Lista de respuestas Opt: Se pueden añadir mas respuestas dependiendo del tipo de error*/
constexpr struct res {
    int RES_OK = 1001;
    int RES_FAIL = 1002;
    int HEALTH_OK = 2001;
    int HEALTH_BAD = 2002;
} res;

/** TODO: Documentar*/
struct dev {
    string modelnumber;
    string firmware_version;
    string hardware_version;
    string serial_number;
};

/**
 * TODO: Documentar clase
 *
 */
class YdLidarX4 {

public:

    /**
     * Default Class constructor
     *
     * TODO: ¿se puede borrar este constructor?
     */
    YdLidarX4();

    /**
     * Class constructor
     *
     * TODO: ¿en que se diferencia del otro constructor?
     * TODO: Documentar constructor
     */
    YdLidarX4(int chunk_size);

    /**
     * Class destructor
     */
    virtual ~YdLidarX4();


    /**
     * TODO: Documentar
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
    struct dev * GetDeviceInfo();

    /**
     * TODO: Documentar
     */
    int GetHealthStatus();

private:
    bool is_connected; // TODO: documentar
    bool is_scanning; // TODO: Documentar

    int health; // TODO: Documentar
    int* sampledata; // TODO: Documentar

    struct dev deviceinfo; // TODO: Documentar

    int sock; // TODO: Documentar

    /**
     * TODO: Documentar
     */
    bool _send_command(int command);

    /**
     * TODO: Documentar
     */
    bool _get_response();

    /**
     * TODO: Documentar
     */
    string _create_command(int command);
};

#endif /* YDLIDARX4H */
