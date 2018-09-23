#ifndef MEMORY_HEADER
#define MEMORY_HEADER

class Memory {
    
    public:
        const static int MEMORY_SIZE = 32;
    
    private:
        int memory[MEMORY_SIZE];
    
    public:
        Memory();
        void reset();
        void set(int address, int value);
        int get(int address);
        void print();
};


#endif