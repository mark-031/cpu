typedef int cpuelem_t;

cpuelem_t getArgc(cpuelem_t cmd)
{
    if(cmd == 1)
        return 1;
    
    if(cmd == 2)
        return 1;
    
    return -1;
}

cpuelem_t isJump(cpuelem_t cmd)
{
    if(cmd == 20)
        return 0;
    
    if(cmd == 21)
        return 0;
    
    return -1;
}