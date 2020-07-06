/* 
 * File:   lidartest.cpp
 * Author: chris
 *
 * Created on 30-jun-2020, 12:25:59
 */

#include <stdlib.h>
#include <iostream>

#include "../Lidar/Lidar.h"

/*
 * Simple C++ Test Suite
 */

Lidar mylidar = new Lidar();

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% lidartest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (lidartest)" << std::endl;
    
    
    
    std::cout << "%TEST_FINISHED% time=0 test1 (lidartest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

