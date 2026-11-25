float kp = 1;
float ki = 1;
float kd = 1;
int L_pin = A0;
int M_pin = A1;
int R_pin = A2;
int L_motor =4;
int R_motor =5;
int ave = 125;

unsigned long prev_time = 0;
unsigned long now = 0;

float error = 0, lastError = 0, integral = 0;
float correction;
void setup() {
  // put your setup code here, to run once:
  pinMode(L_pin, INPUT);
  pinMode(M_pin, INPUT);
  pinMode(R_pin, INPUT);
  pinMode(L_motor, OUTPUT);
  pinMode(R_motor, OUTPUT);
  prev_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  float L = analogRead(L_pin);
  float M = analogRead(M_pin);
  float R = analogRead(R_pin);
  error = R - L;
  now = millis();
  float dt = (now - prev_time) / 1000.0;
  float de = error - lastError;
  float diff = de / dt;
  integral += error * dt;
  float sum = L + M + R;

  if (sum > 0) {
    error = (R - L) / sum;  // Always stays cleanly between -1.0 and +1.0
  } else {
    error = lastError;  // Prevents dividing by 0 if all sensors read 0
  }

  correction = (kp * error) + (ki * integral) + (kd * diff);

  lastError = error;
  prev_time = now;
  analogWrite(L_motor,constrain((ave + correction),0,255));
  analogWrite(R_motor,constrain((ave - corrrection),0,255));
}
