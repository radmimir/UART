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

    init_UART(uart1, 0, 1);
    set_speed(uart1,9600);
    set_parity(uart1,0);
    set_stopb(uart1,0);

    init_UART(uart2, 0, 1);
    set_speed(uart1,115200);
    set_parity(uart1,0);
    set_stopb(uart1,0);
    
};