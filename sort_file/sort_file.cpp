#include <fstream>
#include <cstdio>
#include <string>

#include "create_chunks.h"
#include "merge_chunks.h"
#include "files_name.h"
#include "opt.h"

const size_t MAX_AMOUNT_IN_FIRST_CHUNKS = 100;
const size_t MAX_AMOUNT_MERGE_CHUNKS = 100;

typedef std::ifstream TInputStream;
typedef std::ofstream TOutputStream;

typedef long long TIntegerType;

int main(int argc, char **argv) {
    std::string inputFile = "";
    std::string outputFile = "";
    std::string tmpDir = "";

    ProcessArgs(argc, argv, inputFile, outputFile, tmpDir);
    CheckOptions(inputFile, outputFile, tmpDir);

    const TFilesNames filesNames(inputFile, outputFile, tmpDir);

    size_t numberChunks = CreateChunks<TInputStream, TOutputStream, TIntegerType>(filesNames, MAX_AMOUNT_IN_FIRST_CHUNKS);
    MergeChunks<TInputStream, TOutputStream, TIntegerType>(filesNames, numberChunks, MAX_AMOUNT_MERGE_CHUNKS);

    return 0;
}

