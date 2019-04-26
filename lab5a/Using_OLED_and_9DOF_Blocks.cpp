/******************************************************************************
SparkFun_9DOF_Edison_Block_Example.cpp
Example code for the 9DOF Edison Block
14 Jul 2015 by Mike Hord
https://github.com/sparkfun/SparkFun_9DOF_Block_for_Edison_CPP_Library

Demonstrates the major functionality of the SparkFun 9DOF block for Edison.

** Supports only I2C connection! **

Development environment specifics:
  Code developed in Intel's Eclipse IOT-DK
  This code requires the Intel mraa library to function; for more
  information see https://github.com/intel-iot-devkit/mraa

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include "mraa.hpp"

#include <iostream>
#include <unistd.h>
#include "SFE_LSM9DS0.h"
#include "oled/Edison_OLED.h"
using namespace std;

edOLED oled;

void setupOLED();
void cleanUp();
void displayValues(LSM9DS0 *imu);
void printLines(const char *string, float &val, const char *unit);
int main()
{
  setupOLED();
  LSM9DS0 *imu;
  imu = new LSM9DS0(0x6B, 0x1D);

  uint16_t imuResult = imu->begin();
  cout << hex << "Chip ID: 0x" << imuResult << dec << " (should be 0x49d4)" << endl;

  bool newAccelData = false;
  bool newMagData = false;
  bool newGyroData = false;
  bool overflow = false;

  // Loop and report data
  while (1)
  {
    while ((newGyroData & newAccelData & newMagData) != true)
    {
      if (newAccelData != true)
      {
        newAccelData = imu->newXData();
      }
      if (newGyroData != true)
      {
        newGyroData = imu->newGData();
      }
      if (newMagData != true)
      {
        newMagData = imu->newMData(); // Temp data is collected at the same
                                      //  rate as magnetometer data.
      }
    }

    newAccelData = false;
    newMagData = false;
    newGyroData = false;

    // Of course, we may care if an overflow occurred; we can check that
    //  easily enough from an internal register on the part. There are functions
    //  to check for overflow per device.
    overflow = imu->xDataOverflow() |
               imu->gDataOverflow() |
               imu->mDataOverflow();

    if (overflow)
    {
      cout << "WARNING: DATA OVERFLOW!!!" << endl;
    }

    imu->readAccel();
    imu->readMag();
    imu->readGyro();
    imu->readTemp();

    // Print the "real" values in more human comprehensible units.
    cout << "-------------------------------------" << endl;
    cout << "Gyro x: " << imu->calcGyro(imu->gx) << " deg/s" << endl;
    cout << "Gyro y: " << imu->calcGyro(imu->gy) << " deg/s" << endl;
    cout << "Gyro z: " << imu->calcGyro(imu->gz) << " deg/s" << endl;
    cout << "Accel x: " << imu->calcAccel(imu->ax) << " g" << endl;
    cout << "Accel y: " << imu->calcAccel(imu->ay) << " g" << endl;
    cout << "Accel z: " << imu->calcAccel(imu->az) << " g" << endl;
    cout << "Mag x: " << imu->calcMag(imu->mx) << " Gauss" << endl;
    cout << "Mag y: " << imu->calcMag(imu->my) << " Gauss" << endl;
    cout << "Mag z: " << imu->calcMag(imu->mz) << " Gauss" << endl;
    cout << "-------------------------------------" << endl;
    cleanUp();
    displayValues(imu);
    sleep(1);
  }

  return MRAA_SUCCESS;
}

void setupOLED()
{
  oled.begin();
  oled.clear(PAGE);
  oled.display();
  oled.setFontType(0);
}

void startDisplay(LSM9DS0 *imu)
{
}
void printLines(const char *string, float val, const char *unit)
{
  oled.print(string);
  oled.print(val);
  oled.print(unit);
}
void displayValues(LSM9DS0 *imu)
{
  oled.clear(PAGE);

  oled.setCursor(0, 0);
  printLines("Gyro x: ", imu->calcGyro(imu->gx), " deg/s");
  oled.setCursor(0, 16);
  printLines("Gyro y: ", imu->calcGyro(imu->gy), " deg/s");
  oled.setCursor(0, 32);
  printLines("Gyro z: ", imu->calcGyro(imu->gz), " deg/s");
  
  oled.display();
}
void cleanUp()
{
  oled.clear(PAGE);
  oled.display();
}