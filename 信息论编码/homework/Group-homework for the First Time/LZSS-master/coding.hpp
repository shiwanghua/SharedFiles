#ifndef coding_hpp
#define coding_hpp
#include "utility.hpp"

using namespace std;

void CheckCodingBinaryBuffor(ofstream &codeOutput,char &binaryBuffor,int &binaryPointer);
void CodeData(ofstream &codeOutput,char &binaryBuffor,int &binaryPointer,int codeLength,int codingData);
void CodePair(char codedByte,char &binaryBuffor,int &binaryPointer, ofstream &codeOutput,int byteLength);
void CodeThreesome(int positionInDictionary,int wordLength,int dictionaryLengthBinary,int bufforLengthBinary,ofstream &codeOutput,char &binaryBuffor,int &binaryPointer);
bool Coding(HWND hwnd,HWND g_ProgressBar,HWND g_DataPathTextBox,HWND g_BufforSizeTextBox,HWND g_DictionarySizeTextBox,HWND g_CodedOutputPathTextBox);
void CodingLZSS(fstream &logger,ifstream &data,ofstream &codeOutput,long unsigned dataFilleSize,int dictionaryLength,int bufforLength,const char* codeOutputPath,HWND g_ProgressBar);

#endif 
