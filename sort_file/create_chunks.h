#pragma once

#include <vector>
#include <memory>

#include "file_string.h"
#include "sort_chunk.h"
#include "stream_ptr.h"
#include "files_name.h"

template<typename TInputStream, typename TOutputStream, typename TIntegerType>
size_t CreateChunks(const TFilesNames& filesNames, const size_t maxAmountInChunks);

template<typename TInputStream, typename TOutputStream, typename TIntegerType>
size_t CreateChunks(const TFilesNames& filesNames, const size_t maxAmountInChunks) {
    typedef std::shared_ptr<TInputStream> TSharedPtrInputStream;
    typedef std::shared_ptr<TOutputStream> TSharedPtrOutputStream;

    typedef TFileString<TInputStreamPtr<TSharedPtrInputStream, TIntegerType>, TIntegerType> TFileStringTypeName;

    std::vector<TFileStringTypeName> strings;
    strings.reserve(maxAmountInChunks);

    bool eof = false;

    TSharedPtrInputStream mainPtr(new TInputStream(filesNames.GetInputFile()));
    TInputStreamPtr<TSharedPtrInputStream, TIntegerType> mainStreamPtr(mainPtr);

    TIntegerType countBefore = 0;

    size_t numberChunks = 0;

    while (!eof) {
        strings.clear();

        for (size_t i = 0; i < maxAmountInChunks; ++i) {
            TSharedPtrInputStream inputPtr(new TInputStream(filesNames.GetInputFile()));
            TInputStreamPtr<TSharedPtrInputStream, TIntegerType> streamInputPtr(inputPtr);

            streamInputPtr.ShiftSomeLetters(countBefore);

            strings.push_back(TFileStringTypeName(streamInputPtr, countBefore));

            char c = mainStreamPtr.GetNextLetter();
            while (c) {
                c = mainStreamPtr.GetNextLetter();
                ++countBefore;
                if (c == 0) {
                    break;
                }
            }
            ++countBefore;

            if (mainStreamPtr.IsEof()) {
                strings.pop_back();
                eof = true;
                break;
            }
        }

        SortChunk(strings);

        if (strings.size() != 0) {
            TSharedPtrOutputStream outPtr(new TOutputStream(filesNames.GetOutputIterationChunkFile(0, numberChunks), TOutputStream::out));
            TOutputStreamPtr<TSharedPtrOutputStream, TIntegerType> steramOutPtr(outPtr);

            for (size_t i = 0; i < strings.size(); ++i) {
                strings[i].PrintString(steramOutPtr);
                if (i + 1 < strings.size()) {
                    steramOutPtr.WriteEndl();
                }
            }
            steramOutPtr.Close();

            TSharedPtrOutputStream outNumberPtr(new TOutputStream(filesNames.GetOutputIterationChunkCountFile(0, numberChunks), TOutputStream::out));
            TOutputStreamPtr<TSharedPtrOutputStream, TIntegerType> steramOutNumberPtr(outNumberPtr);
            steramOutNumberPtr.Write(std::to_string(strings.size()));
            steramOutNumberPtr.Close();

            ++numberChunks;
        }
    }
    return numberChunks;
}
