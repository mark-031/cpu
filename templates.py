asm = '''
else if(!strcmp("{cmd.name}", cmdline))
{{
    cmd_ = {cmd.num};
    argc_ = {cmd.argc};
}}
'''

cpu = '''
// command: {cmd.name}
else if(GETCMD() == {cmd.num})
{{
    // process args
{process_args}

    // command code
{cmd.cputext}
    
    // set counter
    SETNEXT({cmd.argc});
}}

'''

processarg_get_value = '''
    size_t flags_ = GETFLAGS();

    for(int i = 0; i < {cmd.argc}; ++i)
    {{
        ARG(i) = GETARG(i);

        if(CHECKFLAG(flags_, Reg))
            ARG(i) = REG(ARG(i));
        
        if(CHECKFLAG(flags_, Mem))
            ARG(i) = MEM(ARG(i));
    }}
'''

processarg_get_target = '''
    size_t flags_ = GETFLAGS();

    for(int i = 0; i < {cmd.argc}; ++i)
    {{
        ARG(i) = GETARG(i);

        if(CHECKFLAG(flags_, Reg) && CHECKFLAG(flags_, Mem))
            ARG(i) = REG(ARG(i));
    }}
'''