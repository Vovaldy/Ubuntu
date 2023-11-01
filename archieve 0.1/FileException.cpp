#include "FileException.h"

#include <string.h> //strcpy()

FileException::FileException(const char *message)
    :_message(new char[strlen(message) + 1])
{
    strcpy(_message, message);
}

const char *FileException::what() const noexcept {
    return _message;
}

FileException::~FileException() {
    delete[] _message;
}