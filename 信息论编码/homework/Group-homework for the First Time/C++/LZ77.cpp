//#include<iostream>
//#include<string>
//#include<vector>
//#include <fstream> 
//#include<algorithm>
//#include "time.h"
//using namespace std;
//#define _for(i,j) for(int i=0;i<j;i++)
//
//class code_rule {
//public:
//    code_rule(unsigned char oo, unsigned char ll, char cc) :o(oo), l(ll), c(cc) {}
//    unsigned char o, l;
//    //code_rule(int oo, int ll, char cc) :o(oo), l(ll), c(cc) {}
//    //int o, l;
//    char c;
//};
//
//bool readFileToString(string file_name, string& fileData);
//void input(int& search_buffer_size, int& look_ahead_size, string& s);
//void encoding(const int search_buffer_size, const int look_ahead_size, const string& s);
//void writeToFile(const vector<code_rule>& cr, const int search_buffer_size, const int look_ahead_size);
//void print_cr(const vector<code_rule>& cr,const string& s, const int& search_buffer_size);
//void readFileToCR(vector<code_rule>&);
//void decoding(const vector<code_rule>& cr, const int search_buffer_size, const int look_ahead_size, const string& sb_init, const string& ori_s);
//void adjust_parameters();
//
//int main() {
//    adjust_parameters();
//    return 0;
//}
//
//bool readFileToString(string file_name, string& fileData)
//{
//    ifstream file(file_name.c_str(), std::ifstream::binary);
//    if (file)
//    {
//        // Calculate the file's size, and allocate a buffer of that size.
//        file.seekg(0, file.end); // 基地址为文件结束处，偏移为0，指针定位到文件结束处
//        const int file_size = int(file.tellg());
//        char* file_buf = new char[file_size + 1];
//        //make sure the end tag \0 of string.
//        memset(file_buf, 0, file_size + 1);
//        // Read the entire file into the buffer.
//        file.seekg(0, ios::beg);  //基地址为文件头，偏移量为0，定位到文件头
//        file.read(file_buf, file_size);
//        fileData = "";
//        fileData.append(file_buf);
//        if (!file)
//        {
//            cout << "error: only " << file.gcount() << " could be read";
//            file.close();
//            delete[]file_buf;
//            return false;
//        }
//        file.close();
//        delete[]file_buf;
//        return true;
//    }
//    else
//    {
//        cout << "error: File open failed.\n";
//        return false;
//    }
//}
//
//void input(int& search_buffer_size, int& look_ahead_size, string& s) {
//    cout << "search buffer size = ";
//    cin >> search_buffer_size;
//    cout << "look ahead buffer = ";
//    cin >> look_ahead_size;
//    /*cout << "string to be coded: ";
//    cin >> s;*/
//    if (readFileToString("./data_base.txt", s))
//    {
//        //cout << "File data is:\r\n" << s << endl;
//    }
//    else
//    {
//        cout << "Failed to open the file, please check the file path." << endl;
//    }
//
//    cout << endl;
//}
//
//void encoding(const int search_buffer_size, const int look_ahead_size, const string& s_ori) {
//    vector<code_rule> cr;      //  存储每一条编码结果
//    int sbi = search_buffer_size - 1, lai = search_buffer_size, sLen = s_ori.length();
//    string s = s_ori + "$";    // '$'是空字符，只出现在末尾, 不算长度
//    while (lai < sLen) {       //  lai是look_ahead_buffer首字符下标
//        int sbj = sbi, max_match_length = 0, max_j = lai; // max_j 最长匹配串在search_buffer的开始位置
//        while (sbj > sbi - search_buffer_size) {          // 遍历search_buffer
//            if (s[sbj] == s[lai]) {
//                int j_match = sbj + 1, laj_match = lai + 1, sup=min(lai+look_ahead_size,sLen); //  sup是上界
//                while (j_match <= sbi && laj_match < sup && s[j_match] == s[laj_match])
//                {
//                    j_match++; laj_match++;
//                }
//                if (j_match == sbi + 1) {                 // 扫到search_buffer尾部，循环 sbj->sbi
//                    j_match = sbj;
//                    while (laj_match < sLen && s[j_match] == s[laj_match]) {
//                        j_match++;
//                        laj_match++;
//                        if (j_match == sbi) j_match = sbj;
//                    }
//                }
//                if (laj_match - lai > max_match_length) {  // 保存最长的匹配点
//                    max_match_length = laj_match - lai;
//                    max_j = sbj;
//                }
//            }
//            sbj--;
//        }
//        cr.push_back({ unsigned char(lai - max_j),unsigned char(max_match_length),s[lai + max_match_length] });
//        sbi += max_match_length + 1;
//        lai = sbi + 1;
//    }
//    s = s.substr(0, s.size() - 1);
//    writeToFile(cr, search_buffer_size, look_ahead_size); // 将结果粗略地写到文件里
//    print_cr(cr, s_ori, search_buffer_size);              // 打印编码效果
//}
//
//void writeToFile(const vector<code_rule>& cr, const int search_buffer_size, const int look_ahead_size) {
//    ofstream out("./LZ77_output.txt");
//    if (out.is_open())
//    {
//       // out << "search_buffer_size = " << search_buffer_size << "\nlook_ahead_size = " << look_ahead_size << endl << endl << "LZ77 compressed code:\n";
//        int crs = cr.size();
//        _for(i, crs) {
//            if (cr[i].c == '\r') out << "(" << int(cr[i].o) << "," << int(cr[i].l) << ",\\r)";
//            else if (cr[i].c == '\n')  out << "(" << int(cr[i].o) << "," << int(cr[i].l) << ",\\n)";
//            else out << "(" << int(cr[i].o) << "," << int(cr[i].l) << "," << cr[i].c << ")";
//           //  if (i % 10 == 0) out << "\n";
//           // else if (i < crs - 1) out << ", ";
//        }
//        out.close();
//    }
//    else cout << "OpenFileFailed.\n";
//}
//
//void print_cr(const vector<code_rule>& cr,const string& s, const int & search_buffer_size) {
//    int crs = cr.size(), compressed_length = sizeof(code_rule) * crs, original_length = s.length() * sizeof(char);
//    if (original_length == 0) {
//        cout << "null string.\n\n";
//        return;
//    }
//    double compression_rate = double(original_length - search_buffer_size - compressed_length) / double(original_length);
////    cout << "size of code_rule: " << sizeof(code_rule) << endl;
//    cout << "Code Rule: (o,l,c)\nCode Number: " << crs << "\n" 
//         << "Compressed Length: " << compressed_length <<"B\nOriginal Length: "<< original_length 
//        <<"B\nCompression Rate:"<<compression_rate<< "\n";
//   /* _for(i, crs) {
//        if(cr[i].c=='\r') cout << "(" << int(cr[i].o) << "," << int(cr[i].l) << ", '\\r')";
//        else if(cr[i].c == '\n')  cout << "(" << int(cr[i].o) << "," << int(cr[i].l) << ", '\\n')";
//        else cout << "(" << int(cr[i].o) << "," << int(cr[i].l) << "," << cr[i].c << ")";
//        if (i % 5 == 0) cout << "\n";
//        else if(i<crs-1) cout << ", ";
//    }*/
//    cout << "\n";
//}
//
//void readFileToCR(vector<code_rule>& cr) {
//    string fileData = "";
//    readFileToString("./LZ77_output.txt",fileData);
//    _for(i, fileData.length()) {
//        if (fileData[i] == '(') {
//            int o = 0; i++;
//            while (fileData[i] != ',') o = o * 10 + fileData[i++] - '0';
//            int l = 0; i++;
//            while (fileData[i] != ',') l = l * 10 + fileData[i++] - '0';
//            char c; i++;
//            if (fileData[i] == '\\' && fileData[i + 1] == 'r') c = '\r';
//            else if (fileData[i] == '\\' && fileData[i + 1] == 'n') c = '\n';
//            else c = fileData[i];
//            cr.push_back({ unsigned char(o),unsigned char(l),c });
//            i += 1;
//        }
//    }
//}
//
//void decoding(const int search_buffer_size, const int look_ahead_size, const string& sb_init,const string& ori_s) {
//    vector<code_rule> cr;
//    readFileToCR(cr);      // 从文件获取编码后的内容
//    string s = sb_init;    // 最初的search_ahead_buffer的内容
//    for (int ri = 0; ri < int(cr.size());ri++) {
//        //if (s != ori_s.substr(0, s.length())) {   // 一旦发现出错就停止匹配
//        //    cerr << s.substr(s.length() - search_buffer_size) << endl << endl;
//        //    cerr << ori_s.substr(s.length() - search_buffer_size, search_buffer_size);
//        //    break;
//        //}
//        if (cr[ri].o == 0) // 只有一个字符，直接加过来
//            s += cr[ri].c;
//        else {
//            const int sb_begin = s.length() - cr[ri].o;
//            int  sbi = sb_begin;
//            string decoded_substr = "";
//            _for(i, cr[ri].l) {
//                decoded_substr += s[sbi];
//                sbi++;
//                if (sbi == s.length()) sbi = sb_begin;
//            }
//            decoded_substr += cr[ri].c;
//            s += decoded_substr;
//        }
//    }
//    if (s[s.length() - 1] == '$') s = s.substr(0, s.length() - 1);
//    if (ori_s == s) cout << "\nCorrect decoding !\n\n";    // 判断解码结果正确性
//    else            cout<<"\nWrong decoding !\n\n";
//}
//
//void adjust_parameters() {
//    while (1) {
//        int search_buffer_size = 7, look_ahead_size = 6;
//        string s = "cabracadabrarrarrad"; //"abcdefghijklmnopqrstuvwxyz";
//        input(search_buffer_size, look_ahead_size, s);
//
//        string sb_init = s.substr(0, search_buffer_size);   //  用于译码
//        clock_t start,finish;
//        double  duration;
//
//        start = clock();
//        encoding(search_buffer_size, look_ahead_size, s);
//        finish = clock();
//        duration = (double)(finish - start) / CLOCKS_PER_SEC;
//        cout << "Encoding Run time: " << duration << "s\n";
//
//        start = clock();
//        decoding(search_buffer_size, look_ahead_size, sb_init, s);
//        finish = clock();
//        duration = (double)(finish - start) / CLOCKS_PER_SEC;
//        cout << "Decoding Run time: " << duration << "s\n\n\n";
//    }
//}