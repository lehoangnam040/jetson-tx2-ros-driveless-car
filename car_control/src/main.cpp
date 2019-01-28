#include <iostream>
#include "api_hal.h"
#include <termios.h>
#include <ros/ros.h>
#include "std_msgs/String.h"
#include <std_msgs/Int8.h>
#include "api_i2c_pwm.h"
ApiHAL hal;
PCA9685 *pca9685 = new PCA9685();
int getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    /* set the terminal to raw mode */
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    /* read a character from the stdin stream without blocking */
    /*   returns EOF (-1) if no character is available */
    character = fgetc(stdin);

    /* restore the original terminal attributes */
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}
int main(int argc, char **argv)
{
 ros::init(argc, argv, "car_control");
 ros::NodeHandle n;
 api_pwm_pca9685_init( pca9685 );
 ros::Rate loop_rate(10);
 hal.initPin();

 while(getkey()!=27)
 {
  unsigned int btn1= -1;
  unsigned int btn4= -1;
  unsigned int btn3= -1;
  hal.gpioGetValue(SW1_PIN, &btn1);
  hal.gpioGetValue(SW4_PIN, &btn4);
  hal.gpioGetValue(SW3_PIN, &btn3);
  if(btn1==HIGH)
  {
   double speed = 20.0;
   api_set_FORWARD_control(pca9685,speed);
   printf("forward\n");   
  }
  if(btn4== HIGH)
  {
   double steer = 20.0;
   api_set_STEERING_control(pca9685, steer);
   printf("turn right 20\n");
  } 
  if(btn3==HIGH)
  {
   double steer = -20.0;
   api_set_STEERING_control(pca9685, steer);
   printf("turn left 20\n");
  }
  ros::spinOnce();
 }
}
