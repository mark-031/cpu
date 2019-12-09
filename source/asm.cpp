#include"../utils/namespaces.cpp"
#include"../utils/asmerr.cpp"
#include"../utils/asmfile.cpp"
#include"settings.hpp"
#include<assert.h>

Namespace_t Namespace;

int processFile(AsmFile &asmfile, FILE* outfile);
int processLine(AsmLine &asmline, const size_t &lineCounter, size_t &cmdCounter, FILE* outfile);
int checkForLabel(AsmLine &asmline, const size_t &lineCounter, const size_t &cmdCounter);

enum InstructionStates
{
    InstructionStateOk           = 0,
    InstructionStateIncorrectArg = 1,
    InstructionStateBroken       = 254,
    InstructionStateFinalized    = 255
};

#define INSTRSTATE(name) (char) InstructionState##name

class Instruction
{
    private:
        int    cmd;
        int*   args;
        int    argc;
        int    counter;

        char   state;

        size_t flags;

        void init_(int cmd, int argc)
        {
            this->cmd     = cmd;
            this->argc    = argc;
            this->counter = 0;
            this->flags   = 0;


            if(argc > 0)
            {
                state = INSTRSTATE(Ok);
                this->args = new int[argc];
            }

            else if (argc == 0)
            {
                state = INSTRSTATE(Ok);
                this->args = nullptr;
            }

            else
            {
                state = INSTRSTATE(Broken);
                this->args = nullptr;
            }
        }

        void recognizeInit_(const char* cmdline)
        {
            int cmd_ = 0;
            int argc_ = 0;

            if(0);

            #include"../build/asmcmd.hpp"

            else if(!strcmp("end", cmdline))
            {
                cmd_  = 0;
                argc_ = 0;
            }

            else
            {
                cmd_  = -1;
                argc_ = -1;
            }

            this->init_(cmd_, argc_);
        }

        void setBroken_()
        {
            this->cmd     = -1;
            this->argc    = -1;
            this->counter = 0;
            this->flags   = 0;

            if(this->state == INSTRSTATE(Ok))
                this->state = INSTRSTATE(Broken);
            
            if(this->args != nullptr)
            {
                delete this->args;
                this->args = nullptr;
            }
        }

    public:
        Instruction(int cmd, int argc)
        {
            this->init_(cmd, argc);
        }

        Instruction(const char* cmdline)
        {
            this->recognizeInit_(cmdline);
        }

        Instruction(AsmLine& line)
        {
            this->recognizeInit_(line[0]);

            if(line.getWordsNumber() != this->argc + 1)
            {
                this->setBroken_();
                return;
            }

            while(this->counter < this->argc)
            {
                if(this->recognizeArg(line[this->counter + 1]) != 0)
                {
                    this->setBroken_();
                    return;
                }
            }
        }

        ~Instruction()
        {
            if(this->state == INSTRSTATE(Ok))
                this->state   = INSTRSTATE(Finalized);

            this->setBroken_();

            delete this->args;
        }

        int isBroken()
        {
            return (int) this->state;
        }

        int isCompleted()
        {
            assert(this->counter <= this->argc);
            return (int) this->argc == this->counter;
        }

        int getArgc()
        {
            return this->argc;
        }

        int addArg(int arg, char argflags)
        {
            if(this->isBroken())
                return 1;

            if(this->isCompleted())
                return 1;
            
            assert(argflags < (1 << MAXFLAGS));
            
            this->args[this->counter++] = arg;
            
            assert((this->flags << MAXFLAGS) >> MAXFLAGS == this->flags);
            this->flags <<= MAXFLAGS;
            this->flags |= argflags;

            return 0;
        }

