#include "ErrorsDumper.h"

void ErrorsDumper::logError(unsigned int currentSample, string destCurrentSample, unsigned int signalCount,
                            unsigned int lastSignalCount, float zero, int bytes) {

    dump(to_string(currentSample) + "; " + destCurrentSample + ": possible error - " +
            to_string(signalCount) + " - " + to_string(lastSignalCount) + "; zero - " + to_string(zero) + "; bytes - " +
            to_string(bytes));
}

ErrorsDumper::ErrorsDumper(string fileName, std::ostream *out): out(out){
    fout = new fstream(fileName.c_str(), ios::out | ios::binary | ios::app);
}

ErrorsDumper::~ErrorsDumper() {
    delete fout;
}

void ErrorsDumper::dump(string txt) {
    *out << txt << endl;
    *fout << txt << endl;
}
