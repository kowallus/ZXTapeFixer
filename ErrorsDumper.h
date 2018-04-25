#ifndef ZXTAPERFIXER_ERRORSDUMPER_H
#define ZXTAPERFIXER_ERRORSDUMPER_H


class ErrorsDumper {
//    memoErr->Lines->SaveToFile(AnsiString(DestName)+"Err.log");
public:
    void logError(unsigned int currentSample, unsigned int destCurrentSample, unsigned int signalCount,
                      unsigned int lastSignalCount, float zero, int bytes);
};


#endif //ZXTAPERFIXER_ERRORSDUMPER_H
