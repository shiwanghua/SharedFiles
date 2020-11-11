#include "coding.hpp"
#include "kmp.hpp"



void CheckCodingBinaryBuffor(ofstream &codeOutput,char &binaryBuffor,int &binaryPointer)
{
	if(IsBinaryBufforFull(binaryPointer))
    {
        codeOutput.write((char*)&binaryBuffor,sizeof binaryBuffor);
        binaryBuffor = 0;
	}
}

void CodeData(ofstream &codeOutput,char &binaryBuffor,int &binaryPointer,int codeLength,int codingData)
{
	for(int i = 0; i < codeLength;i++)
	{	
		CheckCodingBinaryBuffor(codeOutput,binaryBuffor,binaryPointer);
		if( codingData & 1) binaryBuffor += binaryPointer;
   			binaryPointer <<=1; 
			codingData >>=1;//move to right to decrease
	}
	

}

void CodePair(char codedByte,char &binaryBuffor,int &binaryPointer, ofstream &codeOutput,int byteLength)
{
	//Check before saving the flag!
    CheckCodingBinaryBuffor(codeOutput,binaryBuffor,binaryPointer);    	
    
	//save a flag
	binaryBuffor += binaryPointer;
	binaryPointer <<=1;
			
	//Code a Byte			
	CodeData(codeOutput,binaryBuffor,binaryPointer,byteLength,codedByte);
}

void CodeThreesome(int positionInDictionary,int wordLength,int dictionaryLengthBinary,int bufforLengthBinary,ofstream &codeOutput,char &binaryBuffor,int &binaryPointer)
{
	int number = 0;
	
	//Check before save a flag!
	CheckCodingBinaryBuffor(codeOutput,binaryBuffor,binaryPointer);
	//save a flag	
	binaryPointer <<=1; // just move binaryPointer to left for 0
	
	number  = positionInDictionary;
			
	//Code Position
	CodeData(codeOutput,binaryBuffor,binaryPointer,dictionaryLengthBinary,number);
	
	number = wordLength;		
	//Code length 
	CodeData(codeOutput,binaryBuffor,binaryPointer,bufforLengthBinary,number);
			
}

void CodingLZSS
(
	fstream &logger,
	ifstream &data, 
	ofstream &codeOutput,
	long unsigned dataFilleSize,
	int dictionaryLength,
	int bufforLength,
	const char* codeOutputPath,
	HWND g_ProgressBar
 )
{
	char actualTime[80];
	bool readingAllowed = true;//reading from the data flag
	
	//dictionary and buffor variables;
	vector <char> dictionary; //dictionary holder
	vector <char> buffor; //buffor for codding holder
	
	//Variables for P - position in dictionary and C - count matching word length
	int wordLength = 0;	//length of the match word - C
	int positionInDictionary = 0;//position P
	

	char dataByte; //byte readed from data input
	int byteLength = 8;//length for byte
	
	//Holder for binary strings
	char * binaryString = new char[byteLength +1];
	int binaryLength = 0;//length of acutally binaryString
	int binaryThreesomeLength = 0; // lentgth of coding P and C and 0 flag
	int binaryPairLength = byteLength + 1; //length of coding the 1 flag and data byte;
	int binaryCountLength = 0;
	
	int binaryPointer = 1;
	char binaryBuffor = 0;
	
	//Count binary length
	//count dicitonary binary length
	int dictionaryLengthBinary = (int)log2(dictionaryLength) + 1;
	//count buffor binary length
	int bufforLengthBinary = (int) log2(bufforLength) + 1;
	//add two lengths and a bit flag (0,P,C)
	binaryThreesomeLength = dictionaryLengthBinary + bufforLengthBinary + 1;
	
	//LZSS - coding


	GetActualTime(actualTime);
	
	logger 
	<< "========================================" << endl
	<< "Start coding process" << endl
	<< "===========PROPERTIES===================" << endl
	<< "Dictionary length: " << dictionaryLength << "B" << endl
	<< "Buffor length: " << bufforLength << "B" << endl
	<< "Data Size: " << dataFilleSize << "B" << endl
	<< "Actual Date: " << actualTime << endl
	<< "===========CODING PROCESS===============" << endl;
	

	//1. Read the first byte
	data.read((char*)& dataByte, sizeof dataByte);
	
	//2.Fil a dictionary
	for(int i = 0 ; i < dictionaryLength; i++)
	{
		dictionary.push_back(dataByte);
	}

	//3.Write first byte to output
    codeOutput.write((char*)& dataByte, sizeof dataByte);

    
    
    //4.Fill the buffor
    for(int i = 0 ; i < bufforLength; i++)
	{
            data.read((char*)& dataByte, sizeof dataByte);
            if(data.eof()) break;
            buffor.push_back(dataByte);
    }


    //5.Coding loop
    while(buffor.size() != 0)
	{       
	
		//Progress counting
		SendMessage(g_ProgressBar,PBM_SETPOS,CountProgress(dataFilleSize,data.tellg()),0);
        //5.1 Find the longest matching word in dictionary
		Kmp(dictionary,buffor,dictionary.size(),buffor.size(),wordLength,positionInDictionary);
       
        //5.2 Count how many bits is needed to code a word
        binaryCountLength = wordLength * 8;
        
        //5.2.1 Code pair if true
        if(binaryCountLength < binaryThreesomeLength)
        {	
        	CodePair(buffor[0],binaryBuffor,binaryPointer,codeOutput,byteLength);
            
            //5.2.1.1 move coding window
            dictionary.erase(dictionary.begin());//delete first and move
            dictionary.push_back(buffor[0]); //add the byte on back of dictionary
            buffor.erase(buffor.begin());//delete first and move

            //5.2.1.2 read the next byte form data
            if(readingAllowed)  data.read((char*)& dataByte, sizeof dataByte);
            if(data.eof()) readingAllowed = false;
            else buffor.push_back(dataByte);
           
        }
        else
        {	
			//5.2.2 Code threesome (0,P,C)
        	CodeThreesome(positionInDictionary,wordLength,dictionaryLengthBinary,bufforLengthBinary,codeOutput,binaryBuffor,binaryPointer);
        
           	//5.2.2.1 Move coding window
            dictionary.erase(dictionary.begin(),dictionary.begin() + wordLength);
            for(int i = 0; i < wordLength;i++) dictionary.push_back(buffor[i]);
            buffor.erase(buffor.begin(),buffor.begin() + wordLength);

             //5.2.2.2 Read new data form input
            if(readingAllowed) for(int i = 0; i < wordLength;i++)
			{
                data.read((char*)& dataByte, sizeof dataByte);
            	if(data.eof()) 
				{
					readingAllowed = false;
					break;
				}
            	else buffor.push_back(dataByte);
            }

        }
    }
    
    //End of coding process.
    data.close();
    
    //if there is something in binary buffor!
    if(binaryPointer > 1) codeOutput.write((char*)& binaryBuffor, sizeof binaryBuffor);
		
	codeOutput.close();
    
    
    long codeOutputLength = MeasureFilleSize(codeOutputPath);
    long double ratio = (long double)codeOutputLength/dataFilleSize;
    ratio =  1 - ratio;
	long compressionRatio = ratio*100;
	
	
	GetActualTime(actualTime);
    
    logger
    << "========================================" << endl
	<< "End of coding process" << endl
	<< "===========PROPERTIES===================" << endl
	<< "Data Size before coding: " << dataFilleSize << "B" << endl
	<< "Coded Data File Size: " << codeOutputLength << "B" << endl
	<< "Compression ratio: " << compressionRatio << "%" << endl
	<< "Actual Date: " << actualTime  << endl
	<< "========================================" << endl << endl;
    //reset progress bar
    SendMessage(g_ProgressBar,PBM_SETPOS,0,0);
	
}

