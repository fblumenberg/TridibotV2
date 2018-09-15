#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

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

  public:
    Joint(uint8_t number, int min, int max, int maxDeg)
    {
        this->number = number;
        this->min = min;
        this->max = max;
        this->maxDeg = maxDeg;
    }

    Joint(uint8_t number, int min, int max)
    {
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
        return String(number) + " " + String(pulseLength) + " " + String(min) + " " + String(max) + " " + String(maxDeg);
    }
};

Joint leftFrontShoulder(0, 180, 510, 160);
Joint leftFrontLeg(1, 125, 515);
Joint rightFrontShoulder(2, 183, 513, 160);
Joint rightFrontLeg(3, 155, 498);

Joint leftBackShoulder(4, 185, 515, 160);
Joint leftBackLeg(5, 145, 495);
Joint rightBackShoulder(6, 185, 485, 160);
Joint rightBackLeg(7, 150, 500);

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
        joints[i]->turnAbsolute(90);
    }
}
void centerAll2()
{
    for (int i = 0; i < noJoints; i++)
    {
        joints[i]->center();
    }
}

void all(int deg)
{
    for (int i = 0; i < noJoints; i++)
    {
        joints[i]->turnAbsolute(deg);
    }
}

void standUp(){
        joints[0]->center();
        joints[2]->center();
        joints[4]->center();
        joints[6]->center();

        joints[1]->setPin(440);
        joints[3]->setPin(456);
        joints[5]->setPin(460);
        joints[7]->setPin(465);

}

void standUp2(){
        joints[0]->center();
        joints[2]->center();
        joints[4]->center();
        joints[6]->center();

        joints[1]->setMax();
        joints[3]->setMax();
        joints[5]->setMax();
        joints[7]->setMax();

}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("8 channel Servo test!");

    pwm.begin();

    pwm.setPWMFreq(60); // Analog servos run at ~60 Hz updates

    delay(10);

    centerAll();
}

int currentServo = 0;
bool smallTest = false;
uint16_t servoValue = (SERVOMAX - SERVOMIN) / 2 + SERVOMIN;
void loop()
{
    char variable = 'X';
    if (Serial.available())
    {
        variable = Serial.read();
    }

    switch (variable)
    {
    case 'U':
        standUp2();
        servoValue = joints[currentServo]->value();
        delay(50);
        Serial.println(variable);
        break;
    case 'u':
        standUp();
        servoValue = joints[currentServo]->value();
        delay(50);
        Serial.println(variable);
        break;
    case 'C':
        centerAll();
        servoValue = joints[currentServo]->value();
        delay(50);
        Serial.println(variable);
        break;
    case 'c':
        centerAll2();
        servoValue = joints[currentServo]->value();
        delay(50);
        Serial.println(variable);
        break;
    case 'N':
        currentServo++;
        if (currentServo >= noJoints)
            currentServo = 0;
        servoValue = joints[currentServo]->value();
        delay(50);
        Serial.print(variable);
        Serial.println(currentServo);
        break;
    case 'T':
        smallTest = !smallTest;
        delay(50);
        Serial.print(variable);
        Serial.println(smallTest);
        break;
    case 'A':
        servoValue += 10;
        joints[currentServo]->setPin(servoValue);
        delay(50);
        Serial.print(variable);
        Serial.println(servoValue);
        break;
    case 'a':
        servoValue++;
        joints[currentServo]->setPin(servoValue);
        delay(50);
        Serial.print(variable);
        Serial.println(servoValue);
        break;
    case 'S':
        servoValue -= 10;
        joints[currentServo]->setPin(servoValue);
        delay(50);
        Serial.print(variable);
        Serial.println(servoValue);
        break;
    case 's':
        servoValue--;
        joints[currentServo]->setPin(servoValue);
        delay(50);
        Serial.print(variable);
        Serial.println(servoValue);
        break;
    case 'n':
        joints[currentServo]->setMax();
        Serial.println(variable);
        Serial.println(joints[currentServo]->info());
        delay(50);
        break;
    case 'm':
        joints[currentServo]->setMin();
        Serial.println(variable);
        Serial.println(joints[currentServo]->info());
        delay(50);
        break;
    case 'I':
    case 'i':
        Serial.println(variable);
        Serial.println(joints[currentServo]->info());
        break;
    default:
        break;
    }
    if (smallTest)
    {

        all(70);
        delay(500);

        all(100);
        delay(500);
    }
    else
    {
        delay(100);
    }
}
