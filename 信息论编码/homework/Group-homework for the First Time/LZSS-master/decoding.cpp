#include "decoding.hpp"

using namespace std;

bool CheckDecodingBinaryBuffor(ifstream &codeInput,char &binaryBuffor, int &binaryPointer)
{
	char codedByte = 0;
	
	 if(IsBinaryBufforFull(binaryPointer)) //there is no more data in binaryBuffor
	{
		//read next byte
		codeInput.read((char*)& codedByte, sizeof codedByte);
		if(codeInput.eof()) 
		return false;      
		binaryBuffor = codedByte;
	}
	return true;
}

int DecodeData(char &binaryBuffor, int &binaryPointer, ifstream &codeInput,int binaryLength, bool &IsThereIsDataToDecode)
{
	int pointer=1;
	int number = 0;

	for(int i = 0 ; i < binaryLength && IsThereIsDataToDecode; i++)
    {        	
		//if got binary length byte just read next byte
        IsThereIsDataToDecode = CheckDecodingBinaryBuffor(codeInput,binaryBuffor,binaryPointer);
        if(binaryBuffor & 1) number += pointer;
		binaryBuffor >>=1; //decrease the binary buffor
        pointer<<=1;
		binaryPointer <<=1;
    }
    
    return number;
}

bool IsThreesomeSave(char * binaryString,int binaryLength){
    if(binaryString[binaryLength] == '1') return false;
    return true;
}



bool WhatToDecode(char &binaryBuffor,int &binaryPointer)
{
	if(binaryBuffor & 1)
	{
		//Decode pair
		binaryPointer <<=1;
		binaryBuffor >>=1;
		return true;
	}
	//Decode Threesome
	binaryBuffor >>=1;
	binaryPointer <<=1;
	return false;
}

void DecodingLZSS(fstream &logger, ifstream &codeInput, fstream &decodeOutput,int dictionaryLength,int bufforLength,HWND g_ProgressBar,unsigned long inputSize,const char* decodeOutputPath)
{
	vector <char> dictionary;
    vector <char> buffor;
	char codedByte;
	

    int byteLength = 8;
    
    int binaryPointer = 1;
    char binaryBuffor = 0;
    
	int dictionaryBinaryLength = (int) log2(dictionaryLength) +1;
	int bufforBinaryLength = (int) log2(bufforLength) + 1;
	bool IsThereIsDataToDecode = true;
	
	char actualDate[80];
	GetActualTime(actualDate);
	//Start Decoding LZSS!
	logger 
	<< "========================================" << endl
	<< "Start decoding process" << endl
	<< "==========PROPERTIES====================" << endl
	<< "Dictionary length: " << dictionaryLength << "B" << endl
	<< "Buffor length: " << bufforLength << "B" << endl
	<< "Actual Date: " << actualDate << endl
	<< "==========DECODING PROCESS===========" << endl;
	
	//1.Read first byte
    codeInput.read((char*)& codedByte, sizeof codedByte);
  	
    //2.Fill dicitonary with first byte
	for(int i = 0; i < dictionaryLength; i++) dictionary.push_back(codedByte);

    //3.Put first byte in to output file
    decodeOutput.write((char*)& codedByte, sizeof codedByte);
    
    codeInput.read((char*)& codedByte, sizeof codedByte);
    
    binaryBuffor = codedByte;

    //4.Decoding loop
    while(IsThereIsDataToDecode)
    {
    	//Progress counting
		SendMessage(g_ProgressBar,PBM_SETPOS,CountProgress(inputSize,codeInput.tellg()),0);
    	//4.1 Check what is coded
        if(WhatToDecode(binaryBuffor,binaryPointer))
        {
        	//4.1 decoding pair
        	char byte = 0;
            int pointer=1;
            
			//decoding byte
			byte = DecodeData(binaryBuffor,binaryPointer,codeInput,byteLength,IsThereIsDataToDecode);
			
            if(IsThereIsDataToDecode)
            {
            	decodeOutput.write((char*)& byte, sizeof byte);
            	dictionary.push_back(byte);
            	dictionary.erase(dictionary.begin());
			}
        }
        else
        {
        	
			//4.1.2 decoding threesome
			int positionInDictionary = 0;
            int wordLength = 0;
            
            //4.1.2.1 decoding position
            positionInDictionary = DecodeData(binaryBuffor,binaryPointer,codeInput,dictionaryBinaryLength,IsThereIsDataToDecode);
            
            //4.1.2.2 decoding word length 
            wordLength = DecodeData(binaryBuffor,binaryPointer,codeInput,bufforBinaryLength,IsThereIsDataToDecode);
        
            if(IsThereIsDataToDecode)
            { 
				//ofset C + P 
	            int ofset = wordLength + positionInDictionary;
			
				
	            //4.1.2.3 copy the bytes to the buffor and write to output
	            for(int i = positionInDictionary; i < ofset;i++){
	                dictionary.push_back(dictionary[i]);
	                decodeOutput.write((char*)& dictionary[i], sizeof dictionary[i]);
	            }
	
				//4.1.2.4 move decoding window
	            dictionary.erase(dictionary.begin(),dictionary.begin()+wordLength);
	           
        
			}
           
        }
		
	   if(IsThereIsDataToDecode)
	   {
	   		IsThereIsDataToDecode = CheckDecodingBinaryBuffor(codeInput,binaryBuffor,binaryPointer);
	   }
    }
    
    decodeOutput.close();
    
    codeInput.close();
    
	GetActualTime(actualDate);
	long decodeLength = MeasureFilleSize(decodeOutputPath);
    
	logger 
    << "========================================"<<endl
    << "End of decoding process" << endl
    << "==========PROPERTIES====================" << endl
    << "Data Size after decoding: "<<decodeLength<<"B"<< endl
	<< "Actual Date: " << actualDate << endl
	<< "========================================" << endl;
	logger.close();
}

