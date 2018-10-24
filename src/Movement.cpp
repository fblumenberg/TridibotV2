
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "Movement.h"

#define SERVOMIN 150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600 // this is the 'maximum' pulse length count (out of 4096)

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

class Joint
{
    int min;
    int max;
    int maxDeg = 180;
    uint8_t number;
    uint16_t pulseLength;
    String name;

  public:
    Joint(const char *name, uint8_t number, int min, int max, int maxDeg)
    {
        this->name = name;
        this->number = number;
        this->min = min;
        this->max = max;
        this->maxDeg = maxDeg;
    }

    Joint(const char *name, uint8_t number, int min, int max)
    {
        this->name = name;
        this->number = number;
        this->min = min;
        this->max = max;
    }

    void turnAbsolute(uint8_t degrees)
    {
        pulseLength = map(degrees, 0, this->maxDeg, this->min, this->max);
        pwm.setPin(number, pulseLength);
    }

    void setPin(uint16_t pulseLength)
    {
        this->pulseLength = pulseLength;
        pwm.setPin(number, pulseLength);
    }

    void setPercent(uint16_t percent)
    {
        this->pulseLength = (((max - min) * percent) / 100) + min;
        if (this->pulseLength > 4095)
        {
            uint16_t p;

            p = (max - min);
            Serial.println(p);
            p = (max - min) * percent;
            Serial.println(p);
            p = p / 100;
            Serial.println(p);
            p = ((max - min) * percent) / 100;
            Serial.println(p);
            p = (((max - min) * percent) / 100) + min;
            Serial.println(p);
        }
        pwm.setPin(number, pulseLength);
    }

    uint16_t value()
    {
        return pulseLength;
    }

    void center()
    {
        this->pulseLength = (max - min) / 2 + min;
        pwm.setPin(number, this->pulseLength);
    }

    void setMax()
    {
        this->pulseLength = max;
        pwm.setPin(number, this->pulseLength);
    }

    void setMin()
    {
        this->pulseLength = min;
        pwm.setPin(number, this->pulseLength);
    }

    String info()
    {
        int percent = (100 * (pulseLength - min)) / (max - min);
        return name + " " + String(number) + " " + String(pulseLength) + " " + String(percent) + "% " + String(min) + " " + String(max) + " " + String(maxDeg);
    }
};

Joint leftFrontShoulder("leftFrontShoulder", 2, 180, 510, 160);
Joint leftFrontLeg("leftFrontLeg", 3, 125, 515);
Joint rightFrontShoulder("rightFrontShoulder", 0, 183, 513, 160);
Joint rightFrontLeg("rightFrontLeg", 1, 155, 498);

Joint leftBackShoulder("leftBackShoulder", 6, 185, 515, 160);
Joint leftBackLeg("leftBackLeg", 7, 145, 495);
Joint rightBackShoulder("rightBackShoulder", 4, 185, 485, 160);
Joint rightBackLeg("rightBackLeg", 5, 150, 500);

Joint *joints[] = {
    &leftFrontShoulder,
    &leftFrontLeg,
    &rightFrontShoulder,
    &rightFrontLeg,

    &leftBackShoulder,
    &leftBackLeg,
    &rightBackShoulder,
    &rightBackLeg};

int noJoints = sizeof(joints) / sizeof(Joint *);

void centerAll()
{
    for (int i = 0; i < noJoints; i++)
    {
        joints[i]->setPercent(50);
    }
}
void centerAll2()
{
    for (int i = 0; i < noJoints; i++)
    {
        joints[i]->center();
    }
}

void infoAll()
{
    for (int i = 0; i < noJoints; i++)
    {
        Serial.println(joints[i]->info());
    }
}

void all(int deg)
{
    for (int i = 0; i < noJoints; i++)
    {
        joints[i]->turnAbsolute(deg);
    }
}

void standUp()
{
    joints[0]->center();
    joints[2]->center();
    joints[4]->center();
    joints[6]->center();
    /*     joints[1]->setPin(440);
    joints[3]->setPin(456);
    joints[5]->setPin(460);
    joints[7]->setPin(465);
 */
    joints[1]->setPercent(88);
    joints[3]->setPercent(88);
    joints[5]->setPercent(88);
    joints[7]->setPercent(88);
}

void standUp2()
{
    joints[0]->center();
    joints[2]->center();
    joints[4]->center();
    joints[6]->center();

    joints[1]->setMax();
    joints[3]->setMax();
    joints[5]->setMax();
    joints[7]->setMax();
}

uint16_t rightMovement;
uint16_t leftMovement;
uint16_t legUp = 16;
int servoWait = 70;

