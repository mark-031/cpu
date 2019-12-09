#include"asmfile.hpp"

enum AsmFileStates
{
    AsmFileStateOk              = 0,
    AsmFileStateCorruptedFile   = 5,
    AsmFileStateEmptyFile       = 6,
};

#define AFSTATE(name) (char) AsmFileState##name

// AsmLine methods

int AsmLine::countWords_(const char* line_)
{
    int count = 1;

    for( ; *line_ != '\0'; ++line_)
        if(*line_ == ' ') ++count;
    
    return count;
}

const char* AsmLine::operator[](int index)
{
    assert(index < this->wordsNumber);

    if(index >= this->wordsNumber)
        throw;

    return this->words[index];
}

void AsmLine::Init(char* line_, size_t lineNumber_)
{
    assert(line_ != nullptr);

    this->lineNumber  = lineNumber_;
    this->wordsNumber = countWords_(line_);

    this->words = new char*[this->wordsNumber];

    for(int i = 0; *line_ != '\0'; ++i)
    {
        assert(i < this->wordsNumber);

        this->words[i] = line_;
        while(*line_ != ' ' && *line_ != '\0') ++line_;

        if(*line_ == ' ')
            *(line_++) = '\0';
    }
}

void AsmLine::Init(char** words_, int wordsNumber_, size_t lineNumber_)
{
    assert(words_ != nullptr);

    this->wordsNumber = wordsNumber_;
    this->lineNumber = lineNumber_;

    this->words = new char*[wordsNumber_];
    for(int i = 0; i < wordsNumber_; ++i)
        this->words[i] = words_[i];
}

void AsmLine::Finalize()
{
    delete this->words;
}

int AsmLine::getWordsNumber()
{
    return this->wordsNumber;
}

void AsmLine::dump()
{
    printf("AsmLine <%p>\n", this);
    printf("{\n");
    printf("    lineNumber  = " SIZE_T "\n", this->lineNumber);
    printf("    wordsNumber = %d\n", this->wordsNumber);
    printf("    words:");

    for(int i = 0; i < this->wordsNumber; ++i)
        printf(" '%s'", this->words[i]);
    printf("\n");

    printf("}\n");
}

void AsmLine::printline()
{
    printf("%4lu|", this->lineNumber);
    for(int i = 0; i < this->wordsNumber; ++i)
    {
        printf(" %s", this->words[i]);
    }
    printf("\n");
}

// AsmFile methods

int AsmFile::isSpace_(char item)
{
    if(item == ' ' || item == '\t' || item == '\n')
        return 1;
    
    return 0;
}

int AsmFile::standardizeContent_()
{
    size_t processed = 0;
    size_t done = 0;

    this->linesAmount = 1;
    this->wordsAmount = 1;

    while(isSpace_(this->buffer[processed]))
        if(this->buffer[processed] == '\n')
            this->buffer[done++] = this->buffer[processed++];
        else
            ++processed;
    
    while(true)
    {
        if(this->buffer[processed] == '\n')
        {
            if(this->buffer[done - 1] != '\n')
            {
                ++this->linesAmount;
                ++this->wordsAmount;
            }

            this->buffer[done++] = this->buffer[processed++];
            continue;
        }

        else if(this->buffer[processed] == ' ' || this->buffer[processed] == '\t')
        {
            if(this->buffer[done - 1] == '\n' || this->buffer[done - 1] == ' ')
            {
                ++processed;
                continue;
            }

            this->buffer[done++] = ' ';

            ++this->wordsAmount;
            continue;
        }

        else if(this->buffer[processed] == '\0')
        {
            switch (this->buffer[done - 1])
            {
                case '\n':
                    --this->linesAmount;

                case ' ':
                    --this->wordsAmount;
                    this->buffer[done - 1] = '\0';
                    break;
                
                default:
                    this->buffer[done++] = '\0';
                    break;
            }

            break;
        }

        else if(this->buffer[processed] == ';')
        {
            while(this->buffer[processed] != '\n' && this->buffer[processed] != '\0') ++processed;
            continue;
        }

        this->buffer[done++] = this->buffer[processed++];
    }

    this->bufferSize = done;
    this->buffer = (char*) realloc(this->buffer, done);

    assert(this->buffer != nullptr);

    return 0;
}

