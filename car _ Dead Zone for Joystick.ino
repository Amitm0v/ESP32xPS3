#include <Ps3Controller.h>

// Motor Driver Pins
const int enableRightMotor = 22;
const int rightMotorPin1 = 16;
const int rightMotorPin2 = 17;
const int enableLeftMotor = 23;
const int leftMotorPin1 = 18;
const int leftMotorPin2 = 19;
const int statusLED = 2; // LED to indicate connection status

#define MAX_MOTOR_SPEED 250
#define DEADZONE 20  // To prevent unwanted small movements

// PWM Setup
const int PWMFreq = 1000;
const int PWMResolution = 8;
const int RightMotorChannel = 0;
const int LeftMotorChannel = 1;

void notify()
{
    int yAxisValue = Ps3.data.analog.stick.ly; // Forward/Backward
    int xAxisValue = Ps3.data.analog.stick.rx; // Left/Right
    
    if (abs(yAxisValue) < DEADZONE && abs(xAxisValue) < DEADZONE) {
        rotateMotor(0, 0);
        return;
    }
    
    // Calculate motor speeds for proportional steering
    int rightSpeed = (yAxisValue - xAxisValue) * MAX_MOTOR_SPEED / 128;
    int leftSpeed = (yAxisValue + xAxisValue) * MAX_MOTOR_SPEED / 128;
    
    rotateMotor(rightSpeed, leftSpeed);
}

void onConnect()
{
    Serial.println("PS3 Controller Connected!");
    digitalWrite(statusLED, HIGH);
}

void onDisConnect()
{
    Serial.println("PS3 Controller Disconnected!");
    digitalWrite(statusLED, LOW);
    rotateMotor(0, 0);
}

void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
    // Right motor direction
    digitalWrite(rightMotorPin1, rightMotorSpeed > 0);
    digitalWrite(rightMotorPin2, rightMotorSpeed < 0);
    ledcWrite(RightMotorChannel, abs(rightMotorSpeed));
    
    // Left motor direction
    digitalWrite(leftMotorPin1, leftMotorSpeed > 0);
    digitalWrite(leftMotorPin2, leftMotorSpeed < 0);
    ledcWrite(LeftMotorChannel, abs(leftMotorSpeed));
}

void setUpPinModes()
{
    pinMode(enableRightMotor, OUTPUT);
    pinMode(rightMotorPin1, OUTPUT);
    pinMode(rightMotorPin2, OUTPUT);
    
    pinMode(enableLeftMotor, OUTPUT);
    pinMode(leftMotorPin1, OUTPUT);
    pinMode(leftMotorPin2, OUTPUT);
    
    pinMode(statusLED, OUTPUT);
    
    // Set up PWM channels
    ledcSetup(RightMotorChannel, PWMFreq, PWMResolution);
    ledcSetup(LeftMotorChannel, PWMFreq, PWMResolution);
    
    ledcAttachPin(enableRightMotor, RightMotorChannel);
    ledcAttachPin(enableLeftMotor, LeftMotorChannel);
    
    rotateMotor(0, 0);
}

void setup()
{
    Serial.begin(115200);
    setUpPinModes();
    
    Ps3.attach(notify);
    Ps3.attachOnConnect(onConnect);
    Ps3.attachOnDisconnect(onDisConnect);
    Ps3.begin();
    Serial.println("Waiting for PS3 Controller...");
}

void loop()
{
    // Nothing needed here, handled by notify()
}
