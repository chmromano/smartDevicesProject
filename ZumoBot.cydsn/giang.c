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


// Line follower
// Apply codes from assignmnet 2 week 4 line following and adjust with more conditions
// Bonus off track is made by adjust the reflectance threshold and motor turn variables
// All the printf are to check of the codes work and they are all inactive

#define READY "Zumo03/ready"
#define START "Zumo03/start"
#define STOP "Zumo03/stop"
#define TIME "Zumo03/time"
#define MISS "Zumo03/miss"
#define LINE "Zumo03/line"

#define STOPP 3

void robot_following_line (void)
{
    struct sensors_ dig;
    reflectance_set_threshold(15000, 12000, 12000, 12000, 12000, 15000);
    reflectance_start();
    IR_Start();
                                                  
    int slow_speed = 70;                    
    int max_speed = 250;                    
    int fast_turn_speed_1 = 120;           
    int fast_turn_speed_2 = 60;           
    int slow_turn_speed_1 = 50;                    
    int slow_turn_speed_2 = 10;
    int reverse_speed = 250;
    bool reverse = false;                       //miss line, robot reverse
    bool race = false;                          //robot is racing
    bool finish = false;                        //robot at the 2nd finished line
    
    TickType_t start_time = xTaskGetTickCount();
    TickType_t stop_time = xTaskGetTickCount();
    int total_time = 0;
    TickType_t miss_time = xTaskGetTickCount();
    TickType_t back_to_line_time = xTaskGetTickCount();
    
    //printf("Starting\n");

    while (true)
    {
        int count = 0;
        
        motor_start();                  
        motor_forward(0,0);             
    
        //Press button for robot to move to start line
        while(SW1_Read() == 1)
        {
            vTaskDelay(10);
        }
        BatteryLed_Write(1);            
        BatteryLed_Write(0);            
        reflectance_digital(&dig);      
        motor_forward(slow_speed,0);
        
        //loop until line counting = numbers of stop
        //follow the line when 2 inside sensor on black area 
        //stop when count equal stop means all sensors on black area
        
        //when R1, R2 is 1, go straight
        //when R2 is 1, turn right until return to 0
        //when L2 is 1, turn left until return to 0
        
        while(count < STOPP)             
        {
            reflectance_digital(&dig);
            
            //check when robot on black horizontal line
            if (reverse == false && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1)
            {
                count++;
                //printf("count: %d\n", count);
                
                //robot at start line and wait for IR signal to start racing
                if (count == 1)
                {
                    motor_forward(0,0);
                    if (finish == false)
                    {
                        print_mqtt(READY, "line");
                    }
                    IR_wait();
                    start_time = xTaskGetTickCount(); 
                    race = true;
                    motor_forward(max_speed,0);
                    print_mqtt(START, "%d", start_time);
                }
                
                //slow down when moving through the horizontal line so there is no error
                while((dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1) && count < STOPP)
                {
                    if (count > 1)
                    {
                        motor_forward(slow_speed, 0);
                    }
                    reflectance_digital(&dig);
                }
            }
            
            //start the loop from beginning when not in the race
            //to avoid it turns before going to the start line
            else if (race == false)
            {
                motor_forward(slow_speed,0);
                continue;
            }
            
            //when to go forward
            else if (dig.R1 == 1 && dig.L1 == 1 && dig.R2 == 0 && dig.R3 == 0 && dig.L2 == 0 && dig.L3 == 0 )
            {
                if (reverse == true)
                {
                    reverse = false;
                    back_to_line_time = xTaskGetTickCount();
                    print_mqtt(LINE, "%d", back_to_line_time);
                }
                motor_forward(max_speed,0);
            }
            
            //when to turn fast
            else if ((dig.R2 == 1 || dig.R3 == 1) && dig.L1 == 1 && dig.L2 == 0 && dig.L3 == 0)
            {
                motor_turn(fast_turn_speed_1,fast_turn_speed_2,0);
                //printf("turn right\n");
            }
            
            //when to turn fast
            else if ((dig.L2 == 1 || dig.L3 == 1) && dig.R1 == 1 && dig.R2 == 0 && dig.R3 == 0)
            {
                motor_turn(fast_turn_speed_2,fast_turn_speed_1,0);
                //printf("turn left\n");
            }
            
            //when to turn slow
            else if ((dig.R2 == 1 || dig.R3 == 1) && dig.L2 == 0 && dig.L3 == 0)
            {
                motor_turn(slow_turn_speed_1,slow_turn_speed_2,0);
                //printf("turn right\n");
            }
            
            //when to turn slow
            else if ((dig.L2 == 1 || dig.L3 == 1) && dig.R2 == 0 && dig.R3 == 0)
            {
                motor_turn(slow_turn_speed_2,slow_turn_speed_1,0);
                //printf("turn left\n");
            }
            
            //when off track
            else if (dig.R1 == 0 && dig.L1 == 0)
            {
                if (race == false)
                {
                    motor_forward(slow_speed,0);
                }
                else if (reverse == false)
                {
                    miss_time = xTaskGetTickCount();
                    print_mqtt(MISS, "%d", miss_time);
                }
                motor_backward(reverse_speed,0);
                reverse = true;
            }
        }
        
        motor_forward(0,0);
        stop_time = xTaskGetTickCount();
        print_mqtt(STOP, "%d", stop_time);
        
        motor_stop();
        total_time = stop_time - start_time;
        print_mqtt(TIME, "%d", total_time);
        //printf("%d.%d\n", total_time/1000, total_time%1000);
        finish = true;
    }
    while(true)
    {
        vTaskDelay(100);
    }
}










