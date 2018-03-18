#include <stdio.h>
#include <wiringPiI2C.h>
#include <pigpio.h>
#include "BMP280.h"
#include "LM303.h"
#include "gyro.h"
#include "motor.h"
#include "kalman.h"
#include "fusion.h"
#include "communication.h"
#include "Serial.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

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

void error(const char *msg) {
        perror(msg); exit(0);
}
int i;
int j;
int amirisdumb = 1;
// int throttle = 1000;
int main(int argc,char *argv[])
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

        if(initializeInput[0] == 'y') {
                printf("initializing\n");
                motor_initialize();
                delay(5000);
        }
        else{
                printf("not\n");
                gpioInitialise();
                //gpioSetMode(22, PI_INPUT);
        }
        serialBegin();

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
        int input;

        // kalman_t pitchKalman;
        // kalman_t rollKalman;
        //
        // beginKalman(&pitchKalman);
        // beginKalman(&rollKalman);
        //
        // //  motor_update (1000, 1100, 1000, 1100);
        // gyroCalibrate();

        //  motor_update (1000, 1200, 1000, 1200);
        // serialPin output;
        // serialPin throttleOut;
        // beginPin(&output, -90, 90, 17);
        // beginPin(&throttleOut, -40, 100, 27);

        int prevThrottle;
        int prevRoll;
        int prevPitch;
        int finishRead = 0;
        while (1) {


                if(kbhit()) {
                        input = getchar();

                        if(input == 27) {
                                getchar();
                                input = getchar();
                                if(input == 65) {
                                        roll += 1;
                                        printf("up %d\n", roll);
                                }
                                if(input == 66) {
                                        roll -= 1;
                                        printf("down %d\n", roll);
                                }
                        }
                        else if(input == 111) {
                                throttle += 5;
                                printf("throttle up %d\n", throttle);
                        }
                        else if(input == 108) {
                                throttle -= 5;
                                printf("throttle down %d\n", throttle);
                        }
                        else{
                                printf("asdf = %d\n", input);
                        }
                        // send(1,(throttle - 1000) / 5);
                        // delay(10);
                        // send(2,roll + 90);
                        // delay(10);
                }
                /*
                 |==========|=========|=========|
                 |  Action  | Command |  Range  |
                 |==========|=========|=========|
                 | Throttle |   1     |1000-2000| 1000 + 5 unit increments
                 | Roll Deg |   2     | -90 - 90|
                 | PitchDeg |   3     | -90 - 90|
                 | Yaw Deg  |   4     | -90 - 90|
                 |==========|=========|=========|
                 */

                int portno =        3000;
                char *host =        "192.168.1.15";
                char *message_fmt = "GET /data HTTP/1.0\r\n\r\n";

                struct hostent *server;
                struct sockaddr_in serv_addr;
                int sockfd, bytes, sent, received, total;
                char response[4096];
                char message[1024];

                //if (argc < 3) { puts("Parameters: <apikey> <command>"); exit(0); }

                /* fill in the parameters */
                sprintf(message,message_fmt,argv[1],argv[2]);
                // printf("Request:\n%s\n",message);

                /* create the socket */
                sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd < 0) error("ERROR opening socket");

                /* lookup the ip address */
                server = gethostbyname(host);
                if (server == NULL) error("ERROR, no such host");

                /* fill in the structure */
                memset(&serv_addr,0,sizeof(serv_addr));
                serv_addr.sin_family = AF_INET;
                serv_addr.sin_port = htons(portno);
                memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

                /* connect the socket */
                if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
                        error("ERROR connecting");

                /* send the request */
                total = strlen(message);
                sent = 0;
                do {
                        bytes = write(sockfd,message+sent,total-sent);
                        if (bytes < 0)
                                error("ERROR writing message to socket");
                        if (bytes == 0)
                                break;
                        sent+=bytes;
                } while (sent < total);

                /* receive the response */
                memset(response,0,sizeof(response));
                total = sizeof(response)-1;
                received = 0;
                do {
                        bytes = read(sockfd,response+received,total-received);
                        if (bytes < 0)
                                error("ERROR reading response from socket");
                        if (bytes == 0)
                                break;
                        received+=bytes;
                } while (received < total);

                if (received == total)
                        error("ERROR storing complete response from socket");

                /* close the socket */
                close(sockfd);

                /* process response */
                // printf("Response:\n%s\n",response);
                int valid = decodeData(response);
                // printf("valilid %d\n",valid);
                // if(valid == 2) {
                //
                //         sendSerial('q',0);
                // }
                // printf("prevThrottle = %d\n", prevThrottle);
                // printf("throttle = %d\n", throttle);
                // if(gpioRead(17) == 1) {
                //         finishRead = 0;
                // }
                // if(gpioRead(17) == 0 && finishRead == 0) {
                //printf("on\n");

                // int throttle = 1200;
                // int roll = 0;
                // int pitch = 5;
                // int yaw = 37;

                uint8_t sendDataArray[4];

                sendDataArray[0] = (uint8_t)((throttle - 1000)/5);
                sendDataArray[1] = roll + 90;
                sendDataArray[2] = pitch + 90;
                sendDataArray[3] = (uint8_t)((255.0f/360.0f)*yaw);

                delay(10);
                uint8_t fail;
                do {
                        fail = 0;

                        uint8_t commandArray[4] = {'a', 'b', 'c', 'd'};


                        for(i = 0; i < 4; i++) {
                                sendSerial(commandArray[i], sendDataArray[i]);
                        }
                        int* output;
                        output = serialDecoder(testSerial());

                        printf("\r                                   \r");
                        for(i = 0; i<4; i++) {

                                if(sendDataArray[i] != output[i]) {
                                        printf("\r                                   \r");
                                        printf("SendData: %d\tOutput: %d\r", sendDataArray[i], output[i]);
                                        fail = 1;
                                }
                                printf("%d\t", output[i]);
                        }

                        if(fail) {
                                delay(100);
                        }
                } while (fail);
                //sendDatav2(10,10);
                //sendDbtav2(10,10);
                //sendData();
                //printf("a\n");

                // sendSerial('a', (throttle - 1000)/5);
                // testSerial();
                // sendSerial('b', (roll+90));
                // testSerial();
                // sendSerial('c', (pitch +90));
                // testSerial();
                // sendSerial('d', (yaw+180));
                // testSerial();
                // printf("%d\n", amirisdumb);
                // if(amirisdumb < 10) {
                //         amirisdumb++;
                // }
                // else{
                //         amirisdumb = 0;
                // }
                //sendDatav2(10,(throttle - 1000) / 5);

                // while (gpioRead(17) == 0) ;
                // sendDatav2(2,(roll + 90));
                // while (gpioRead(17) == 0) ;
                // sendDatav2(3,(pitch + 90));
                // gpioWrite(27,1); // Start 0
                // delay(1);
                // gpioWrite(27,0); // 1
                // //printf("off\n");
                // delay(1);
                // gpioWrite(27,1); // 2
                // //printf("on\n");
                // delay(1);
                // gpioWrite(27,1); // 3
                // //printf("off\n");
                // delay(1);
                // gpioWrite(27,0); // 4
                // //printf("on\n");
                // delay(1);
                // gpioWrite(27,1); // 5
                //
                // delay(1);
                // gpioWrite(27,0);
                // finishRead = 1;



                // if(!(prevThrottle == throttle)) {
                //         printf("t\n");
                //         sendData(1,(throttle - 1000) / 5);
                //
                //         prevThrottle = throttle;
                // }
                // // printf("a = %d\n", roll);
                // // printf("b = %d\n", prevRoll);
                // if(!(prevRoll == roll)) {
                //         sendData(2,(roll + 90));
                //         printf("r\n");
                //         prevRoll = roll;
                // }
                // if(!(prevPitch == pitch)) {
                //         sendData(3,pitch + 90);
                //         printf("p\n");
                //         prevPitch = pitch;
                // }
                // int startPos = 0;
                // for(i = 0; i < 250; i++) {
                //         if(response[i] == '@') {
                //                 startPos = i;
                //                 break;
                //         }
                // }
                //
                // char temp[4];
                //
                // for(i = 0; i < 4; i++) {
                //         temp[i] = response[startPos+1+i];
                // }
                // startPos+=5;
                // throttle = atoi(temp);
                //
                // for(i = 0; i<4; i++) {
                //         temp[i] = '0';
                // }
                //
                // int endPos = 0;
                // for(i =0; i<5; i++) {
                //         if(response[startPos + i+1] == '_') {
                //                 endPos = startPos + i;
                //                 break;
                //         }
                // }
                // for(i = (endPos - startPos); i > 0; i--) {
                //         temp[i] = response[startPos + i];
                // }
                // printf("%c\n", response[endPos]);
                // printf("%c\n", response[startPos]);
                // printf("%d\n", endPos - startPos);
                // printf("%s\n", temp);



                // if(response[startPos+1] == '-') {
                //         startPos++;
                //         temp[1] = '-';
                // }
                // for(i = 0; i < 2; i++) {
                //         temp[i+] = response[startPos+1+i];
                // }
                // printf("here %s\n",temp);

                //printf("throttle = %d\n", recievedData[0]);


                delay(10);



                // printf("roll = %f\n", pitchKalman.angle);
                // delay(100);
        }


        gpioTerminate();
        printf("were done\n");
        return 0;
}
