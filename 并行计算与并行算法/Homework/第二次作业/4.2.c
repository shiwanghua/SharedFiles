#include <iostream>
using namespace std;
typedef unsigned short us;
int main() {
    char a = 13, b = 22, c = 43, d = 64, e = 99;
    cout << (((us)(char)(a + b + c + d + e)) & (us)255) << endl;
    cout << (((us)(char)(a * b * c * d * e)) & (us)255) << endl;
    cout << (((us)(char)(a | b | c | d | e)) & (us)255) << endl;
    cout << (((us)(char)(a & b & c & d & e)) & (us)255) << endl;
    cout << (((us)(char)(a || b || c || d || e)) & (us)255) << endl;
    cout << (((us)(char)(a && b && c && d && e)) & (us)255) << endl;
    return 0;
}
