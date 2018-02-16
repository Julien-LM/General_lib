/* 
 * File:   structures.h
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on February 16, 2018, 5:07 PM
 */

#ifndef STRUCTURES_H
#define	STRUCTURES_H


typedef struct Time_struct Time;
struct Time_struct {
    unsigned char century;
    unsigned char years;
    unsigned char months;
    unsigned char days;
    unsigned char hours;
    unsigned char minutes;
    unsigned char seconds;
};

// UART reception
typedef struct UART_struct UART;
struct UART_struct {
    unsigned char UART_reception_buffer[RECEPTION_BUFFER_SIZE];
    unsigned char UART_reception_index;
    unsigned char UART_parsing_in_progress;
    unsigned char UART_reception_overflow;
};

// Temperature storage
typedef struct storage_struct MEM;
struct storage_struct {
    unsigned char data_storage[DATA_STORAGE_SIZE];
    unsigned short data_storage_index;
    unsigned char data_storage_overflow;
    unsigned long temp_sample_rate;
};

#endif	/* STRUCTURES_H */
