
// command: push
else if(GETCMD() == 1)
{
    // process args

    size_t flags_ = GETFLAGS();

    for(int i = 0; i < 1; ++i)
    {
        ARG(i) = GETARG(i);

        if(CHECKFLAG(flags_, Reg))
            ARG(i) = REG(ARG(i));
        
        if(CHECKFLAG(flags_, Mem))
            ARG(i) = MEM(ARG(i));
    }


    // command code
    if(!CHECKFLAG(flags_, Mem) && !CHECKFLAG(flags_, Reg))
        PUSH(cpuelem_t(ARG(0) * ACCURACY));
    else
        PUSH(ARG(0));
    
    // set counter
    SETNEXT(1);
}


// command: pop
else if(GETCMD() == 2)
{
    // process args

    size_t flags_ = GETFLAGS();

    for(int i = 0; i < 1; ++i)
    {
        ARG(i) = GETARG(i);

        if(CHECKFLAG(flags_, Reg) && CHECKFLAG(flags_, Mem))
            ARG(i) = REG(ARG(i));
    }


    // command code
    cpuelem_t item = {};
    
    POP(item);

    if(CHECKFLAG(flags_, Mem))
        MEM(ARG(0)) = item;
    else if(CHECKFLAG(flags_, Reg))
        REG(ARG(0)) = item;
    
    // set counter
    SETNEXT(1);
}


// command: in
else if(GETCMD() == 4)
{
    // process args


    // command code
    cpuelem_t item = {};

    while(true)
    {
        printf("input: ");

        if(scanf(CPUELEM_T, &item))
            break;
        
        while(getchar() != '\n');
    }

    PUSH(cpuelem_t(item * ACCURACY));
    
    // set counter
    SETNEXT(0);
}


// command: out
else if(GETCMD() == 5)
{
    // process args


    // command code
    cpuelem_t item = {};

    POP(item);

    printf("output: %.3f\n", item / ACCURACY);
    
    // set counter
    SETNEXT(0);
}


// command: mul
else if(GETCMD() == 10)
{
    // process args


    // command code
    cpuelem_t item1 = {};
    POP(item1);
    cpuelem_t item2 = {};
    POP(item2);
    
    PUSH(cpuelem_t((item1 / ACCURACY) * item2));
    
    // set counter
    SETNEXT(0);
}


// command: add
else if(GETCMD() == 11)
{
    // process args


    // command code
    cpuelem_t item1 = {};
    POP(item1);
    cpuelem_t item2 = {};
    POP(item2);
    
    PUSH(item1 + item2);
    
    // set counter
    SETNEXT(0);
}


// command: sub
else if(GETCMD() == 12)
{
    // process args


    // command code
    cpuelem_t item1 = {};
    POP(item1);
    cpuelem_t item2 = {};
    POP(item2);
    
    PUSH(item1 - item2);
    
    // set counter
    SETNEXT(0);
}


// command: div
else if(GETCMD() == 13)
{
    // process args


    // command code
    cpuelem_t item1 = {};
    POP(item1);
    cpuelem_t item2 = {};
    POP(item2);
    
    PUSH(cpuelem_t((item1 * ACCURACY) / item2));
    
    // set counter
    SETNEXT(0);
}


// command: sqrt
else if(GETCMD() == 14)
{
    // process args


    // command code
    cpuelem_t item = {};
    POP(item);

    float itemf = item / ACCURACY;

    item = cpuelem_t(sqrt(itemf) * ACCURACY);
    
    PUSH(item);
    
    // set counter
    SETNEXT(0);
}


// command: call
else if(GETCMD() == 18)
{
    // process args

    size_t flags_ = GETFLAGS();

    for(int i = 0; i < 1; ++i)
    {
        ARG(i) = GETARG(i);

        if(CHECKFLAG(flags_, Reg))
            ARG(i) = REG(ARG(i));
        
        if(CHECKFLAG(flags_, Mem))
            ARG(i) = MEM(ARG(i));
    }


    // command code
    CALLPUSH((cpuelem_t) COUNTER + 3);
    COUNTER = (size_t) ARG(0);
    return 1;
    
    // set counter
    SETNEXT(1);
}


// command: return
else if(GETCMD() == 19)
{
    // process args


    // command code
    cpuelem_t item = {};
    CALLPOP(item);

    COUNTER = (size_t) item;
    return 1;
    
    // set counter
    SETNEXT(0);
}


// command: jmp
else if(GETCMD() == 20)
{
    // process args

    size_t flags_ = GETFLAGS();

    for(int i = 0; i < 1; ++i)
    {
        ARG(i) = GETARG(i);

        if(CHECKFLAG(flags_, Reg))
            ARG(i) = REG(ARG(i));
        
        if(CHECKFLAG(flags_, Mem))
            ARG(i) = MEM(ARG(i));
    }


    // command code
    COUNTER = ARG(0);
    return 1;
    
    // set counter
    SETNEXT(1);
}


// command: jl
else if(GETCMD() == 21)
{
    // process args

    size_t flags_ = GETFLAGS();

    for(int i = 0; i < 1; ++i)
    {
        ARG(i) = GETARG(i);

        if(CHECKFLAG(flags_, Reg))
            ARG(i) = REG(ARG(i));
        
        if(CHECKFLAG(flags_, Mem))
            ARG(i) = MEM(ARG(i));
    }


    // command code
    cpuelem_t item = {};

    POP(item);

    if(item < 0)
    {
        COUNTER = ARG(0);
        return 1;
    }
    
    // set counter
    SETNEXT(1);
}

