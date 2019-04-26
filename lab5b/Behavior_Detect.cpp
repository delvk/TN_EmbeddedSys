#include "mraa.hpp"

#include <iostream>
#include <unistd.h>
#include <stdlib.h> // Gives us atoi
#include <stdio.h>
#include "SFE_LSM9DS0.h"
#include "oled/Edison_OLED.h"
#include "math.h"

#define STANDING "STANDING"
#define WALKING "WALKING"

using namespace std;


void setupOLED();
void cleanUp();
void displayStatus(float &delta);

edOLED oled;

int main()
{
  const float g = 9.81;
  float ai = 0;
  LSM9DS0 *imu;
  imu = new LSM9DS0(0x6B, 0x1D);
  setupOLED();

  uint16_t imuResult = imu->begin();
  cout << hex << "Chip ID: 0x" << imuResult << dec << " (should be 0x49d4)" << endl;

  bool newAccelData = false;
  bool newMagData = false;
  bool newGyroData = false;
  bool overflow = false;

  // Loop and report data
  while (1)
  {
    cleanUp();
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
    // overflow = imu->xDataOverflow() |
    //            imu->gDataOverflow() |
    //            imu->mDataOverflow();

    // if (overflow)
    // {
    //   cout << "WARNING: DATA OVERFLOW!!!" << endl;
    // }

    imu->readAccel();
    imu->readMag();
    imu->readGyro();
    imu->readTemp();

    //calculate
    float ax = g * abs(imu->calcAccel(imu->ax));
    float ay = g * abs(imu->calcAccel(imu->ay));
    float az = g * abs(imu->calcAccel(imu->az));
    float axyz = sqrt(ax * ax + ay * ay + az * az);
    float delta = abs(axyz - ai);

    ai = axyz;

    // Print the "real" values in more human comprehensible units.
    cout << "\n-------------------------------------" << endl;
    displayStatus(delta);
    cout << "\n-------------------------------------" << endl;
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
void displayStatus(float &delta)
{
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.print("delta: ");
  oled.print(delta);
  oled.setCursor(0, 16);
  char * status;
  if (delta < 0.3)
  {
    // cout << "STANDING" << endl;
    // oled.print("\nSTANDING");
    status=(char *)STANDING;
  }
  else
  {
    // cout << "STANDING" << endl;
    // oled.print("\nSTANDING");
    status=(char *)WALKING;
  }
  cout<<status<<endl;
  oled.print(status);
  oled.display();
}
void cleanUp()
{
  oled.clear(PAGE);
  oled.display();
}