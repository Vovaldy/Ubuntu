/*
ФОРМАТ АРХИВА:

(все данные заносятся в бинарном виде БЕЗ РАЗДЕЛИТЕЛЕЙ)

ДИРЕКТОРИЯ:
    <ContentType::DirectoryBegin>
        <size_t - длина наименования>
        <char[] - наименование директории без терминального ноля>
        <содержимое папки>
    <ContentType::DirectoryEnd>

ФАЙЛ:
    <ContentType::FileBegin>
        <size_t - длина наименования>
        <char[] - наименование файла без терминального ноля>
        <size_t - размер файла в байтах>
        <содержимое файла без сжатия>
*/

/*
оба метода класса работают рекурсивно, сохраняя текущий путь в поле path
если скомпилировать этот файл с флагом ECHO_PRINT, path будет выводиться
при добавлении/извлечении каждого файла
*/

#include "Archieve.h"
#include "FileException.h"

#ifdef ECHO_PRINT
#include <iostream> //std::cout
#endif

#include <string.h>    //strcpy(), memset()
#include <dirent.h>    //DIR
#include <sys/stat.h>  //mkdir()
#include <sys/types.h> //mkdir()

enum class ContentType {
    DirectoryBegin = 0, DirectoryEnd, FileBegin
};

Archieve::Archieve(const char *fname, int mode, int rights)
    :file(fname, mode, rights),
    _rights(rights)
{
    memset(path, 0, sizeof(path));
}

//добавление в архив наименования файла/директории (не пути, а именно наименования, оно берётся из path)
void Archieve::addName() {

    /*
    алгоритм работы:
    path: "[полный/путь/к/файлу/или/папке/]будет записано только это название[/]"

    перед наименованием записывается его длина (size_t)
    */

    size_t end = strlen(path);

    while (end > 0 && (path[end] == '/' || path[end] == '\0'))
        --end;

    size_t beg = end;

    while (beg > 0 && path[beg] != '/')
        --beg;

    if(path[beg] == '/')
        ++beg;

    size_t nameLen = end - beg + 1;

    file.write(&nameLen, sizeof(nameLen));

    file.write(path + beg, nameLen);
}

//добавление в архив файла (берётся из path) с его наименованием и размером
void Archieve::addFile() {
    ContentType flag = ContentType::FileBegin;

    File toAdd(path, O_RDONLY);

    file.write(&flag, sizeof(flag));

    addName();

    size_t sizeFile = toAdd.size();

#ifdef ECHO_PRINT
    std::cout << "adding: \"" << path << "\"\n";
#endif

    file.write(&sizeFile, sizeof(sizeFile));

    file.write(&toAdd, sizeFile);
}

//рекурсивное добавление файла/директории из path в архив
void Archieve::addRecursive() {
    DIR *dir                = nullptr;
    struct dirent *entry    = nullptr;
    ContentType contentType = ContentType::DirectoryBegin;

    //если добавляется только один файл
    if (!(dir = opendir(path))) {
        addFile();

        return;
    }

    //если задано корректное имя директории, а не просто обозначена текущая/вышестоящая директория,
    //это имя заносится в архив
    //если задана текущая/вышестоящая директория, по очереди добавляются её элементы
    bool correctFolderName = (strcmp(path, ".") != 0 && strcmp(path, "..") != 0);

    //если задано корректное имя директории, а не просто обозначена текущая/вышестоящая директория,
    //это имя заносится в архив
    //если задана текущая/вышестоящая директория, по очереди добавляются её элементы
    if(correctFolderName) {
        contentType = ContentType::DirectoryBegin;

        file.write(&contentType, sizeof(contentType));

        addName();
    }

    //обход содержимого директории
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0))
            continue;

        //добавление пути к элементу директории в path
        size_t len = strlen(path);
        path[len] = '/';
        strcpy(path + len + 1, entry->d_name);

        if (entry->d_type == DT_DIR)
            addRecursive(); //рекурсивное добавление поддиректории
        else
            addFile(); //добавление файла

        path[len] = '\0';
    }

    //если задано корректное имя директории, а не просто обозначена текущая/вышестоящая директория,
    //это имя заносится в архив
    //если задана текущая/вышестоящая директория, по очереди добавляются её элементы
    if(correctFolderName) {
        contentType = ContentType::DirectoryEnd;

        file.write(&contentType, sizeof(contentType));
    }

    closedir(dir); //закрытие директории
}

void Archieve::add(const char *fileOrDir) {
    strcpy(path, fileOrDir);

    addRecursive();
}

//извлечение наименования файла/директории из архива в path
void Archieve::extractName(size_t pathlen) {
    size_t nameLen = 0;

    file.read(&nameLen, sizeof(nameLen));

    if(!pathlen)
        pathlen = strlen(path);

    path[pathlen] = '/';

    file.read(path + pathlen + 1, nameLen);

    path[pathlen + 1 + nameLen] = '\0';
}

//извлечение файла из архива в path
void Archieve::extractFile() {
    size_t oldLen = strlen(path);

    extractName(oldLen);

    size_t extractedSize = 0;

    file.read(&extractedSize, sizeof(extractedSize));

    File extractedFile(path, O_WRONLY | O_CREAT, _rights);

#ifdef ECHO_PRINT
    std::cout << "extracting: \"" << path << "\"\n";
#endif

    extractedFile.write(&file, extractedSize);

    path[oldLen] = '\0';
}

//рекурсивное извлечение всего содержимого архива в директорию path
void Archieve::extractRecursive() {
    ContentType content = ContentType::FileBegin;

    //цикл до окончания файла
    while (true) {
        if(file.read(&content, sizeof(content)) == 0)
            break;

        if(content == ContentType::DirectoryEnd)
            break; //для вфхода из рекурсии

        if(content == ContentType::FileBegin)
            extractFile();

        if(content == ContentType::DirectoryBegin) {
            size_t oldLen = strlen(path);

            extractName(oldLen);

            if(mkdir(path, _rights) == -1)
                throw FileException("cannot create dir");

            extractRecursive();

            path[oldLen] = '\0';
        }
    }

}

void Archieve::extract(const char *directory) {
    strcpy(path, directory);

    file.setPos(0);

    extractRecursive();
}
