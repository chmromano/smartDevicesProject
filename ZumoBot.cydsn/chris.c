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
#include "alex.h"
#include "giang.h"



//Robot project maze solving
#define READY "Zumo03/ready"
#define START "Zumo03/start"
#define STOP "Zumo03/stop"
#define TIME "Zumo03/time"
#define POSITION "Zumo03/position"
#define SPEED 75
#define AVOID 13

//Function for 90 degree right turns
int maze_right_turn(int speed)
{
    struct sensors_ dig;
    reflectance_set_threshold(11000, 11000, 11000, 11000, 11000, 11000);
    reflectance_digital(&dig);
    
    while(dig.L3 == 1 || dig.L2 == 1 || dig.L1 == 1 || dig.R1 == 1)
    {
        reflectance_digital(&dig);
        SetMotors(0, 1, speed, speed, 0);
    }
    
    while(dig.L1 != 1 || dig.R1 != 1)
    {
        reflectance_digital(&dig);
        SetMotors(0, 1, speed, speed, 0);
    }
    
    return 0;
}

//Function for 90 degree left turns
int maze_left_turn(int speed)
{
    struct sensors_ dig;
    reflectance_set_threshold(11000, 11000, 11000, 11000, 11000, 11000);
    reflectance_digital(&dig);
    
    while( dig.L1 == 1 || dig.R1 == 1 || dig.R2 == 1 || dig.R3 == 1)
    {
        reflectance_digital(&dig);
        SetMotors(1, 0, speed, speed, 0);
    }
    
    while(dig.L1 != 1 || dig.R1 != 1)
    {
        reflectance_digital(&dig);
        SetMotors(1, 0, speed, speed, 0);
    }
    
    return 0;
}

//Function to move to first start line
int ready_maze(void)
{
    struct sensors_ dig;
    bool moving = true;
    reflectance_set_threshold(11000, 11000, 11000, 11000, 11000, 11000);
    reflectance_digital(&dig);
    
    motor_forward(SPEED,0);
    
    while(moving)
    {
        reflectance_digital(&dig);
        if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1)
        {
            moving = false;
        }
    }
    
    motor_forward(0,0);
    
    return 0;
}
//Function to move to maze (avoids printing coordinates for first line(0,-1))
int start_maze(void)
{
    struct sensors_ dig;
    bool moving = true;
    reflectance_set_threshold(11000, 11000, 11000, 11000, 11000, 11000);
    reflectance_digital(&dig);
    
    motor_forward(SPEED,0);

    while(moving == true)
    {
        reflectance_digital(&dig);
        if(dig.L3 == 0 && dig.L2 == 0 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 0 && dig.R3 == 0)
        {
            moving = false;
        }
    }
    
    return 0;
}

//Function with motor delay to center robot at intersections
int center_intersection(int speed)
{
    motor_forward(speed, 22500/speed);
    return 0;
}

