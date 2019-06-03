#include <vector>
#include <memory>

#include "files_name.h"
#include "stream_ptr.h"
#include "sort_chunk.h"
#include "files_name.h"
#include "file_chunk_string.h"

template<typename TInputStream, typename TOutputStream, typename TIntegerType>
void MergeChunks(const TFilesNames& filesNames, size_t numberChunks, size_t maxAmountMergeChunks);

template<class TChunkString>
typename std::vector<TChunkString>::iterator FindLowerBoundChunk(typename std::vector<TChunkString>::iterator begin
                                                                , typename std::vector<TChunkString>::iterator end
                                                                , TChunkString* newString) {
    if (begin == end) {
        return begin;
    }

    if (begin + 1 == end) {
        if (begin->LessWhileReadingStrings(newString)) {
            return end;
        }
        return begin;
    }

    typename std::vector<TChunkString>::iterator middle = begin + (end - begin) / 2;

    bool less = middle->LessWhileReadingStrings(newString);

    if (less) {
        return FindLowerBoundChunk(middle, end, newString);
    } else {
        return FindLowerBoundChunk(begin, middle, newString);
    }
}

template<class TChunkString, typename TIntegerType>
void InsertNewString(std::vector<TChunkString>& chunks) {
    if (chunks.size() == 0) {
        return;
    }

    if (chunks[0].IsLastString()) {
        for (int i = 1; i < chunks.size(); ++i) {
            chunks[i-1] = chunks[i];
        }
        chunks.pop_back();
        return;
    }

    TChunkString newString = chunks[0].NextString();

    auto iterForNewElement = FindLowerBoundChunk(chunks.begin() + 1, chunks.end(), &newString);
    size_t indexNewElement = 0;
    while (chunks.begin() + indexNewElement != iterForNewElement) {
        ++indexNewElement;
    }
    --indexNewElement;

    for (size_t i = 1; i <= indexNewElement; ++i) {
        chunks[i - 1] = chunks[i];
    }
    chunks[indexNewElement] = newString;
}

template<typename TInputStream, typename TOutputStream, typename TIntegerType>
void MergeIterationChunks(TIntegerType iteration, TIntegerType numberChunks, TIntegerType beginNumber, TIntegerType endNumber, const TFilesNames& filesNames) {
    typedef std::shared_ptr<TInputStream> TSharedPtrInputStream;
    typedef std::shared_ptr<TOutputStream> TSharedPtrOutputStream;

    typedef TFileChunkString<TInputStreamPtr<TSharedPtrInputStream, TIntegerType>, TIntegerType> TChunkStringTypeName;

    std::vector<TChunkStringTypeName> chunks;

    TIntegerType numberAllStrings = 0;

    for (TIntegerType i = beginNumber; i < endNumber; ++i) {
        TSharedPtrInputStream ptr(new TInputStream(filesNames.GetOutputIterationChunkFile(iteration, i)));
        TInputStreamPtr<TSharedPtrInputStream, TIntegerType> streamPtr(ptr);

        TSharedPtrInputStream ptrNumber(new TInputStream(filesNames.GetOutputIterationChunkCountFile(iteration, i)));
        TInputStreamPtr<TSharedPtrInputStream, TIntegerType> streamPtrNumber(ptrNumber);
        TIntegerType numberStrings;
        streamPtrNumber.ReadNumber(numberStrings);
        streamPtrNumber.Close();

        chunks.push_back(TChunkStringTypeName(streamPtr, numberStrings));
        numberAllStrings += numberStrings;
    }

    SortChunk(chunks);

    TSharedPtrOutputStream outPtr(new TOutputStream(filesNames.GetOutputIterationChunkFile(iteration + 1, numberChunks), TOutputStream::out));
    TOutputStreamPtr<TSharedPtrOutputStream, TIntegerType> streamOutPtr(outPtr);
    while (chunks.size() > 0) {
        chunks[0].PrintString(streamOutPtr);
        if (!chunks[0].IsLastString()) {
            streamOutPtr.WriteEndl();
        }
        InsertNewString<TChunkStringTypeName, TIntegerType>(chunks);
    }
    streamOutPtr.Close();

    TSharedPtrOutputStream outNumberPtr(new TOutputStream(filesNames.GetOutputIterationChunkCountFile(iteration + 1, numberChunks), TOutputStream::out));
    TOutputStreamPtr<TSharedPtrOutputStream, TIntegerType> steramOutNumberPtr(outNumberPtr);
    steramOutNumberPtr.Write(std::to_string(numberAllStrings));
    steramOutNumberPtr.Close();

    for (TIntegerType i = beginNumber; i < endNumber; ++i) {
        std::remove(filesNames.GetOutputIterationChunkFile(iteration, i).c_str());
        std::remove(filesNames.GetOutputIterationChunkCountFile(iteration, i).c_str());
    }

    for (size_t i = 0; i < chunks.size(); ++i) {
        chunks[i].CloseFile();
    }
}

template<typename TInputStream, typename TOutputStream, typename TIntegerType>
void MergeChunks(const TFilesNames& filesNames, size_t numberChunks, size_t maxAmountMergeChunks) {
    size_t iteration = 0;
    try {
        while (numberChunks > 1) {
            for (size_t i = 0; maxAmountMergeChunks * (i + 1) <= numberChunks; ++i) {
                MergeIterationChunks<TInputStream, TOutputStream, TIntegerType>(iteration, i, i * maxAmountMergeChunks,
                                                                                (i + 1) * maxAmountMergeChunks, filesNames);
            }
            if (numberChunks % maxAmountMergeChunks > 0) {
                MergeIterationChunks<TInputStream, TOutputStream, TIntegerType>(iteration, numberChunks / maxAmountMergeChunks,
                                                                                (numberChunks / maxAmountMergeChunks) * maxAmountMergeChunks, numberChunks, filesNames);
            }

            ++iteration;
            numberChunks = numberChunks % maxAmountMergeChunks ? numberChunks / maxAmountMergeChunks + 1 : numberChunks / maxAmountMergeChunks;
        }
    } catch (const std::exception& e) {
        for (TIntegerType i = 0; i < numberChunks; ++i) {
            std::remove(filesNames.GetOutputIterationChunkFile(iteration, i).c_str());
            std::remove(filesNames.GetOutputIterationChunkCountFile(iteration, i).c_str());
            std::remove(filesNames.GetOutputIterationChunkFile(iteration + 1, i).c_str());
            std::remove(filesNames.GetOutputIterationChunkCountFile(iteration + 1, i).c_str());
        }
        std::cerr << e.what() << std::endl;
    }
    std::rename(filesNames.GetOutputIterationChunkFile(iteration, 0).c_str(), filesNames.GetResultFile().c_str());
    std::remove(filesNames.GetOutputIterationChunkCountFile(iteration, 0).c_str());
}

