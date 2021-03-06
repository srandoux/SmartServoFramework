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
 * \file mainwindow.h
 * \date 27/03/2014
 * \author Emeric Grange <emeric.grange@gmail.com>
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "advancescanner.h"
#include "settings.h"

#include "../../src/ControllerAPI.h"

#include <QMainWindow>

class QCheckBox;
class QComboBox;
class QPushButton;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    QAction *refreshAction;
    QAction *rebootAction;
    QAction *resetAction;
    QTimer *selfRefreshTimer;
    QWidget *loadingTabWidget;

    //! "Advance Scanner" window
    AdvanceScanner *asw;

    //! "Settings" window
    Settings *stw;

    struct SerialPortHelper
    {
        // GUI elements
        QCheckBox *deviceName;
        QComboBox *deviceProtocol;
        QPushButton *deviceScan;
        // Controller
        int deviceControllerProtocol;
        ControllerAPI *deviceController;
    };

    bool scan_running = false;

    //! List of all serial ports (gui elements, controller) handled by the application
    std::vector <SerialPortHelper *> serialPorts;

    int tableServoSerie;
    int tableServoModel;
    bool tableAutoSelection;

    void loading(bool enabled);

    int getCurrentController(ControllerAPI *&ctrl);
    int getCurrentServo(ControllerAPI *&ctrl, int &id);
    int getCurrentServo(Servo *&servo);

    int getRegisterNameFromTableIndex(const int servo_serie, const int servo_model, int table_index);
    int getTableIndex(const int servo_serie, const int servo_model, const int reg_name);

    void generateRegisterTable(const int servo_serie, const int servo_model);
    void generateRegisterTableDynamixel(const int servo_serie, const int servo_model);
    void generateRegisterTableHerkuleX(const int servo_serie, const int servo_model);

    void cleanRegisterTable();
    void toggleServoPanel(bool status);

    void resizeEvent(QResizeEvent *event);
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void about();
    void aboutQt();

    void servoSelection();

    void servoUpdate();
    void updateRegisterTableDynamixel(Servo *servo_dxl, const int servoSerie, const int servoModel);
    void updateRegisterTableHerkuleX(Servo *servo_hkx, const int servoSerie, const int servoModel);

    void errorHandling(Servo *servo, const int servoSerie, const int servoModel);
    void clearErrors();

    void resetServo();
    void rebootServo();
    void refreshServo();

    void toggleRunningMode();
    void toggleTorque(bool torque);
    void toggleLED(bool led);
    void moveMaxTorque(int torque);
    void moveTorqueLimit(int limit);
    void moveMovingSpeed(int speed);
    void moveCWLimit(int limit);
    void moveCCWLimit(int limit);
    void moveServo(int goal);

    void modifier(int row, int column);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void resizeTabWidgetContent();

    void advanceScannerStart();
    void advanceScannerStop();

    void settingsStart();
    void settingsStop();

    void scanSerialPorts();
    void scanServos();

    /*!
     * For each serial port, try to start a controller and scan for servos.
     * If scan set to "auto", we try DynamixelController first (protocol v1, then v2)
     * at available default speeds (1Mb/s and 57.6kb/s).
     * If no success with Dynamixel, try with HerkuleXController at 115.2kb/s.
     * Note: Controllers are using 10Hz default refresh rates only, to avoid
     * wasting CPU resources.
     *
     * If successfull, add the newly created controller to the corresponding entry
     * into the SerialPortHelper vector.
     */
    void scanServos(QString port_qstring);
};

#endif // MAINWINDOW_H
