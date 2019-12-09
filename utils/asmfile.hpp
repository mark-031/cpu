#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<exception>

const int MAXWORDSPERLINE = 8;

#define SIZE_T "%lu"

class AsmLine
{
    private:
        size_t lineNumber;
        char** words;
        int    wordsNumber;

        int countWords_(const char* line_);

    public:
        const char* operator[](int index);

        void Init(char* line_, size_t lineNumber_);

        void Init(char** words_, int wordsNumber_, size_t lineNumber_);

        void Finalize();

        int getWordsNumber();

        void dump();

        void printline();
};

class AsmFile
{
    private:
        char*    buffer;
        size_t   bufferSize;
        size_t   wordsAmount;
        size_t   linesAmount;
        AsmLine* lines;
        char     state;

        int isSpace_(char item);

        int skipEmptyLines_(size_t &current, size_t &lineCounter);

        int standardizeContent_();

        int splitToLines_();

        int init_(FILE* file);

        size_t fileSize_(FILE* file);

    public:
        AsmFile(const char* path);

        AsmFile(FILE* file);

        ~AsmFile();

        size_t getLinesAmount();

        void dump();

        int checkState();

        AsmLine& operator[](size_t index);
};