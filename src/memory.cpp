#include "mbed.h"
#include "memory.h"

Memory::Memory()
{
    reset();
}

/*
 * Reset all memory locations to 0.
 */
void Memory::reset()
{
    for (int i = 0; i < Memory::MEMORY_SIZE; i++) {
        this->memory[i] = 0;
    }
}

/*
 * Store value in memory
 */
void Memory::set(int address, int value)
{
    if (address < Memory::MEMORY_SIZE) {
        this->memory[address] = value;   
    }
}

/*
 * Retrieve value from memory
 */
int Memory::get(int address)
{
    if (address < Memory::MEMORY_SIZE) {
        return this->memory[address];   
    } else {
        return 0;
    }
}

/*
 * Print current memory content to console
 */
void Memory::print()
{
    int i = 0, c = 0;
    while (i < Memory::MEMORY_SIZE) {
        c = (c + 1) % 4;
        printf("\t[%#04x]: %6d", i, this->memory[i]);
        if (!c) {
            printf("\r\n");
        }
        i++;
    }
}