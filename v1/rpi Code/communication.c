#include "communication.h"
int throttle;
int roll;
int yaw;
int pitch;
int startPos;
int decodeData(char response []){
        int i;
        startPos = 0;
        for(i = 100; i < 500; i++) {
                if(response[i] == '@') {
                        startPos = i;
                        break;
                }
        }

        char temp[4];
        //printf("%c\n", response[startPos + 1]);
        if(response[startPos + 1] == 'n') {
                return -1;
        }
        else if(response[startPos + 1] == 't') {
                return 1;
        }
        else if(response[startPos + 1] == 'f') {
                return 0;
        }
        else if(response[startPos + 1] == 'q') {
                return 2;
        }


        // for(i = 0; i < 4; i++) {
        //         temp[i] = response[startPos+1+i];
        // }
        // startPos+=5;
        // throttle = atoi(temp);
        throttle = decodeByte(response);
        roll = decodeByte(response);
        pitch = decodeByte(response);
        yaw = decodeByte(response);

        // printf("%d\n", decodeByte(response));
        // printf("%d\n", decodeByte(response));
        // printf("%d\n", decodeByte(response));
        // printf("%d\n", decodeByte(response));
}

int decodeByte(char response []){
        int i;
        short negative = 1;
        char temp[5];
        for(i = 0; i<5; i++) {
                temp[i] = '0';
        }

        int endPos = 0;
        for(i =0; i<5; i++) {
                if(response[startPos + i+1] == '_') {
                        endPos = startPos + i;
                        break;
                }
        }
        int length = endPos - startPos;
        for(i = length; i > 0; i--) {
                if(i == 1 && response[startPos+1] == '-') {
                        negative = -1;
                        break;
                }
                temp[(5 - length) + i] = response[startPos + i];
        }
        startPos += length+1;

        return atoi(temp) * negative;
}