#if 0
   
// Assignment 1 week 5 - Measuring time between 2 button presses

#define BUTTON "Zumo03/Giang/Button"

//when button is pressed 1st time, button press = true, press time = time
//if 1st button press true, 2nd button pressed, 2nd time - 1st time = measured time

void zmain(void)
{
    TickType_t previous_time = 0;       //set to 0 to avoid print start time of 1st time press button
    printf("Press button!\n");
    bool pressed = false;
    while(true) {
        // Press button
        while(SW1_Read() == 0) {
            pressed = true;
            vTaskDelay(10);
        }
        // Release button
        if (SW1_Read() == 1 && pressed )
        {
            pressed = false;
            TickType_t current_time = xTaskGetTickCount();
            int time = current_time - previous_time;
            if (previous_time > 0)              //only print time when button presse >= 2 times
            {
                print_mqtt(BUTTON, "%d.%d s\n", time/1000, time%1000);
            }
            previous_time = current_time;
        }
    }
    while(true)
    {
        vTaskDelay(100);
    }
}
#endif

#if 0
    
// Assignemnt 2 week 4 Robot sensor following line

#define STOP 2

void zmain(void)
{
    struct sensors_ dig;             //call function sensor
    int count = 0;
    
    reflectance_set_threshold(10000, 10000, 10000, 10000, 10000, 10000);       //set sensor threshold all to 10000
    
    reflectance_start();            //start reflectance sensor
    IR_Start();                     //start IR receiver
    motor_start();                  //start motor
    motor_forward(0,0);             //not move yet
    
    while(SW1_Read());
    BatteryLed_Write(1);            //led on
    vTaskDelay(2000);               //delay 2s
    BatteryLed_Write(0);            //led off
    
    reflectance_digital(&dig);      //Making Reflectance Sensor's period to digital
    motor_forward(70,0);           //Moving forward
    printf("Start\n");
    
    //loop until line counting = numbers of stop
    //follow the line when 2 inside sensor on black area 
    //stop when count equal stop means all sensors on black area
    
    //when R1, R2 is 1, go straight
    //when R2 is 1, turn right until return to 0
    //when L2 is 1, turn left until return to 0
    
    while(count < STOP)             
    {
        reflectance_digital(&dig);
        
        if (dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1)
        {
            count++;
            
            while((dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1) && count < STOP)
            {
                reflectance_digital(&dig);
            }
        }
        
        if (dig.R1 == 1 && dig.L1 == 1 && dig.R2 == 0 && dig.R3 == 0 && dig.L2 == 0 && dig.L3 == 0 )
        {
            motor_forward(70,0);
            //printf("forward\n");   (just to check if it works, printf here can make the robot confused)
        }
        
        else if ((dig.R2 == 1 || dig.R3 == 1) && dig.L2 == 0 && dig.L3 == 0)
        {
            motor_turn(30,0,0);
            //printf("turn right\n");
        }
        
        else if ((dig.L2 == 1 || dig.L3 == 1) && dig.R2 == 0 && dig.R3 == 0)
        {
            motor_turn(0,35,0);
            //printf("turn left\n");
        }
    }
    
    motor_forward(0,0);         //robot stops moving
    printf("End\n");
    motor_stop();
    
     while(true)
    {
        vTaskDelay(100);
    }
}

