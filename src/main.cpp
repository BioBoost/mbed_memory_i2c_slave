#include "mbed.h"

DigitalOut aliveLED(LED1);
Serial pc(USBTX, USBRX); // tx, rx

const int SLAVE_ADDRESS = 0x28;

int main() {
  pc.baud(115200);
  printf("Starting mBed I2C Memory Slave emulator\r\n");
  printf("Slave is available @ SLAVE_ADDRESS = 0x%x\r\n", SLAVE_ADDRESS);

  while (1) {
    aliveLED = !aliveLED;
    Thread::wait(500);
  }
}
