#include "kmp.hpp"

void KmpNext(std::vector <char> buffor, int tab[],int length){
    int b = -1;
    tab[0] = -1;

    for(int i = 1; i <= length; i++){
        while((b > -1)&& (buffor[b]!= buffor[i-1])){
            b = tab[b];
        }
        b++;
        if((i==length)||(buffor[i]!= buffor[b]))
            tab[i] = b;
        else tab[i]=tab[b];
    }
}

void Kmp(std::vector <char> dictionary, std::vector <char> buffor,int dictionaryLength, int bufforLength,int &wordLength , int &positionInDictionary){
    
    //Reset
    wordLength = 0;
    positionInDictionary = 0;
    
    bool founded = false; //if true dont continue for searching
    int length = bufforLength+1;
    int pointer = 0;
    int tab[length];//kmpNext
    KmpNext(buffor,tab,length);
    
    for(int i = 0; i < dictionaryLength && !founded; i++){
        while((pointer>-1)&& (buffor[pointer]!=dictionary[i])){
            pointer=tab[pointer];
        }
        pointer++;
        if(pointer > wordLength) {
            wordLength = pointer;
            positionInDictionary = i - pointer + 1;
            if(wordLength == bufforLength)
			{
				founded = true;
				break;
			}
        }
    }
}

