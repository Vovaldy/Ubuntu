#ifndef ARCHIEVE_H
#define ARCHIEVE_H

#include "File.h"

#include <linux/limits.h> //PATH_MAX

//файл-архив (создание/добавление элеметов, распаковка)
class Archieve {
private:

    File file;

    int _rights;

    char path[PATH_MAX];

    //добавление в архив наименования файла/директории (не пути, а именно наименования, оно берётся из path)
    void addName();

    //добавление в архив файла (берётся из path) с его наименованием и размером
    void addFile();

    //рекурсивное добавление файла/директории из path в архив
    void addRecursive();

    //извлечение наименования файла/директории из архива в path
    void extractName(size_t pathlen = 0);

    //извлечение файла из архива в path
    void extractFile();

    //рекурсивное извлечение всего содержимого архива в директорию path
    void extractRecursive();

public:

    Archieve(const char *fname, int mode = O_RDWR | O_CREAT, int rights = S_IRWXU);

    void add(const char *fileOrDir);

    void extract(const char *directory);
};

#endif