/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Motor.h"
#include "Ultra.h"
#include "Nunchuk.h"
#include "Reflectance.h"
#include "Gyro.h"
#include "Accel_magnet.h"
#include "LSM303D.h"
#include "IR.h"
#include "Beep.h"
#include "mqtt_sender.h"
#include <time.h>
#include <sys/time.h>
#include "serial1.h"
#include <unistd.h>
#include <stdlib.h>

// tank turn
void tank_turn(int8 speed, uint32 delay);
//left 90 degree tank turn
void tank_leftturn90();
//right 90 degree tank turn
void tank_rightturn90();
//left 270 degree tank turn
void tank_leftturn270();
//right 270 degree tank turn
void tank_rightturn270();
//180 degree tank turn
void tank_turn180();
// tank moving little back
void motor_littleback();
// tank randomtrunturn
void tank_randomturn();
// tank moving foward at speed of 50
void motor_forward50();


/* [] END OF FILE */
