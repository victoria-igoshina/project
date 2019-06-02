#pragma once

#include <string>

class TFilesNames {
    private:
        std::string InputFile;
        std::string OutputFile;
        std::string TmpDir;

        TFilesNames() = delete;

    public:
        TFilesNames(const std::string& inputFile, const std::string& outputFile, const std::string& tmpDir)
            : InputFile(inputFile)
            , OutputFile(outputFile)
            , TmpDir(tmpDir)
        {}
        ~TFilesNames() {}

        std::string GetInputFile() const {
            return InputFile;
        }

        std::string GetResultFile() const {
            return OutputFile;
        }

        std::string GetOutputIterationChunkFile(size_t iteration, size_t chunk) const {
            return TmpDir + "/output_" +  std::to_string(iteration) + "_" + std::to_string(chunk);
        }
        std::string GetOutputIterationChunkCountFile(size_t iteration, size_t chunk) const {
            return TmpDir + "/output_" + std::to_string(iteration) + "_" + std::to_string(chunk) + "_count";
        }
};