//Function for maze solving project
void robot_project_maze(void){
    
    //Define dig struct
    struct sensors_ dig;
    
    //Defining necessary variables
    int orientation = 1;
    //Orientations; 0 = east, 1 = north, 2 = west, 3 = south
    //Init orientation = 1 = north
    int distance;
    int start_time;
    int finish_time;
    int elapsed_time;
    int position[2] = {0, 0};
    bool maze_not_finished = true;
    TickType_t time;
    
    //Starting necessary devices and setting reflectance threshold
    motor_start();
    Ultra_Start();
    reflectance_start();
    IR_Start();
    motor_forward(0,0);
    reflectance_set_threshold(11000, 11000, 11000, 11000, 11000, 11000);
    
    vTaskDelay(100);
    
    //Wait for button press to start program
    while(SW1_Read());
    BatteryLed_Write(1);
    vTaskDelay(500);
    BatteryLed_Write(0);
    
    //Robot moves forward until the first line and prints READY to mqtt
    ready_maze();
    print_mqtt(READY, "maze");
    
    //Robot waits for IR signal, prints START to mqtt and then starts moving
    IR_wait();
    time = xTaskGetTickCount();
    start_time = time;
    print_mqtt(START, "%d", start_time);
    start_maze();
    
    //Loop  for the maze solving algorithm
    while(maze_not_finished)
    {
        //Constantly refresh distance and reflectance values
        distance = Ultra_GetDistance();
        reflectance_digital(&dig);
        
        //Conditional for finish line
        if(position[0] == 0 && position[1] == 14)
        {
            while(dig.L1 == 1 || dig.R1 == 1)
            {
                reflectance_digital(&dig);
                
                if(dig.L3 == 0 && dig.L2 == 0 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 0 && dig.R3 == 0)
                {
                    motor_forward(SPEED,0);
                }
                else if(dig.L1 == 1 && dig.R1 == 0)
                {
                    motor_turn(0,SPEED,0);
                }
                else if(dig.L1 == 0 && dig.R1 == 1)
                {
                    motor_turn(SPEED,0,0);
                }
            }
            
            motor_forward(0, 0);
            maze_not_finished = false;
        }
        //Go straight
        else if(dig.L3 == 0 && dig.L2 == 0 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 0 && dig.R3 == 0)
        {
            motor_forward(SPEED,0);
        }
        //Turn left if straying right
        else if(dig.L1 == 1 && dig.R1 == 0)
        {
            motor_turn(0,SPEED,0);
        }
        //Turn right if straying left
        else if(dig.L1 == 0 && dig.R1 == 1)
        {
            motor_turn(SPEED,0,0);
        }
        //Conditional for intersections
        else if((dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1) || (dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1))
        {
            print_mqtt(POSITION, "%d %d", position[0], position[1]);
            
            //If orientation is north and obstacle is present
            if(orientation == 1 && distance <= AVOID)
            {
                center_intersection(SPEED);
                //Depending on robot coordinates turn left or right
                if(position[0] < 0)
                {
                    maze_right_turn(SPEED);
                    
                    //Check for obstacle after turn left or right
                    distance = Ultra_GetDistance();
                    //If obstacle is present do 180 degree turn left and update orientation to west
                    if(distance <= AVOID)
                    {
                        maze_left_turn(SPEED);
                        maze_left_turn(SPEED);
                        orientation = 2;
                    }
                    //Else update orientation to east
                    else
                    {
                        orientation = 0;
                    }
                }
                else if(position[0] >= 0)
                {
                    maze_left_turn(SPEED);
                    
                    //Check for obstacle after turn
                    distance = Ultra_GetDistance();
                    //If obstacle is present do 180 degree turn right and update orientation to east
                    if(distance <= AVOID)
                    {
                        maze_right_turn(SPEED);
                        maze_right_turn(SPEED);
                        orientation = 0;
                    }
                    //Else update orientation to west
                    else
                    {
                        orientation = 2;
                    }
                }
            }
            
            
            
            else if(orientation == 1 && position[0] > 0 && position[1] >= 11)
            {
                center_intersection(SPEED);
                maze_left_turn(SPEED);
                if(orientation == 3)
                {
                    orientation = 0;
                }
                else
                {
                    orientation++;
                }
            }
            else if(orientation == 1 && position[0] < 0 && position[1] >= 11)
            {
                center_intersection(SPEED);
                maze_right_turn(SPEED);
                if(orientation == 0)
                {
                    orientation = 3;
                }
                else
                {
                    orientation--;
                }
            }
            
            
            //Else if orientation is east
            else if(orientation == 0)
            {
                center_intersection(SPEED);
                
                //Turn left
                maze_left_turn(SPEED);
                
                //Check for obstacle
                distance = Ultra_GetDistance();
                //If there is obstacle and position is at right edge, turn left
                //and update orientation to west
                if(distance <= AVOID && position[0] == 3)
                {
                    maze_left_turn(SPEED);
                    orientation = 2;
                }
                //Else if there is obstacles turn right
                else if(distance <= AVOID)
                {
                    maze_right_turn(SPEED);
                }
                //Else update orientation to north
                else
                {
                    orientation = 1;
                }
            }
            //Else if orientatio is west
            else if(orientation == 2)
            {
                center_intersection(SPEED);
                
                //Turn right
                maze_right_turn(SPEED);
                
                //Check for obstacle
                distance = Ultra_GetDistance();
                //If there is obstacle and position is at left edge, turn right
                //and update orientation to east
                if(distance <= AVOID && position[0] == -3)
                {
                    maze_right_turn(SPEED);
                    orientation = 0;
                }
                //Else if there is obstacles turn left
                else if(distance <= AVOID)
                {
                    maze_left_turn(SPEED);
                }
                //Else update orientation to north
                else
                {
                    orientation = 1;
                }
            }
            //


            

            if(orientation == 0)
            {
                position[0]++;
            }
            else if(orientation == 1)
            {
                position[1]++;
            }
            else if(orientation == 2)
            {
                position[0]--;
            }
            else if(orientation == 3)
            {
                position[1]--;
            }
            
            //This lets the while loop run just once per intersection
            //(otherwise it would constantly try to update)
            while((dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1) || (dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1))
            {
                reflectance_digital(&dig);
            }
        }
    }
    
    
    time = xTaskGetTickCount();
    finish_time = time;
    print_mqtt(STOP, "%d", finish_time);
    elapsed_time = finish_time - start_time;
    print_mqtt(TIME, "%d", elapsed_time);
    
    while(true)
    {
        vTaskDelay(100);
    }
}















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
//function to test things
void zmain(void)
{
    #define READY "Zumo03/ready"
    #define START "Zumo03/start"
    #define POSITION "Zumo03/position"
    #define SPEED 25
    
    //Defining necessary variables
    struct sensors_ dig;
    int start = 1;
    int mid = 1;
    //Orientations; 0 = east, 1 = north, 2 = west, 3 = south
    //Init orientation = 1 = north
    int orientation = 1;
    int distance;
    int position[2] = {0, 0};
    TickType_t var = xTaskGetTickCount();
    
    //Starting necessary devices
    motor_start();
    Ultra_Start();
    reflectance_start();
    IR_Start();
    motor_forward(0,0);
    reflectance_set_threshold(11000, 11000, 11000, 11000, 11000, 11000);
    
    vTaskDelay(100);
    
    //Wait for button press to start program
    while(SW1_Read());
    BatteryLed_Write(1);
    vTaskDelay(500);
    BatteryLed_Write(0);
    
    //Robot moves forward until the first line
    motor_forward(SPEED,0);
    while(start == 1)
    {
        reflectance_digital(&dig);
        if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && start == 1)
        {
            motor_forward(0,0);
            start = 0;
            print_mqtt(READY, "maze");
        }
    }
    //Robot waits for IR signal then start moving
    IR_wait();
    var = xTaskGetTickCount();
    print_mqtt(START, "%d", var);
    motor_forward(SPEED,0);

    while(mid == 1)
    {
        reflectance_digital(&dig);
        if(dig.L3 == 0 && dig.L2 == 0 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 0 && dig.R3 == 0)
        {
            mid = 0;
        }
    }
    
    while(true){
        
        distance = Ultra_GetDistance();
        reflectance_digital(&dig);

        if((dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1) || (dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1))
        {
            motor_forward(SPEED, 900);
            while(dig.L3 == 1 || dig.L2 == 1 || dig.L1 == 1 || dig.R1 == 1)
            {
                reflectance_digital(&dig);
                SetMotors(0,1, SPEED, SPEED, 0);
            }
            
            while(dig.L1 != 1 || dig.R1 != 1)
            {
                reflectance_digital(&dig);
                SetMotors(0,1, SPEED, SPEED, 0);
            }
            
            motor_forward(0,0);
        }
    }   
}
#endif













