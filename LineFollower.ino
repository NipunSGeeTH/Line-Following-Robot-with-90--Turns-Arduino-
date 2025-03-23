#define IR1 A0
#define IR2 A1
#define IR3 A2
#define IR4 A3
#define IR5 A4
#define IR6 A5
#define IR7 A6
#define IR8 A7

#define RMotorA 2
#define RMotorB 3
#define RMotorpwm 8

#define LMotorA 4
#define LMotorB 5
#define LMotorpwm 9

int MotorBasedspeed = 28; 
#define maxSpeed 34

int IR_val[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int IR_weights[8] = {30, 20, 17, 4, -4, -17, -20, -30};

int LMotorspeed = 0;
int RMotorspeed = 0;
int speedAdjust = 0;

float P, I, D;
float error = 0;
float previousError = 0;
float kp = 8; 
float kd = 10; 
float ki = 0.0;

void setup() {
  pinMode(13, OUTPUT);

  pinMode(LMotorA, OUTPUT);
  pinMode(LMotorB, OUTPUT);
  pinMode(LMotorpwm, OUTPUT);
  pinMode(RMotorA, OUTPUT);
  pinMode(RMotorB, OUTPUT);
  pinMode(RMotorpwm, OUTPUT);

  set_forward();
}

void loop() {
  read_IR();
  int turnDirection = check_90_degree_turn();
  if (turnDirection != 0) {
    // Go straight for 1 second before taking the turn
    set_forward();
    analogWrite(LMotorpwm, MotorBasedspeed);
    analogWrite(RMotorpwm, MotorBasedspeed);
    delay(400); // Move forward for 1 second

    make_90_degree_turn(turnDirection);
  } else {
    PID_control();
    set_speed();
  }
}

void read_IR() {
  digitalWrite(13, HIGH); // Turn on indicator LED
  for (int i = 0; i < 8; i++) {
    IR_val[i] = analogRead(A0 + i) > 900 ? 1 : 0; // Read IR sensor values
  }
  digitalWrite(13, LOW); // Turn off indicator LED
}

int check_90_degree_turn() {
  // Check if all left side sensors detect black
  if (IR_val[0] == 1 && IR_val[1] == 1 && IR_val[2] == 1 && IR_val[3] == 1) {
    return -1; // Indicates a left turn
  }
  // Check if all right side sensors detect black
  if (IR_val[4] == 1 && IR_val[5] == 1 && IR_val[6] == 1 && IR_val[7] == 1) {
    return 1; // Indicates a right turn
  }
  return 0; // No 90-degree turn detected
}

void make_90_degree_turn(int direction) {
  // Stop the robot
  stop();
  delay(400); // Wait for 1 second

  int turnSpeed = MotorBasedspeed + 2;

  if (direction == 1) {
    // Make 90-degree turn to the left
    digitalWrite(LMotorA, HIGH);
    digitalWrite(LMotorB, LOW);
    digitalWrite(RMotorA, LOW);
    digitalWrite(RMotorB, HIGH);
  } else if (direction == -1) {
    // Make 90-degree turn to the right
    digitalWrite(LMotorA, LOW);
    digitalWrite(LMotorB, HIGH);
    digitalWrite(RMotorA, HIGH);
    digitalWrite(RMotorB, LOW);
  }

  analogWrite(LMotorpwm, turnSpeed);
  analogWrite(RMotorpwm, turnSpeed);

  // Keep turning until one of the middle sensors (IR4 or IR5) detects black
  while (true) {
    read_IR();  // Continuously check sensors during turning
    if (IR_val[3] == 1 || IR_val[4] == 1) {
      break; // Exit loop when middle sensors see black
    }
  }

  // Resume forward movement
  set_forward();
}

void set_speed() {
  analogWrite(RMotorpwm, RMotorspeed); // Set right motor speed
  analogWrite(LMotorpwm, LMotorspeed); // Set left motor speed
}

void set_forward() {
  digitalWrite(LMotorB, HIGH); // Left motor forward
  digitalWrite(LMotorA, LOW);
  digitalWrite(RMotorB, HIGH); // Right motor forward
  digitalWrite(RMotorA, LOW);
  set_speed(); // Apply motor speeds
}

void stop() {
  digitalWrite(LMotorB, LOW); // Stop left motor
  digitalWrite(LMotorA, LOW);
  digitalWrite(RMotorB, LOW); // Stop right motor
  digitalWrite(RMotorA, LOW);
  analogWrite(LMotorpwm, 0); // Set motor speeds to 0
  analogWrite(RMotorpwm, 0);
}

void PID_control() {
  error = 0;
  for (int i = 0; i < 8; i++) {
    error += IR_weights[i] * IR_val[i]; // Calculate weighted error
  }

  P = error; // Proportional term
  I += error; // Integral term
  D = error - previousError; // Derivative term
  previousError = error;

  if (error == 0) {
    I = 0; // Reset integral term if error is zero
  }

  speedAdjust = (kp * P + ki * I + kd * D); // Calculate speed adjustment

  LMotorspeed = MotorBasedspeed + speedAdjust; // Adjust left motor speed
  RMotorspeed = MotorBasedspeed - speedAdjust; // Adjust right motor speed

  // Constrain motor speeds to valid range
  LMotorspeed = constrain(LMotorspeed, 0, maxSpeed);
  RMotorspeed = constrain(RMotorspeed, 0, maxSpeed);
}