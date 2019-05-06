#include <iostream>
#include <fstream>
#include "oled/Edison_OLED.h"
#include "gpio/gpio.h"
#include "math.h"
#include <unistd.h> // for usleep
#include <stdlib.h> // Gives us atoi
#include <stdio.h>
#include <string>
#include <time.h>
#include "mraa.hpp"
#include "SFE_LSM9DS0.h"

gpio BUTTON_UP(47, INPUT);
gpio BUTTON_DOWN(44, INPUT);
gpio BUTTON_LEFT(165, INPUT);
gpio BUTTON_RIGHT(45, INPUT);
gpio BUTTON_SELECT(48, INPUT);
gpio BUTTON_A(49, INPUT);
gpio BUTTON_B(46, INPUT);

using namespace std;

edOLED oled;
bool repeat = true;

void setupOLED();
void startDisplay();
void cleanUp();
void displayValues(LSM9DS0 *imu);
void collectData(LSM9DS0 *imu);
void printLines(const char *string, float &val, const char *unit);
void printGuide();
void printFinished(int count);
void askingReplay(int &count);
int main()
{
  setupOLED();
  LSM9DS0 *imu;
  imu = new LSM9DS0(0x6B, 0x1D);
  imu->begin();
  ofstream myfile;
  bool newAccelData = false;
  bool newMagData = false;
  bool newGyroData = false;
  bool overflow = false;
  bool tiep = true;
  int count = 0;

  // Loop and report data
  startDisplay();
  while (1)
  {
    
    while ((BUTTON_A.pinRead() == HIGH))
    {
      if (BUTTON_UP.pinRead() != HIGH)
      {
        printFinished(count);
        return 0;
      }
    }
    // filename ="exam1.txt"
    char filename[30];
    sprintf(filename, "%d.txt", count);
    myfile.open(filename, ios::trunc);
    // cleanUp();
    printGuide();
    while ((BUTTON_B.pinRead() == HIGH) && tiep)
    {
      if (BUTTON_B.pinRead() != HIGH)
        tiep = false;
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
      cout<<"Recording..."<<endl;
      cout << "-------------------------------------" << endl;

      /* WRITE TO FILE */
      myfile << imu->calcAccel(imu->ax) << "," << imu->calcAccel(imu->ay) << "," << imu->calcAccel(imu->az) << endl;
      usleep(5);// 1 milisecond
    }
    myfile.close();
    askingReplay(count);
    count++;
  }
  return MRAA_SUCCESS;
}
void askingReplay(int &count)
{
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.print(count);
  oled.print(".txt");
  oled.setCursor(0, 16);
  oled.print("press A to repeat or UP for stop");
  oled.display();
}
void printFinished(int count)
{
  oled.clear(PAGE);
  oled.setCursor(4, 16);
  oled.print("collect ");
  oled.print(count);
  oled.print(" data");
  oled.display();
}
void setupOLED()
{
  oled.begin();
  oled.clear(PAGE);
  oled.display();
  oled.setFontType(0);
}
void printGuide()
{
  oled.clear(PAGE);
  oled.setCursor(4, 16);
  oled.print("Press B to stop recording");
  oled.display();
}
void startDisplay()
{
  oled.clear(PAGE);
  oled.setCursor(4, 16);
  oled.print("Press A to start recording");
  oled.display();
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