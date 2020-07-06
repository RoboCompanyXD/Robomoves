/*
 * File:   IRobotInstructionSet.h
 * Author: Gorka Montero
 *
 * Created on 2012
 *
 * Modified by Borja Gómez
 */

#ifndef IROBOTINSTRUCTIONSET_H
#define IROBOTINSTRUCTIONSET_H

#pragma once
#include "Instruction.h"
#include <malloc.h>

//Contains the sensor opcodes that we need to use if we want to query the sensor values
namespace iRobotSensors {
    static const char BUMPERS_AND_WHEELDROPS = (char) 7;
    static const char WALL = (char) 8;
    static const char CLIFFLEFT = (char) 9;
    static const char CLIFFFRONTLEFT = (char) 10;
    static const char CLIFFFRONTRIGHT = (char) 11;
    static const char CLIFFRIGHT = (char) 12;
    static const char VIRTUALWALL = (char) 13;
    static const char MOTOR_OVERCURRENTS = (char) 14;
    static const char DIRTDETECT = (char) 15;
    //static const   char UNUSED1 = (  char) 16;
    static const char IRBYTE = (char) 17;
    static const char BUTTONS = (char) 18;
    static const char DISTANCE = (char) 19;
    static const char ANGLE = (char) 20;
    static const char CHARGINGSTATE = (char) 21;
    static const char VOLTAGE = (char) 22;
    static const char CURRENT = (char) 23;
    static const char TEMPERATURE = (char) 24;
    static const char CHARGE = (char) 25;
    static const char CAPACITY = (char) 26;
    static const char WALLSIGNAL = (char) 27;
    static const char CLIFFLEFTSIGNAL = (char) 28;
    static const char CLIFFFRONTLEFTSIGNAL = (char) 29;
    static const char CLIFFFRONTRIGHTSIGNAL = (char) 30;
    static const char CLIFFRIGHTSIGNAL = (char) 31;
    //static const   char UNUSED2 = (  char) 32;
    //static const   char UNUSED3 = (  char) 33;
    static const char CHARGINGSOURCES = (char) 34;
    static const char OIMODE = (char) 35;
    static const char SONGNUMBER = (char) 36;
    static const char SONGPLAYING = (char) 37;
    static const char STREAMPACKETS = (char) 38;
    static const char VELOCITY = (char) 39;
    static const char RADIUS = (char) 40;
    static const char RIGHTVELOCITY = (char) 41;
    static const char LEFTVELOCITY = (char) 42;
    static const char ENCODERLEFT = (char) 43;
    static const char ENCODERRIGHT = (char) 44;
    static const char LIGHTBUMPER = (char) 45;
    static const char LIGHTBUMPLEFT = (char) 46;
    static const char LIGHTBUMPFRONTLEFT = (char) 47;
    static const char LIGHTBUMPCENTERLEFT = (char) 48;
    static const char LIGHTBUMPCENTERRIGHT = (char) 49;
    static const char LIGHTBUMPFRONTRIGHT = (char) 50;
    static const char LIGHTBUMPRIGHT = (char) 51;
    static const char IROPCODELEFT = (char) 52;
    static const char IROPCODERIGHT = (char) 53;
    static const char LEFTMOTORCURRENT = (char) 54;
    static const char RIGHTMOTORCURRENT = (char) 55;
    static const char MAINBRUSHCURRENT = (char) 56;
    static const char SIDEBRUSHCURRENT = (char) 57;
    static const char STASIS = (char) 58;
}
//Contains the iRobot Create instruction opcodes
namespace iRobotInstructions {
    static const char START = (char) 128;
    static const char BAUD = (char) 129;
    static const char CONTROL = (char) 130; //@deprecated
    static const char SAFE = (char) 131;
    static const char FULL = (char) 132;
    static const char SPOT = (char) 134;
    static const char COVER = (char) 135;
    //static const   char DEMO = (  char) 136;
    static const char DRIVE = (char) 137;
    //static const   char LOW_SIDE_DRIVERS = (  char) 138;
    static const char LEDS = (char) 139;
    static const char SONG = (char) 140;
    static const char PLAY = (char) 141;
    static const char SENSORS = (char) 142;
    static const char COVER_AND_DOCK = (char) 143;
    //static const   char PWM_LOW_SIDE_DRIVERS = (  char) 144;
    static const char DRIVE_DIRECT = (char) 145;
    //static const   char DIGITAL_OUTPUTS = (  char) 147;
    static const char STREAM = (char) 148;
    static const char QUERY_LIST = (char) 149;
    static const char PAUSE_RESUME_STREAM = (char) 150;
    //static const   char SEND_IR = (  char) 151;
    //static const   char SCRIPT = (  char) 152;
    //static const   char PLAY_SCRIPT = (  char) 153;
    //static const   char SHOW_SCRIPT = (  char) 154;
    //static const   char WAIT_TIME = (  char) 155;
    //static const   char WAIT_DISTANCE = (  char) 156;
    //static const   char WAIT_ANGLE = (  char) 157;
    //static const   char WAIT_EVENT = (  char) 158;