int AsmFile::skipEmptyLines_(size_t &current, size_t &lineCounter)
{
    while(this->buffer[current] == '\n')
    {
        current++;
        lineCounter++;
    }

    return 0;
}

int AsmFile::splitToLines_()
{
    assert(this->buffer != nullptr);

    size_t infileLineCounter = 1;
    size_t lineCounter       = 0;

    size_t processed = 0;
    size_t done      = 0;

    char* words[MAXWORDSPERLINE] = {};
    int   wordsCounter = 0;

    this->skipEmptyLines_(processed, infileLineCounter);

    words[wordsCounter++] = this->buffer;

    char item = '\0';

    while(true)
    {
        assert(wordsCounter <= MAXWORDSPERLINE);

        item = this->buffer[processed++];

        switch (item)
        {
            case '\0':
            case '\n':
                this->lines[lineCounter++].Init(words, wordsCounter, infileLineCounter++);

                this->skipEmptyLines_(processed, infileLineCounter);
                
                wordsCounter = 0;

            case ' ':
                this->buffer[done++] = '\0';
                words[wordsCounter++] = this->buffer + done;
                break;
            
            default:
                this->buffer[done++] = item;
                break;
        }

        if(item == '\0')
            break;
    }

    assert(this->linesAmount == lineCounter);

    if(done == 0)
    {
        free(this->buffer);
        this->state = AFSTATE(EmptyFile);
    }

    this->buffer = (char*) realloc(this->buffer, done);
    this->bufferSize = done;
    assert(this->buffer != nullptr);

    return 0;
}

int AsmFile::init_(FILE* file)
{
    this->state = AFSTATE(Ok);

    this->bufferSize = fileSize_(file);
    this->buffer = (char*) calloc(this->bufferSize, sizeof(char));

    assert(this->buffer != nullptr);

    fread(this->buffer, sizeof(char), this->bufferSize, file);

    this->wordsAmount = 0;
    this->linesAmount = 0;

    this->standardizeContent_();

    if(this->checkState() != AFSTATE(Ok))
        return 1;

    this->lines = new AsmLine[this->linesAmount];

    this->splitToLines_();

    return 0;
}

size_t AsmFile::fileSize_(FILE* file)
{
    size_t size = 0;
    size_t current = ftell(file);

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, current, SEEK_SET);

    return size;
}

AsmFile::AsmFile(const char* path)
{
    FILE* file = nullptr;

    file = fopen(path, "r");

    assert(file != nullptr);

    this->init_(file);

    fclose(file);
}

AsmFile::AsmFile(FILE* file)
{
    this->init_(file);
}

AsmFile::~AsmFile()
{
    if(this->checkState() != AFSTATE(Ok))
        return;
    
    assert(this->buffer != nullptr);
    assert(this->lines  != nullptr);
    
    for(size_t i = 0; i < this->linesAmount; ++i)
        this->lines[i].Finalize();
    
    delete this->buffer;
    delete this->lines;
}

size_t AsmFile::getLinesAmount()
{
    return this->linesAmount;
}

void AsmFile::dump()
{
    printf("buffer[%p]\n", this->buffer);
    printf("lines[%p]\n", this->lines);
    printf("{\n");

    for(size_t i = 0; i < this->linesAmount; ++i)
        this->lines[i].printline();

    printf("}\n");
    printf("wordsAmount = " SIZE_T "\n", this->wordsAmount);
    printf("linesAmount = " SIZE_T "\n", this->linesAmount);
    printf("bufferSize  = " SIZE_T "\n", this->bufferSize);
}

int AsmFile::checkState()
{
    return (int) this->state;
}

AsmLine& AsmFile::operator[](size_t index)
{
    assert(index < this->linesAmount);

    if(index >= this->linesAmount)
        throw;

    return this->lines[index];
}