#if 0
//Function for week 5 assignment 3

#define TOPIC "Zumo03/Chris/Lap"

void zmain(void){
    
    //Defining necessary variables
    struct sensors_ dig;
    int start = 1, middle = 1, finish = 1;
    TickType_t var = xTaskGetTickCount();
    int start_time, finish_time, elapsed_time;
    
    reflectance_set_threshold(11000, 11000, 11000, 11000, 11000, 11000);
    
    //Starting necessary devices
    motor_start();
    reflectance_start();
    IR_Start();
    motor_forward(0,0);
    
    //Wait for button press to start program
    while(SW1_Read());
    BatteryLed_Write(1);
    vTaskDelay(1000);
    BatteryLed_Write(0);
    
    //Robot moves forward until the first line
    motor_forward(25,0);
    while(start == 1){
        reflectance_digital(&dig);
        if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && start == 1){
            motor_forward(0,0);
            start = 0;
        }
    }
    
    //Robot waits for IR signal then start moving
    IR_wait();
    var = xTaskGetTickCount();
    start_time = var;
    motor_forward(25,0);
    
    //Robot counts lines until it has to stop
    while(middle == 1){
        reflectance_digital(&dig);
        if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
            middle = 0;
            while(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
                reflectance_digital(&dig);
            }
        }
    }
    
    while(finish == 1){
        reflectance_digital(&dig);
        var = xTaskGetTickCount();
        if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
            finish = 0;
            motor_forward(0,0);
            motor_stop();
            finish_time = var;
            elapsed_time = finish_time - start_time;
            print_mqtt(TOPIC, "Elapsed time: %d ms", elapsed_time);
        }
    }
    
    while(true){
        vTaskDelay(100); // sleep (in an infinite loop)
    }
}
#endif