#endif

#if 0
    
// Assignemnt 1 week 4 Robot sensor go straight line

#define STOP 5

void zmain(void)
{
    struct sensors_ dig;             //call function sensor
    int count = 0;
    
    reflectance_set_threshold(10000, 10000, 10000, 10000, 10000, 10000);       //set sensor threshold all to 10000
    
    reflectance_start();            //start reflectance sensor
    IR_Start();                     //start IR receiver
    motor_start();                  //start motor
    motor_forward(0,0);             //not move yet
    
    while(SW1_Read());
    BatteryLed_Write(1);            //led on
    vTaskDelay(2000);               //delay 2s
    BatteryLed_Write(0);            //led off
    
    reflectance_digital(&dig);      //Making Reflectance Sensor's period to digital
    motor_forward(70,0);           //Moving forward
    printf("Start\n");
    
    //loop until line counting = numbers of stop
    //continue moving forward and counting when all sensors on black area (1)
    
    while(count < STOP)             
    {
        reflectance_digital(&dig);
        if (dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1)
        {
            count++;
            
            while((dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1) && count < STOP)
            {
                reflectance_digital(&dig);
            }
        }
    }
    
    motor_forward(0,0);         //robot stops moving
    printf("End\n");
    motor_stop();
    
     while(true)
    {
        vTaskDelay(100);
    }
}

#endif
/*    
// Testing another way to edit codes in personal file (Assignemnt 1 week 4)

#define STOP 5

void senor_robot_straight_map(void)
{
    struct sensors_ dig;             //call function sensor
    int count = 0;
    
    reflectance_set_threshold(10000, 10000, 10000, 10000, 10000, 10000);       //set sensor threshold all to 10000
    
    reflectance_start();            //start reflectance sensor
    IR_Start();                     //start IR receiver
    motor_start();                  //start motor
    motor_forward(0,0);             //not move yet
    
    while(SW1_Read());
    BatteryLed_Write(1);            //led on
    vTaskDelay(2000);               //delay 2s
    BatteryLed_Write(0);            //led off
    
    reflectance_digital(&dig);      //Making Reflectance Sensor's period to digital
    motor_forward(70,0);           //Moving forward
    printf("Start\n");
    
    //loop until line counting = numbers of stop
    //continue moving forward and counting when all sensors on black area (1)
    
    while(count < STOP)             
    {
        reflectance_digital(&dig);
        if (dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1)
        {
            count++;
            
            while((dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1) && count < STOP)
            {
                reflectance_digital(&dig);
            }
        }
    }
    
    motor_forward(0,0);         //robot stops moving
    printf("End\n");
    motor_stop();
    
     while(true)
    {
        vTaskDelay(100);
    }
}
*/

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


/* [] END OF FILE */
