#pragma once

// C library headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Linux headers
#include <fcntl.h> 
#include <errno.h> 
#include <termios.h> 
#include <unistd.h> 

const int FILENAME_MAXSIZE = 12;
const int buf_len = 128;
typedef struct UART
{
    unsigned char* data;
    int fd;
    struct termios settings; 
} UART;

int get_baud(int baud);//Setting baudrate constants
int get_count_bits(int bits);//Setting number of bits constant
int init_UART(UART* uart, int sp_number, unsigned char receive);//Initialisation of serial port
int set_speed(UART* uart, int baud_rate);// Setting baud rate of sp
int set_parity(UART* uart, unsigned char parity);// Setting parity bit, parity = 0 or 1
int set_stopb(UART* uart, unsigned char stopb);// Setting stop bits' count, stopb = 0(1 bit) or 1(2 bits)
int set_number_bits(UART* uart, unsigned char bits);// Setting number of bits in byte, bits = 5,6,7 or 8.
void receive_callback(UART* uart, unsigned char* received_data);
void send_callback();
int send(UART* uart, unsigned char* send_data);//Tranceive function
int receive(UART* uart, unsigned char* received_data);//Receive function
void free_UART(UART* uart);
