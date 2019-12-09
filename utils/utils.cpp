#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<stdlib.h>
#include"utils.hpp"

#define SIZE_T "%lu"

/*
    MaxValue_ more than any other pointer
    MaxWarnValue_ more than every other warning pointer
*/
enum Pointers
{
    PtrMaxValue_        = 256,
    PtrMaxWarnValue_    = 128,
    
    // Warnings
    PtrNull             = 0,
    PtrEndWork          = 5,

    // Errors
    PtrAllocError       = 129,
    PtrFileReadError    = 131,
    PtrFileSizeError    = 133,
    PtrFileNotFound     = 135,
};

#define PTR(name, type) (type*) Ptr##name

void ptrassert(void* pointer, int line, char* name)
{
    if(pointer >= PTR(MaxValue_, void))
        return;
    
    if(pointer < PTR(MaxWarnValue_, void))
    {
        // Warning
        printf("[PTRWarning] \"%s\" on line %d (code: %lu)\n", name, line, (unsigned long) pointer);

        if (pointer == PTR(Null, void))
            printf("Nullptr\n");
        else if(pointer == PTR(EndWork, void))
            printf("End of work with pointer\n");
        else
            printf("Unknow error\n");
    }
    else
    {
        // Error
        printf("[PTRError] \"%s\" on line %d (code: %lu)\n", name, line, (unsigned long) pointer);

        if(pointer == PTR(AllocError, void))
            printf("Allocate error\n");
        
        else if(pointer == PTR(FileReadError, void))
            printf("File read error\n");
        
        else if(pointer == PTR(FileSizeError, void))
            printf("File size error\n");
        
        else if(pointer == PTR(FileNotFound, void))
            printf("File not found\n");
        
        else
            printf("Unknown error\n");

        exit(1);
    }
}

#ifdef PTRDEBUG
    #define ptrassert(pointer) ptrassert(pointer, __LINE__, #pointer)
#else
    #define ptrassert(pointer)
#endif

void utils::loadIoFiles(int argc, char* argv[], FILE* &input, FILE* &output)
{
    input  = nullptr;
    output = nullptr;
    
    for(int i = 1; i < argc; ++i)
    {
        if(!strcmp(argv[i], "-i") || !strcmp(argv[i], "--input"))
        {
            if(input != nullptr)
                fclose(input);

            input = fopen(argv[++i], "r");

            assert(input != nullptr);
            continue;
        }

        if(!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output"))
        {
            if(output != nullptr)
                fclose(output);

            output = fopen(argv[++i], "w");

            assert(output != nullptr);
            continue;
        }
    }
}

void utils::closeIoFiles(FILE* &input, FILE* &output)
{
    if(input != nullptr)
        fclose(input);

    if(output != nullptr)
        fclose(output);
    
    input  = nullptr;
    output = nullptr;
}

char* utils::readText(FILE* file)
{
    assert(file);

    long textSize = utils::getFileSize(file);

    if(textSize == -1L)
        return PTR(FileSizeError, char);
    
    char* buffer = (char*) calloc(textSize, sizeof(char));

    if(buffer == nullptr)
        return PTR(AllocError, char);
    
    size_t readCount = fread(buffer, sizeof(char), textSize, file);

    if(readCount == 0)
    {
        free(buffer);
        return PTR(FileReadError, char);
    }

    assert(readCount <= (size_t) textSize);

    return buffer;
}

void utils::closeText(char* &text)
{
    if(text > PTR(MaxValue_, char))
        free(text);
    
    text = PTR(EndWork, char);
}

long utils::getFileSize(FILE* file)
{
    assert(file);

    long size = 0L;

    long cur_pos = ftell(file);

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, cur_pos, SEEK_SET);

    return size;
}

char** utils::splitByLines(char* text, size_t linesCount)
{
    char** lines = (char**) calloc(linesCount, sizeof(char*));
    size_t counter = 0;

    while(*text)
    {
        lines[counter++] = text;

        while(*text != '\n' && *text != '\0') ++text;

        if(*text == '\n')
        {
            *text = '\0';
            text++;
        }
    }

    return lines;
}

char* utils::copyStr(const char* line)
{
    size_t size = strlen(line) + 1;
    char* buf = (char*) calloc(size, sizeof(char));

    if(buf == nullptr)
        return nullptr;

    strcpy(buf, line);
    return buf;
}