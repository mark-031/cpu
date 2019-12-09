
else if(!strcmp("push", cmdline))
{
    cmd_ = 1;
    argc_ = 1;
}

else if(!strcmp("pop", cmdline))
{
    cmd_ = 2;
    argc_ = 1;
}

else if(!strcmp("in", cmdline))
{
    cmd_ = 4;
    argc_ = 0;
}

else if(!strcmp("out", cmdline))
{
    cmd_ = 5;
    argc_ = 0;
}

else if(!strcmp("mul", cmdline))
{
    cmd_ = 10;
    argc_ = 0;
}

else if(!strcmp("add", cmdline))
{
    cmd_ = 11;
    argc_ = 0;
}

else if(!strcmp("sub", cmdline))
{
    cmd_ = 12;
    argc_ = 0;
}

else if(!strcmp("div", cmdline))
{
    cmd_ = 13;
    argc_ = 0;
}

else if(!strcmp("sqrt", cmdline))
{
    cmd_ = 14;
    argc_ = 0;
}

else if(!strcmp("call", cmdline))
{
    cmd_ = 18;
    argc_ = 1;
}

else if(!strcmp("return", cmdline))
{
    cmd_ = 19;
    argc_ = 0;
}

else if(!strcmp("jmp", cmdline))
{
    cmd_ = 20;
    argc_ = 1;
}

else if(!strcmp("jl", cmdline))
{
    cmd_ = 21;
    argc_ = 1;
}
