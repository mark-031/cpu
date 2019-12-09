namespace utils
{
    /**
     * Load IO files by arguments
     * @param argc Count of argc
     * @param argv Array of args
     * @param input Link to input FILE*
     * @param output Link to output FILE*
     */
    void loadIoFiles(int argc, char* argv[], FILE* &input, FILE* &output);

    /**
     * Close IO files
     * @param input Link to input FILE*
     * @param output Link to output FILE*
     */
    void closeIoFiles(FILE* &input, FILE* &output);

    /**
     * Read text from file
     * @param file File
     * @return Pointer to star of the text or error pointer
     */
    char* readText(FILE* file);

    /**
     * Finishes work with text opened by readText(...)
     * @param file Link to start of the text
     */
    void closeText(char* &text);

    /**
     * Get size of file in bytes
     * @param file File
     * @return Size of file
     */
    long getFileSize(FILE* file);

    /**
     * Split text by lines
     * @param test Text
     * @param linesCount Count of lines in text
     * @return Array of pointers on lines
     */
    char** splitByLines(char* text, size_t linesCount);

    /**
     * Copy line
     * @param line Line
     * @return Pointer on start of copied line
     */
    char* copyStr(const char* line);
}