#ifndef utility_hpp
#define utility_hpp
#include <windows.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <commctrl.h>
#include <iostream>
#include <math.h>
#include <string>
#include <cstddef>
#include <time.h>


#define POINTER_BOUNDARY 256
#define MY_EXTENSION ".lzsstk"
#define ID_CHECKBOX 503



using namespace std;

char* ReadFromTextBox(HWND textBox);
int ConvertStringToInt(char * integerString);
long unsigned MeasureFilleSize(const char * filePath);
WPARAM CountProgress(long unsigned filleSize, long unsigned fillePointer);
int CountBinaryLength(int binaryPairLength,int wordLength);
bool IsBinaryBufforFull(int &binaryPointer);
bool CheckExtension(string fileName);
void GetActualTime(char *timeHour);

#endif
