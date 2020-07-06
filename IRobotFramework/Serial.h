/*
 * File:   Serial.h
 * Author: @bgamecho
 *
 * Created on October 13, 2015, 6:44 PM
 *
 * Modified by Borja Gómez
 */

#ifndef SERIAL_H
#define	SERIAL_H

//#pragma once
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <wiringPi.h>
#include <wiringSerial.h>

/*
 * TODO: Documentar clase
 * - TODO: Para qué sirve
 * - TODO: Documentar TODAS las variables
 * - TODO: Documentar TODAS las funciones y procedimientos
 */
class Serial {
private:
    const char * portName;
    int fd_serial;
    int flag_debug;
    int time;

public:
    Serial(void);
    Serial(const char * type);
    ~Serial(void);
    int connect();
    int connect(int baudrate);
    int send(char *data, int size);
    int receive(char *buffer, size_t size);
    void disconnect();
    void setVerboseMode(int val);
    int clean();

    //PRUEBA
    int receive_all(char *buffer);
};

#endif	/* SERIAL_H */
