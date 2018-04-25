//---------------------------------------------------------------------------

#ifndef TapeFixH
#define TapeFixH
//---------------------------------------------------------------------------

#include "ErrorsDumper.h"
#include "DecodingLog.h"
#include "TapeCoder.h"
#include "utils/wave.h"

class TapeReconstructor {

private:
    int bytes;
    int blockPart;
    int XORByte;
    int olderByte;
    int lastByte;
    int lastByteEND;
    int byte;
    int bit;
    int bitCount;
    int pulseCount;
    int badCount;
    unsigned int i;
    int signalType;
    unsigned int lastSignalCount;

    void dodajBit(int bit, int fullSignal);

public:
    TapeReconstructor(WaveFile* source, TapeCoder* destCoder, DecodingLog* decLog, ErrorsDumper* errDump, int leftDelay, bool leftChan, bool rightChan);
    ~TapeReconstructor();

};

#endif