#if 0
//Function for week 5 assignment 2
    
#define TURN "Zumo03/Chris/Turn"
    
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
            
            //Took this from Alex
            srand(xTaskGetTickCount());
            int n = rand() % 2;
            if (n==0) {
                print_mqtt(TURN, "Left");
                SetMotors(1,0, 25, 25, 1050);
            }else{
                print_mqtt(TURN, "Right");
                SetMotors(0,1, 25, 25, 1050);
            }
        }
    }
}
#endif



#if 0
//Function for assignment 1 week 5
//SW1_Read() = 1 when button not pressed
//SW1_Read() = 0 when button is pressed

#define TOPIC "Zumo03/Chris/Button"

void zmain(void){
    
    TickType_t var = xTaskGetTickCount();
    int first_press = 0, last_press = 0, time;

    while(true){
        var = xTaskGetTickCount();
        if(SW1_Read() == 0){
            last_press = var;
            time = last_press - first_press;
            print_mqtt(TOPIC, "Elapsed time: %d ms", time);
            first_press = last_press;
            while(SW1_Read() == 0);
        }
    }
}
#endif



#if 0
//Function for  assignment 2 week 4
    
#define STOP 2

void zmain(void){
    
    //Defining necessary variables
    struct sensors_ dig;
    int start = 1;
    int count = 0;
    
    reflectance_set_threshold(11000, 11000, 11000, 11000, 11000, 11000);
    
    //Starting necessary devices
    motor_start();
    reflectance_start();
    IR_Start();
    motor_forward(0,0);
    
    //Wait for button press to start program
    while(SW1_Read());
    BatteryLed_Write(1);
    vTaskDelay(1000);
    BatteryLed_Write(0);
    
    //Robot moves forward until the first line
    motor_forward(50,0);
    while(start == 1){
        reflectance_digital(&dig);
        if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && start == 1){
            motor_forward(0,0);
            start = 0;
        }
    }
    
    //Robot waits for IR signal then start moving
    IR_wait();
    motor_forward(100,0);
    
    //Simple line following algorithm using only central sensors
    while(count < STOP){
        reflectance_digital(&dig);
        if(dig.L3 == 0 && dig.L2 == 0 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 0 && dig.R3 == 0){
            motor_forward(100,0);
        }else if((dig.L3 == 1 || dig.L2 == 1) && dig.R2 == 0 && dig.R3 == 0){
            motor_turn(0,100,0);
        }else if((dig.R3 == 1 || dig.R2 == 1) && dig.L2 == 0 && dig.L3 == 0){
            motor_turn(100,0,0);
        }
        
        if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
            count++;
            while((dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1) && count < STOP){
                reflectance_digital(&dig);
            }
        }
    }
    
    //Stop robot
    motor_forward(0,0);
    motor_stop();
    while(true)
    {
        vTaskDelay(100); // sleep (in an infinite loop)
    }
}
#endif

