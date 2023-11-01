#include "File.h"
#include "FileException.h"

#include <unistd.h> //close(), open(), read(), write()
#include <iostream> //cerr
#include <sstream>  //stringstream
#include <sys/sendfile.h> //sendfile()
#include <sys/types.h>    //fstat()
#include <sys/stat.h>     //fstat()

File::File(const char *path, int flags, int rights)
    :fd(open(path, flags, rights))
{
    if(fd == -1) {
        std::stringstream errStream;

        errStream << "cannot open file \"" << path << '\"';

        throw FileException(errStream.str().c_str());
    }
}

File::File(const char *path, int flags)
    :fd(open(path, flags))
{
    if(fd == -1) {
        std::stringstream errStream;

        errStream << "cannot open file \"" << path << '\"';

        throw FileException(errStream.str().c_str());
    }
}

size_t File::sendFile(File *from, File *to, size_t bytes) const {
    size_t res = 0;
    ssize_t sended = 0;
    off_t toPos = to->getPos();

    //цикл нужен, т. к. sendfile() не всегда отправляет все данные за раз
    while(res < bytes) {
        sended = sendfile(to->fd, from->fd, NULL, bytes - res);

        if(sended == -1)
            throw FileException("cannot sendfile");

        toPos += static_cast<off_t>(sended);

        //sendfile() с параметром offset = NULL изменяет позицию только в файловом дескрипторе out_fd
        //во втором файловом дескрипторе позицию приходится изменять вручную
        to->setPos(toPos);

        res += static_cast<size_t>(sended);
    }

    return res;
}

size_t File::read(void *buf, size_t bytes) {
    ssize_t res = ::read(fd, buf, bytes);

    if(res == -1)
        throw FileException("cannot read file");

    return static_cast<size_t>(res);
}

size_t File::write(const void *buf, size_t bytes) {
    ssize_t res = 0;

    if((res = ::write(fd, buf, bytes)) == -1)
        throw FileException("cannot write to file");

    return static_cast<size_t>(res);
}

//чтение в другой файл
size_t File::read(File *dest, size_t bytes) {
    return sendFile(this, dest, bytes);
}

//запись из другого файла
size_t File::write(File *src, size_t bytes) {
    return sendFile(src, this, bytes);
}

//чтение в другой файл целиком
size_t File::read(File *dest) {
    return read(dest, size());
}

//запись из другого файла целиком
size_t File::write(File *src) {
    return write(src, src->size());
}

//размер файла в байтах
size_t File::size() {
    struct stat fileInfo;

    if(fstat(fd, &fileInfo) == -1)
        throw FileException("cannot fstat");

    return static_cast<size_t>(fileInfo.st_size);
}

//получение позиции "курсора" (порядковй номер байта)
off_t File::getPos() const {
    off_t res = 0;

    if((res = lseek(fd, 0, SEEK_CUR)) == -1)
        throw FileException("cannot get file offset");
    
    return res;
}

//установка "курсора" в файле (порядковй номер байта)
void File::setPos(off_t pos) {
    if(lseek(fd, pos, SEEK_SET) == -1)
        throw FileException("cannot set file offset");
}

//закрытие файла
File::~File() {
    if(close(fd) == -1)
        std::cerr << "cannot close file\n";
}
