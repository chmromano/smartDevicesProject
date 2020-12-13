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





void robot_project_maze(void);
void maze_right_turn(int speed);
void maze_left_turn(int speed);
void ready_maze(int speed);
void update_coordinates(int orientation, int *p_x, int *p_y);
void center_intersection(int speed);



//motor_turn(50, 15, 5986); gives a roughly 360 degree turn
//Premade function for a right turn with radius as parameter
void right_turn(int radius);
//Premade function for a left turn with radius as parameter
void left_turn(int radius);

void week3assignment2_chris(void);
void week4assignment1_chris(void);
void line_following_prototype(void);
void assignment2week4_chris(void);
void assignment1week5_chris(void);
void week3assignment3_chris(void);
void week5assignmnent3_chris(void);
/* [] END OF FILE */