bool Decoding
(
	HWND hwnd,
	HWND g_ProgressBar,
	HWND g_DataPathTextBox,
	HWND g_BufforSizeTextBox,
	HWND g_DictionarySizeTextBox,
	HWND g_DecodedOutputPathTextBox
)
{
	fstream logger;
    ifstream codeInput;
    fstream decodeOutput;
    int dictionaryLength;
    int bufforLength;
    long unsigned inputSize;
    
    string somePath = ReadFromTextBox(g_DataPathTextBox);
 
    const char * codeInputPath = somePath.c_str();

    char * decodeOutputPath = ReadFromTextBox(g_DecodedOutputPathTextBox);

    
    //Adding loger
	bool bChecked =( IsDlgButtonChecked( hwnd, ID_CHECKBOX ) == BST_CHECKED );
	if(bChecked) logger.open("log.txt",ios::out | ios::app);
    
    codeInput.open(codeInputPath,ios::binary);
    
    //check 
    if(!codeInput.good())
    {
    	MessageBox( hwnd, "Cannot open the code file", "Error", MB_ICONERROR );	
    	return false;
	}
	
	if(CheckExtension(somePath))
    {
    	MessageBox( hwnd, "Wrong extension! Need lzsstk!", "Error", MB_ICONERROR );	
    	return false;
	}
	  
	codeInput.close();
	
	inputSize = MeasureFilleSize(codeInputPath);
	
	codeInput.open(codeInputPath,ios::binary);
	
	char * dictionaryLengthString = ReadFromTextBox(g_DictionarySizeTextBox);
	char * bufforLengthString = ReadFromTextBox(g_BufforSizeTextBox);	
	dictionaryLength = ConvertStringToInt(dictionaryLengthString);
	bufforLength = ConvertStringToInt(bufforLengthString);
	

	
	decodeOutput.open(decodeOutputPath,ios::out | ios::binary);
	if(!decodeOutput.good())
	{
		MessageBox( hwnd, "Cannot create the decode file", "Error", MB_ICONERROR );	
    	return false;
	}

	DecodingLZSS(logger,codeInput,decodeOutput,dictionaryLength,bufforLength,g_ProgressBar,inputSize,decodeOutputPath);
	
    //reset progress bar
    SendMessage(g_ProgressBar,PBM_SETPOS,0,0);
    return true;
}
