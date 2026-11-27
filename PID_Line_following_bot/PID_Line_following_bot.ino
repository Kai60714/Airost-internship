float kp = 70.0;
float ki = 0.0;
float kd = 0.0;

const int L_pin = A3;
const int M_pin = A2;
const int R_pin = A1;

const int L_motor = 7;
const int R_motor = 9;
int ave = 220;

const float L_white = 675.0, L_black = 980.0; // A3
const float M_white = 452.0, M_black = 964.0; // A2
const float R_white = 585.0, R_black = 970.0; // A1

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
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float L = constrain((analogRead(L_pin) - L_white)/(L_black-L_white),0.0,1.0);//A3
  float M = constrain((analogRead(M_pin) - M_white)/(M_black-M_white),0.0,1.0);//A2
  float R = constrain((analogRead(R_pin) - R_white)/(R_black-R_white),0.0,1.0);//A1
  float sum = L + M + R;

  now = millis();
  float dt = (now - prev_time) / 1000.0;

  if (sum > 0.0) {
   error = (R - L) / sum;
  } else {
    error = lastError;  // Prevents divide 0 when all sensor read 0
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
  Serial.println(constrain((ave + correction), 0, 255));
  analogWrite(L_motor, constrain((ave + correction), 0, 255));
  analogWrite(R_motor, constrain((ave - correction), 0, 255));
}