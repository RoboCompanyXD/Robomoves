/* 
 * File:   newsimpletest.cpp
 * Author: ehu
 *
 * Created on 30-jun-2020, 12:28:16
 */


#include <cstdlib>
#include <stdlib.h>
#include <poll.h>
#include <termios.h>
#include <unistd.h>
#include <wiringPi.h>
#include "../OCVCam/OCVCam.h"

/*
 * Simple C++ Test Suite
 */


//void test1() {
//    std::cout << "newsimpletest test 1" << std::endl;
//}
//
//void test2() {
//    std::cout << "newsimpletest test 2" << std::endl;
//    std::cout << "%TEST_FAILED% time=0 testname=test2 (newsimpletest) message=error message sample" << std::endl;
//}
//
//int main(int argc, char** argv) {
//    std::cout << "%SUITE_STARTING% newsimpletest" << std::endl;
//    std::cout << "%SUITE_STARTED%" << std::endl;
//
//    std::cout << "%TEST_STARTED% test1 (newsimpletest)" << std::endl;
//    test1();
//    std::cout << "%TEST_FINISHED% time=0 test1 (newsimpletest)" << std::endl;
//
//    std::cout << "%TEST_STARTED% test2 (newsimpletest)\n" << std::endl;
//    test2();
//    std::cout << "%TEST_FINISHED% time=0 test2 (newsimpletest)" << std::endl;
//
//    std::cout << "%SUITE_FINISHED% time=0" << std::endl;
//
//    return (EXIT_SUCCESS);
//}


using namespace std;
using namespace cv;


OCVCam *micam; 


PI_THREAD(thread1) {
    while (1) micam->AnalyzeCam();   
};

int main(int argc, char** argv) {
    micam = new OCVCam();
    while (1){
        cout << "TEST X:" << micam->x << "   Y:" << micam->y << "   area:" << micam->area << endl;
    }
}
