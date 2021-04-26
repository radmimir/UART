#include "UART_API.h"
//Additional functions
int get_baud(int baud) 
{
    switch (baud) {
    case 0:
        return B0;
    case 4800:
        return B4800;
    case 9600:
        return B9600;
    case 19200:
        return B19200;
    case 38400:
        return B38400;
    case 57600:
        return B57600;
    case 115200:
        return B115200;
    default: 
        return -1;
    }
}

int get_count_bits(int bits)
{
    switch (bits) {
    case 5:
        return CS5;
    case 6:
        return CS6;
    case 7:
        return CS7;
    case 8:
        return CS8;
    default: 
        return -1;
    }
}

int init_UART(UART* uart, int sp_number, unsigned char receive)
{
    char* path = (char*)malloc(sizeof(char)*FILENAME_MAXSIZE);
    char* sp_num = (char*)malloc(sizeof(int));
    sprintf(sp_num,"%d",sp_number);
    memcpy(path, "/dev/ttyS",10);
    path = strcat(path, (const char*)sp_num);
    printf("%s %s %s \n",path,sp_num, path);
    uart->fd = open(path, O_RDWR, O_SYNC | O_NOCTTY );
    if (uart->fd < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    };
    if(tcgetattr(uart->fd, &uart->settings) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }
    if (receive) //if receiving data
    {
        uart->settings.c_cflag |= (CREAD | CLOCAL | CBAUDEX);
    }
    else 
    {
        uart->settings.c_cflag &= ~(CREAD);
        uart->settings.c_cflag |= (CLOCAL | CBAUDEX);
    }
    //Setting blocking mode waiting indefinetely
    uart->settings.c_cc[VMIN] = buf_len;
    uart->settings.c_cc[VTIME] = 0;
    uart->settings.c_cflag &= ~CBAUD;
    // Setting non-canonical mode, non-echo, erasing
    uart->settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); 
    if(tcsetattr(uart->fd, TCSANOW, &uart->settings) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }
    free(path);
    if (errno) return -1;
    else return uart->fd;
}

int set_speed(UART* uart, int baud_rate)
{
    if(tcgetattr(uart->fd, &uart->settings) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    int baud = get_baud(baud_rate);
    if (!baud) return -1;
    int ispeed = cfsetispeed(&uart->settings, baud);
    int ospeed = cfsetospeed(&uart->settings, baud);
    if (ispeed == ospeed && ospeed == baud_rate) 
    {
        if(tcsetattr(uart->fd, TCSANOW, &uart->settings) != 0) {
            printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
            return -1;
        }
        return 1;
    }
    else return -1;
}

int set_parity(UART* uart, unsigned char parity)
{
    if(tcgetattr(uart->fd, &uart->settings) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    if (parity) uart->settings.c_cflag |= PARENB;
        else uart->settings.c_cflag &= ~PARENB;
    if(tcsetattr(uart->fd, TCSANOW, &uart->settings) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    return 1;
}

int set_stopb(UART* uart, unsigned char stopb)
{
    if(tcgetattr(uart->fd, &uart->settings) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    if (stopb) uart->settings.c_cflag |= CSTOPB;
        else uart->settings.c_cflag &= ~CSTOPB;
    if(tcsetattr(uart->fd, TCSANOW, &uart->settings) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    return 1;
}

int set_number_bits(UART* uart, unsigned char bits)
{
    if(tcgetattr(uart->fd, &uart->settings) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    int bits_const = get_count_bits(bits);
    if (bits_const < 5 || bits_const > 8){
        printf("Incorrect bits input bits=%d, must be 5,6,7 or 8\n", bits);
        return -1;
    }
    uart->settings.c_cflag &= ~CSIZE;
    uart->settings.c_cflag |= bits_const  ;
    if(tcsetattr(uart->fd, TCSANOW, &uart->settings) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    return 1;
}

void receive_callback(UART* uart, unsigned char* received_data)
{
    memcpy(uart->data,received_data, buf_len);
    printf("Received %d bytes successfully\n",buf_len);
}

void send_callback()
{
    printf("Sent %d bytes successfully:\n",buf_len);
}

int send(UART* uart, unsigned char* send_data)
{
    ssize_t result = 0;
    int send_count = 0;
    while (send_count < buf_len)
    {
        result = write(uart->fd, send_data + send_count, (buf_len - send_count));
        if (result < 0)
        {
            printf("Error %i in writing: %s\n", errno, strerror(errno));
            return -1;
        }
        send_count += result;
    }
    send_callback(send_data);
    return result;
}

int receive(UART* uart, unsigned char* received_data)
{
    int receive_count = 0;//total count of received bytes
    ssize_t result = 0;
    while (receive_count < buf_len)
    {
        result = read(uart->fd, received_data + receive_count, buf_len - receive_count);
        if(result < 0)
        {
            printf("Error %i in reading: %s\n", errno, strerror(errno));
            return -1;
        }
        receive_count += result;
    }
    receive_callback(uart, received_data);
    return result;
}


void free_UART(UART* uart)
{
    free(uart->data);
    close(uart->fd);
    free(uart);
}