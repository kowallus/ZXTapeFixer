#ifndef ZXTAPERFIXER_ERRORSDUMPER_H
#define ZXTAPERFIXER_ERRORSDUMPER_H

#include <string>
#include <fstream>

using namespace std;

class ErrorsDumper {

    std::fstream* fout;
    std::ostream* out;

    void dump(string txt);

public:
    ErrorsDumper(string fileName, std::ostream* out);

    virtual ~ErrorsDumper();

    void logError(unsigned int currentSample, string destCurrentSample, unsigned int signalCount,
                  unsigned int lastSignalCount, float zero, int bytes);
};


#endif //ZXTAPERFIXER_ERRORSDUMPER_H
