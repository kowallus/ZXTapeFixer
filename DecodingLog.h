#ifndef ZXTAPERFIXER_DECODINGLOG_H
#define ZXTAPERFIXER_DECODINGLOG_H

#include <string>
#include <fstream>

using namespace std;

class DecodingLog {
    std::fstream* fout;
    std::ostream* out;

    void log(string txt);

public:
    DecodingLog(string fileName, std::ostream* out);

    virtual ~DecodingLog();

    void logBlockEnd(int lastByteEND, string destCurrentSample, int bytes, int lastByte, int XORByte, int olderByte,
                     int byte);

    void logBlockStart(unsigned int currentSample, string destCurrentSample);

    void logLargePulse(unsigned int currentSample, string destCurrentSample);
};


#endif //ZXTAPERFIXER_DECODINGLOG_H
