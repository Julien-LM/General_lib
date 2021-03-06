/* 
 * File:   i2c.h
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on December 11, 2017, 2:42 PM
 */

#ifndef I2C_H
#define	I2C_H

#include "structures.h"

// Wait for buffer empty
void I2C_Master_Wait()
{
  while (SSP1STATbits.R_nW || (SSP1CON2 & 0x1F));
}

// Wait for ack from device
unsigned char I2C_Master_Wait_Ack(void) {
    if(SSP1CON2bits.ACKSTAT) {
        LED_RED = 1;
        return(1);
    }
    LED_RED = 0;
    return(0);
}

void I2C_Master_Start()
{
    SSP1CON2bits.SEN = 1;
    while(SSP1CON2bits.SEN);
}

void I2C_Master_RepeatedStart()
{
    SSP1CON2bits.RSEN = 1;
    while(SSP1CON2bits.RSEN);
}

void I2C_Master_Stop()
{
    SSP1CON2bits.PEN = 1;
    while(SSP1CON2bits.PEN);
}

void I2C_Master_Ack(void) {
    SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKEN = 1;
    while(SSP1CON2bits.ACKEN);
}

void I2C_Master_nAck(void) {
    SSP1CON2bits.ACKDT = 1;
    SSP1CON2bits.ACKEN = 1;
    while(SSP1CON2bits.ACKEN);
}

void I2C_Master_Write_control_byte(unsigned char control_code, unsigned char chip_select, unsigned char read_write_bit)
{
  SSP1BUF = (0xF0 & (control_code << 4)) + (0x0E & (chip_select << 1)) + read_write_bit;
  while(SSP1STATbits.BF);
  I2C_Master_Wait();
  I2C_Master_Wait_Ack();
}

void I2C_Master_Write(unsigned char data)
{
  SSP1BUF = data;
  while(SSP1STATbits.BF);
  I2C_Master_Wait();
  I2C_Master_Wait_Ack();
}

unsigned short I2C_Master_Read(void)
{
  unsigned short temp;
  SSP1CON2bits.RCEN = 1;
  while(!SSP1STATbits.BF);
  temp = SSP1BUF;
  I2C_Master_Wait();
  return temp;
}


void acknowledge_polling(unsigned char control_code, 
        unsigned char chip_select) {
    do {
        I2C_Master_Start();                     // Start condition
        I2C_Master_Write_control_byte(control_code, chip_select, 
                WRITE_BIT);  
    } while (I2C_Master_Wait_Ack());
}

#endif	/* I2C_H */

