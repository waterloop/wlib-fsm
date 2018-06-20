#include <Cosa/UART.hh>
#include <wlib/state_machine>

enum { BAUD_RATE = 9600 };

void setup() {
    uart.begin(BAUD_RATE);
}


void loop() {
    delay(50);
}

