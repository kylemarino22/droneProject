#ifndef _pid_h_
#define _pid_h_



typedef struct pid {
        float lastError;
        float iGain = 0.0005;
        float dGain = 0.8; // 0.8
        float pGain = 0.7; // 0.7
        float offSet = 10;
        float direction;
        float lastD;
        float errorArray[20];
        float stabilized[5];

        int counter;
        int flag;
        float i;
} pid;

extern pid rollPID;
extern pid pitchPID;
extern pid yawPID;

float calculatePID(pid *pid, float angle, float input);


#endif
