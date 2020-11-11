#ifndef kmp_hpp
#define kmp_hpp
#include "utility.hpp"

void KmpNext(std::vector <char> buffor, int tab[],int length);
void Kmp(std::vector <char> dictionary, std::vector <char> buffor,int dictionaryLength, int bufforLength,int &wordLength , int &positionInDictionary);

#endif 