    //Actualizacion:
    static const char POWER = (char) 133;
    static const char MAX_CLEAN = (char) 136;
    static const char MOTORS = (char) 138;
    static const char PWM_MOTORS = (char) 144;
    static const char DRIVE_PWM = (char) 146;
    static const char SCHEDULING_LEDS = (char) 162;
    static const char DIGIT_LEDS_RAW = (char) 163;
    static const char DIGIT_LEDS_ASCII = (char) 164;
    static const char BUTTONS = (char) 165;
    static const char SCHEDULE = (char) 167;
    static const char SET_DAY_TIME = (char) 168;
    static const char STOP = (char) 173;
}
//Contains the event opcodes that we need to use if we want the iRobot to wait one of them
namespace iRobotEvents {
    static const int WHEEL_DROP = 1;
    static const int FRONT_WHEEL_DROP = 2;
    static const int LEFT_WHEEL_DROP = 3;
    static const int RIGHT_WHEEL_DROP = 4;
    static const int BUMP = 5;
    static const int LEFT_BUMP = 6;
    static const int RIGHT_BUMP = 7;
    static const int VIRTUAL_WALL = 8;
    static const int WALL = 9;
    static const int CLIFF = 10;
    static const int LEFT_CLIFF = 11;
    static const int FRONT_LEFT_CLIFF = 12;
    static const int FRONT_RIGHT_CLIFF = 13;
    static const int RIGHT_CLIFF = 14;
    static const int HOME_BASE = 15;
    static const int ADVANCE_BUTTON = 16;
    static const int PLAY_BUTTON = 17;
    static const int DIGITAL_INPUT_0 = 18;
    static const int DIGITAL_INPUT_1 = 19;
    static const int DIGITAL_INPUT_2 = 20;
    static const int DIGITAL_INPUT_3 = 21;
    static const int OIMODE_PASSIVE = 22;
}
//Contains the sensor package opcodes that we need to use if we want to query some sensor values at the same time
namespace iRobotSensorPackage {
    static const char SENSORS_7_26 = (char) 0; //rango 0-25  ->Actualizar 26   chars Cliff, bumper, weels, virtual wall
    static const char SENSORS_DIGITAL_BUMPER_CLIFF_WALL_VIRTUALWALL = (char) 1; //rango 0-7   ->Actualizar 10   chars (  chars 8,9 sin usar) IR,Buttons,Distance,Angle
    static const char SENSORS_IR_BUTTONS_DISTANCE_ANGLE = (char) 2; //rango 10-15 ->Actualizar 6    chars
    static const char SENSORS_BATERY = (char) 3; //rango 16-25 ->Actualizar 10   chars
    static const char SENSORS_ANALOG_CLIFF_WALL_VIRTUALWALL = (char) 4; //rango 27-39 ->Actualizar 14   chars
    static const char SENSORS_35_42 = (char) 5; //rango 40-51 ->Actualizar 12   chars
    static const char SENSORS_ALL = (char) 6; //Rango 0-51  ->Actualizar 52 Bytes
    static const char C2_SENSORS_ALL = (char) 100;
    static const char C2_NEW = (char) 101;
    static const char C2_LIGHTBUMP_ANALOG = (char) 106;
    static const char C2_MOTORS = (char) 107;
}

class iRobotInstructionSet {
private:
    //************************************
    // Method:    intHightByte
    // Returns:   char						[out]	Hight byte of the number
    // Parameter: short num					[in]	The number
    //************************************
    char intHightByte(short num);

    //************************************
    // Method:    intLowByte
    // Returns:   char						[out]	Low byte of the number
    // Parameter: short num					[in]	The number
    //************************************
    char intLowByte(short num);
public:

    //************************************
    // Method:    sensorReturn
    // Returns:   int						[out]	How many bytes generates the sensor as response
    // Parameter: char sensor				[in]	The sensor id
    //************************************
    int sensorReturn(char sensor);

    iRobotInstructionSet(void);
    ~iRobotInstructionSet(void);
    Instruction start();
    Instruction baud(char code);
    Instruction control();
    Instruction safe();
    Instruction full();
    Instruction spot();
    Instruction cover();
    //Instruction demo(  char code);
    Instruction drive(int speed, int radius);
    //Instruction lowSideDrivers(  char outputBit);
    Instruction leds(int ledBit, int ledColor, int ledIntensity);
    Instruction song(int songNumber, int songSize, char *song);
    Instruction playSong(int songNumber);
    Instruction updateSensor(char sensorId);
    Instruction coverAndDock();
    //Instruction pwmLowSideDrivers(int driver2,int driver1, int driver0);
    Instruction driveDirect(int rightVelocity, int leftVelocity);
    //Instruction digitalOutputs(int outputBits);
    Instruction stream(char* sensorIdList, int size);
    Instruction queryList(char * sensorIdList, int size);
    Instruction PauseResumeStream(bool bol);
    //Instruction sendIr(int data);
    //Instruction script(int *commandList, int size);
    //Instruction playScript();
    //Instruction showScript();
    //Instruction waitTime(int seconds);
    //Instruction waitDistance(int mm);
    //Instruction waitAngle(int degrees);
    //Instruction waitEvent(int eventId);

    //Actualizacion:
    Instruction stop();
    Instruction power();
    Instruction max_clean();
    Instruction motors(char code);
    Instruction pwm_motors(int main, int side, int vacuum);
    Instruction drive_pwm(int rightPwm, int leftPwm);
    Instruction scheduling_leds(char weekday, char scheduling);
    Instruction digit_leds_raw(char digit3, char digit2, char digit1, char digit0); // not availabe in current firmware
    Instruction digit_leds_ascii(char digit3, char digit2, char digit1, char digit0); // not availabe in current firmware
    Instruction buttons(char code);
    Instruction schedule(int days, int sunHour, int sunMin, int monHour, int monMin, int tueHour, int tueMin, int wedHour, int wedMin, int thuHour, int thuMin, int friHour, int friMin, int satHour, int satMin);
    Instruction set_day_time(int day, int hour, int min);
};

#endif // IROBOTINSTRUCTIONSET_H