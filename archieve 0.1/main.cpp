/*
Программа - архиватор

использование:
    распаковка:
        ./archieve -x archieveFileName directoryToExtract
        ./archieve --extract archieveFileName directoryToExtract
    
    архивация:
        ./archieve -a fileOrDirName0 fileOrDirName1 -o archieveFileName
        ./archieve --archieve fileOrDirName0 fileOrDirName1 -o archieveFileName
        ./archieve -o archieveFileName fileOrDirName0 fileOrDirName1
*/

#include "FileException.h"
#include "Archieve.h"

#include <string.h> //strcmp()
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) try {
    if(argc < 2)
        throw std::invalid_argument("ожидаются аргументы");

    if(!strcmp(argv[1], "-x") || !strcmp(argv[1], "--extract")) {
        if(argc > 4)
            throw std::invalid_argument("аргументов слишком много: archieve --extract inputFile outputDir");
        
        if(argc < 4)
            throw std::invalid_argument("аргументов слишком мало: archieve --extract inputFile outputDir");

        Archieve arc(argv[2], O_RDONLY);
        arc.extract(argv[3]);
    }
    else if(!strcmp(argv[1], "-o") || !strcmp(argv[1], "-a") || !strcmp(argv[1], "--archieve")) {
        if(argc < 4) {
            if(!strcmp(argv[1], "-o"))
                throw std::invalid_argument("аргументов слишком мало: archieve -o outputFile inputFileOrDir0 inputFileOrDir1 ...");
            else
                throw std::invalid_argument("аргументов слишком мало: archieve --archieve inputFileOrDir0 inputFileOrDir1 ... -o outputFile ");
        }

        int indexFlag = -1;

        for(int i = 1; i < argc; ++i)
            if(!strcmp(argv[i], "-o")) {
                if(i + 1 >= argc)
                    throw std::invalid_argument("после ключа \"-o\" ожидается название архива");
                
                indexFlag = i;

                break;
            }

        if(indexFlag == -1)
            throw std::invalid_argument("нужно указать выходной файл (после ключа -o)");

        Archieve arc(argv[indexFlag + 1], O_WRONLY | O_CREAT | O_TRUNC);
        
        for(int i = 2; i < argc; ++i) {
            if(i == indexFlag || i == (indexFlag + 1))
                continue;

            arc.add(argv[i]);
        }
    }
    else
        throw std::invalid_argument("первый аргумент - режим работы --extract (-x) или --archieve (-a)");

/*
#ifdef ARCHIEVER
    Archieve arc(argv[2]);
    arc.add(argv[1]);
#endif

#ifdef EXTRACTOR
    Archieve arc(argv[1]);
    arc.extract(argv[2]);
#endif
*/

    return 0;
}
catch(const std::exception &ex) {
    cerr << "ОШИБКА: " << ex.what() << endl;

    return -1;
}