#ifndef FILE_H
#define FILE_H

#include <stddef.h> //size_t
#include <fcntl.h>  //O_WRONLY, O_CREAT, O_TRUNC, S_IRWXU, ...

//обёртка некоторых "сишных" файловых опреаций
//при возникновении ошибок выкидывает исключение "FileException"
class File {
private:

    int fd;

    size_t sendFile(File *from, File *to, size_t bytes) const;

public:

    File(const char *path, int flags, int rights);
    File(const char *path, int flags);

    size_t read(void *buf, size_t bytes);

    //чтение в другой файл
    size_t read(File *dest, size_t bytes);

    //чтение в другой файл целиком
    size_t read(File *dest);

    size_t write(const void *buf, size_t bytes);

    //запись из другого файла
    size_t write(File *src, size_t bytes);

    //запись из другого файла целиком
    size_t write(File *src);

    //размер файла в байтах
    size_t size();

    //установка "курсора" в файле (порядковй номер байта)
    void setPos(off_t pos);

    //получение позиции "курсора" (порядковй номер байта)
    off_t getPos() const;

    //закрытие файла
    ~File();

    File(const File &) = delete;

    void operator=(const File &) = delete;
};

#endif