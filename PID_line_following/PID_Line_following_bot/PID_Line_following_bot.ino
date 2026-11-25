float kp = 1;
float ki = 0;
float kd = 0;
int L2_pin = A0;
int L1_pin = A1;
int R1_pin = A2;
int R2_pin = A3;
int L_motor = 4;
int R_motor = 5;
int ave = 125;

unsigned long prev_time = 0;
unsigned long now = 0;

float error = 0, lastError = 0, integral = 0;
float correction;

void setup() {
  // put your setup code here, to run once:
  pinMode(L2_pin, INPUT);
  pinMode(L1_pin, INPUT);
  pinMode(R1_pin, INPUT);
  pinMode(R2_pin, INPUT);
  pinMode(L_motor, OUTPUT);
  pinMode(R_motor, OUTPUT);
  prev_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  float L2 = analogRead(L2_pin);
  float L1 = analogRead(L1_pin);
  float R1 = analogRead(R1_pin);
  float R2 = analogRead(R2_pin);
  float sum = L2 + L1 + R1 + R2;

  now = millis();
  float dt = (now - prev_time) / 1000.0;

  if (sum > 0) {
    error = (-2 * L2 - 1 * L1 + 1 * R1 + 2 * R2) / (2.0 * sum);  // Always stays cleanly between -1.0 and +1.0
  } else {
    error = lastError;  // Prevents dividing by 0 if all sensors read 0
  }
  if (dt > 0.0) {
    float de = error - lastError;
    float diff = de / dt;
    integral += error * dt;
    integral = constrain(integral, -50.0, 50.0);
    correction = (kp * error) + (ki * integral) + (kd * diff);
  }
  lastError = error;
  prev_time = now;
  analogWrite(L_motor, constrain((ave + correction), 0, 255));
  analogWrite(R_motor, constrain((ave - correction), 0, 255));
}