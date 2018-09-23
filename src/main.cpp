#include "mbed.h"
#include "memory.h"

// This code requires a serious refactor for next year

DigitalOut aliveLED(LED1);
Serial pc(USBTX, USBRX); // tx, rx
I2CSlave slave(p28, p27);

const int SLAVE_ADDRESS = 0x28;
const int I2C_FREQUENCY = 400000;
const int I2C_BUFFER_SIZE = 6;

enum COMMAND { PUSH, PULL, CLEAR, PRINT };

#define intToByte(pbytebuff,intval) (*((int*)(pbytebuff)) = intval)
#define byteToInt(pbytebuff,pintval) (*(pintval) = *((int*)(pbytebuff)))

int main() {
    pc.baud(115200);
    pc.printf("Starting mBed I2C Memory Slave emulator\r\n");
    pc.printf("Size of integer is %d bytes\r\n", sizeof(int));
    
    // Alive LED
    int cAlive = 0;
    
    // Configure I2C
    slave.frequency(I2C_FREQUENCY);
    pc.printf("Slave is working @ %dHz\r\n", I2C_FREQUENCY);
    slave.address(SLAVE_ADDRESS);
    pc.printf("Slave is available @ SLAVE_ADDRESS = 0x%x\r\n", SLAVE_ADDRESS);

    // Setup memory
    Memory memory;
    pc.printf("Size of memory buffer is %d elements\r\n", Memory::MEMORY_SIZE);
    memory.print();
    pc.printf("Awaiting commands from master ...\r\n");

    // I2C buffer
    char buffer[I2C_BUFFER_SIZE];
    
    // Internal address pointer
    int pointer = 0;
    
    while (1) {
        int rec = slave.receive();
        switch (rec) {
            case I2CSlave::ReadAddressed:
            {
                int value = memory.get(pointer);
                buffer[0] = pointer;
                intToByte(buffer+1, value);
                if (!slave.write(buffer, 5)) {
                    pc.printf("Retrieving and sending to master %d@%d\r\n", value, pointer);
                } else {
                    pc.printf("Failed to send to master %d@%d\r\n", value, pointer);
                }
                break;
            }
            case I2CSlave::WriteAddressed:
            {
                // First we read the command byte
                int command = slave.read();
                
                // Check the command
                switch (command)
                {
                    case PUSH:
                        // Expect 5 more bytes [address] [int value]
                        if(!slave.read(buffer, 5)) {
                            int address = buffer[0];
                            int value;
                            byteToInt(buffer+1, &value);
                            pc.printf("Storing %d@%d\r\n", value, address);
                            memory.set(address, value);
                        } else {
                            pc.printf("PUSH received with missing address/data\r\n");   
                        }
                        break;
                    
                    case PULL:
                        // Expect 1 more byte [address]
                        if(!slave.read(buffer, 1)) {
                            int address = buffer[0];
                            pc.printf("Setting pointer to %d\r\n", address);
                            if (address < Memory::MEMORY_SIZE) {
                                pointer = address;
                            } else {
                                pc.printf("Address out of boundary\r\n");    
                            }
                        } else {
                            pc.printf("PULL received with missing address\r\n");   
                        }
                        break;
                    
                    case CLEAR:
                        pc.printf("Clearing the memory\r\n");    
                        memory.reset();
                        slave.stop();
                        break;
                    
                    case PRINT:
                        memory.print();
                        slave.stop();
                        break;
                    
                    default:
                        pc.printf("Unknown command byte\r\n");
                }
            }
        }
        
        // Clear buffer
        for (int i = 0; i < I2C_BUFFER_SIZE; i++) {
            buffer[i] = 0;   
        }
        
        // Alive LED
        cAlive = (cAlive + 1) % 500000;
        if (!cAlive) {
            aliveLED = !aliveLED;
        }
    }
}
