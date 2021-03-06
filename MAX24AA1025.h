/* 
 * File:   24AA1025.h
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 * 
 * Requirements: 
 *
 * Created on February 21, 2018, 5:33 PM
 */

#ifndef MAX24AA1025_H
#define	MAX24AA1025_H

#include "structures.h"

void I2C_Master_Write_control_byte_spe(unsigned char control_code, 
        unsigned char block_select, unsigned char chip_select, 
        unsigned char read_write_bit) {
    
    unsigned char chip_sel = 0;
    
    chip_sel = (0x01 & (block_select << 1)) + (0x03 & chip_select); 
    I2C_Master_Write_control_byte(control_code, chip_sel, 
            read_write_bit); 
}

void EEPROM_write_byte(unsigned char data, unsigned long address) {
    I2C_Master_Start();                     // Start condition
    I2C_Master_Write_control_byte_spe(CONTROL_CODE_24AA1025,
            ((0x10000 & address) >> 0x10), CHIP_SEL_N0_24AA, WRITE_BIT); 
    I2C_Master_Write((0xFF00 & address) >> 0x08);   // Write address high byte
    I2C_Master_Write(0xFF & address);               // Write address low byte
    I2C_Master_Write(data);
    I2C_Master_Stop();                              // Stop condition
    acknowledge_polling(CONTROL_CODE_24AA1025, CHIP_SEL_N0_24AA);
}

void EEPROM_write_page(unsigned char data[], unsigned long address) {
    int i=0;
    I2C_Master_Start();                     // Start condition
    I2C_Master_Write_control_byte_spe(CONTROL_CODE_24AA1025,
            ((0x10000 & address) >> 0x10), CHIP_SEL_N0_24AA, WRITE_BIT); 
    I2C_Master_Write((0xFF00 & address) >> 0x08);   // Write address high byte
    I2C_Master_Write(0xFF & address);               // Write address low byte
    for(i=0; i<MEM_PAGE_SIZE; i++) {
        I2C_Master_Write(data[i]);        
    }
    I2C_Master_Stop();                              // Stop condition
    acknowledge_polling(CONTROL_CODE_24AA1025, CHIP_SEL_N0_24AA);
}

unsigned char EEPROM_read_current_address(void) {
    unsigned char read_data;

    I2C_Master_Start();                     // Start condition
    I2C_Master_Write_control_byte_spe(CONTROL_CODE_24AA1025, 0,
            CHIP_SEL_N0_24AA, READ_BIT);
    read_data = I2C_Master_Read();
    I2C_Master_Stop();                      // Stop condition
    
    return(read_data);
}

unsigned char EEPROM_read_random_address(unsigned long address) {
    unsigned char read_data;
    
    I2C_Master_Start();                     // Start condition
    I2C_Master_Write_control_byte_spe(CONTROL_CODE_24AA1025,
            ((0x10000 & address) >> 0x10), CHIP_SEL_N0_24AA, WRITE_BIT); 
    I2C_Master_Write((0xFF00 & address) >> 0x08);   // Write address high byte
    I2C_Master_Write(0xFF & address);               // Write address low byte
    I2C_Master_Start();                     // Start condition
    I2C_Master_Write_control_byte_spe(CONTROL_CODE_24AA1025,
            ((0x10000 & address) >> 0x10), CHIP_SEL_N0_24AA, READ_BIT);
    read_data = I2C_Master_Read();
    I2C_Master_Stop();                      // Stop condition
    
    return(read_data);
}

unsigned char EEPROM_read_sequential(unsigned long address,
        unsigned char* read_data, unsigned short nb_data_read) {
    int i=0;
    I2C_Master_Start();                     // Start condition
    I2C_Master_Write_control_byte_spe(CONTROL_CODE_24AA1025,
            ((0x10000 & address) >> 0x10), CHIP_SEL_N0_24AA, WRITE_BIT); 
    I2C_Master_Write((0xFF00 & address) >> 0x08);   // Write address high byte
    I2C_Master_Write(0xFF & address);               // Write address low byte
    I2C_Master_Start();                     // Start condition
    I2C_Master_Write_control_byte_spe(CONTROL_CODE_24AA1025,
            ((0x10000 & address) >> 0x10), CHIP_SEL_N0_24AA, READ_BIT);
    for(i=0; i<nb_data_read; i++) {
        read_data[i] = I2C_Master_Read();
        if(i == nb_data_read-1){
            I2C_Master_nAck();
        } else {
            I2C_Master_Ack();
        }
    }
    I2C_Master_Stop();                      // Stop condition    
}

#endif	/* 24AA1025_H */

