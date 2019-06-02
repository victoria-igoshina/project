#pragma once

#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>

bool ProcessArgs(int argc, char** argv, std::string& inputFile, std::string& outputFile, std::string& tmpDir);
void CheckOptions(const std::string& inputFile, const std::string& outputFile, const std::string& tmpDir);

void PrintHelp() {
    std::cout <<
        "--inputFile <fname>:           Path to input file             REQUIRED\n"
        "--outputFile <fname>:          Path to file with result       REQUIRED\n"
        "--tmpDir <dir>:                Directory for temporary files  OPTIONAL\n"
        "--help:                        Show help\n";
    exit(1);
}

bool ProcessArgs(int argc, char** argv, std::string& inputFile, std::string& outputFile, std::string& tmpDir) {
    const char* const short_opts = "o:i:t:h";
    const option long_opts[] = {
            {"inputFile", required_argument, nullptr, 'i'},
            {"outputFile", required_argument, nullptr, 'o'},
            {"tmpDir", required_argument, nullptr, 't'},
            {"help", no_argument, nullptr, 'h'},
            {nullptr, no_argument, nullptr, 0}
    };

    while (auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) {
        if (opt == -1)
            break;

        switch (opt) {
        case 'o':
            outputFile = std::string(optarg);
            break;

        case 'i':
            inputFile = std::string(optarg);
            break;

        case 't':
            tmpDir = std::string(optarg);
            break;

        case 'h': // -h or --help
        case '?': // Unrecognized option
        default:
            PrintHelp();
            break;
        }
    }
}

void CheckOptions(const std::string& inputFile, const std::string& outputFile, const std::string& tmpDir) {
    struct stat info;

    if (stat(tmpDir.c_str(), &info) != 0 ) {
        std::cout << "cannot access " << tmpDir << std::endl;
        exit(1);
    } else if (!(info.st_mode & S_IFDIR)) {
        std::cout << tmpDir << " is not directory" << std::endl;
        exit(1);
    }
    
    // Check mandatory parameters:
    if (inputFile.empty()) {
        std:: cout << "-i or --inputFile is mandatory!" << std::endl;
        exit(1);
    }
    if (outputFile.empty()) {
        std:: cout << "-o or --outputFile is mandatory!" << std::endl;
        exit(1);
    }
}