void baseMovement()
{
    leftFrontLeg.setPercent(88 - legUp);
    delay(servoWait);
    rightBackLeg.setPercent(88 - legUp);
    delay(servoWait);

    leftFrontShoulder.setPercent(50 + leftMovement);
    rightBackShoulder.setPercent(50 - rightMovement);
    rightFrontShoulder.setPercent(50 + rightMovement);
    leftBackShoulder.setPercent(50 - leftMovement);

    delay(servoWait);

    leftFrontLeg.setPercent(88);
    delay(servoWait);
    rightBackLeg.setPercent(88);
    delay(servoWait);

    leftBackLeg.setPercent(88 - legUp);
    delay(servoWait);
    rightFrontLeg.setPercent(88 - legUp);
    delay(servoWait);

    rightFrontShoulder.setPercent(50 - rightMovement);
    leftBackShoulder.setPercent(50 + leftMovement);
    leftFrontShoulder.setPercent(50 - leftMovement);
    rightBackShoulder.setPercent(50 + rightMovement);
    delay(servoWait);

    leftBackLeg.setPercent(88);
    delay(servoWait);
    rightFrontLeg.setPercent(88);
    delay(servoWait);
}

void walkForward()
{
    leftMovement = 16;
    rightMovement = 16;
    baseMovement();
}
void walkBackward()
{
    leftMovement = -16;
    rightMovement = -16;
    baseMovement();
}
void turntLeft()
{
    leftMovement = -16;
    rightMovement = 16;
    baseMovement();
}
void turntRight()
{
    leftMovement = 16;
    rightMovement = -16;
    baseMovement();
}

int currentServo = 0;
bool smallTest = false;
uint16_t servoValue = (SERVOMAX - SERVOMIN) / 2 + SERVOMIN;

void setupMovement()
{
    pwm.begin();
    pwm.setPWMFreq(60); // Analog servos run at ~60 Hz updates

    centerAll();
}

void handleCommand(char cmd, String arg)
{
    switch (cmd)
    {
    case CMD_FORWARD:
        walkForward();
        break;
    case CMD_BACKWARD:
        walkBackward();
        break;
    case CMD_LEFT:
        turntLeft();
        break;
    case CMD_RIGHT:
        turntRight();
        break;
    case CMD_STAND:
        standUp();
        break;
    case CMD_CENTER:
        centerAll();
        break;
    case CMD_SETSPEED:
        break;
    case CMD_SETBRIGHTNES:
        break;
    case '8':
        walkForward();
        break;
    case '2':
        walkBackward();
        break;
    case '4':
        turntLeft();
        break;
    case '6':
        turntRight();
        break;
    case 'U':
        standUp2();
        servoValue = joints[currentServo]->value();
        delay(50);
        Serial.println(cmd);
        break;
    case 'u':
        standUp();
        servoValue = joints[currentServo]->value();
        delay(50);
        Serial.println(cmd);
        break;
    // case 'C':
    //     centerAll();
    //     servoValue = joints[currentServo]->value();
    //     delay(50);
    //     Serial.println(cmd);
    //     break;
    case 'c':
        centerAll2();
        servoValue = joints[currentServo]->value();
        delay(50);
        Serial.println(cmd);
        break;
    case 'N':
        currentServo++;
        if (currentServo >= noJoints)
            currentServo = 0;
        servoValue = joints[currentServo]->value();
        delay(50);
        Serial.print(cmd);
        Serial.println(currentServo);
        break;
    case 'T':
        smallTest = !smallTest;
        delay(50);
        Serial.print(cmd);
        Serial.println(smallTest);
        break;
    case 'A':
        servoValue += 10;
        joints[currentServo]->setPin(servoValue);
        delay(50);
        Serial.print(cmd);
        Serial.println(servoValue);
        break;
    case 'a':
        servoValue++;
        joints[currentServo]->setPin(servoValue);
        delay(50);
        Serial.print(cmd);
        Serial.println(servoValue);
        break;
    // case 'S':
    //     servoValue -= 10;
    //     joints[currentServo]->setPin(servoValue);
    //     delay(50);
    //     Serial.print(cmd);
    //     Serial.println(servoValue);
    //     break;
    case 's':
        servoValue--;
        joints[currentServo]->setPin(servoValue);
        delay(50);
        Serial.print(cmd);
        Serial.println(servoValue);
        break;
    case 'n':
        joints[currentServo]->setMax();
        Serial.println(cmd);
        Serial.println(joints[currentServo]->info());
        delay(50);
        break;
    case 'm':
        joints[currentServo]->setMin();
        Serial.println(cmd);
        Serial.println(joints[currentServo]->info());
        delay(50);
        break;
    case 'I':
        Serial.println(cmd);
        infoAll();
        break;
    case 'i':
        Serial.println(cmd);
        Serial.println(joints[currentServo]->info());
        break;
    default:
        break;
    }
}
