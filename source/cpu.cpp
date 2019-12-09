#include<stdio.h>
typedef int cpuelem_t;
#define CPUELEM_T "%d"
typedef cpuelem_t stackunit_t;
#include"../utils/stack.cpp"
#include"../utils/utils.cpp"
#include"settings.hpp"
#include<math.h>

#define SIZE_T "%lu"

const size_t STACKSIZE     = 128;
const size_t CALLSTACKSIZE = 32;
const int    REGCOUNT      = 4;
const float  ACCURACY      = 100.0f;

union elem_t
{
    cpuelem_t elem;
    size_t    size;
};

class Memory
{
    private:
        cpuelem_t* data;
        size_t size;

        friend class CPU;
    public:
        Memory(size_t size_)
        {
            this->data = new cpuelem_t[size_];
            this->size = size_;
        }

        ~Memory()
        {
            delete this->data;
            this->size = 0;
        }

        cpuelem_t& operator[](size_t index)
        {
            assert(this->size > index);

            return this->data[index];
        }
};

class Cpu
{
    private:
        cpuelem_t reg[REGCOUNT];
        stack_t* stack;
        stack_t* callstack;

        Memory* memory;

        elem_t* elems;
        size_t  amount;
        size_t  counter;

        char* readTape_(const char* path)
        {
            FILE* file = fopen(path, "r");

            if(file == nullptr)
                return nullptr;

            char* buffer = utils::readText(file);

            fclose(file);

            return buffer;
        }

        size_t countOfElems_(const char* buffer)
        {
            size_t amount_ = 1;

            for(; *buffer != '\0'; ++buffer)
                if(*buffer == ' ' || *buffer == '\n') ++amount_;

            return amount_;
        }

        void readItems_(const char* buffer)
        {
            size_t i = 0;
            while(true)
            {
                sscanf(buffer, "%lu", (this->elems + i));
                i++;

                while(*buffer != ' ' && *buffer != '\0' && *buffer != '\n') ++buffer;

                if(*buffer == '\0')
                    break;
                
                ++buffer;
            }
        }

    public:
        Cpu()
        {
            for(int i = 0; i < REGCOUNT; ++i)
                this->reg[i] = 0;

            StackInit(this->stack, STACKSIZE);
            StackInit(this->callstack, CALLSTACKSIZE);

            this->elems   = nullptr;
            this->counter = 0;

            this->memory    = nullptr;
        }

        ~Cpu()
        {
            StackFinilize(&this->stack);
            StackFinilize(&this->callstack);

            if(this->isMemorySet())
                delete this->memory;

            if(this->elems)
                delete this->elems;
        }

        int setMemory(size_t count)
        {
            if(this->memory != nullptr)
                delete this->memory;

            this->memory = new Memory(count);

            return 0;
        }

        int unsetMemory()
        {
            if(this->memory == nullptr)
                return -1;
            
            delete this->memory;


            return 0;
        }

        Memory* getMemory()
        {
            return this->memory;
        }

        cpuelem_t* getReg(int index)
        {
            return this->reg + index;
        }

        int isMemorySet()
        {
            if(this->memory == nullptr)
                return 0;

            return 1;
        }

        int loadTape(const char* path)
        {
            if(this->elems != nullptr)
                delete this->elems;

            char* buffer = this->readTape_(path);

            if(buffer == nullptr)
                return 1;

            this->amount = this->countOfElems_(buffer);

            this->elems = new elem_t[this->amount];

            this->readItems_(buffer);
            
            free(buffer);

            return 0;
        }
        
        //#define CPUDEBUG
        #include"cpuprocessdef.hpp"

        int processCmd()
        {
            STARTPROCESSCMD();

            cpuelem_t args_[MAXARGC] = {};

            if(0);

            #include"../build/cpucmd.hpp"
            
            else if(GETCMD() == 0)
            {
                printf("END\n");

                ENDPROCESSCMD();

                return 0;
            }

            else
            {
                printf("[ERROR!] Unknown command " CPUELEM_T "\n", GETCMD());

                ENDPROCESSCMD();

                return 0;
            }

            ENDPROCESSCMD();

            return 1;
        }
        
        #include"cpuprocessundef.hpp"

        void dump()
        {
            for(size_t i = 0; i < this->amount; ++i)
                printf("%lu ", this->elems[i].size);

            printf("\n");
        }
};

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("No input file\n");
        return 1;
    }

    Cpu cpu;

    if(cpu.loadTape(argv[1]))
    {
        printf("File load error\n");
        return 1;
    }

    cpu.setMemory(128);

    while(cpu.processCmd());
}