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
#include "giang.h"

#if 0
// Assignment 1 week 3 Robot run on floor
    
void zmain(void)
{
    printf("\nThis is assignment 1 week 3\n");
    
    motor_start();                  //start motor
    printf("\nSTART\n");
    motor_forward(0,0);             //not move yet
    
    vTaskDelay(3000);
    
    motor_forward(50,6500);
    motor_turn(50,0,1000);          //turn right sharp
    motor_forward(50,5000);
    motor_turn(55,0,1000);          //turn right sharp
    motor_forward(50,6000);
    motor_turn(70,0,1000);          //turn right inside
    motor_forward(50,2800);
    motor_turn(35,0,1000);          //turn right 45 degree
    motor_forward(50,3500);
    
    motor_forward(0,0);             //stop moving
    printf("\nGOAL\n");
    motor_stop();                   //motor stop

    while(true)
    {
        vTaskDelay(100); // sleep (in an infinite loop)
    }
 }   
#endif

#if 0
// Example
void zmain(void)
{
    printf("\nHello\n");
    
    motor_start();
    motor_forward(0,0);
    
    vTaskDelay(3000);
    
    motor_forward(100,2000);
    motor_turn(200,50,2000);
    motor_turn(50,200,2000);
    motor_backward(100,2000);
    
    motor_forward(0,0);
    
    motor_stop();

    while(true)
    {
        vTaskDelay(100); // sleep (in an infinite loop)
    }
 }   
#endif

/* [] END OF FILE */
