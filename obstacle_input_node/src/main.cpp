#include <iostream>
#include "api_hal.h"
#include <termios.h>
#include <ros/ros.h>
#include "std_msgs/String.h"
#include <std_msgs/Int8.h>

ApiHAL hal;
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
 ros::init(argc, argv, "obstacle");
 ros::NodeHandle n;
 ros::Publisher detect = n.advertise<std_msgs::Int8>("obstacle", 1000);
 ros::Rate loop_rate(10);
 hal.initPin();
 unsigned int data = -1;
 while(getkey()!=27)
 {
  hal.gpioGetValue(SS2_PIN,&data);
  std_msgs::Int8 msg;
  msg.data = data;
  detect.publish(msg); 
  hal.setCursor(0,0);
  if (data==0){
   hal.print("co vat can");
  }else{hal.print("            ");}
  ros::spinOnce();
 }
}

