#pragma once

#include <cstdio>

template<class TFilePtr, typename TIntegerType>
class TFileString {
    protected:
        TFilePtr FilePtr;
        char Letter;
        TIntegerType CountLettersBeforeString;
        size_t Id;
        bool End;

        void SetLetter(const char& bufLetter) {
            Letter = bufLetter;
        }

    public:
        TFileString()
            : FilePtr(NULL)
            , CountLettersBeforeString(0)
            , Letter(0)
            , Id(0)
            , End(false)
        {}
        TFileString(const TFilePtr& filePtr, const TIntegerType& countLettersBeforeString = 0)
            : FilePtr(filePtr)
            , CountLettersBeforeString(countLettersBeforeString)
            , Letter(0)
            , Id(0)
            , End(false)
        {}
        ~TFileString() {}

        bool operator < (const TFileString& rightStringFile) const {
            if (Id < rightStringFile.GetId()) {
                return true;
            } else if (Id > rightStringFile.GetId()) {
                return false;
            } else if (rightStringFile.IsEnd()) {
                return false;
            } else if (End) {
                return true;
            } else {
                return Letter < rightStringFile.GetLetter();
            }
        }

        char GetNextLetter() {
            if (End) {
                return 0;
            }

            char bufLetter = FilePtr.GetNextLetter();

            if (bufLetter == 0) {
                End = true;
            }

            SetLetter(bufLetter);
            return bufLetter;
        }

        size_t GetId() const {
            return Id;
        }

        char GetLetter() const {
            return Letter;
        }

        TIntegerType GetCountLettersBeforeString() const {
            return CountLettersBeforeString;
        }

        void SetId(size_t id) {
            Id = id;
        }

        void SetFilePtr(const TFilePtr& filePtr) {
            FilePtr = filePtr;
        }

        void CloseFile() {
            FilePtr.Close();
        }

        bool IsEnd() const {
            return End;
        }

        template<typename TOutFilePtr>
        void PrintString(TOutFilePtr out) {
            FilePtr.ShiftSomeLetters(CountLettersBeforeString);
            End = false;

            char bufLetter = GetNextLetter();
            while (bufLetter != 0) { 
                out.Write(bufLetter);
                bufLetter = GetNextLetter();
            }
        }
};
