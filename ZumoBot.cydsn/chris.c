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

/*========================================

GENERAL INFORMATION AND DOCUMENTATION

motor_turn(50, 15, 5986); gives a roughly 360 degree turn

==========================================*/
#include "chris.h"

//Premade function for a right turn with radius as parameter
void right_turn(int radius){
    int delay_value = (5986 * radius)/360;
    motor_turn(50, 15, delay_value);
}

//Premade function for a left turn with radius as parameter
void left_turn(int radius){
    int delay_value = (5986 * radius)/360;
    motor_turn(15, 50, delay_value);
}

#if 0
//Function for week 3 assignment 2
void zmain(void)
{
    //Starting motor and ultrasonic sensor
    motor_start();
    Ultra_Start();
    //Allows first distance reading to be !=0
    vTaskDelay(100);
    
    //Allows functions to be executed indefinitely
    while(true){
        int distance = Ultra_GetDistance();
        motor_forward(25,0);
        //Performs required actions when distance is short enough
        if(distance < 10){
            motor_forward(0, 250);
            motor_backward(25,1400);
            motor_forward(0, 250);
            left_turn(120);
        }
        //Prints to console only if distance changes to prevent console spamming
        if(distance !=  Ultra_GetDistance()){
            printf("Distance = %d\n", distance);
        }
    }
}
#endif

#if 0
//function to test things

void zmain(void)
{
    motor_start();
    while(true){
        motor_turn(10,0,0);
    }
    motor_stop();
    
    while(true)
    {
        vTaskDelay(100); // sleep (in an infinite loop)
    }
}
#endif
/* [] END OF FILE */
