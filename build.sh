gcc main.c BMP280.c LM303.c gyro.c motor.c -lwiringPi -lpthread -lpigpio -lm  -o run

sudo ./run
