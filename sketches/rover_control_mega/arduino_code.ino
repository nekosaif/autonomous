/*
Arduino : BTS 1
EN_PIN 40, 41 : R_EN, L_EN
PWM_PIN 8, 9 : R_PWM, L_PWM

Arduino : BTS 2
EN_PIN 42, 43 : R_EN, L_EN
PWM_PIN 10, 11 : R_PWM, L_PWM
*/

#define BRAKEVCC 0
#define CW 1
#define CCW 2
#define BRAKEGND 3

#define WHEEL_A 0
#define WHEEL_B 1


byte readByte;


const int enAPinWheel[2] = {6, 10};
const int enBPinWheel[2] = {7, 11};

const int pwmAPinWheel[2] = {4, 8};
const int pwmBPinWheel[2] = {5, 9};


const int START_DELAY = 300 / 255;
const int STOP_DELAY = 300 / 255;


int MAX_SPEED;
float MUL = 0.5;
float MUL_DRIFT = 0.25;


bool isMovingForward = false;
bool isMovingReverse = false;
bool isMovingRight = false;
bool isMovingLeft = false;
bool isDriftingRight = false;
bool isDriftingLeft = false;


void setup()
{
    Serial.begin(9600);

    wheelSetup();

    MAX_SPEED = 150;
}

void loop()
{
    if (Serial.available() > 0)
    {
        readByte = Serial.read();

        if (readByte == 'w')
        {
            wheel_onward(255, true);
        }
        else if (readByte == 's')
        {
            wheel_onward(255, false);
        }
        else if (readByte == 'a')
        {
            wheel_steering(255, true);
        }
        else if (readByte == 'd')
        {
            wheel_steering(255, false);
        }
        else if (readByte == 'q')
        {
            wheel_drift(255, true);
        }
        else if (readByte == 'e')
        {
            wheel_drift(255, false);
        }
        else if (readByte == '1')
        {
            MAX_SPEED = 100;
        }
        else if (readByte == '2')
        {
            MAX_SPEED = 120;
        }
        else if (readByte == '3')
        {
            MAX_SPEED = 150;
        }
        else if (readByte == '4')
        {
            MAX_SPEED = 255;
        }
        else if (readByte == '5')
        {
            MAX_SPEED = 255;
        }
        else if (readByte == '6')
        {
            MAX_SPEED = 255;
        }
        else if (readByte == '7')
        {
            MAX_SPEED = 255;
        }
        else if (readByte == '8')
        {
            MAX_SPEED = 255;
        }
        else if (readByte == '9')
        {
            MAX_SPEED = 255;
        }
        else if (readByte == '0')
        {
            MAX_SPEED = 255;
        }
        else if (readByte == '-')
        {
            stop_all();
        }
    }
    Serial.flush();
}

void wheelSetup()
{
    for (int i = 0; i < 2; i++)
    {
        pinMode(enAPinWheel[i], OUTPUT);
        pinMode(enBPinWheel[i], OUTPUT);
        pinMode(pwmAPinWheel[i], OUTPUT);
        pinMode(pwmBPinWheel[i], OUTPUT);

        analogWrite(pwmAPinWheel[i], 0);
        analogWrite(pwmBPinWheel[i], 0);

        digitalWrite(enAPinWheel[i], LOW);
        digitalWrite(enBPinWheel[i], LOW);
    }
}


void wheelGo(uint8_t wheel, uint8_t mode, uint8_t speed)
{
    digitalWrite(enAPinWheel[wheel], HIGH);
    digitalWrite(enBPinWheel[wheel], HIGH);
    switch (mode)
    {
    case CW:
        analogWrite(pwmAPinWheel[wheel], speed);
        analogWrite(pwmBPinWheel[wheel], 0);
        break;
    case CCW:
        analogWrite(pwmAPinWheel[wheel], 0);
        analogWrite(pwmBPinWheel[wheel], speed);
        break;

    default:
        return;
    }
    return;
}

