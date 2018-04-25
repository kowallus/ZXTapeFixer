#ifndef ZXTAPERFIXER_TAPECODER_H
#define ZXTAPERFIXER_TAPECODER_H

#include <string>
#include "utils/Wave.h"

using namespace std;

class TapeCoder {
private:
    string destName;
    int sampleRate;

    int maxPauseInSeconds = 0;
    int splitOutputWavesOverMB = 0;

    WaveFile* dest;
    int nrOrder = 0;

    string generateNextNameInfix();
    string getCurrentNameInfix();
    void openNewOutputDest();

    int zerosCount = 0;
    unsigned int currentAbsoluteStartOffset = 0;

public:
    TapeCoder(string destName, int sampleRate, int maxPauseInSeconds = 0, int splitOutputWavesOverMB = 0);

    void writeBit(int lastSignalCount, int signalType, int fullSignal);

    ~TapeCoder();

    void writeSample(double value);

    void writeZero();

    string getDestOffset(unsigned int sourceOffset);

    void possibleSplit(unsigned int sourceOffset);

};


#endif //ZXTAPERFIXER_TAPECODER_H
