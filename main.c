#include<stdio.h>
#include<wiringPiI2C.h>
#include<pigpio.h>
#include "BMP280.h"
#include "LM303.h"
#include "gyro.h"
#include "motor.h"
#include<stdint.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

float generalDirection[5];
float gdFinal;
int8_t direction = 1;


int kbhit(void)
{
struct termios oldt, newt;
int ch;
int oldf;

tcgetattr(STDIN_FILENO, &oldt);
newt = oldt;
newt.c_lflag &= ~(ICANON | ECHO);
tcsetattr(STDIN_FILENO, TCSANOW, &newt);
oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

ch = getchar();

tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
fcntl(STDIN_FILENO, F_SETFL, oldf);

if(ch != EOF)
{
  ungetc(ch, stdin);
  return 1;
}

return 0;
} //kbhit


int main()
{

  int i;
  int j;
  float distance;
  char initializeInput[10];
  BMP_begin();
  LM303_accel_begin();
  delay(20);
  LM303_mag_begin();
  gyro_begin();
  printf("initialize?\n");
  scanf("%s", initializeInput);

  if(initializeInput[0] == 'y'){
    printf("initializing\n");
    motor_initialize();
    delay(5000);
  }
  else{
    printf("not\n");
    gpioInitialise();
  }


  float asdf[20];
  int asdf1[20];
  int asdf2[20];
  int8_t headingFlip;
  float temp;
  float headingArray[40];
  float rollArray[40];
  float pitchArray[40];
  float accelx[20];
  float accely[20];
  float accelz[20];
  float magx[40];
  float magy[40];
  float magz[40];
  int counter = 0;
  float finalRoll;
  float finalheading;
  float finalPitch;
  int xchangeValue;
  int prevXchangeValue = 0;
  int ychangeValue;
  int motorPower;
  float pid_max_roll = 400;
  float pid_error_temp;
  float pid_i_mem_roll;
  float pid_output_roll;
  float pid_output_pitch;
  float pid_last_roll_d_error;
  float pid_p_gain_roll = 1.5;
  float pid_i_gain_roll = 0.02;
  float pid_d_gain_roll = 7.0;
  float motor1 = 1200;
  float motor2 = 1200;
  int throttle = 1200;
  float xPrevDifference;
  float xIntegral;
  float tempXOutput;
  float yPrevDifference;
  float tempYOutput;
  float pid_i_output;
  float pid_d_output;
  float pid_p_output;
  pid_i_mem_roll = 0;
  pid_last_roll_d_error = 0;
  int magStep = 0;
  float faccelx;
  float faccely;
  float faccelz;
  float yaw = 0;
  int input;

  motor_update (1300, 1100, 1300, 1100);



  while (1){

    // LM303_accel_read();
    // LM303_mag_read();

    // float P_output = (_fusion_data.roll - 0) * P_gain;
    // float I_output = prev_integral + (_fusion_data.roll - 0) * I_gain;
    // prev_integral = I_output;
    //
    // float D_output = ((_fusion_data.roll - 0) -
    //                  (prev_derivative_accel - prev_derivative_reciever)) * D_gain;
    //
    // float PID_output = P_output + I_output + D_output;




     for(i = 0; i < 20; i++){
      LM303_accel_read();

      accelx[i] = _accel_data.x;
      accely[i] = _accel_data.y;
      accelz[i] = _accel_data.z;

      }



    _accel_data.x = meanTrimmer(accelx);
    // _accel_data.y = meanTrimmer(accely);
    // _accel_data.z = meanTrimmer(accelz);

    // // if(magStep == 0){
    // //   for(i = 0; i < 24; i++){
    // //     LM303_mag_read();
    // //     magx[i] = _mag_data.x;
    // //     magy[i] = _mag_data.y;
    // //     magz[i] = _mag_data.z;
    // //   }
    // //
    // //   faccelx = _accel_data.x;
    // //   faccely = _accel_data.y;
    // //   faccelz = _accel_data.z;
    // // }
    //
    //
    //
    //
    //
    //
    //
    // // _mag_data.x = meanTrimmer(magx);
    // //
    // // _mag_data.y = meanTrimmer(magy);
    // // printf("\ncalculated mag y = %f\n\n", _mag_data.y);
    // // printf("mag y = %f\n", _mag_data.y);
    // // _mag_data.z = magMeanTrimmer(magz);
    // // printf("mag z = %f\n", _mag_data.z);
    //
    //
    //
    //
    //
    //
    // // printf("roll%f\n", _fusion_data.roll);
    // // printf("pitch= %f\n", _fusion_data.pitch );
    // //
    // // printf("heading = %f\n", _fusion_data.heading);
    // //
    // // printf("corrected heading = %f\n", headingCorrect(_fusion_data.heading));
    //
    // // finalRoll = rollCorrect(test1);
    //
    // // pid_error_temp = _fusion_data.roll - 0;
    // // pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
    // // if(pid_i_mem_roll > pid_max_roll){
    // //   pid_i_mem_roll = pid_max_roll;
    // // }
    // // else if (pid_i_mem_roll < pid_max_roll * -1){
    // //   pid_i_mem_roll = pid_max_roll * -1;
    // // }
    // //
    // // pid_output_roll = pid_p_gain_roll * pid_error_temp + pid_i_mem_roll
    // //  + pid_d_gain_roll * (pid_error_temp - pid_last_roll_d_error);
    // //
    // // if(pid_output_roll > pid_max_roll){
    // //   pid_output_roll = pid_max_roll;
    // // }
    // // else if (pid_output_roll < pid_max_roll * -1){
    // //   pid_output_roll = pid_max_roll * -1;
    // // }
    // //
    // // pid_last_roll_d_error = pid_error_temp;
    // //
    // // motor_update(0,throttle + (int)pid_output_roll , 0, throttle - (int)pid_output_roll);
    //
    // // if(_fusion_data.roll > 30 && _fusion_data.roll < 40){
    // //   pid_output_roll = 75;
    // // }
    // // else if(_fusion_data.roll > 40){
    // //   pid_output_roll = 100;
    // // }
    // // else if(_fusion_data.roll < -30 && _fusion_data.roll > -40){
    // //   pid_output_roll = -75;
    // // }
    // // else if(_fusion_data.roll <-20  && _fusion_data.roll > -30){
    // //   pid_output_roll = -50;
    // // }
    // // else if(_fusion_data.roll >20  && _fusion_data.roll <30){
    // //   pid_output_roll = 50;
    // // }
    // // else if(_fusion_data.roll <0  && _fusion_data.roll > -20){
    // //   pid_output_roll = -25;
    // // }
    // // else if(_fusion_data.roll >0  && _fusion_data.roll <20){
    // //   pid_output_roll = 25;
    // // }
    // // else{
    // //   pid_output_roll = -100;
    // // }
    // //pitch calculation
    // // if( _accel_data.x > 4 &&  _accel_data.x < 6){
    // //   pid_output_pitch = 75;
    // // }
    // // else if( _accel_data.x > 6){
    // //   pid_output_pitch = 100;
    // // }
    // // else if( _accel_data.x < -30 &&  _accel_data.x > -40){
    // //   pid_output_pitch = -75;
    // // }
    // // else if( _accel_data.x <-20  &&  _accel_data.x > -30){
    // //   pid_output_pitch = -50;
    // // }
    // // else if( _accel_data.x >20  &&  _accel_data.x <30){
    // //   pid_output_pitch = 50;
    // // }
    // // else if( _accel_data.x <0  &&  _accel_data.x > -20){
    // //   pid_output_pitch = -25;
    // // }
    // // else if( _accel_data.x >0  &&  _accel_data.x <20){
    // //   pid_output_pitch = 25;
    // // }
    // // else{
    // //   pid_output_pitch = -100;
    // // }
    //
    // tempXOutput = (_accel_data.x - 0) * -7.5;
    //
    // if((_accel_data.x - 0) < 0.5 && (_accel_data.x - 0) > - 0.5){
    //   //pid_output_roll = tempYOutput;
    //   //do nothing
    // }
    // else{
    //
    //   if((xPrevDifference - tempXOutput) < 5
    //   &&(xPrevDifference - tempXOutput) > -5)
    //   {
    //     if(_accel_data.x > 0){
    //       pid_output_pitch -= 0;
    //     }
    //     else{
    //       pid_output_pitch +=0;
    //     }
    //   }
    //   else{
    //     pid_output_pitch = tempXOutput;
    //   }
    //   xPrevDifference = tempXOutput;
    //
    //   if(pid_output_pitch > 200){
    //     pid_output_pitch = 200;
    //   }
    //   if(pid_output_pitch < -200){
    //     pid_output_pitch = -200;
    //   }
    // }
    //
    //
    //
    // tempYOutput = (_accel_data.y - 0) * -7.5;
    //
    // if((_accel_data.y - 0) < 0.5 && (_accel_data.y - 0) > - 0.5){
    //   //pid_output_roll = tempYOutput;
    //   //do nothing
    // }
    // else{
    //   if((yPrevDifference - tempYOutput) < 5
    //   &&(yPrevDifference - tempYOutput) > -5)
    //   {
    //     if(_accel_data.y > 0){
    //       pid_output_roll -= 0;
    //     }
    //     else{
    //       pid_output_roll +=0;
    //     }
    //   }
    //   else{
    //     pid_output_roll = tempYOutput;
    //   }
    //   yPrevDifference = tempYOutput;
    //
    //   if(pid_output_roll> 200){
    //     pid_output_roll = 200;
    //   }
    //   if(pid_output_roll < -200){
    //     pid_output_roll = -200;
    //   }
    // }
    //
    // // printf("data %f\n", _accel_data.x);
    // // if(magStep == 2){
    // //   _accel_data.x = faccelx;
    // //   _accel_data.y = faccely;
    // //   _accel_data.z = faccelz;
    // //   _mag_data.x = average(magx);
    // //   _mag_data.y = average(magy);
    // //   _mag_data.z = average(magz);
    // //   fusionCalc();
    // //   // printf("heading %f\n", _fusion_data.heading);
    // //
    // // }
    //
    //
    // magStep++;
    // if(magStep == 3){
    //   magStep = 0;
    // }
    //
    //
    // if(((_fusion_data.heading - 0) >5 )|| (((180 - _fusion_data.heading) - 0) > 5)){
    //
    //
    //
    //   if((_fusion_data.heading - 0) < ((180 - _fusion_data.heading) - 0)){
    //     yaw += 10;
    //   }
    //   else{
    //     yaw -= 10;
    //   }
    // }
    //
    // //printf("heading %f\n",_fusion_data.heading);
    //
    // if(yaw > 100){
    //   yaw = 100;
    // }
    // if(yaw < -100){
    //   yaw = -100;
    // }


    // if(kbhit()){
    //   input = getchar();
    //
    //   if(input == 27){
    //     getchar();
    //     input = getchar();
    //     if(input == 65){
    //       throttle += 10;
    //       printf("up %d\n", throttle);
    //     }
    //     if(input == 66){
    //       throttle -= 10;
    //       printf("down %d\n", throttle);
    //     }
    //   }
    //
    // }



    // motor_update(
    // ((throttle - (int)pid_output_pitch)* 1.005),
    // (throttle + (int)pid_output_roll) * 0.95 - (int)yaw,
    // (throttle + (int)pid_output_pitch),
    // (throttle - (int)pid_output_roll) - (int)yaw);

    // //
    // printf("motor 1 %f\n",throttle + pid_output_pitch);
    // printf("motor 2 %f\n",throttle - pid_output_pitch);

    pid_error_temp = (0 - _accel_data.x);

    pid_p_output = pid_error_temp * 5.4;

    pid_i_mem_roll += pid_error_temp;
    pid_d_output = (pid_error_temp - pid_last_roll_d_error) * 8;
    pid_i_output = pid_i_mem_roll * 0.02;

    if(pid_d_output > 50){
      pid_d_output = 50;
    }
    if(pid_d_output < -50){
      pid_d_output = -50;
    }
    if(pid_i_output > 50){
      pid_i_output = 50;
    }
    if(pid_i_output < -50){
      pid_i_output = -50;
    }
    if(pid_error_temp < 0.5 && pid_error_temp > -0.5){
      pid_d_output = 0;
    }
    pid_output_pitch = pid_p_output + pid_i_output + pid_d_output;


    pid_last_roll_d_error = pid_error_temp;

    // motor_update(
    //   (throttle + (int)pid_output_pitch) + 20,
    //   (0),
    //   (throttle - (int)pid_output_pitch),
    //   (0)
    // );
    motor_update((1300 - (int)pid_output_pitch ) *1.008 ,1000,1300 + (int)pid_output_pitch,1000);
    // printf("accel %f\n", _accel_data.x);


    //
    // if (_accel_data.x > 0.1 && _accel_data.x < -0.1) {
    //
    //   _accel_data.x = 0;
    // }
    // xchangeValue = 0;
    //
    // // if (_accel_data.y > 0.1 && _accel_data.y < -0.1) {
    // //
    // //   _accel_data.y = 0;
    // // }
    // // ychangeValue = 0;
    //
    // if (_motor_data.m2 >= 1100 && _motor_data.m2 <= 1400) {
    //
    //   if (_motor_data.m4 >= 1300 && _motor_data.m4 <= 1500) {
    //
    //     xchangeValue = rollCorrect(test1) ;
    //     motor_change (0,(xchangeValue) ,0, (xchangeValue * -1));
    //   }
    //   else{
    //     if(_motor_data.m4 >= 1500){
    //       _motor_data.m4 = 1500;
    //       printf("greater than 1500\n");
    //     }
    //     else{
    //       _motor_data.m4 = 1300;
    //     }
    //
    //     xchangeValue = rollCorrect(test1) ;
    //     motor_change (0, xchangeValue ,0, 0);
    //   }
    //
    // }
    // else{
    //   if(_motor_data.m2 >= 1400){
    //     _motor_data.m2 = 1400;
    //   }
    //   else{
    //     _motor_data.m2 = 1100;
    //   }
    //
    //
    //   xchangeValue = rollCorrect(test1);
    //   motor_change (0, 0 ,0, (xchangeValue * -1));
    // }









      //
      // printf("heading %f\n", headingCorrect(test3));
      // printf("heading %f\n", headingCorrect(test3));
      // printf("roll = %f\n", rollCorrect(test1));
      // printf("pitch = %f\n\n", pitchCorrect(test2));
      //
      // printf("=========\n\n");
      //
      // printf("motor1 = %d\n", _motor_data.m2);
      // printf("motor2 = %d\n", _motor_data.m4);



  }


  gpioTerminate();
  printf("were done\n");
  return 0;
}
