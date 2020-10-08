// ColorBallSorter
// Copyright (C) 2020 Andrey Shmaykhel
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <Arduino.h>
#include <Servo.h>
#include <TroykaColorSensor.h>

enum Pin {
    bucket_servo_pin = 10,
    top_servo_pin = 9,
    bottom_servo_pin = 7,
    color_sensor_backlight_pin = 5
};

enum Color {
    blue = 0,
    red = 1,
    yellow = 2
};

Servo bucket_servo, top_servo, bottom_servo;

TroykaColorSensor color_sensor;

void setup() {
    // Initialize the servos
    bucket_servo.attach(bucket_servo_pin);
    top_servo.attach(top_servo_pin);
    bottom_servo.attach(bottom_servo_pin);

    // Initialize the color sensor
    color_sensor.begin();
    analogWrite(color_sensor_backlight_pin, 100);

    // Start Serial connection
    Serial.begin(115200);
}

// Identifies color from RGB
Color identify_color(RGB rgb) {
    // If blue component is greater than red and green components
    if (rgb.blue > rgb.red && rgb.blue > rgb.green) {
        return blue;
    }
    // If red component is much greater than green and greater than blue
    // component
    if (rgb.red - 50 > rgb.green && rgb.red > rgb.blue) {
        return red;
    }
    // If red and green components are almost the same
    return yellow;
}

void loop() {
    // Get color from the sensor
    RGB rgb = color_sensor.colorRead();
    Color color = identify_color(rgb);
    switch (color) {
    case blue:
        // TODO: Rotate to blue hole
        break;
    
    case red:
        // TODO: Rotate to red hole
        break;
    
    case yellow:
        // TODO: Rotate to yellow hole
        break;
    
    default:
        break;
    }
}