#if 0
//Prototype function for  project 2 (line following)
    
#define STOP 3

void zmain(void){
    
    //Defining necessary variables
    struct sensors_ dig;
    int start = 1;
    int count = 0;
    
    reflectance_set_threshold(11000, 11000, 11000, 11000, 11000, 11000);
    
    //Starting necessary devices
    motor_start();
    reflectance_start();
    IR_Start();
    motor_forward(0,0);
    
    //Wait for button press to start program
    while(SW1_Read());
    BatteryLed_Write(1);
    vTaskDelay(1000);
    BatteryLed_Write(0);
    
    //Robot moves forward until the first line
    motor_forward(50,0);
    while(start == 1){
        reflectance_digital(&dig);
        if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && start == 1){
            motor_forward(0,0);
            start = 0;
        }
    }
    
    //Robot waits for IR signal then start moving
    IR_wait();
    motor_forward(100,0);
    
    //Simple line following algorithm using only central sensors
    while(count < STOP){
        reflectance_digital(&dig);
        if((dig.L3 == 0 && dig.L2 == 0 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 0 && dig.R3 == 0) || (dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1)){
            motor_forward(100,0);
        }else if((dig.L3 == 1 || dig.L2 == 1) && dig.R2 == 0 && dig.R3 == 0){
            motor_turn(0,100,0);
            if(dig.L1 == 0 && dig.R1 == 0){
                printf("\nMiss\n");
            }
        }else if((dig.R3 == 1 || dig.R2 == 1) && dig.L2 == 0 && dig.L3 == 0){
            motor_turn(100,0,0);
            if(dig.L1 == 0 && dig.R1 == 0){
                printf("\nMiss\n");
            }
        }
        
        if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
            count++;
            while((dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1) && count < STOP){
                reflectance_digital(&dig);
            }
        }
    }
    
    //Stop robot
    motor_forward(0,0);
    motor_stop();
    while(true)
    {
        vTaskDelay(100); // sleep (in an infinite loop)
    }
}
#endif

#if 0
//Function for week 4 assignment 1

#define STOP 5

void zmain(void){
    
    //Defining necessary variables
    struct sensors_ dig;
    int count = 0;
    int start = 1;
    
    reflectance_set_threshold(11000, 11000, 11000, 11000, 11000, 11000);
    
    //Starting necessary devices
    motor_start();
    reflectance_start();
    IR_Start();
    motor_forward(0,0);
    
    //Wait for button press to start program
    while(SW1_Read());
    BatteryLed_Write(1);
    vTaskDelay(1000);
    BatteryLed_Write(0);
    
    //Robot moves forward until the first line
    motor_forward(25,0);
    while(start == 1){
        reflectance_digital(&dig);
        if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && start == 1){
            motor_forward(0,0);
            start = 0;
        }
    }
    
    //Robot waits for IR signal then start moving
    IR_wait();
    motor_forward(25,0);
    
    //Robot counts lines until it has to stop
    while(count < STOP){
        reflectance_digital(&dig);
        if(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
            count++;
            while(dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1){
                reflectance_digital(&dig);
            }
        }
    }
    
    //Stop robot
    motor_forward(0,0);
    motor_stop();
    while(true)
    {
        vTaskDelay(100); // sleep (in an infinite loop)
    }
}
#endif

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
void zmain(void){
    motor_stop();
    while(true)
    {
        vTaskDelay(100);
    }
}
#endif
/* [] END OF FILE */
