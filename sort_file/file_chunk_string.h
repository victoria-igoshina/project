#pragma once

#include "file_string.h"

template<class TFilePtr, typename TIntegerType>
class TFileChunkString: public TFileString<TFilePtr, TIntegerType> {
    private:
        TIntegerType CurrentString;
        TIntegerType CountStrings;

    public:
        TFileChunkString()
            : CurrentString(0)
            , CountStrings(0)
        {}
        TFileChunkString(const TFilePtr& filePtr, const TIntegerType& countStrings)
            : TFileString<TFilePtr, TIntegerType> (filePtr)
            , CountStrings(countStrings)
            , CurrentString(0)
        {}

        const TIntegerType& GetCountStrings() {
            return CountStrings;
        }

        const TIntegerType& GetCurrentString() {
            return CurrentString;
        }

        void FillDefaultValues() {
            this->End = false;
            this->SetId(0);
            this->SetLetter(0);
        }

        void FillNewPtr() {
            this->FilePtr.ShiftSomeLetters(this->GetCountLettersBeforeString());
            FillDefaultValues();
        }

        TIntegerType CalculateCountLettersInString() {
            FillNewPtr();
            TIntegerType countLettersInString = 0;
            char c = this->GetNextLetter();
            while (c != 0) {
                ++countLettersInString;
                c = this->GetNextLetter();
            }
            ++countLettersInString;
            return countLettersInString;
        }

        bool LessWhileReadingStrings(TFileChunkString<TFilePtr, TIntegerType>* middle) {
            bool less = false;
            bool end = false;

            middle->FillNewPtr();
            this->FillNewPtr();

            while (!end && !less) {
                middle->GetNextLetter();
                this->GetNextLetter();

                less = *this < *middle;
                if (*middle < *this) {
                    return false;
                }

                if (middle->IsEnd() && this->IsEnd()) {
                    end = true;
                }
            }

            return less;
        }

        TFileChunkString NextString() {
            ++CurrentString;
            this->CountLettersBeforeString += this->CalculateCountLettersInString();
            FillDefaultValues();
            return *this;
        }

        bool IsLastString() {
            return CurrentString == CountStrings;
        }
};

