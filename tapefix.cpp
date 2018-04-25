#include "tapefix.h"

// best version - requires high-pass filter (>150Hz) preprocessing

/*    */

TapeReconstructor::TapeReconstructor(WaveFile* source, TapeCoder* destCoder, DecodingLog* decLog, ErrorsDumper* errDump, int leftDelay):
    destCoder(destCoder) {
    WaveFile *source2ndChannel = new WaveFile();

    bytes = -2;
    blockPart = 0;
    XORByte = 0;
    olderByte = 0;
    lastByte = 0;
    lastByteEND = 0;
    byte = 0;
    bit = -1;
    bitCount = 8;
    pulseCount = 0;
    badCount = 0;

    float sample;
    float zero = 0;

    int goodSignal[SIGNAL_BUFFER_SIZE] = {0};

    signalType = 0;
    unsigned int signalCount = 0;
    lastSignalCount = 0;
    int lastSignal = 0;
    bool signalChange;
    int GoodSignalCount = 0;

    if (source->GetNumChannels() > 1) {
        source2ndChannel->CopyFrom(*source);
        source2ndChannel->ReadSample(sample);
        if (leftDelay > 0) {
            for (int i = 0; i < leftDelay * 2; i++)
                source2ndChannel->ReadSample(sample);
        } else {
            leftDelay = -leftDelay;
            for (int i = 0; i < leftDelay * 2; i++)
                source->ReadSample(sample);
        }
    }

    for (i = 0; i < source->GetNumSamples() - leftDelay - 1; i++) {

        source->ReadSample(sample);
        if (source->GetNumChannels() > 1) {
            float stereo;
            source2ndChannel->ReadSample(stereo);
            sample += stereo;
        }
        sample += zero;

// modification o signal amplitude
        if (sample > 0) {
            signalChange = signalType != 1;
            signalType = 1;
            if (blockPart < 2)
                if ((GoodSignalCount > minGood) && (blockPart > 0)) destCoder->writeSample(0.08);
                else destCoder->writeZero();
        } else {
            signalChange = signalType != -1;
            signalType = -1;
            if (blockPart < 2)
                if ((GoodSignalCount > minGood) && (blockPart > 0)) destCoder->writeSample(-0.08);
                else destCoder->writeZero();
        }

//  signal modification
        if (signalChange) {
            if ((badCount > 2) || (GoodSignalCount <= minGood)) {
                if (blockPart == 2) {
                    if (bit == -1) for (unsigned int k = 0; k < signalCount; k++) destCoder->writeZero();
                    else for (unsigned int k = 0; k < lastSignalCount + signalCount; k++) destCoder->writeZero();
                    decLog->logBlockEnd(lastByteEND, destCoder->getDestOffset(i), bytes, lastByte, XORByte, olderByte, byte);
                }
                zero = 0;
                blockPart = 0;
                bit = -1;
                pulseCount = 0;
                badCount = 0;
            } else {
                switch (blockPart) {
                    case 1:
                        if ((signalCount > 3) && (signalCount < 17) && ((pulseCount > 20) || (bit == 0)))
                            if (++bit == 1) {
                                decLog->logBlockStart(i, destCoder->getDestOffset(i));
                                blockPart = 2;
                                bit = -1;
                                bytes = -2;
                                lastByte = 0;
                                byte = 0;
                                XORByte = 0;
                                bitCount = 8;
                                lastSignalCount = 0;
                            } else if (signalCount > 13)
                                decLog->logLargePulse(i, destCoder->getDestOffset(i));
                    case 0:
                        if ((signalCount > 17) && (signalCount < 35)) {
                            bit = -1;
                            if (++pulseCount == 20)
                                blockPart = 1;
                        } else pulseCount = 0;
                        break;
                    case 2:
                        int fullSignal = lastSignalCount + signalCount;
                        if ((signalCount > 5) && (signalCount < 27) && (fullSignal < SIGNAL_BUFFER_SIZE) &&
                            ((fullSignal > 15) || (bit == -1))) {
                            if (lastSignalCount == 0)
                                lastSignalCount = signalCount;
                            else {
                                if (fullSignal < 32) {
                                    if ((lastSignalCount < 17) && (signalCount < 17))
                                        badCount = 0;
                                    writeBit(0, fullSignal);
                                } else {
                                    if ((lastSignalCount > 15) && (signalCount > 15))
                                        badCount = 0;
                                    writeBit(1, fullSignal);
                                }
                            }
                        } else {
                            if ((signalCount < 5) || (signalCount > 26))
                                errDump->logError(i, destCoder->getDestOffset(i), signalCount, lastSignalCount, zero, bytes);

                            if (lastSignalCount == 0)
                                lastSignalCount = signalCount;
                            else {
                                if (signalCount > 28) fullSignal -= 20;
                                if (fullSignal < 32) writeBit(0, fullSignal);
                                else writeBit(1, fullSignal);
                            }

                            if (signalCount > 28) {
                                // write bit 0 - copy of the next code
                                lastSignalCount = lastSignalCount ? 0 : 1;

                                writeBit(0, 20);
                                lastSignalCount = signalCount - 20;
                            }

                            ++badCount; // ending detection
                        }

                        break;
                }
            }

            if (++lastSignal == sigAvg) lastSignal = 0;
            if ((signalCount > source->GetSampleRate() / 700) ||
                (signalCount < source->GetSampleRate() / 15000)) {
                GoodSignalCount -= goodSignal[lastSignal];
                goodSignal[lastSignal] = 0;
                if (GoodSignalCount < minGood)
                    destCoder->possibleSplit(i);
            } else {
                GoodSignalCount -= goodSignal[lastSignal];
                GoodSignalCount += (goodSignal[lastSignal] = 1);
            }
            signalCount = 1;
        } else signalCount++;

        for (int j = 1; j < source->GetNumChannels(); j++) {
            source->ReadSample(sample);
            source2ndChannel->ReadSample(sample);
        }
    }

    if (source->GetNumChannels() > 1) source2ndChannel->Close();
    delete (source2ndChannel);

}

void TapeReconstructor::writeBit(int bit, int fullSignal) {
    byte += (bit<<(--bitCount));

    if (bitCount == 0) {
        lastByteEND = i;
        XORByte ^= lastByte;
        olderByte = lastByte;
        lastByte = byte;
        byte = 0;
        bitCount = 8;
        bytes++;
    }

    destCoder->writeBit(lastSignalCount, signalType, fullSignal);

    lastSignalCount = 0;
}

TapeReconstructor::~TapeReconstructor() {

}
