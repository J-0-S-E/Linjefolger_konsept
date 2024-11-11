#include <Arduino.h>
#include <QTRSensors.h>

QTRSensors qtr;  // Opprett QTR-sensorobjekt
uint8_t sensor_pins[] = {1, 2, 3, 4, 5, 6, 7, 8}; // Sensor pins
int weights[] = {4, 3, 2, 1, -1, -2, -3, -4}; // Vekt for hver sensor
int motor_R_F = 9;  // Motor_Right_Forward
int motor_L_F = 10; // Motor_Left_Forward

uint16_t sensor_values[8]; // Array for sensorverdier, må være uint16_t
int sensorweight = 0;
int minimum = 500;

void setup() {
    // Initialiser QTR-sensorene
    qtr.setTypeAnalog();
    qtr.setSensorPins(sensor_pins, 8); // Sett opp sensorene

    pinMode(motor_R_F, OUTPUT);
    pinMode(motor_L_F, OUTPUT);

    // Kalibrer sensorene
    for (int i = 0; i < 250; i++) {
        qtr.calibrate();
        delay(20);
    }
}

void loop() {
    sensorweight = 0; // Nullstill vekten før ny lesing

    // Les kalibrerte sensorverdier
    qtr.readCalibrated(sensor_values);

    // Beregn vekten basert på aktive sensorer
    for (int i = 0; i < 8; i++) {
        if (sensor_values[i] > minimum) {
            sensorweight += weights[i];
        }
    }

    // Nullstill sensorvekten hvis ingen sensorer er aktive
    bool noSensorActive = true;
    for (int i = 0; i < 8; i++) {
        if (sensor_values[i] > minimum) {
            noSensorActive = false;
            break;
        }
    }
    if (noSensorActive) {
        sensorweight = 100;
    }

    // Motorstyring basert på sensorvekt
    switch (sensorweight) {
        case 100: // Ingen sensorer aktive
            digitalWrite(motor_R_F, LOW);
            digitalWrite(motor_L_F, LOW);
            break;

        case 4: // Sving til høyre
            digitalWrite(motor_R_F, LOW);
            digitalWrite(motor_L_F, HIGH);
            break;

        case 3:
            analogWrite(motor_R_F, 255 / 4);
            digitalWrite(motor_L_F, HIGH);
            break;

        case 2:
            analogWrite(motor_R_F, 255 / 3);
            digitalWrite(motor_L_F, HIGH);
            break;

        case 1:
            analogWrite(motor_R_F, 255 / 2);
            digitalWrite(motor_L_F, HIGH);
            break;

        case 0: // Rett frem
            digitalWrite(motor_R_F, HIGH);
            digitalWrite(motor_L_F, HIGH);
            break;

        case -1:
            digitalWrite(motor_R_F, HIGH);
            analogWrite(motor_L_F, 255 / 2);
            break;

        case -2:
            digitalWrite(motor_R_F, HIGH);
            analogWrite(motor_L_F, 255 / 3);
            break;

        case -3:
            digitalWrite(motor_R_F, HIGH);
            analogWrite(motor_L_F, 255 / 4);
            break;

        case -4: // Sving til venstre
            digitalWrite(motor_R_F, HIGH);
            digitalWrite(motor_L_F, LOW);
            break;
    }
}