//void motorOff(uint8_t motor)
//{
//    digitalWrite(inAPin[motor], LOW);
//    digitalWrite(inBPin[motor], LOW);
//    analogWrite(pwmPin[motor], 0);
//    digitalWrite(enPin[motor], LOW);
//}

void wheelOff(uint8_t wheel)
{
    digitalWrite(enAPinWheel[wheel], LOW);
    digitalWrite(enBPinWheel[wheel], LOW);
    analogWrite(pwmAPinWheel[wheel], 0);
    analogWrite(pwmBPinWheel[wheel], 0);
}


void wheel_onward(int pwmSpeed, bool isForward)
{
    if (!isMovingForward && !isMovingReverse)
    {
        for (int i = 0; i < MAX_SPEED; ++i) 
        {
            wheelGo(WHEEL_A, isForward ? CW : CCW, i*0.93);
            wheelGo(WHEEL_B, isForward ? CW : CCW, i);
            delay(START_DELAY);
        }
        if (isForward)
        {
            isMovingForward = true;
        }
        else
        {
          isMovingReverse = true;
        }
    }
}

void wheel_steering(int pwmSpeed, bool isRight)
{
    if (!isMovingRight && !isMovingLeft)
    {
        for (int i = 0; i < MAX_SPEED*MUL; ++i) 
        {
            wheelGo(WHEEL_A, isRight ? CW : CCW, i);
            wheelGo(WHEEL_B, isRight ? CCW : CW, i);
            delay(START_DELAY);
        }
        if (isRight)
        {
            isMovingRight = true;
        }
        else
        {
          isMovingLeft = true;
        }
    }
}

void wheel_drift(int pwmSpeed, bool isRight)
{
    if (!isDriftingRight && !isDriftingLeft)
    {
        for (int i = 0; i < MAX_SPEED*MUL; ++i) 
        {
            wheelGo(WHEEL_A, isRight ? CW : CCW, isRight ? i : i*MUL_DRIFT);
            wheelGo(WHEEL_B, isRight ? CW : CCW, isRight ? i*MUL_DRIFT : i);
            delay(START_DELAY);
        }
        if (isRight)
        {
            isDriftingRight = true;
        }
        else
        {
          isDriftingLeft = true;
        }
    }
}

void stop_all()
{
    if (isMovingForward)
    {
        for (int i = MAX_SPEED; i > 1; --i) 
        {
            wheelGo(WHEEL_A, isMovingForward ? CW : CCW, i);
            wheelGo(WHEEL_B, isMovingForward ? CW : CCW, i);
            delay(STOP_DELAY);
        }
        isMovingForward = false;
    }
    if (isMovingReverse)
    {
        for (int i = MAX_SPEED; i > 1; --i) 
        {
            wheelGo(WHEEL_A, isMovingForward ? CW : CCW, i);
            wheelGo(WHEEL_B, isMovingForward ? CW : CCW, i);
            delay(STOP_DELAY);
        }
        isMovingReverse = false;
    }
    if (isMovingRight)
    {
        for (int i = MAX_SPEED*MUL; i > 1; --i) 
        {
            wheelGo(WHEEL_A, isMovingRight ? CW : CCW, i);
            wheelGo(WHEEL_B, isMovingRight ? CCW : CW, i);
            delay(STOP_DELAY);
        }
        isMovingRight = false;
    }
    if (isMovingLeft)
    {
        for (int i = MAX_SPEED*MUL; i > 1; --i) 
        {
            wheelGo(WHEEL_A, isMovingRight ? CW : CCW, i);
            wheelGo(WHEEL_B, isMovingRight ? CCW : CW, i);
            delay(STOP_DELAY);
        }
        isMovingLeft = false;
    }

//    motorOff(MOTOR_A);
//    motorOff(MOTOR_B);
//    motorOff(MOTOR_C);
//    motorOff(MOTOR_D);
//    motorOff(MOTOR_E);
//    motorOff(MOTOR_F);
//    wheelOff(WHEEL_A);
//    wheelOff(WHEEL_B);
}