#include "fsUtils.h"

std::string getFileExtension(const std::string& filePath) 
{
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos == std::string::npos || dotPos == filePath.length() - 1) 
    {
        return ""; // No extension found or the dot is at the end of the string
    }

    return filePath.substr(dotPos + 1);
}