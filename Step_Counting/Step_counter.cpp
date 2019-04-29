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

// gpio BUTTON_UP(47, INPUT);
// gpio BUTTON_DOWN(44, INPUT);
// gpio BUTTON_LEFT(165, INPUT);
// gpio BUTTON_RIGHT(45, INPUT);
// gpio BUTTON_SELECT(48, INPUT);
gpio BUTTON_A(49, INPUT);
gpio BUTTON_B(46, INPUT);

using namespace std;

void setupOLED();
void startDisplay();
void cleanUp();
void printFinished();
void printStep();
void stepCouting(LSM9DS0 *imu);

// GLOBAL VARIABLE
edOLED oled;
bool repeat = true;
bool peak = false;
int count = 0;
float a_before = 0.0;
float a_after = 0.0;
float a_current = 0.0;
int k = 0, D = 0;
int idx = 0;

int main()
{
  //Initial oled
  setupOLED();

  //Initial 9DOF
  LSM9DS0 *imu;
  bool newAccelData = false;
  imu = new LSM9DS0(0x6B, 0x1D);
  imu->begin();

  //Starting program
  startDisplay();

  //Wait for user to press A to start
  while (BUTTON_A.pinRead() == HIGH)
    ;

  //User pressed, clean oled
  cleanUp();

  //Stop condition, otherwise looping
  while (BUTTON_B.pinRead() == HIGH)
  {
    if (!newAccelData)
    {
      newAccelData = imu->newXData();
    }
    imu->readAccel();
    imu->readTemp();
    // Start step counting
    stepCouting(imu);
    //Interval 5ms
    usleep(5);
  }
  printFinished();
  return MRAA_SUCCESS;
}

void stepCouting(LSM9DS0 *imu)
{
  //Get all accelerator data
  float ax = calcAccel(imu->ax);
  float ay = calcAccel(imu->ay);
  float az = calcAccel(imu->az);

  //calculate a = sqrt(x^2+y^2+z^2)
  float a = sqrt(ax * ax + ay * ay + az * az);

  int old_count = count;

  /*Start algorithm*/
  if (idx == 0)
  {
    a_before = 0;
    a_current = a;
    printStep();
  }
  else if (idx == 1)
  {
    a_after = a;
  }
  else
  {
    if (a_before < a_current && a_current > a_after)
    {
      // peak = true;
      if (k != 0)
      {
        D = idx - k - 1;
        // cout << "D= " << D << endl;
        if (D > 2)
          count++;
      }
      k = idx;
    }
    a_before = a_current;
    a_current = a_after;
    a_after = a;
  }

  //Print couting
  if (count != old_count)
    printStep();

  //Increase index
  idx++;
  return;
}
void printStep()
{
  cout << "step: " << count << endl;
  oled.setCursor(0, 0);
  oled.print("Counting\n");
  oled.print("Step: %d", count);
  oled.display();
}
void setupOLED()
{
  oled.begin();
  oled.clear(PAGE);
  oled.display();
  oled.setFontType(0);
}
void startDisplay()
{
  oled.clear(PAGE);
  oled.setCursor(4, 16);
  oled.print("Press A to start counting\n");
  oled.print("Press B to stop recording");
  oled.display();
}
void cleanUp()
{
  oled.clear(PAGE);
  oled.display();
}
void printFinished()
{
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.print("Finished\n");
  oled.print("Step count: %d", count);
  oled.display();
}