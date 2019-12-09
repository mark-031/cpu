#include<stdio.h>
#include<string.h>

class AsmErr
{
    private:
        int   code;
        char* description;
        void* data;

    public:
        AsmErr();

        ~AsmErr();

        void set(int code_, char* description_, void* data_);

        int getcode();

        void* getdata();

        void errprint();
};