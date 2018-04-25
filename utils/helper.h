#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include <ctime>
#include <string>

using namespace std;

void clock_checkpoint();
unsigned long long int clock_millis();

string toString(unsigned long long value);
string toMB(unsigned long long value, unsigned char decimalPlaces);
string toString(long double value, unsigned char decimalPlaces);

bool hasSuffix(const std::string &str, const std::string &suffix);


#endif // HELPER_H_INCLUDED
