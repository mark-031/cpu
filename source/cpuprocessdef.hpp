#define GETCMD_() this->elems[this->counter].elem
#define GETFLAGS_() this->elems[this->counter + 1].size
#define GETARG_(num) this->elems[this->counter + num + 2].elem
#define ARG_(num) args_[num]
#define PUSH_(item) StackPush(this->stack, item)
#define POP_(item) StackPop(this->stack, &item)
#define MEM_(index) (*this->memory)[index]
#define REG_(index) this->reg[index]
#define SETNEXT_(argc) if(argc == 0) this->counter += 1; else this->counter += 2 + argc

#ifdef CPUDEBUG
    #define STARTPROCESSCMD() printf("\n--- [START PROCESS COMMAND] ---\n")

    #define ENDPROCESSCMD() printf("--- [END PROCESS COMMAND] --- \n")
    
    #define GETFLAGS()                                                              \
        (                                                                           \
            printf("GETFLAGS() = " CPUELEM_T "\n", GETFLAGS_()),                    \
            GETFLAGS_()                                                             \
        )

    #define GETARG(num)                                                             \
        (                                                                           \
            printf("GETARG(" CPUELEM_T ") = " CPUELEM_T "\n", num, GETARG_(num)),   \
            GETARG_(num)                                                            \
        )
    
    #define ARG(num)                                                                \
        (                                                                           \
            printf("ARG(" CPUELEM_T ") = " CPUELEM_T "\n", num, ARG_(num)),         \
            ARG_(num)                                                               \
        )

    #define PUSH(item)                                                              \
        (                                                                           \
            printf("PUSH(" CPUELEM_T ")\n", item),                                  \
            PUSH_(item)                                                             \
        )

    #define POP(item)                                                               \
        (                                                                           \
            POP_(item),                                                             \
            printf("POP(" CPUELEM_T ")\n", item)                                    \
        )

    #define MEM(index)                                                              \
        (                                                                           \
            printf("MEM(" CPUELEM_T ") = " CPUELEM_T "\n", index, MEM_(index)),     \
            MEM_(index)                                                             \
        )

    #define REG(index)                                                              \
        (                                                                           \
            printf("REG(" CPUELEM_T ") = " CPUELEM_T "\n", index, REG_(index)),     \
            REG_(index)                                                             \
        )

    #define SETNEXT(argc)                                                           \
        {                                                                           \
            printf("SETNEXT(" CPUELEM_T ")\n", argc);                               \
            printf("Before: counter = " CPUELEM_T "\n", this->counter);             \
            SETNEXT_(argc);                                                         \
            printf("After:  counter = " CPUELEM_T "\n", this->counter);             \
        }
#else
    #define STARTPROCESSCMD()
    #define ENDPROCESSCMD()
    #define GETFLAGS() GETFLAGS_()
    #define GETARG(num) GETARG_(num)
    #define ARG(num) ARG_(num)
    #define PUSH(item) PUSH_(item)
    #define POP(item) POP_(item)
    #define MEM(index) MEM_(index)
    #define REG(index) REG_(index)
    #define SETNEXT(argc) SETNEXT_(argc)
#endif

//#define GETCMD() (printf("GETCMD() = " CPUELEM_T "\n", GETCMD_()), GETCMD_())
#define GETCMD() GETCMD_()
#define COUNTER this->counter
#define CALLPUSH(item) StackPush(this->callstack, item)
#define CALLPOP(item) StackPop(this->callstack, &item)