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
    bottom_servo_pin = 6,
    rotation_servo_pin = 7,
    color_sensor_backlight_pin = 5
};

enum Color {
    blue = 0,
    red = 1,
    yellow = 2
};

enum RotationAngle {
    input_hole = 0,
    blue_hole = 60,
    red_hole = 90,
    yellow_hole = 120
};

enum BottomAngle {
    bottom_low = 45,
    bottom_high = 80
};

enum TopAngle {
    top_low = 100,
    top_high = 30
};

enum BucketAngle {
    bucket_low = 80,
    bucket_high = 150
};

Servo bucket_servo, top_servo, bottom_servo, rotation_servo;

TroykaColorSensor color_sensor;

void setup() {
    // Initialize the servos
    bucket_servo.attach(bucket_servo_pin);
    top_servo.attach(top_servo_pin);
    bottom_servo.attach(bottom_servo_pin);
    rotation_servo.attach(rotation_servo_pin);

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
    // Raise
    top_servo.write(top_low);
    bottom_servo.write(bottom_high);
    delay(500);
    // Rotate to input hole
    rotation_servo.write(input_hole);
    delay(2000);
    // Lower
    bucket_servo.write(bucket_high);
    top_servo.write(top_low);
    bottom_servo.write(bottom_low);
    delay(1000);
    // Grab the ball
    bucket_servo.write(bucket_low);
    delay(1000);
    // Raise
    top_servo.write(top_high);
    bottom_servo.write(bottom_high);
    delay(1000);
    // Get color from the sensor
    RGB rgb = color_sensor.colorRead();
    Color color = identify_color(rgb);
    switch (color) {
    case blue:
        // Rotate to blue hole
        rotation_servo.write(blue_hole);
        break;
    
    case red:
        // Rotate to red hole
        rotation_servo.write(red_hole);
        break;
    
    case yellow:
        // Rotate to yellow hole
        rotation_servo.write(yellow_hole);
        break;
    
    default:
        break;
    }
    delay(2000);
    // Lower
    top_servo.write(top_low);
    bottom_servo.write(bottom_low);
    delay(1000);
    // Release the ball
    bucket_servo.write(bucket_low);
    delay(1000);
}
