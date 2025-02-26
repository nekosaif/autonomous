#define R_PWM_RIGHT 4
#define L_PWM_RIGHT 5
#define R_EN_RIGHT 6
#define L_EN_RIGHT 7

#define R_PWM_LEFT 8
#define L_PWM_LEFT 9
#define R_EN_LEFT 10
#define L_EN_LEFT 11

void setup() {
  // Initialize motor control pins
  pinMode(R_PWM_RIGHT, OUTPUT);
  pinMode(L_PWM_RIGHT, OUTPUT);
  pinMode(R_EN_RIGHT, OUTPUT);
  pinMode(L_EN_RIGHT, OUTPUT);
  
  pinMode(R_PWM_LEFT, OUTPUT);
  pinMode(L_PWM_LEFT, OUTPUT);
  pinMode(R_EN_LEFT, OUTPUT);
  pinMode(L_EN_LEFT, OUTPUT);

  // Initialize all EN pins to LOW
  digitalWrite(R_EN_RIGHT, LOW);
  digitalWrite(L_EN_RIGHT, LOW);
  digitalWrite(R_EN_LEFT, LOW);
  digitalWrite(L_EN_LEFT, LOW);

  // Start serial communication
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to connect
  Serial.println("Ready to receive commands...");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    int spaceIndex = input.indexOf(' ');
    char command = input.charAt(0);
    int pwm = 0;

    if (spaceIndex != -1) {
      String pwmStr = input.substring(spaceIndex + 1);
      pwm = pwmStr.toInt();
      pwm = constrain(pwm, 0, 255); // Ensure PWM stays within valid range
    }

    switch (command) {
      case 'F': // Forward
        rightMotorForward(pwm);
        leftMotorForward(pwm);
        break;
      case 'B': // Backward
        rightMotorReverse(pwm);
        leftMotorReverse(pwm);
        break;
      case 'L': // Left turn (spin)
        rightMotorForward(pwm);
        leftMotorReverse(pwm);
        break;
      case 'R': // Right turn (spin)
        leftMotorForward(pwm);
        rightMotorReverse(pwm);
        break;
      case 'S': // Stop
        rightMotorStop();
        leftMotorStop();
        break;
      default:
        Serial.println("Invalid command!");
        break;
    }
  }
}

// Right motor control functions
void rightMotorForward(int speed) {
  digitalWrite(R_EN_RIGHT, HIGH);
  digitalWrite(L_EN_RIGHT, HIGH);
  analogWrite(R_PWM_RIGHT, speed);
  analogWrite(L_PWM_RIGHT, 0);
}

void rightMotorReverse(int speed) {
  digitalWrite(R_EN_RIGHT, HIGH);
  digitalWrite(L_EN_RIGHT, HIGH);
  analogWrite(R_PWM_RIGHT, 0);
  analogWrite(L_PWM_RIGHT, speed);
}

void rightMotorStop() {
  analogWrite(R_PWM_RIGHT, 0);
  analogWrite(L_PWM_RIGHT, 0);
  digitalWrite(R_EN_RIGHT, LOW);
  digitalWrite(L_EN_RIGHT, LOW);
}

// Left motor control functions
void leftMotorForward(int speed) {
  digitalWrite(R_EN_LEFT, HIGH);
  digitalWrite(L_EN_LEFT, HIGH);
  analogWrite(R_PWM_LEFT, speed);
  analogWrite(L_PWM_LEFT, 0);
}

void leftMotorReverse(int speed) {
  digitalWrite(R_EN_LEFT, HIGH);
  digitalWrite(L_EN_LEFT, HIGH);
  analogWrite(R_PWM_LEFT, 0);
  analogWrite(L_PWM_LEFT, speed);
}

void leftMotorStop() {
  analogWrite(R_PWM_LEFT, 0);
  analogWrite(L_PWM_LEFT, 0);
  digitalWrite(R_EN_LEFT, LOW);
  digitalWrite(L_EN_LEFT, LOW);
}
