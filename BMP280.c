#include "BMP280.h"
#include<stdio.h>
#include<stdlib.h>
#include<wiringPiI2C.h>
#include<math.h>


void BMP_begin(){
   if(!wiringPiI2CSetup(BMP_Address)){
      printf("fail to init i2c!");

   }
   BMP_fd = wiringPiI2CSetup(BMP_Address);

   if(wiringPiI2CReadReg8(BMP_fd,0xD0) != 0x58){
      printf("BMP not connected %x", wiringPiI2CReadReg8(BMP_fd,0xD0));
   }

   wiringPiI2CWriteReg8(BMP_fd, 0xF4, 0x3F);
}

void readData(){
   uint8_t status;
   uint8_t bit_position;
   uint8_t bit_status;
   uint8_t mask;
   bit_position = 3;

   do {
      status = wiringPiI2CReadReg8(BMP_fd,0xF3);
      mask =  1 << bit_position;
      bit_status = status & mask;
   } while(bit_status == 1);

   _BMP_calib_Data.adcT = wiringPiI2CReadReg8(BMP_fd, 0xFA);
   _BMP_calib_Data.adcT <<= 8;
   _BMP_calib_Data.adcT |= wiringPiI2CReadReg8(BMP_fd, 0xFB);
   _BMP_calib_Data.adcT <<= 8;
   _BMP_calib_Data.adcT |= wiringPiI2CReadReg8(BMP_fd, 0xFC);
   _BMP_calib_Data.adcT >>= 4;

   _BMP_calib_Data.adcP = wiringPiI2CReadReg8(BMP_fd, 0xF7);
   _BMP_calib_Data.adcP <<= 8;
   _BMP_calib_Data.adcP |= wiringPiI2CReadReg8(BMP_fd, 0xF8);
   _BMP_calib_Data.adcP <<= 8;
   _BMP_calib_Data.adcP |= wiringPiI2CReadReg8(BMP_fd, 0xF9);
   _BMP_calib_Data.adcP >>= 4;

   _BMP_calib_Data.t1 = wiringPiI2CReadReg16(BMP_fd, 0x88);
   _BMP_calib_Data.t2 = wiringPiI2CReadReg16(BMP_fd, 0x8A);
   _BMP_calib_Data.t3 = wiringPiI2CReadReg16(BMP_fd, 0x8C);

   _BMP_calib_Data.p1 = wiringPiI2CReadReg16(BMP_fd, 0x8E);
   _BMP_calib_Data.p2 = wiringPiI2CReadReg16(BMP_fd, 0x90);
   _BMP_calib_Data.p3 = wiringPiI2CReadReg16(BMP_fd, 0x92);
   _BMP_calib_Data.p4 = wiringPiI2CReadReg16(BMP_fd, 0x94);
   _BMP_calib_Data.p5 = wiringPiI2CReadReg16(BMP_fd, 0x96);
   _BMP_calib_Data.p6 = wiringPiI2CReadReg16(BMP_fd, 0x98);
   _BMP_calib_Data.p7 = wiringPiI2CReadReg16(BMP_fd, 0x9A);
   _BMP_calib_Data.p8 = wiringPiI2CReadReg16(BMP_fd, 0x9C);
   _BMP_calib_Data.p9 = wiringPiI2CReadReg16(BMP_fd, 0x9E);

}

float readTemperature(){

   readData();

   uint32_t var1 = ((((_BMP_calib_Data.adcT >> 3) - ((int32_t)_BMP_calib_Data.t1 << 1)))
    * ((int32_t)_BMP_calib_Data.t2)) >> 11;
   uint32_t var2 = (((((_BMP_calib_Data.adcT >> 4) - ((int32_t)_BMP_calib_Data.t1)) *
   ((_BMP_calib_Data.adcT >> 4) - ((int32_t)_BMP_calib_Data.t1))) >> 12) * ((int32_t)_BMP_calib_Data.t3)) >> 14;
   BMP_tfine = var1 + var2;
   float T = ((BMP_tfine * 5) + 128) >> 8;
   T /= 100;
   return T;
}

float readPressure(){


   readTemperature();

   int64_t p, Pvar1, Pvar2;

   Pvar1 = ((int64_t)BMP_tfine / 2) - 64000;
   Pvar2 = (Pvar1 * Pvar1 * (int64_t)_BMP_calib_Data.p6)>>15;
   Pvar2 = Pvar2 + ((Pvar1 * (int64_t)_BMP_calib_Data.p5) * 2);
   Pvar2 = (Pvar2/4) + (((int64_t)_BMP_calib_Data.p4) << 16);
   Pvar1 = (((((int64_t)_BMP_calib_Data.p3) * Pvar1) * Pvar1) >> 19) + ((((int64_t)_BMP_calib_Data.p2) * Pvar1)) >> 19;
   Pvar1 = ((Pvar1 >>15) + 1) * ((int64_t)_BMP_calib_Data.p1);

   if(Pvar1 == 0){
   return 0;
   }

   p = 1048576 - _BMP_calib_Data.adcP;
   p = ((p - (Pvar2 >> 12)) * 6250) / Pvar1;
   Pvar1 = (((int64_t)_BMP_calib_Data.p9) * p *p) >> 31;
   Pvar2 = (((int64_t)_BMP_calib_Data.p8) * p) >> 15;
   p = p + ((Pvar1 + Pvar2 + ((int64_t)_BMP_calib_Data.p7)) / 16);
   BMP_finalP = (p);
   return BMP_finalP;

}

float readAltitude(float seaLevelPa){

   readPressure();

   float altitude;
   float pressure = BMP_finalP;
   pressure /= 100;
   altitude = 44330 * (1.0 - pow(pressure / seaLevelPa, 0.1903));

   return altitude;

}


