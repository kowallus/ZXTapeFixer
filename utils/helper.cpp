#include "helper.h"

#include <sstream>

NullBuffer null_buffer;
std::ostream nout(&null_buffer);

clock_t checkpoint;

void clock_checkpoint() {
    checkpoint = clock();
}

unsigned long long int clock_millis() {
    return (clock() - checkpoint) * (unsigned long long int) 1000 / CLOCKS_PER_SEC;
}


string toString(unsigned long long value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
};

string toMB(unsigned long long value, unsigned char decimalPlaces) {
    std::ostringstream oss;
    int power = 1000000;
    oss << value / power;
    if (decimalPlaces > 0) {
        oss << ".";
        for(int i = 0; i < decimalPlaces; i++) 
            oss << ((value / (power /= 10)) % 10);
        
    }
    return oss.str();
}

string toString(long double value, unsigned char decimalPlaces) {
    std::ostringstream oss;
    oss << (long long int) value;
    if (decimalPlaces > 0) {
        oss << ".";
        int power = 1000000;        
        unsigned long long decimals = (value - (long long int) value) * power;
        for(int i = 0; i < decimalPlaces; i++) 
            oss << ((decimals / (power /= 10)) % 10);
    }
    return oss.str();
}

bool hasSuffix(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}
