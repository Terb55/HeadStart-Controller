# HeadStartArduinoCode

This project contains the firmware for a pressure-activated haptic headrest.
The system uses two rotary encoders, two force sensors, and a vibration motor module to provide adjustable haptic feedback when pressure is applied to the headrest.

### Features:
- Intensity encoder controls vibration strength
- Sensitivity encoder controls how long pressure must be applied before activation
- Two FSR force sensors detect pressure on the headrest
- On/Off button to toggle the whole system
- LED indicators show current intensity and sensitivity values
- Vibration motors activate when pressure passes a threshold for long enough

### How It Works:
- When the system is ON, turning the encoders adjusts:
  - intensityValue → vibration PWM level
  - sensitivityValue → required hold time for activation
  - FSR readings are inverted and checked against a minimum force threshold.
  - If pressure is held long enough (based on sensitivity), the vibration motors turn on at the selected intensity.
  - Releasing pressure immediately turns the motors off.

### Pin Summary:
- Encoders: (3,2) for intensity, (5,4) for sensitivity
- Force sensors: A0, A1
- LEDs: 9 (Intensity), 10 (Sensitivty)
- System button + LED: 7, 8
- Vibration output: 11
