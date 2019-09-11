/* 
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

using namespace std;

// Lista de comandos posibles para enviar

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

// Lista de respuestas Opt: Se pueden añadir mas respuestas dependiendo del tipo
// de error

constexpr struct res {
	int RES_OK = 1001;
	int RES_FAIL = 1002;
	int HEALTH_OK = 2001;
	int HEALTH_BAD = 2002;

} res;

struct dev {
	string modelnumber;
	string firmware_version;
	string hardware_version;
	string serial_number;
};

#define BUFF_SIZE 2048

#define PORT 25138

class YdLidarX4 {
public:
	YdLidarX4();
	YdLidarX4(int chunk_size);
	virtual ~YdLidarX4();

	bool Connect();
	bool Disconnect();
	bool Reset();
		
	bool StartScanning();
	bool StopScanning();
	int* GetSampleData();
		
	struct dev * GetDeviceInfo();
	int GetHealthStatus();
		
private:
	
	bool is_connected;
	bool is_scanning;

	int health;
	int* sampledata;

	struct dev deviceinfo;

	int sock;

	bool _send_command(int command);
	bool _get_response();
	string _create_command(int command);
};

#endif /* YDLIDARX4H */
