#ifndef decoding_hpp
#define decoding_hpp
#include "utility.hpp"

using namespace std;

bool Decoding(HWND hwnd,HWND g_ProgressBar,HWND g_CodedOutputPathTextBox,HWND g_BufforSizeTextBox,HWND g_DictionarySizeTextBox,HWND g_DecodedOutputPathTextBox);
void DecodingLZSS(fstream &logger, ifstream &codeInput, fstream &decodeOutput,int dictionaryLength,int bufforLength,HWND g_ProgressBar,unsigned long inputSize,const char* decodeOutputPath);
bool IsThreesomeSave(char * binaryString,int binaryLength);
bool WhatToDecode(char &binaryBuffor,int &binaryPointer);
bool CheckDecodingBinaryBuffor(ifstream &codeInput,char &binaryBuffor, int &binaryPointer);
int DecodeData(char &binaryBuffor, int &binaryPointer, ifstream &codeInput,int binaryLength, bool &IsThereIsDataToDecode);

#endif
