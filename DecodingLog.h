#ifndef ZXTAPERFIXER_DECODINGLOG_H
#define ZXTAPERFIXER_DECODINGLOG_H


class DecodingLog {
//    memo->Lines->SaveToFile(AnsiString(DestName)+".log");
public:
    void logBlockEnd(int lastByteEND, unsigned int destCurrentSample, int bytes, int lastByte, int XORByte, int olderByte,
                         int byte);

    void logBlockStart(unsigned int currentSample, unsigned int destCurrentSample);

    void logLargePulse(unsigned int currentSample, unsigned int destCurrentSample);
};


#endif //ZXTAPERFIXER_DECODINGLOG_H
