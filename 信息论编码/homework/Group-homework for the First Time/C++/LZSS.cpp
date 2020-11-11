//#include<iostream>
//#include<string>
//#include<vector>
//#include <fstream> 
//#include<algorithm>
//#include "time.h"
//using namespace std;
//#define _for(i,j) for(int i=0;i<j;i++)
//
//bool readFileToString(string file_name, string& fileData);
//void input(int& search_buffer_size, string& s);
//void encoding(const int search_buffer_size, const string& s_ori);
//void writeToFile(const string& s_encoded);
//void decoding(string&, const string& ori_s);
//void adjust_parameter();
//
//int main() {
//	adjust_parameter();
//	//int a = 127;
//	//char c =  char(a);
//	//string s = string(1, c);
//	//cout << c << endl;
//	//cout << s << endl;
//	//char cs = s[0];
//	//cout << s.length() << endl;
//	//cout << int(cs) << endl;1000
//	//cout << (cs == c) << endl;
//
//	return 0;
//}
//
//bool readFileToString(string file_name, string& fileData)
//{
//	ifstream file(file_name.c_str(), std::ifstream::binary);
//	if (file)
//	{
//		// Calculate the file's size, and allocate a buffer of that size.
//		file.seekg(0, file.end); // 基地址为文件结束处，偏移为0，指针定位到文件结束处
//		const int file_size = int(file.tellg());
//		char* file_buf = new char[file_size + 1];
//		//make sure the end tag \0 of string.
//		memset(file_buf, 0, file_size + 1);
//		// Read the entire file into the buffer.
//		file.seekg(0, ios::beg);  //基地址为文件头，偏移量为0，定位到文件头
//		file.read(file_buf, file_size);
//		fileData = "";
//		fileData.append(file_buf);
//		if (!file)
//		{
//			cout << "error: only " << file.gcount() << " could be read";
//			file.close();
//			delete[]file_buf;
//			return false;
//		}
//		file.close();
//		delete[]file_buf;
//		return true;
//	}
//	else
//	{
//		cout << "error: File open failed.\n";
//		return false;
//	}
//}
//
//void input(int& search_buffer_size, string& s) {
//	cout << "search buffer size = ";
//	cin >> search_buffer_size;
//
//	if (!readFileToString("./data_base.txt", s))
//		cout << "Failed to open the file, please check the file path." << endl;
//	cout << endl;
//}
//
//void encoding(const int search_buffer_size, const string& s_ori) {
//	clock_t encode_start, encode_finish;
//	encode_start = clock();
//	double  encode_duration;
//	int code_num = 0, sbi = 4, lai = 5, sLen = s_ori.length(); // LZSS不可能对前5个字符编码
//	int max_match_num = 0;                                     // 记录所有编码中最长的一次匹配的字符个数
//	string s_encoded = s_ori.substr(0, 5);                     // 直接编码到字符串里
//	while (lai < sLen) {                                       // look_ahead_buffer最左端在lai，lai=sbj+1
//		int sbj = sbi, max_reduce_length = 0;                  // 从search_buffer的最后一位sbj往前搜索
//		int sb_min = max(-1, sbi - search_buffer_size);        // 搜索的下界
//		string best_code = string(1, s_ori[lai]);              // 记录search_buffer里最佳的一个编码
//		while (sbj > sb_min) {                                 // 遍历search_buffer
//			if (s_ori[sbj] == s_ori[lai]) { // search_buffer的j位置匹配到look_ahead_buffer的i位置，开始一轮匹配
//				int j_match = sbj + 1, laj_match = lai + 1;
//				while (j_match <= sbi && laj_match < sLen && s_ori[j_match] == s_ori[laj_match])
//				{
//					j_match++; laj_match++;
//				}
//				if (j_match == sbi + 1) {                      // 扫到search_buffer尾部，循环 sbj->sbi
//					j_match = sbj;
//					while (laj_match < sLen && s_ori[j_match] == s_ori[laj_match]) {
//						j_match++;
//						laj_match++;
//						if (j_match == sbi) j_match = sbj;     // 仍可继续循环
//					}
//				}
//				int match_num = laj_match - lai, ahead_num = lai - sbj;
//				// 直接把数字转为string太长了
//				//string wish_code = "$" + to_string(ahead_num) + "," + to_string(match_num) + "$";
//			/*	if (lai > 9820) {
//					cout << s_encoded.substr(s_encoded.length() - 20, 20) << endl;
//					cout << s_ori.substr(s_ori.length() - 20, 22) << endl;
//				}*/
//				string aheads = "";
//				if (ahead_num < 127) aheads = string(1, char(ahead_num));// 数较小时用一个char存储再转为string
//				else aheads = string(1, char(ahead_num / 127)) + string(1, char(ahead_num % 127));
//
//				char matchc = char(match_num);                           // 不超过126, 127用于编码左右边界字符
//				string matchs = string(1, matchc);
//				string boundary = string(1, char(127));
//				//  matchs只会是一个字符，所以中间的逗号可以省掉！
//				string wish_code = boundary + aheads + matchs + boundary;// 构造这轮search期望的编码  
//
//				int reduce_length = match_num - wish_code.length();      // 计算这个编码缩短了多少
//				if (reduce_length > max_reduce_length) {                 // 如果缩短了更多，就记录它
//					best_code = wish_code;
//					max_reduce_length = reduce_length;
//				}
//			}
//			sbj--;
//		}
//		if (best_code.length() > 1) code_num++;                          // 记录缩短次数（编码个数）
//		s_encoded += best_code;
//		sbi += max_reduce_length + int(best_code.length());              // 右移滑窗
//		max_match_num = max(max_match_num, max_reduce_length + int(best_code.length()));
//		lai = sbi + 1;
//	}
//	encode_finish = clock();
//	encode_duration = (double)(encode_finish - encode_start) / CLOCKS_PER_SEC;
//
//
//	if (s_ori.length() == 0) {
//		cout << "null string.\n\n";
//		return;
//	}
//	double compression_rate = double(s_ori.length() - s_encoded.length()) / double(s_ori.length());
//	cout << "Encoding Run time: " << encode_duration << "s" << endl;
//	cout << "max match number: " << max_match_num << endl;
//	cout << "code num(压缩次数): " << code_num << endl;
//	cout << "Compressed Length: " << s_encoded.length() << "B\nOriginal Length: " << s_ori.length()
//		<< "B\nCompression Rate: " << compression_rate << "\n\n";
//
//	writeToFile(s_encoded);
//	decoding(s_encoded, s_ori);
//}
//
//void writeToFile(const string& s_encoded) {
//	ofstream out("./LZSS_output.txt");
//	if (out.is_open())
//	{
//		out << s_encoded;
//		out.close();
//	}
//	else cout << "OpenFileFailed.\n";
//}
//
//// 由于存到文本文件里然后取出来时 '\r\n'会变成'\r\r\n'，于是直接传入压缩后的string和原始string，不再从文本文件里拿压缩后的string
//void decoding(string& s_encoded, const string& s_ori) {  // 编码字符串，原始字符串
//	clock_t decode_start, decode_finish;
//	decode_start = clock();
//	double  decode_duration;
//	string s_decoded = "";								 // 解码字符串
//	char boundary = char(127);							 // 边界字符
//	int s_en_len = s_encoded.length();
//	//readFileToString("./LZSS_output.txt", s_encoded);
//	_for(i, s_en_len - 3) {                               // 最后3个字符不可能编码，同时要防止下面if越界
//		if (s_encoded[i] != boundary)                    // 此位置未编码压缩
//			s_decoded += s_encoded[i];
//		else {
//			int lai = s_decoded.length();                // look_ahead初始位置下标
//			int ahead_num = 0, match_num = 0;            // 往前看字符个数
//			i++;
//			//while (s_encoded[i] != ',') ahead_num = ahead_num * 10 + s_encoded[i++] - '0';
//			//while (s_encoded[++i] != '$') match_num = match_num * 10 + s_encoded[i] - '0';
//			// 用逗号作分隔，可能ahead包括了这个逗号！！！
//			//while (s_encoded[i] != ',')   ahead_num = ahead_num * 128 + int(char(s_encoded[i++]));
//			if (s_encoded[i + 2] == boundary)            // 现在必须先判断+2而不是+3的位置了!!! (之前必须先判断+3)
//				ahead_num = int(char(s_encoded[i++]));   // ahead_num是1位127进制数
//			else {                                       // 否则ahead_num是2位127进制数
//				ahead_num = int(char(s_encoded[i])) * 127 + int(char(s_encoded[i + 1]));
//				i += 2;
//			}
//
//			char c = s_encoded[i]; i++;                  // 此时s_encoded[i] == boundary
//			match_num = int(c);                          // 匹配的字符个数不会超过127（实际上一半都不到）
//
//			if (s_encoded[i] != boundary) {              // 错误检测
//				cerr <<"Error Occurred: "<< s_encoded.substr(i - 20, 22) << endl;
//				break;
//			}
//
//			const int sb_begin = lai - ahead_num;        // 从sb_begin开始获取解码后的字符
//			int  sbi = sb_begin;  
//			_for(j, match_num) {                         // 可兼容循环遍历search_buffer的匹配子串
//				s_decoded += s_decoded[sbi];
//				sbi++;
//				if (sbi == lai) sbi = sb_begin;          // 循环
//			}
//
//		}                
//		if (s_decoded != s_ori.substr(0, s_decoded.length())) { 
//			cerr << s_encoded.substr(i - 20, 22) << endl;
//			cerr << s_decoded.substr(s_decoded.length() - 20, 22) << endl;
//			cerr << s_ori.substr(s_decoded.length() - 20, 22) << endl;
//			break;
//		}
//	}
//	s_decoded += s_encoded.substr(max(0, s_en_len - 3), min(3, s_en_len));
//
//	decode_finish = clock();
//	decode_duration = (double)(decode_finish - decode_start) / CLOCKS_PER_SEC;
//	cout << "Decoding Run time: " << decode_duration << "s\n";
//	if (s_ori == s_decoded) cout << "\nCorrect decoding !\n\n";
//	else cout << "\nWrong decoding !\n\n";
//}
//
//void adjust_parameter() {
//	while (1) {
//		int search_buffer_size = 7, look_ahead_size = 6;
//		string s_ori = "aaaaaaaaaaaaa";
//		input(search_buffer_size, s_ori);
//		encoding(search_buffer_size, s_ori);
//		//decoding(search_buffer_size, s_ori);
//	}
//}