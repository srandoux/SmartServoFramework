/*!
 * This file is part of SmartServoFramework.
 * Copyright (c) 2014, INRIA, All rights reserved.
 *
 * SmartServoFramework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software. If not, see <http://www.gnu.org/licenses/lgpl-3.0.txt>.
 *
 * \file ex_controller.cpp
 * \date 17/03/2014
 * \author Emeric Grange <emeric.grange@inria.fr>
 */

/*
 * Simple test program: initialize the "Controller API" with serial port
 * auto-detection, register 4 servos, then let you control these 4 servos
 * with the keypad:
 *
 * - Use "kp-middle" (or 5) to set all the servo at their initial positions (in
 * the middle of their step range).
 * - Use "kp-left" (4) and "kp-right" (6) to move servo 1.
 * - Use "kp-up" (8) and "kp-down" (2) to move servo 2.
 * - Use "kp-up/left" (7) and "kp-down/right" (3) to move servo 3.
 * - Use "kp-up/right" (9) and "kp-down/left" (1) to move servo 4.
 * - Use any other key to exit the program.
 */

// Smart Servo Framework
#include "../src/DynamixelController.h"

// C++ standard libraries
#include <iostream>
#include <thread>

/* ************************************************************************** */

// ID of the servos you want to use in this test program:
#define ID_SERVO_1       1
#define ID_SERVO_2       2
#define ID_SERVO_3       3
#define ID_SERVO_4       4

/* ************************************************************************** */

int main(int argc, char *argv[])
{
    std::cout << std::endl << "======== Smart Servo Framework Tester ========" << std::endl;

    // Initialize a Dynamixel "controller" instance
    DynamixelController ctrl;

    // Initialize a serial link for the controller
    // You can specify the serial port path directly if you know it. Ex: "/dev/ttyUSB0" for a Linux system; "//./COM1" for a Windows system.
    // Note: serial port "auto-detection" will only work if a single serial port adapter is connected to your computer, or if the fisrt one detected is the one connected to your devices.
    std::string deviceName = "auto";
    if (ctrl.connect(deviceName, 1) == 0)
    {
        std::cerr << "> Failed to open a serial link for our ControllerAPI! Exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << std::endl << "======== Dynamixel Servos ========" << std::endl;

    // Instanciate AX-12 servos
    ServoAX s1(ID_SERVO_1, 12);
    ServoAX s2(ID_SERVO_2, 12);
    ServoAX s3(ID_SERVO_3, 12);
    ServoAX s4(ID_SERVO_4, 12);

    // Register them to the controller
    ctrl.registerServo(&s1);
    ctrl.registerServo(&s2);
    ctrl.registerServo(&s3);
    ctrl.registerServo(&s4);

    // Wait for the controller to register our servos
    ctrl.waitUntilReady();

    std::cout << std::endl << "======== MAIN LOOP ========" << std::endl;

    std::cout << std::endl << "> Use the keypad to move (press ESC or any other key to exit)" << std::endl;
    system("/bin/stty raw");

    bool running = true;
    while (running)
    {
        // Always get CLI cursor back to column 0
        std::cout << "\033[10D";

        // Move order?
        int key = getchar();
        switch (key)
        {
        case 52: // kp 4
            s1.moveGoalPosition(-32);
            break;
        case 54: // kp 6
            s1.moveGoalPosition(+32);
            break;

        case 50: // kp 2
            s2.moveGoalPosition(-32);
            break;
        case 56: // kp 8
            s2.moveGoalPosition(+32);
            break;

        case 51: // kp 3
            s3.moveGoalPosition(-32);
            break;
        case 55: // kp 7
            s3.moveGoalPosition(+32);
            break;

        case 49: // kp 1
            s4.moveGoalPosition(-32);
            break;
        case 57: // kp 9
            s4.moveGoalPosition(+32);
            break;

        case 53: // kp 5
            s1.setGoalPosition(512);
            s2.setGoalPosition(512);
            s3.setGoalPosition(512);
            s4.setGoalPosition(512);
            break;

        case 0x1b: // Escape
        default:
            system("/bin/stty cooked");
            std::cout << "\033[10D"; // Get CLI cursor back to column 0
            std::cout << "> EXIT (key was: " << key << ")" << std::endl;
            running = false;
            break;
        }
    }

    std::cout << std::endl << "======== EXITING ========" << std::endl;

    // Stop controller and close serial device(s)
    ctrl.disconnect();

    return EXIT_SUCCESS;
}

/* ************************************************************************** */
