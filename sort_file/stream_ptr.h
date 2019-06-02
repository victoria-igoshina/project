#pragma once

#include <iostream>

template<class TStream, typename TIntegerType>
class TStreamPtr {
    private:
    protected:
        TStream Stream;

        TStreamPtr ()
            :Stream(NULL)
        {}
        TStreamPtr(const TStream& stream)
            :Stream(stream)
        {}
        ~TStreamPtr() {}

    public:
        void Close() {
            Stream->close();
        }
};

template<class TStream, typename TIntegerType>
class TInputStreamPtr: public TStreamPtr<TStream, TIntegerType> {
    public:
        TInputStreamPtr ()
            : TStreamPtr<TStream, TIntegerType>()
        {}
        TInputStreamPtr(const TStream& stream)
            : TStreamPtr<TStream, TIntegerType>(stream)
        {}
        ~TInputStreamPtr() {}

        void ShiftSomeLetters(const TIntegerType& shiftCounts) {
            this->Stream->clear();
            this->Stream->seekg(shiftCounts);
        }

        char GetNextLetter() {
            char c;
            if (this->Stream->get(c)) {
                if (c == '\n') {
                    return 0;
                }
                return c;
            }
            return 0;
        }

        void ReadNumber(TIntegerType& buf) {
            *(this->Stream) >> buf;
        }

        bool IsEof() {
            return this->Stream->eof();
        }
};

template<class TStream, typename TIntegerType>
class TOutputStreamPtr: public TStreamPtr<TStream, TIntegerType> {
    public:
        TOutputStreamPtr ()
            : TStreamPtr<TStream, TIntegerType>()
        {}
        TOutputStreamPtr(const TStream& stream)
            : TStreamPtr<TStream, TIntegerType>(stream)
        {}
        ~TOutputStreamPtr() {}

        template <typename T>
        void Write(const T& c) {
            *(this->Stream) << c;
        }

        void WriteEndl() {
            *(this->Stream) << std::endl;
        }
};

