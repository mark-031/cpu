#include"namespaces.hpp"

const int AVERAGEWORDLEN    = 8;
const int INITNAMECOUNT     = 16;
const int MINREALLOCADDSIZE = 128;

#define SIZE_T "%lu"

struct name_t
{
    size_t value;
    char type;
    char word[1];
};

enum NamespaceType
{
    NamespaceLabel = 1,
    NamespaceFunc  = 2,
};

#define NTYPE(name) (char) Namespace##name

enum NamespaceStates
{
    NamespaceStateOk          = 0,
    NamespaceStateFinalized   = 1,
    NamespaceStateAllocErr    = 17,
    NamespaceStateBadCounter  = 29,
};

#define NSTATE(name) (char) NamespaceState##name

enum NamesapceErrCodes
{
    NamespaceAllocError = 19,
    NamespaceItemExist  = 27,
};

class Namespace_t
{
    private:
        name_t* buffer;
        size_t  bufferSize;

        size_t* items;
        size_t  itemsAmount;
        size_t  itemsCounter;

        char state;

        int bufferRealloc_()
        {
            size_t addSize = 2 * this->bufferSize;

            void* buf = nullptr;

            while(buf == nullptr && addSize > MINREALLOCADDSIZE)
            {
                addSize /= 2;
                buf = realloc(this->buffer, this->bufferSize + addSize);
            }

            if(buf == nullptr)
                return NamespaceAllocError;
            
            this->buffer = (name_t*) buf;
            this->bufferSize += addSize;
            printf("New size = " SIZE_T "\n", this->bufferSize);

            return 0;
        }

        int itemsRealloc_()
        {
            size_t addItemsAmmount = 2 * this->itemsAmount;

            void* buf = nullptr;

            while(buf == nullptr && addItemsAmmount > 0)
            {
                addItemsAmmount /= 2;
                buf = realloc(this->items, (this->itemsAmount + addItemsAmmount) * sizeof(size_t));
            }

            if(buf == nullptr)
                return NamespaceAllocError;

            this->items = (size_t*) buf;
            this->itemsAmount += addItemsAmmount;

            return 0;
        }

        int cantAddName_(size_t wordLen)
        {
            return this->bufferSize < this->items[this->itemsCounter] + NameSize(wordLen);
        }

        int itemsResizeCheck_()
        {
            if(this->itemsCounter + 1 == this->itemsAmount)
                return this->itemsRealloc_();
            
            return 0;
        }

        int bufferResizeCheck_(size_t wordLen)
        {
            if(this->cantAddName_(wordLen))
                return this->bufferRealloc_();

            return 0;
        }

        name_t* getItem_(size_t index)
        {
            size_t offset = this->items[index];
            return (name_t*) (((char*) this->buffer) + offset);
        }
    
    public:
        Namespace_t()
        {
            this->bufferSize = INITNAMECOUNT * NameSize(AVERAGEWORDLEN);
            this->buffer = (name_t*) calloc(bufferSize, 1);

            this->itemsAmount = INITNAMECOUNT;
            this->items = (size_t*) calloc(this->itemsAmount, sizeof(size_t));

            if(this->buffer == nullptr || this->items == nullptr)
                this->state = NSTATE(AllocErr);
            
            else
                this->state = NSTATE(Ok);

            this->itemsCounter = 0;

            this->items[this->itemsCounter] = 0;
        }
        
        ~Namespace_t()
        {
            this->bufferSize     = 0;
            this->itemsAmount    = 0;
            this->itemsCounter   = 0;

            this->state = NSTATE(Finalized);

            if(this->buffer != nullptr)
                free(this->buffer);
            
            if(this->items != nullptr)
                free(this->items);
        }

        int checkState()
        {
            if(this->state)
                return this->state;
            
            if(this->itemsCounter > this->itemsAmount)
                return this->state = NSTATE(BadCounter);
        }

        int addName(const char* word, size_t value, char type)
        {
            if(this->findName(word) != nullptr)
                return NamespaceItemExist;

            size_t wordLen = strlen(word) + 1;

            if(this->itemsResizeCheck_())
                return NamespaceAllocError;
            
            if(this->bufferResizeCheck_(wordLen))
                return NamespaceAllocError;

            name_t* current = this->getItem_(this->itemsCounter++);

            current->type  = type;
            current->value = value;

            strcpy(current->word, word);

            this->items[this->itemsCounter] = this->items[this->itemsCounter - 1] + NameSize(wordLen);

            return 0;
        }

        int printDump(const char* name = nullptr)
        {
            if(name == nullptr)
                printf("Dump for Namespace_t\n");
            else
                printf("Dump \"%s\"\n", name);
                
            printf("{\n");
            printf("    buffer[%p]\n", this->buffer);
            printf("    bufferSize   = " SIZE_T "\n\n", this->bufferSize);
            
            printf("    items[%p]\n", this->items);
            printf("    itemsCounter = " SIZE_T "\n", this->itemsCounter);
            printf("    itemsAmount  = " SIZE_T "\n\n", this->itemsAmount);

            printf("    state = %d\n", (int) this->state);

            for(size_t i = 0; i < this->itemsCounter; ++i)
            {
                name_t* current = this->getItem_(i);

                printf("\n");
                printf("    item[" SIZE_T "]\n", i);
                printf("    {\n");
                printf("        value = " SIZE_T "\n", current->value);
                printf("        type  = %d\n", current->type);
                printf("        word  = %s\n", current->word);
                printf("    }\n");
            }

            printf("}\n");

            return 0;
        }

        const name_t* findName(const char* word)
        {
            const name_t* current = nullptr;

            for(size_t i = 0; i < this->itemsCounter; ++i)
            {
                current = this->getItem_(i);

                if(!strcmp(current->word, word))
                    return current;
            }

            return nullptr;
        }
};

size_t NameSize(int wordLen)
{
    return sizeof(name_t) + (wordLen - 1) * sizeof(char);
}