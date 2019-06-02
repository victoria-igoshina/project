#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <getopt.h>


void PrintHelp() {
    std::cout <<
        "--outputFile <fname>:         Path to generated file        REQUIRED\n"
        "--numStrings <num>:           Number of strings in file     REQUIRED\n"        
        "--numLetters <num>:           Path to file with test text   REQUIRED\n"
        "--help:                       Show help\n";
    exit(1);
}


template <typename TIntegerType>
TIntegerType FromString(const std::string& str) = delete;


template <>
int FromString<int>(const std::string& str) {
    return std::stoi(str);
}


template <>
long long FromString<long long>(const std::string& str) {
    return std::stoll(str);
}


template <typename TIntegerType>
void ProcessArgs(int argc, char** argv, std::string& outputFile, TIntegerType& numStrings, TIntegerType& numLetters) {
    const char* const short_opts = "o:n:l:h";
    const option long_opts[] = {
            {"outputFile", required_argument, nullptr, 'o'},
            {"numStrings", required_argument, nullptr, 'n'},
            {"numLetters", required_argument, nullptr, 'l'},
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
        case 'n':
            numStrings = FromString<TIntegerType>(std::string(optarg));
            break;
        case 'l':
            numLetters = FromString<TIntegerType>(std::string(optarg));
            break;
        case 'h': // -h or --help
        case '?': // Unrecognized option
        default:
            PrintHelp();
            break;
        }
    }
}


template <typename TIntegerType>   
void CheckOptions(const std::string& outputFile, const TIntegerType& numStrings, const TIntegerType& numLetters) {
    if (outputFile.empty()) {
       std::cout << "-o or --outputFile is mandatory!" << std::endl;
       exit(1);
    }
    if (numStrings == 0) {
       std::cout << "-n or --numStrings is mandatory!" << std::endl;
       exit(1);
    }
    if (numLetters == 0) {
       std::cout << "-l or --numLetters is mandatory!" << std::endl;
       exit(1);
    }
}


template <class TFilePtr, typename TIntegerType>
void GenerateRandomFile(TFilePtr filePtr, const TIntegerType& numStrings, const TIntegerType& numLetters) {
    // генерурием строки с символами #32 - #126 включительно

    std::random_device randomCharDevice;
    std::mt19937 randomCharEngine(randomCharDevice());
    std::uniform_int_distribution<short> charDistribution(32, 126);
    
    std::random_device randomLengthDevice;
    std::mt19937 randomLengthEngine(randomLengthDevice());
    std::uniform_int_distribution<TIntegerType> lengthDistribution(0, numLetters);
    
    for (TIntegerType strIdx = 0; strIdx < numStrings; ++strIdx) {
        TIntegerType randomLength = lengthDistribution(randomLengthEngine);
        for (TIntegerType charIdx = 0; charIdx < randomLength; ++charIdx) {
            char randomChar = static_cast<char>(charDistribution(randomCharEngine));
            (*filePtr) << randomChar;
        }
        (*filePtr) << std::endl;
    }
}


int main(int argc, char **argv)
{
    typedef long long TIntegerType;
    typedef std::shared_ptr<std::fstream> TFstreamSharedPtr;

    std::string outputFile = "";
    TIntegerType numStrings = 0;
    TIntegerType numLetters = 0;

    ProcessArgs<TIntegerType>(argc, argv, outputFile, numStrings, numLetters);
    CheckOptions<TIntegerType>(outputFile, numStrings, numLetters);

    TFstreamSharedPtr filePtr = TFstreamSharedPtr(new std::fstream(outputFile, std::fstream::out));
    
    GenerateRandomFile<TFstreamSharedPtr, TIntegerType>(
        filePtr, numStrings, numLetters
    );
    
    filePtr->close();
   
	return 0;
}
