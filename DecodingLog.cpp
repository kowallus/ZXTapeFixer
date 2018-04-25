#include "DecodingLog.h"

void DecodingLog::logBlockEnd(int lastByteEND, unsigned int destCurrentSample, int bytes, int lastByte, int XORByte,
                              int olderByte, int byte) {
/*    memo->Lines->Add(AnsiString(lastByteEND) + "; " + AnsiString(destCurrentSample) + ": End of Block");
    memo->Lines->Add(AnsiString("Bytes : ") + bytes);
    memo->Lines->Add(AnsiString("XOR byte : ") + lastByte + ", should be : " + XORByte + "; older : " +
                     olderByte);
    if ((lastByte != XORByte) && ((XORByte ^ lastByte) == olderByte))
        memo->Lines->Add(AnsiString("Block byte too long :-)"));
    memo->Lines->Add(AnsiString("next Byte : ") + byte);
    memo->Lines->Add("");*/
}

void DecodingLog::logBlockStart(unsigned int currentSample, unsigned int destCurrentSample) {
//    memo->Lines->Add(AnsiString(currentSample) + "; " + AnsiString(destCurrentSample) + ": Start of block");
}

void DecodingLog::logLargePulse(unsigned int currentSample, unsigned int destCurrentSample) {
//    memo->Lines->Add(AnsiString(currentSample) + "; " + AnsiString(destCurrentSample) + ": Too large Sync Pulse?");
}
