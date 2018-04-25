//---------------------------------------------------------------------------

#ifndef TapeFixH
#define TapeFixH
//---------------------------------------------------------------------------

static const int SIGNAL_BUFFER_SIZE = 50;

#include "ErrorsDumper.h"
#include "DecodingLog.h"
#include "TapeCoder.h"
#include "utils/wave.h"

class TapeReconstructor {

private:

    int blockPart;

    // used in writeBit method:
    int XORByte;
    int olderByte;
    int lastByte;
    int lastByteEND;
    int byte;
    int bit;
    int bitCount;
    int bytes;
    unsigned int lastSignalCount;

    int signalType;

    int pulseCount;
    int badCount;
    unsigned int i;

    const int sigAvg = 30;
    const int minGood = 29;

    TapeCoder* destCoder;

    void writeBit(int bit, int fullSignal);

public:
    TapeReconstructor(WaveFile* source, TapeCoder* destCoder, DecodingLog* decLog, ErrorsDumper* errDump, int leftDelay);

    ~TapeReconstructor();
};

#endif