bool Coding(
	HWND hwnd,
	HWND g_ProgressBar,
	HWND g_DataPathTextBox,
	HWND g_BufforSizeTextBox,
	HWND g_DictionarySizeTextBox,
	HWND g_CodedOutputPathTextBox
)
{
	fstream logger;
	ifstream data;//data input holder
	ofstream codeOutput;//code output fille holder
	const char * codeOutputPath;
	char * dataInputPath;
	long unsigned dataFilleSize = 0;

	int dictionaryLength = 0; //dictionary length
	int bufforLength = 0;//buffor length
	
	//Read from the textboxes
	char * dictionaryLengthString = ReadFromTextBox(g_DictionarySizeTextBox);
	char * bufforLengthString = ReadFromTextBox(g_BufforSizeTextBox);
	
	dictionaryLength = ConvertStringToInt(dictionaryLengthString);
	bufforLength = ConvertStringToInt(bufforLengthString);
	
	//Check if is ok
	if(dictionaryLength <= 0)
	{
		MessageBox( hwnd, "Wrong value in dictionary size", "Error", MB_ICONERROR );	
		return false;
	}
	
	if(bufforLength <=0)
	{
		MessageBox(hwnd,"Wrong value in buffor size", "Error",MB_ICONERROR);
		return false;
		
	}
	
	dataInputPath = ReadFromTextBox(g_DataPathTextBox);
	
	dataFilleSize = MeasureFilleSize(dataInputPath);
	
	string somePath =  ReadFromTextBox(g_CodedOutputPathTextBox);
	somePath.append(MY_EXTENSION);
	
	codeOutputPath = somePath.c_str();
	
	
	
	//Try to open the filles
	
	
	//logger
	bool bChecked =( IsDlgButtonChecked( hwnd, ID_CHECKBOX ) == BST_CHECKED );
	if(bChecked) logger.open("log.txt",ios::out|ios::app);
	
	data.open(dataInputPath,ios::binary);
	
	//check 
	if(!data.good())
	{
		MessageBox(hwnd,"Cannot open the data", "Error",MB_ICONERROR);
		return false;
	}
	

		//1.1 if data is empty return;
	if(data.eof())
	{
		MessageBox(hwnd,"There is no data in file to coding!", "Error",MB_ICONERROR);
		return false;
	} 
	
	
	//Create output code fille
	codeOutput.open(codeOutputPath,ios::out);
	
	//check
	if(!codeOutput.good())
	{
		MessageBox(hwnd,"Cannot create the code output fille", "Error",MB_ICONERROR);
		return false;
	}
	
    codeOutput.close();//close 
	//open in binary mode and app
	codeOutput.open(codeOutputPath, ios::app |ios::binary);
	
	CodingLZSS(logger,data,codeOutput,dataFilleSize,dictionaryLength,bufforLength,codeOutputPath,g_ProgressBar);
	
	
    return true;
}
