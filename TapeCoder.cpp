#include "TapeCoder.h"

#include <iostream>

static const string WAV_EXTENSION = ".wav";

TapeCoder::TapeCoder(string destName, int sampleRate, int maxPauseInSeconds, int splitOutputWavesOverMB):
    destName(destName),
    sampleRate(sampleRate),
    maxPauseInSeconds(maxPauseInSeconds),
    splitOutputWavesOverMB(splitOutputWavesOverMB) {
    dest = new WaveFile();
    openNewOutputDest();
}

TapeCoder::~TapeCoder() {
    dest->Close();
    delete(dest);
}

void TapeCoder::writeBit(int lastSignalCount, int signalType, int fullSignal) {
    if (lastSignalCount==0) {
        if (signalType==1) {
            for(int k = 0; k < fullSignal / 2; k++) writeSample(-0.1);
            for(int k = 0; k < fullSignal - fullSignal / 2; k++) writeSample(0.1);
        } else {
            for(int k = 0; k < fullSignal / 2; k++) writeSample(0.1);
            for(int k = 0; k < fullSignal - fullSignal / 2; k++) writeSample(-0.1);
        }
    } else {
        if (signalType==-1) {
            for(int k = 0; k < fullSignal / 2; k++) writeSample(-0.1);
            for(int k = 0; k < fullSignal - fullSignal / 2; k++) writeSample(0.1);
        } else {
            for(int k = 0; k < fullSignal / 2; k++) writeSample(0.1);
            for(int k = 0; k < fullSignal - fullSignal / 2; k++) writeSample(-0.1);
        }
    }
}

void TapeCoder::writeSample(double value) {
    zerosCount = 0;
    dest->WriteSample(value);
}

void TapeCoder::writeZero() {
    if (maxPauseInSeconds && zerosCount >= maxPauseInSeconds * sampleRate) {
        currentAbsoluteStartOffset++;
        return;
    }
    zerosCount++;
    dest->WriteSample(0.0);
}

string TapeCoder::getDestOffset(unsigned int sourceOffset) {
    return (splitOutputWavesOverMB?"(" + getCurrentNameInfix() + ") ":"") + to_string(sourceOffset - currentAbsoluteStartOffset);
}

void TapeCoder::possibleSplit(unsigned int sourceOffset) {
    if (splitOutputWavesOverMB && sourceOffset - currentAbsoluteStartOffset > splitOutputWavesOverMB * 1000000) {
        currentAbsoluteStartOffset = sourceOffset;
        dest->Close();
        openNewOutputDest();
    }
}

string TapeCoder::generateNextNameInfix() {
    ++nrOrder;
    return getCurrentNameInfix();
}

string TapeCoder::getCurrentNameInfix() {
    return splitOutputWavesOverMB?(nrOrder < 10?"0":"") + to_string(nrOrder):"";
}

void TapeCoder::openNewOutputDest() {
    string fileName = destName + generateNextNameInfix() + WAV_EXTENSION;
    if(!dest->OpenWrite(fileName.c_str())) {
        cerr << "Cannot open an output file: " << fileName << "\n";
        exit(EXIT_FAILURE);
    }
    dest->SetupFormat(sampleRate,8,1);
}
