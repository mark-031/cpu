#include"asmerr.hpp"

AsmErr asmerr_;

#define ASMERRCODE(code, name, description) AsmError##name = code,
enum AsmErrors
{
    #include"asmerrors.hpp"
};
#undef ASMERRCODE

#define ASMERR(name) AsmError##name

AsmErr::AsmErr()
{
    this->set(ASMERR(Ok), nullptr, nullptr);
}

AsmErr::~AsmErr()
{
    delete this->description;
}

void AsmErr::set(int code_, char* description_, void* data_)
{
    this->code = code_;

    if(description_ == nullptr)
        this->description = nullptr;
    
    else
    {
        size_t len = strlen(description_);
        this->description = new char[len];
        strcpy(this->description, description_);
    }

    this->data = data_;
}

int AsmErr::getcode()
{
    return this->code;
}

void* AsmErr::getdata()
{
    return this->data;
}

#define ASMERRASSERT(expr_, errname_, descr_, data_)    \
if(!(expr_))                                            \
{                                                       \
    asmerr_.set(ASMERR(errname_), descr_, &(data_));      \
    return 1;                                           \
}

#define ASMERRCHECK()                               \
if(asmerr_.getcode() != ASMERR(Ok))                 \
    return 1

#define ASMERRPRINT()                               \
if(asmerr_.getcode() != ASMERR(Ok))                 \
    asmerr_.errprint()
