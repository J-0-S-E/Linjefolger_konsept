#include <Arduino.h>
#include <QTRSensors.h>

int sensors[] = {1, 2, 3, 4, 5, 6, 7, 8};   // Sensor pins
int weights[] = {4, 3, 2, 1, -1, -2, -3, -4};    // Vekt for hver sensor
int motor_R_F = 9;  // Motor_Right_Forward
int motor_L_F = 10; // Motor_Left_Forward

void setup() {
    for (int i = 0; i < 8; i++) {
        pinMode(sensors[i], INPUT);
    }
    pinMode(motor_R_F, OUTPUT);
    pinMode(motor_L_F, OUTPUT);
}

int sensorweight = 0;
int minimum = 500;

void loop() {
    sensorweight = 0; // Nullstill vekten før ny lesing

    // Les sensorene og beregn vekten
    for (int i = 0; i < 8; i++) {
        if (analogRead(sensors[i]) > minimum) {
            sensorweight += weights[i];
        }
    }

    // Nullstill sensorvekten hvis ingen sensorer er aktive
    bool noSensorActive = true;
    for (int i = 0; i < 8; i++) {
        if (analogRead(sensors[i]) > minimum) {
            noSensorActive = false;
            break;
        }
    }
    if (noSensorActive) {
        sensorweight = 100;
    }

    // Bruk switch for motorstyring basert på sensorvekten
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
