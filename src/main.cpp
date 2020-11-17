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
    bucket_servo_pin = 9,
    top_servo_pin = A3,
    bottom_servo_pin = A0,
    rotation_servo_pin = 6,
    color_sensor_backlight_pin = 5  
};

enum Color {
    blue = 0,
    red = 1,
    yellow = 2,
    white = 3
};

enum RotationAngle {
    input_hole = 3,
    init_angle = 20,
    blue_hole = 60,
    red_hole = 95,
    yellow_hole = 135
};

enum BottomAngle {
    bottom_low = 12,
    bottom_output = 17,
    bottom_intermediate = 30,
    bottom_high = 70
};

enum TopAngle {
    top_low = 60,
    top_intermediate = 20,
    top_high = 0
};

enum BucketAngle {
    bucket_grab = 0,
    bucket_intermediate = 60,
    bucket_release = 120
};

Servo bucket_servo, top_servo, bottom_servo, rotation_servo;

TroykaColorSensor color_sensor;

void (*reset) (void) = 0;

void setup() {
    // Initialize the servos
    rotation_servo.write(init_angle);
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
    Serial.print("R: ");
    Serial.println(rgb.red);
    Serial.print("G: ");
    Serial.println(rgb.green);
    Serial.print("B: ");
    Serial.println(rgb.blue);
    if (rgb.red > 180 && rgb.green > 180 && rgb.blue > 180) {
        Serial.println("White");
        return white;
    }
    if (rgb.blue > rgb.red && rgb.blue > rgb.green) {
        Serial.println("Blue");
        return blue;
    }
    // If red component is much greater than green and greater than blue
    // component
    if (rgb.red - 50 > rgb.green && rgb.red > rgb.blue) {
        Serial.println("Red");
        return red;
    }
    // If red and green components are almost the same
    Serial.println("Yellow");
    return yellow;
}

// Rotates servo smoothly
void smooth_servo_rotate(Servo servo, int angle, float rate) {
    int initial_angle = servo.read();
    int diff = angle - initial_angle;
    if (diff == 0) {
        return;
    }
    if (diff > 0) {
        for(float a = initial_angle; a <= angle; a += rate) {
            servo.write((int) a);
            delay(10);
        }
        return;
    }
    for(float a = initial_angle; a >= angle; a -= rate) {
        servo.write((int) a);
        delay(10);
    }
    return;
}

void loop() {
    // Init position
    smooth_servo_rotate(bottom_servo, bottom_high, 0.5);
    smooth_servo_rotate(top_servo, top_low, 0.5);
    smooth_servo_rotate(rotation_servo, init_angle, 0.5);
    smooth_servo_rotate(rotation_servo, input_hole, 0.5);
    // Lower
    smooth_servo_rotate(bottom_servo, bottom_intermediate, 0.5);
    smooth_servo_rotate(bucket_servo, bucket_intermediate, 0.5);
    smooth_servo_rotate(top_servo, top_low, 0.5);
    // Grab
    smooth_servo_rotate(bottom_servo, bottom_low, 0.5);
    smooth_servo_rotate(bucket_servo, bucket_grab, 0.5);
    // Raise
    smooth_servo_rotate(bottom_servo, bottom_intermediate, 0.5);
    smooth_servo_rotate(top_servo, top_intermediate, 0.5);
    smooth_servo_rotate(bottom_servo, bottom_high, 0.5);
    smooth_servo_rotate(top_servo, top_high, 0.5);
    // Get color from the sensor
    RGB rgb = color_sensor.colorRead();
    Color color = identify_color(rgb);
    switch (color) {
    case blue:
        // Rotate to blue hole
        smooth_servo_rotate(rotation_servo, blue_hole, 0.5);
        smooth_servo_rotate(top_servo, top_intermediate, 0.5);
        smooth_servo_rotate(bottom_servo, bottom_output, 0.5);
        smooth_servo_rotate(top_servo, top_low, 0.5);
        smooth_servo_rotate(bucket_servo, bucket_release, 0.5);
        break;
    
    case red:
        // Rotate to red hole
        smooth_servo_rotate(rotation_servo, red_hole, 0.5);
        smooth_servo_rotate(top_servo, top_intermediate, 0.5);
        smooth_servo_rotate(bottom_servo, bottom_output, 0.5);
        smooth_servo_rotate(top_servo, top_low, 0.5);
        smooth_servo_rotate(bucket_servo, bucket_release, 0.5);
        break;
    
    case yellow:
        // Rotate to yellow hole
        smooth_servo_rotate(rotation_servo, yellow_hole, 0.5);
        smooth_servo_rotate(top_servo, top_intermediate, 0.5);
        smooth_servo_rotate(bottom_servo, bottom_output, 0.5);
        smooth_servo_rotate(top_servo, top_low, 0.5);
        smooth_servo_rotate(bucket_servo, bucket_release, 0.5);
        break;
    
    default:
        break;
    }
}
