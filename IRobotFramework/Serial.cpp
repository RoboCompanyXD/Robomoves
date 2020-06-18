/*
 * File:   Serial.cpp
 * Author: @bgamecho
 *
 * Created on October 13, 2015, 6:44 PM
 *
 * Modified by Borja Gómez
 */

#include "Serial.h"

using namespace std;

/*
 * Serial Class Constructor
 * TODO: documentar
 * - Para qué sirve esta clase?
 */
Serial::Serial(void) {
    portName = "/dev/ttyUSB0";
    flag_debug = 0;
}

/*
 * TODO: docum
 */
Serial::Serial(const char * type) {
    portName = type;
    flag_debug = 0;
    time = 0;
}

/*
 * Serial Class Destructor
 */
Serial::~Serial(void) {
}

/*
 * TODO: docum
 */
int Serial::connect() {
    fd_serial = serialOpen(portName, 57600);
    if (fd_serial < 0) {
        if (flag_debug) {
            std::cout << "[V]\tError opening port: " << portName << std::endl;
        }
        return false;
    }
    return true;
}

/*
 * TODO: docum
 */
int Serial::connect(int baudrate) {
    fd_serial = serialOpen(portName, baudrate);
    if (fd_serial < 0) {
        if (flag_debug) {
            std::cout << "[V]\tError opening port: " << portName << std::endl;
        }
        return false;
    }
    return true;
}

/*
 * TODO: docum
 */
int Serial::send(char *data, int size) {
    clean();
    int index;
    if (flag_debug) {
        std::cout << "[V]\tSend command: " << std::endl;
        std::cout << "[V]\t\tCommand size: " << size << std::endl;
    }
    for (index = 0; index < size; index++) {
        serialPutchar(fd_serial, data[index]);
        if (flag_debug) {
            std::cout << "[V]\t\t" << index << " - " << +data[index] << std::endl;
        }
        //delay(10); // wait 10 ms to send the command
        delay(0);
    }
    return 0;
}

/*
 * TODO: docum
 */
int Serial::receive(char *buffer, size_t size) {
    int index;
    if (flag_debug) {
        std::cout << "[V]\tReceive command: " << std::endl;
    }
    //delay(10); // wait some time before receiving data
    int val = serialDataAvail(fd_serial);
    while (val < (int) size) {
        val = serialDataAvail(fd_serial);
    }
    if (val != (int) size) {
        return -1;
    }
    for (index = 0; index < (int) size; index++) {
        if (flag_debug) {
            val = serialDataAvail(fd_serial);
            std::cout << "[V]\t\tBytes available for reading: " << val << std::endl;
        }
        int aux = serialGetchar(fd_serial);
        buffer[index] = (char) aux;
        if (flag_debug) {
            std::cout << "[V]\t\t" << index << " Read:  " << +aux << std::endl;
        }
    }
    val = serialDataAvail(fd_serial);
    if (val > 0) {
        return -1;
    }
    return 0;
}

/*
 * TODO: docum
 */
void Serial::disconnect() {
    serialClose(fd_serial);
}

/*
 * TODO: docum
 */
void Serial::setVerboseMode(int val) {
    if (val == 0) {
        flag_debug = 0;
    } else {
        flag_debug = 1;
    }
}

/*
 * TODO: docum
 */
int Serial::clean() {

    serialFlush(fd_serial);
    return 0;
}

/*
 * TODO: docum
 */
int Serial::receive_all(char *buffer) {
    int val = serialDataAvail(fd_serial);
    int index;
    for (index = 0; index < val; index++) {
        if (flag_debug) {
            std::cout << "[V]\t\tBytes available for reading: " << val << std::endl;
        }
        int aux = serialGetchar(fd_serial);
        buffer[index] = (char) aux;
        if (flag_debug) {
            std::cout << "[V]\t\t" << index << " Read:  " << +aux << std::endl;
        }
    }
    return val;
}
