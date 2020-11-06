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
#endif

#if 1
//function to test turns
//motor_turn(50, 15, 5986); gives a roughly 360 degree turn
void zmain(void){
    motor_start();
    right_turn(90);
    left_turn(180);
    motor_stop();
    
    while(true)
    {
        vTaskDelay(100); // sleep (in an infinite loop)
    }
}
#endif
/* [] END OF FILE */
