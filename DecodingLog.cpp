#include "DecodingLog.h"

void DecodingLog::logBlockEnd(int lastByteEND, string destCurrentSample, int bytes, int lastByte, int XORByte,
                              int olderByte, int byte) {

    string txt = to_string(lastByteEND) + "; " + destCurrentSample + ": End of Block\n";
    txt +=  "Bytes : " + to_string(bytes) + "\n";
    txt +=  "XOR byte : " + to_string(lastByte) + ", should be : " + to_string(XORByte) + "; older : " +
            to_string(olderByte) + "\n";
    if ((lastByte != XORByte) && ((XORByte ^ lastByte) == olderByte))
        txt += "Block byte too long :-)\n";
    txt += "next Byte : " + to_string(byte) + "\n";
    log(txt);
}

void DecodingLog::logBlockStart(unsigned int currentSample, string destCurrentSample) {
    log(to_string(currentSample) + "; " + destCurrentSample + ": Start of block");
}

void DecodingLog::logLargePulse(unsigned int currentSample, string destCurrentSample) {
    log(to_string(currentSample) + "; " + destCurrentSample + ": Too large Sync Pulse?");
}

DecodingLog::DecodingLog(string fileName, std::ostream *out): out(out){
    fout = new fstream(fileName.c_str(), ios::out | ios::binary | ios::app);
}

DecodingLog::~DecodingLog() {
    delete fout;
}

void DecodingLog::log(string txt) {
    *out << txt << endl;
    *fout << txt << endl;
}
