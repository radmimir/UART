#include "./../API/UART_API.c"

UART* mock_UART()
{
    UART* test_uart = &(UART){
        };
    return test_uart;
}

void test_init_UART(UART* uart)
{
    uart->fd = init_UART(uart, 3, 1);
    if (uart->fd >= 0) printf("%s", "Test 1 init passed\n");
    else printf("%s", "Test 1 init failed\n");
}

void test_set_speed(UART* uart)
{
    int baud_rate = 9600;
    set_speed(uart, baud_rate);
    if(tcgetattr(uart->fd, &uart->settings) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }
    int baud = get_baud(baud_rate);
    if ((cfgetispeed(&uart->settings) & baud) && (cfgetospeed(&uart->settings) & baud) && (errno == 0))
    {
        printf("%s", "Test 2 set_speed passed\n");
    }
    else printf("%s", "Test 2 set_speed failed\n");
}

void test_set_parity(UART* uart)
{
    unsigned char parity = 1;
    set_parity(uart, parity);
    if(tcgetattr(uart->fd, &uart->settings) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }
    if ((uart->settings.c_cflag & PARENB) && (errno == 0))
    {
        printf("%s", "Test 3 set_parity passed\n");
    }
    else printf("%s", "Test 3 set_parity failed\n");
}

void test_set_stopb(UART* uart)
{
    unsigned char stopb = 1;
    set_stopb(uart, stopb);
    if(tcgetattr(uart->fd, &uart->settings) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }
    if ((uart->settings.c_cflag & stopb) && (errno == 0))
    {
        printf("%s", "Test 4 set_stopb passed\n");
    }
    else printf("%s", "Test 4 set_stopb failed\n");
}

void test_set_number_bits(UART* uart)// Setting number of bits in byte, bits = 5,6,7 or 8.
{
    unsigned char bits = 8;
    int bits_const = get_count_bits(bits);
    set_number_bits(uart, bits_const);
    if(tcgetattr(uart->fd, &uart->settings) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }
    if ((uart->settings.c_cflag & bits_const) && (errno == 0))
    {
        printf("%s", "Test 5 set_number_bits passed\n");
    }
    else printf("%s", "Test 5 set_number_bits failed\n");
}

int main()
{
    UART* uart = mock_UART();
    test_init_UART(uart);
    test_set_speed(uart);
    test_set_parity(uart);
    test_set_stopb(uart);
    test_set_number_bits(uart);
    return 0;
}