        int recognizeArg(const char* argline)
        {
            if(this->isBroken())
                return 1;

            size_t len      = strlen(argline);
            int    argflags = 0;
            int    arg      = -1;

            char* line = new char[len];

            if(argline[0] == '[')
            {
                if(argline[len - 1] != ']')
                    return 1;

                strcpy(line, argline + 1);
                
                len -= 2;

                line[len] = '\0';

                ADDFLAG(argflags, Mem);
            }

            else
            {
                strcpy(line, argline);
            }

            if(len == 2 && (line[1] == 'x') && (line[0] >= 'a' && line[0] <= 'd'))
            {
                ADDFLAG(argflags, Reg);

                arg = line[0] - 'a';
            }

            else if(sscanf(line, "%d", &arg))
                ;
            
            else
            {
                const name_t* name = Namespace.findName(line);
                
                if(name == nullptr)
                    arg = -1;
                else
                    arg = name->value;
            }

            delete line;

            return this->addArg(arg, argflags);
        }

        void printDump()
        {
            printf("Dump for Instruction\n");
            printf("cmd     = %d\n", this->cmd);
            printf("argc    = %d\n", this->argc);
            printf("counter = %d\n", this->counter);
            printf("flags   = %lu\n\n", this->flags);

            printf("state   = %d\n", (int) this->state);

            printf("args <%p>\n", this->args);
            
            if(this->args != nullptr)
            {
                printf("{\n");
                for(int i = 0; i < this->counter; ++i)
                    printf("    args[%d] = %d\n", i, this->args[i]);
                printf("}\n");
            }
        }

        int writeToFile(FILE* file)
        {
            assert(file != nullptr);

            if(this->isBroken())
                return 1;

            assert(this->state == INSTRSTATE(Ok));

            if(!this->isCompleted())
                return 1;

            fprintf(file, "%d", this->cmd);

            if(this->argc > 0)
            {
                fprintf(file, " %lu", this->flags);

                for(int i = 0; i < this->counter; ++i)
                    fprintf(file, " %d", this->args[i]);
            }
            
            fprintf(file, "\n");

            return 0;
        }
};

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("No input file\n");
        return 1;
    }

    AsmFile asmfile(argv[1]);

    if(asmfile.checkState() != AFSTATE(Ok))
    {
        printf("Read file error\n");
        return 1;
    }

    FILE* outfile = stdout;

    if(argc > 2)
        outfile = fopen(argv[2], "w");

    processFile(asmfile, nullptr);
    processFile(asmfile, outfile);
    ASMERRPRINT();
}

int processFile(AsmFile &asmfile, FILE* outfile)
{
    size_t linesCount = asmfile.getLinesAmount();
    size_t cmdCounter = 0;

    for(size_t line = 0; line < linesCount; ++line)
    {
        processLine(asmfile[line], line, cmdCounter, outfile);
        ASMERRCHECK();
    }

    return 0;
}

int processLine(AsmLine &asmline, const size_t &lineCounter, size_t &cmdCounter, FILE* outfile)
{
    if(checkForLabel(asmline, lineCounter, cmdCounter) == 0)
        return 0;
    
    ASMERRCHECK();

    int argc = 0;

    Instruction instr = Instruction(asmline);

    if(outfile != nullptr)
        ASMERRASSERT(instr.writeToFile(outfile) == 0,
                     SyntaxError, "Unexpected syntax error", asmline);

    if(instr.getArgc() == 0)
        cmdCounter += 1;
    else
        cmdCounter += instr.getArgc() + 2;

    return 0;
}

int checkForLabel(AsmLine &asmline, const size_t &lineCounter, const size_t &cmdCounter)
{
    int len = strlen(asmline[0]);

    if(asmline[0][len - 1] != ':')
        return 1;

    ASMERRASSERT(asmline.getWordsNumber() == 1,
                 WrongNumOfArgs, "Label declatration error", asmline);

    char* line = new char[len];
    strcpy(line, asmline[0]);
    line[len - 1] = '\0';

    Namespace.addName(line, cmdCounter, NTYPE(Label));

    delete line;

    return 0;
}

void AsmErr::errprint()
{
    AsmLine* line = (AsmLine*) this->data;
    printf("[ERROR] Code %d\n", this->code);

    #define ASMERRCODE(code_, name_, description_)   \
    else if(this->code == code_)                     \
        printf(#description_ "\n");

    if(0);

    #include"../utils/asmerrors.hpp"

    else
        printf("Description undefined!\n");

    #undef ASMERRCODE()

    if(line != nullptr)
        line->printline();
}