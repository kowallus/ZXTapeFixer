#include <iostream>

#include <unistd.h>

#include "utils/helper.h"
#include "tapefix.h"

using namespace std;

static const string APP_SUFFIX("_ZXTF");
static const string ERR_SUFFIX("_err");
static const string WAV_EXTENSION = ".wav";
static const string TXT_EXTENSION = ".txt";
static const string LOG_EXTENSION = ".log";

int main(int argc, char**argv) {

    bool verbose = false;

    int phaseShiftInSamples = 0;
    int maxPauseInSeconds = 0;
    int splitOutputWavesOverMB = 0;

    int opt; // current option

    while ((opt = getopt(argc, argv, "p:r:s:v?")) != -1) {
        switch (opt) {
            case 'v':
                verbose = true;
                break;
            case 'p':
                phaseShiftInSamples = atoi(optarg);
                if (phaseShiftInSamples <= -100 || phaseShiftInSamples >= 100) {
                    fprintf(stderr, "%s: Expected -100 < p < 100\n", argv[0]);
                    fprintf(stderr, "try '%s -?' for more information\n", argv[0]);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'r':
                maxPauseInSeconds = atoi(optarg);
                if (maxPauseInSeconds <= 0) {
                    fprintf(stderr, "%s: Expected r > 0\n", argv[0]);
                    fprintf(stderr, "try '%s -?' for more information\n", argv[0]);
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                splitOutputWavesOverMB = atoi(optarg);
                if (splitOutputWavesOverMB <= 0) {
                    fprintf(stderr, "%s: Expected s > 0\n", argv[0]);
                    fprintf(stderr, "try '%s -?' for more information\n", argv[0]);
                    exit(EXIT_FAILURE);
                }
                break;
            case '?':
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-p channels_phase_shift_in_samples] [-r maximum_output_pause_in_seconds] [-s split_output_waves_threshold_in_MB] [-v verbose] <zx_tape_wav_filename>\n\n",
                        argv[0]);
                fprintf(stderr, "-p [-100 < p < 100] \n-r [r > 0] \n-s [s > 0] \n-v verbose (dump_consoles)\n\n");
                exit(EXIT_FAILURE);
        }
    }

    if (optind > (argc - 1)) {
        fprintf(stderr, "%s: Expected one argument after options (found %d)\n", argv[0], argc-optind);
        fprintf(stderr, "try '%s -?' for more information\n", argv[0]);

        exit(EXIT_FAILURE);
    }

    string file_name = argv[optind];
    if (hasSuffix(file_name, WAV_EXTENSION)) {
        file_name = file_name.substr(0, file_name.length() - 4);
    }
    string wav_in_file_name = file_name + WAV_EXTENSION;
    string dest_file_name = file_name + APP_SUFFIX;
    string dump_file_name = file_name + APP_SUFFIX + TXT_EXTENSION;
    string err_file_name =  file_name + APP_SUFFIX + ERR_SUFFIX + LOG_EXTENSION;

    cout << "Processing tape: " << wav_in_file_name << " ... ";

    WaveFile source;
    if (!source.OpenRead(wav_in_file_name.c_str())) {
        cerr << "Cannot open file: " << wav_in_file_name << "\n";
        exit(EXIT_FAILURE);
    }
    TapeCoder coder(dest_file_name, source.GetSampleRate(), maxPauseInSeconds, splitOutputWavesOverMB);

    DecodingLog decLog(dump_file_name, verbose?&cout:&nout);
    ErrorsDumper errDump(err_file_name, verbose?&cerr:&nout);

    TapeReconstructor(&source, &coder, &decLog, &errDump, phaseShiftInSamples);

    source.Close();

    cout << "done!" << endl;

    return 0;
}