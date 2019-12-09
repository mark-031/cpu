enum CMDFlags
{
    CMDReg = 1 << 0,
    CMDMem = 1 << 1,
};

#define ADDFLAG(item, flag) item = item | CMD##flag
#define CHECKFLAG(item, name) (item & CMD##name)

const int MAXFLAGS = 4;
const int MAXARGC  = 4;