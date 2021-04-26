#include "./../API/UART_API.c"

UART* mock_UART()
{
    UART* uart = &(UART){
        .data = (unsigned char*)malloc(buf_len*sizeof(unsigned char))
        };
    return uart;
};

int main()
{
    UART* uart1 = mock_UART();
    UART* uart2 = mock_UART();
    //Initialising, setting options
    init_UART(uart1, 1, 1);
    set_speed(uart1,9600);
    set_parity(uart1,0);
    set_stopb(uart1,0);
    set_number_bits(uart1,8);

    init_UART(uart2, 2, 0);
    set_speed(uart2,115200);
    set_parity(uart2,0);
    set_stopb(uart2,0);
    set_number_bits(uart2,8);

    unsigned char* receved_data = (unsigned char*)malloc(buf_len*sizeof(unsigned char));
    int count_received = receive(uart1,receved_data);
    if (count_received == buf_len) 
    {
        memcpy(uart1->data, receved_data, count_received);
    }
    int count_send = send(uart2,receved_data);
    if (count_send == count_received) printf("Success transfer\n");
    free_UART(uart1);
    free_UART(uart2);
    free(receved_data);
    return 0;
};
