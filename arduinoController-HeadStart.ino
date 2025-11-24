// ===== Intensity Encoder Pins =====
const int CLK1 = 3;
const int DT1  = 2;

// ===== Sensitivity Encoder Pins =====
const int CLK2 = 5;
const int DT2  = 4;

// ===== Force Pins =====
const int forcePin1 = A0;
const int forcePin2 = A1;

// ===== LED Pins =====
const int LED1 = 9;  // Intensity LED
const int LED2 = 10; // Sensitivity LED

// ===== ON/OFF BUTTON =====
const int onOff = 7;
const int onOffLED = 8;

// ===== Vibration Pin =====
const int vibrationPin = 11;

// ===== Variables =====
int intensityValue = 0;
int sensitivityValue = 0;

unsigned long pressureStartTime = 0;
const unsigned long minPressTime = 20;
bool isPressing = false;

int stepSize = 15;

int minForce = 150;
int forceValue1;
int forceValue2;

int previousForceValue1;
int previousForceValue2;

int lastCLK1State;
int lastCLK2State;

// ===== System Toggle Flag =====
bool systemOn = false;

// ===== Button helper variables =====
bool lastButtonState = HIGH;
bool buttonHandled = false;

// ===== Setup =====
void setup() {
  Serial.begin(9600);

  pinMode(onOff, INPUT_PULLUP);

  pinMode(CLK1, INPUT);
  pinMode(DT1, INPUT);
  pinMode(CLK2, INPUT);
  pinMode(DT2, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(onOffLED, OUTPUT);
  pinMode(vibrationPin, OUTPUT);

  // Initialize encoder states
  lastCLK1State = digitalRead(CLK1);
  lastCLK2State = digitalRead(CLK2);

  // Start LEDs and vibration off
  analogWrite(LED1, intensityValue);
  analogWrite(LED2, sensitivityValue);
  analogWrite(vibrationPin, 0);
}

// ===== Encoder update function =====
void updateEncoder(int CLK, int DT, int &lastState, int &value, int ledPin, const char *label) {
  int currentCLK = digitalRead(CLK);

  if (lastState == LOW && currentCLK == HIGH) {
    if (digitalRead(DT) == LOW) value += stepSize; // CW
    else value -= stepSize; // CCW

    value = constrain(value, 0, 255);
    analogWrite(ledPin, value);

    previousForceValue1 = analogRead(forcePin1);
    previousForceValue2 = analogRead(forcePin2);

    Serial.print(label);
    Serial.print(" | Direction: ");
    Serial.print((digitalRead(DT) != currentCLK) ? "CW" : "CCW");
    Serial.print(" | Value: ");
    Serial.println(value);

    delay(25); // debounce
  }

  lastState = currentCLK;
}

// ===== ON/OFF helper function =====
void handleOnOffButton() {
    static bool lastButtonState = HIGH;
    static bool buttonHandled = false;

    bool buttonState = digitalRead(onOff);

    if (buttonState == LOW && lastButtonState == HIGH && !buttonHandled) {
      systemOn = !systemOn;
      buttonHandled = true;

      delay(100);

      Serial.print("System is now: ");
      Serial.println(systemOn ? "ON" : "OFF");                                                                  

      if (!systemOn) {
        analogWrite(vibrationPin, 0);
        digitalWrite(LED1, 0);
        digitalWrite(LED2, 0);
        digitalWrite(onOffLED, LOW);
      } else {
        digitalWrite(LED1, intensityValue);
        digitalWrite(LED2, sensitivityValue);
        digitalWrite(onOffLED, HIGH);
      }
    }

    if (buttonState == HIGH) buttonHandled = false;

    lastButtonState = buttonState;
}

// ===== Main loop =====
void loop() {
  handleOnOffButton();

  if (systemOn) {
    updateEncoder(CLK1, DT1, lastCLK1State, intensityValue, LED1, "Intensity");
    updateEncoder(CLK2, DT2, lastCLK2State, sensitivityValue, LED2, "Sensitivity");

    forceValue1 = 1023 - analogRead(forcePin1);
    forceValue2 = 1023 - analogRead(forcePin2);


    if (forceValue1 > minForce || forceValue2 > minForce) { // Test to see how much force
      if (!isPressing) {
        isPressing = true;
        pressureStartTime = millis();
      }

      if (millis() - pressureStartTime >= 5100 - (minPressTime * sensitivityValue)) {
        analogWrite(vibrationPin, intensityValue);
      }

    } else {
      analogWrite(vibrationPin, 0);
      isPressing = false;
      pressureStartTime = 0;
    } 
  }
}