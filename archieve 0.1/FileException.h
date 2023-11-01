#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H

#include <stdexcept>

class FileException : public std::exception {
private:

    char *_message;

public:

    FileException(const char *message);

    const char *what() const noexcept override;

    ~FileException();

    FileException(const FileException &) = delete;
    void operator=(const FileException &) = delete;
};

#endif