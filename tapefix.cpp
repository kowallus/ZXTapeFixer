#include "tapefix.h"

    TapeReconstructor::TapeReconstructor(WaveFile* source, TapeCoder* destCoder, DecodingLog* decLog, ErrorsDumper* errDump, int leftDelay, bool leftChan, bool rightChan) {
/*    Source = new WaveFile();
    WaveFile* SourceStereo = new WaveFile();
    Dest = new WaveFile();
    int nrOrder=0;
    TFileRun* sox = new TFileRun(Owner);
    sox->FileName = "sox";

// wersja najlepsza - wymaga filtru gornoprzepustowego > 150 Hz
    if (Source->OpenRead(SourceName)) {
        if(Dest->OpenWrite((AnsiString(DestName)+(++nrOrder)+".wav").c_str())) {
            Dest->SetupFormat(Source->GetSampleRate(),8,1);
            int fileStart=0;

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

            int goodSignal[50];
            int sigAvg = 30;
            int minGood = 29;
            for(int i = 0; i < 50; i++) goodSignal[i] = 0;
            signalType = 0;
            unsigned int signalCount = 0;
            lastSignalCount = 0;
            int lastSignal = 0;
            bool signalChange;
            int GoodSignalCount = 0;

            if (Source->GetNumChannels() > 1) {
                SourceStereo->OpenRead(SourceName);
                SourceStereo->ReadSample(sample);
                if (leftDelay > 0) {
                    for(int i = 0; i < leftDelay * 2; i++)
                        SourceStereo->ReadSample(sample);
                } else {
                    leftDelay = -leftDelay;
                    for(int i = 0; i < leftDelay * 2; i++)
                        Source->ReadSample(sample);
                }

            }

            for(i = 0; i < Source->GetNumSamples() - leftDelay - 1; i++) {

                if (leftChan) Source->ReadSample(sample);
                else sample = 0;
                if (Source->GetNumChannels() > 1) {
                    float stereo;
                    SourceStereo->ReadSample(stereo);
                    if (rightChan) sample += stereo;
                }
                sample += zero;

// zamiana amplitudy sygna�u
                if (sample > 0) {
                    signalChange = signalType != 1;
                    signalType = 1;
                    if (blockPart < 2)
                        if ((GoodSignalCount > minGood) && (blockPart > 0)) Dest->WriteSample(0.08);
                        else Dest->WriteSample(0.0);
                    }
                else {
                    signalChange = signalType != -1;
                    signalType = -1;
                    if (blockPart < 2)
                    if ((GoodSignalCount > minGood) && (blockPart > 0)) Dest->WriteSample(-0.08);
                    else Dest->WriteSample(0.0);
                }

//  zmiana sygnalu
                if (signalChange) {

                    if ((badCount > 2) || (GoodSignalCount <= minGood)) {
                        if (blockPart==2) {
                            if (bit==-1) for(unsigned int k = 0; k < signalCount; k++) Dest->WriteSample(0.0);
                            else for(unsigned int k = 0; k < lastSignalCount + signalCount; k++) Dest->WriteSample(0.0);
                            memo->Lines->Add(AnsiString(lastByteEND)+"; "+AnsiString(i - fileStart)+": End of Block");
                            memo->Lines->Add(AnsiString("Bytes : ") + bytes);
                            memo->Lines->Add(AnsiString("XOR byte : ") + lastByte + ", should be : " + XORByte + "; older : " + olderByte);
                            if ((lastByte != XORByte) && ((XORByte^lastByte) == olderByte))
                                memo->Lines->Add(AnsiString("Block byte too long :-)"));
                            memo->Lines->Add(AnsiString("next Byte : ") + byte);
                            memo->Lines->Add("");
                        }
                        zero = 0; blockPart = 0; bit = -1; pulseCount = 0; badCount = 0;
                    }
                    else {
                        switch (blockPart) {
                            case 1: if ((signalCount>3) && (signalCount<17) && ((pulseCount > 20) || (bit==0)))
                                        if (++bit == 1) {
                                            memo->Lines->Add(AnsiString(i)+"; "+AnsiString(i - fileStart)+": Start of block");
                                            blockPart=2; bit=-1; bytes = -2; lastByte = 0; byte = 0; XORByte=0; bitCount = 8; lastSignalCount = 0;
                                        } else if (signalCount>13) memo->Lines->Add(AnsiString(i)+"; "+AnsiString(i - fileStart)+": Too large Sync Pulse?");
                            case 0: if ((signalCount>17) && (signalCount<35))
                                    {
                                        bit = -1;
                                        if (++pulseCount == 20)
                                            blockPart = 1;
                                    } else pulseCount = 0;
                                    break;
                            case 2: int fullSignal = lastSignalCount + signalCount;
                                    if ((signalCount>5) && (signalCount<27) && (fullSignal < 50) && ((fullSignal > 15) || (bit==-1))) {
                                        if (lastSignalCount == 0)
                                            lastSignalCount = signalCount;
                                        else {
                                            if (fullSignal < 32) {
                                                if ((lastSignalCount < 17) && (signalCount < 17))
                                                    badCount = 0;
                                                dodajBit(0, fullSignal);
                                            }
                                            else {
                                                if ((lastSignalCount > 15) && (signalCount > 15))
                                                    badCount = 0;
                                                dodajBit(1, fullSignal);
                                            }
                                        }
                                    } else {
                                        if ((signalCount<5) || (signalCount>26))
                                            memoErr->Lines->Add(AnsiString(i)+"; "+AnsiString(i - fileStart)+": possible error - "+signalCount+" - "+lastSignalCount+"; zero - "+zero+"; bytes - "+bytes);

                                        if (lastSignalCount == 0)
                                            lastSignalCount = signalCount;
                                        else {
                                            if (signalCount > 28) fullSignal -= 20;
                                            if (fullSignal < 32) dodajBit(0, fullSignal);
                                            else dodajBit(1, fullSignal);
                                        }

                                        if (signalCount > 28) {
                                            // wpisz bit 0 - kopia tego co ni�ej
                                            lastSignalCount = lastSignalCount ? 0 : 1;

                                            dodajBit(0, 20);
                                            lastSignalCount = signalCount - 20;
                                        }

                                        ++badCount; // wykrywanie ko�c�wki
                                    }

                                    break;
                        }
                    }

                    if (++lastSignal == sigAvg) lastSignal = 0;
                    if ((signalCount > Source->GetSampleRate()/700) ||
                        (signalCount < Source->GetSampleRate()/15000)) {
                        GoodSignalCount -= goodSignal[lastSignal];
                        goodSignal[lastSignal]=0;
                        if ((GoodSignalCount < minGood) && (i - fileStart > 8000000)) {
                            fileStart = i;
                            Dest->Close();
                            sox->Parameters = AnsiString(DestName)+nrOrder+".wav "+AnsiString(DestName)+nrOrder+".voc";
                            sox->Execute();
                            Dest->OpenWrite((AnsiString(DestName)+(++nrOrder)+".wav").c_str());
                            Dest->SetupFormat(Source->GetSampleRate(),8,1);
                        }
                    } else {
                        GoodSignalCount -= goodSignal[lastSignal];
                        GoodSignalCount += (goodSignal[lastSignal]=1);
                    }
                    signalCount = 1;
                } else signalCount++;

                for (int j = 1; j < Source->GetNumChannels(); j++) {
                    Source->ReadSample(sample);
                    SourceStereo->ReadSample(sample);
                }

            }
        }
    }
    Source->Close();
    if (Source->GetNumChannels() > 1) SourceStereo->Close();
    Dest->Close();
    sox->Parameters = AnsiString(DestName)+nrOrder+".wav "+AnsiString(DestName)+nrOrder+".voc";
    sox->Execute();
    memo->Lines->SaveToFile(AnsiString(DestName)+".log");
    memoErr->Lines->SaveToFile(AnsiString(DestName)+"Err.log");*/
}

void TapeReconstructor::dodajBit(int bit, int fullSignal) {
/*
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

    if (lastSignalCount==0) {
        if (signalType==1) {
            for(int k = 0; k < fullSignal / 2; k++) Dest->WriteSample(-0.1);
            for(int k = 0; k < fullSignal - fullSignal / 2; k++) Dest->WriteSample(0.1);
        } else {
            for(int k = 0; k < fullSignal / 2; k++) Dest->WriteSample(0.1);
            for(int k = 0; k < fullSignal - fullSignal / 2; k++) Dest->WriteSample(-0.1);
        }
    } else {
        if (signalType==-1) {
            for(int k = 0; k < fullSignal / 2; k++) Dest->WriteSample(-0.1);
            for(int k = 0; k < fullSignal - fullSignal / 2; k++) Dest->WriteSample(0.1);
        } else {
            for(int k = 0; k < fullSignal / 2; k++) Dest->WriteSample(0.1);
            for(int k = 0; k < fullSignal - fullSignal / 2; k++) Dest->WriteSample(-0.1);
        }
    }
    lastSignalCount = 0;
*/
}

TapeReconstructor::~TapeReconstructor() {
}