#include "utility.hpp"


using namespace std;

char* ReadFromTextBox(HWND textBox)
{
	int length = GetWindowTextLength(textBox);
	LPSTR buffor =( LPSTR ) GlobalAlloc( GPTR, length + 1 );
	GetWindowText( textBox, buffor, length + 1 );
	return buffor;
}

int ConvertStringToInt(char * integerString)
{
	int convertResult = 0;
	stringstream ss;
	ss << integerString;
	ss >> convertResult;
	ss.str("");
	ss.clear();
	return convertResult;
}

long unsigned MeasureFilleSize(const char * filePath)
{
	ifstream inputFille;
	inputFille.open(filePath,ios::binary);
	if(!inputFille.good())
	{
		return 0;
	}
	inputFille.seekg(0,ios::end);
	int result = inputFille.tellg();
	inputFille.close();
	return result;
}


WPARAM CountProgress(long unsigned filleSize, long unsigned fillePointer)
{
	float progress =  (float)fillePointer/filleSize;
	progress *= 100;
	return (WPARAM) progress;
}

int CountBinaryLength(int binaryPairLength,int wordLength)
{
	int result = 0;
	result = binaryPairLength * wordLength;
	return result;
}

bool IsBinaryBufforFull(int &binaryPointer)
{
	if(binaryPointer == POINTER_BOUNDARY)
	{
		binaryPointer = 1;
		return true;
	}
	return false;
}

bool CheckExtension(string myCheckingString)
{
	string myPerfectExtension = "lzsstk";
	size_t found = myCheckingString.find_last_of(".");
	if(myPerfectExtension.compare(myCheckingString.substr(found+1))!=0)
	{
		return true;
	}
	
	return false;
}
void GetActualTime(char* timeHour)
{
	struct tm * date;
	time_t start;

	time( &start);
	date = localtime(&start);
	
	strftime( timeHour, 80, "%c", date );
	
}

