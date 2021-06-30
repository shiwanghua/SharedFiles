/*
 * Author: Swhua
 */

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include <unordered_map>
#include <vector>
#include <wctype.h>

using namespace std;

#define BufferSize 1024
#define FINISH_MSG 0 // 其他进程发给0号进程，表示完成word-count-map任务
#define READY_MSG 1 // 其他进程发给0号进程，表示准备好了
#define LENGTH_MSG 2 // 0号进程发给其他进程，表示需计数的文本大小
#define TEXT_MSG 3 // 0号进程发给其他进程，表示需计数的文本
#define STOP_MSG 4 // 0号进程发给其他进程，表示可以进行reduce了
// 以BufferSize为单位长度轮询地发数据给进程
void big_file_word_count_fixed() {
    int p, id, i, length;
    string word;
    unordered_map<string, int> wordCount;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (!id) {
        int nextp = 0; // 下一次发送给这个进程号
        char ch;       // connector: 行末换行用该字符隔开单词
        // 读这么多个字符后遇到第一个非字母才停止读，避免单词分裂
        int length_upper_bount = BufferSize - 10;
        int count;
        ifstream f("big_file/big_100.txt");
        MPI_Request pending;

        while (true) {
            length = 0;
            ostringstream buf;
            while (length_upper_bount != length && f.get(ch)) {
                buf.put(ch);
                length++;
            }
            // 读满最后这个单词
            while (length < BufferSize && f.get(ch) && iswalpha(ch)) {
                buf.put(ch);
                length++;
            }
            if (!length) {
                f.close();
                break;
            }
            nextp = nextp % (p - 1) + 1;
            MPI_Send(&length, 1, MPI_INT, nextp, LENGTH_MSG, MPI_COMM_WORLD);
            MPI_Send(buf.str().c_str(), length, MPI_CHAR, nextp, TEXT_MSG,
                     MPI_COMM_WORLD);
            // printf("send to p%d, length=%d\n", nextp, length);
            // fflush(stdout);
        }

        for (i = 1; i < p; i++)
            MPI_Isend(&length, 1, MPI_INT, i, LENGTH_MSG, MPI_COMM_WORLD,
                      &pending);

        // 每接受到一个进程的结束信号就读取那个进程的结果文件（reduce）
        for (i = 1; i < p; i++) {
            MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, FINISH_MSG,
                     MPI_COMM_WORLD, &status);
            // 部分结果
            string path = "big_file/p" + to_string(status.MPI_SOURCE) + ".txt";
            ifstream p_result(path);
            while (!p_result.eof()) {
                p_result >> word >> count;
                wordCount[word] += count;
            }
            p_result.close();
            remove(path.c_str());
        }
        vector<pair<string, int>> wordCountv;
        for (auto wc : wordCount) {
            wordCountv.push_back(wc);
        }
        sort(wordCountv.begin(), wordCountv.end(),
             [](const pair<string, int> &a, const pair<string, int> &b) {
                 return a.second > b.second;
             });
        ofstream o("big_file/final_result.txt");
        int totalNum = 0;
        for (auto a : wordCountv) {
            o << a.first << " " << a.second << "\n";
            totalNum += a.second;
        }
        o << "Total Number: " << totalNum;
        o.close();
    } else {
        char buffer[BufferSize];
        while (true) {
            MPI_Recv(&length, 1, MPI_INT, 0, LENGTH_MSG, MPI_COMM_WORLD,
                     &status);
            if (!length) // 长度为0代表可以结束了
                break;
            MPI_Recv(buffer, length, MPI_CHAR, 0, TEXT_MSG, MPI_COMM_WORLD,
                     &status);
            string text(buffer, length);
            length = text.length();
            i = 0;
            while (i < length) {
                word = "";
                while (i < length && text[i] != ',' && text[i] != '.' &&
                       text[i] != '\n') {
                    if (text[i] == ' ') { // 如果是行末的空格，则有可能有连字符
                        if (i + 1 < length && text[i + 1] == '\n') {
                            // printf("--------------%d------------\n", i);
                            // fflush(stdout);
                            if (i > 1 &&
                                text.substr(i - 2, 2) != "¬") // i 有可能小于2！
                                break; // 行末，单词结束
                            else
                                word = word.substr(0, word.length() -
                                                          2); // 删除连字符
                            i++;
                        } else
                            break; // 行内的空格，单词结束
                    } else if (iswalpha(text[i]))
                        word += tolower(text[i]);
                    else
                        break; // , . \n ' ' ? ! “ ”
                    i++;
                }
                i++;
                if (word != "")
                    wordCount[word]++;
            }
        }

        ofstream o("big_file/p" + to_string(id) + ".txt");
        for (auto a : wordCount) {
            o << a.first << " " << a.second << "\n";
        }
        o.close();
        MPI_Send(NULL, 0, MPI_INT, 0, FINISH_MSG, MPI_COMM_WORLD);
    }
}

// 根据进程个数把文本平均分，每个进程只收一次文本
void big_file_word_count_avg() {
    int p, id, i, length, bufferSize;
    string word;
    unordered_map<string, int> wordCount;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (!id) {
        char ch; // connector: 行末换行用该字符隔开单词
        int count;
        ifstream f("big_file/big_100.txt");
        MPI_Request pending;

        // 也可以根据文本长度和进程个数调整BufferSize大小,
        // 减少发送次数，但这样刚开始时后面的进程等待的时间较长
        f.seekg(0, std::ios::end); // go to the end
        length = f.tellg();        // report location (this is the length)
        // printf("length=%d\n", length);
        // fflush(stdout);
        bufferSize = length / (p - 1); // BufferSize
        f.seekg(0, std::ios::beg);     // go back to the beginning
        // char *buffer = new char[length];
        // f.read(buffer, length);

        for (i = 1; i < p; i++) {
            length = 0;
            ostringstream buf;
            if (i == p - 1) { // 把剩下的全部发给最后一个进程
                while (f.get(ch)) {
                    buf.put(ch);
                    length++;
                }
            } else { // 前面的进程
                while (length < bufferSize && f.get(ch)) {
                    buf.put(ch);
                    length++;
                }
                // 读满最后这个单词
                while (f.get(ch) && iswalpha(ch)) {
                    buf.put(ch);
                    length++;
                }
            }

            MPI_Send(&length, 1, MPI_INT, i, LENGTH_MSG, MPI_COMM_WORLD);
            if (!length) { // 防止进程多文本短，每个进程连1个字符都分不到的情况
                f.close();
                break;
            }
            MPI_Send(buf.str().c_str(), length, MPI_CHAR, i, TEXT_MSG,
                     MPI_COMM_WORLD);
            // printf("send to p%d, length=%d\n", i, length);
            // fflush(stdout);
        }

        // 每接受到一个进程的结束信号就读取那个进程的结果文件（reduce）
        for (i = 1; i < p; i++) {
            MPI_Recv(&length, 1, MPI_INT, MPI_ANY_SOURCE, FINISH_MSG,
                     MPI_COMM_WORLD, &status);
            // printf("mark1: %d\n", status.MPI_SOURCE);
            // fflush(stdout);
            // 部分结果
            string path = "big_file/p" + to_string(status.MPI_SOURCE) + ".txt";
            ifstream p_result(path);
            while (!p_result.eof()) {
                p_result >> word >> count;
                wordCount[word] += count;
            }
            p_result.close();
            remove(path.c_str());
        }
        vector<pair<string, int>> wordCountv;
        for (auto wc : wordCount) {
            wordCountv.push_back(wc);
        }
        sort(wordCountv.begin(), wordCountv.end(),
             [](const pair<string, int> &a, const pair<string, int> &b) {
                 return a.second > b.second;
             });
        ofstream o("big_file/final_result.txt");
        int totalNum = 0;
        for (auto a : wordCountv) {
            o << a.first << " " << a.second << "\n";
            totalNum += a.second;
        }
        o << "Total Number: " << totalNum;
        o.close();
    } else {

        MPI_Recv(&length, 1, MPI_INT, 0, LENGTH_MSG, MPI_COMM_WORLD, &status);
        // printf("receive in p%d, length=%d\n", id, length);
        // fflush(stdout);
        if (!length)
            MPI_Send(&length, 1, MPI_INT, 0, FINISH_MSG, MPI_COMM_WORLD);
        char buffer[length];
        MPI_Recv(buffer, length, MPI_CHAR, 0, TEXT_MSG, MPI_COMM_WORLD,
                 &status);
        string text(buffer, length);
        i = 0;
        // printf("%s\n", text.c_str());
        // fflush(stdout);
        while (i < length) {
            word = "";
            while (i < length) {
                if (text[i] == ' ') { // 如果是行末的空格，则有可能有连字符
                    if (i + 1 < length && text[i + 1] == '\n') {
                        // printf("--------------%d------------\n", i);
                        // fflush(stdout);
                        if (i > 1 &&
                            text.substr(i - 2, 2) != "¬") // i 有可能小于2！
                            break; // 行末，单词结束
                        else
                            word =
                                word.substr(0, word.length() - 2); // 删除连字符
                        i++;
                    } else
                        break; // 行内的空格，单词结束
                } else if (iswalpha(text[i]))
                    word += tolower(text[i]);
                else
                    break; // , . \n ' ' ? ! “ ”
                i++;
            }
            i++;
            if (word != "")
                wordCount[word]++;
        }

        ofstream o("big_file/p" + to_string(id) + ".txt");
        for (auto a : wordCount)
            o << a.first << " " << a.second << "\n";
        o.close();
        MPI_Send(&length, 1, MPI_INT, 0, FINISH_MSG, MPI_COMM_WORLD);
    }

    // const string path = "big_file/p" + to_string(1) + ".txt";
    // FILE *o = fopen(path.c_str(), "w");
    // for (auto a : wordCount) {
    //     fprintf(o, "%s %d\n", a.first.c_str(), a.second);
    //     cout << a.first << " " << a.second << endl;
    // }
    // fclose(o);
}

struct SmallFile {
    int index, length;
};

// 已知有100个文件
void small_file_word_count(int type) { // type=1: 对文件从大到小排序
    int p, id, i, length;
    string word, path;
    unordered_map<string, int> wordCount;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (!id) {
        int fileNo = 0; // 下一次发这个文件
        char ch;        // connector: 行末换行用该字符隔开单词
        int count, numTerminated = 0, source;
        int numFile[p] = {0}; // 每个进程处理了多少个文件
        int sendNum = 0, fileNum = 100;
        MPI_Request pending_s, pending_r;

        // 读取每个文件的大小
        SmallFile smallFile[fileNum];
        for (i = 0; i < fileNum; i++)
            smallFile[i].index = i;

        if (type)
            // 根据文本长度对文本排序，把长文本均匀分配给进程
            for (i = 0; i < fileNum; i++) {
                string num;
                stringstream ss;
                ss << setw(2) << setfill('0') << i; // 补0
                ss >> num;
                path = "small_file/small_1" + num + ".txt";
                ifstream f(path);
                f.seekg(0, std::ios::end);
                smallFile[i].index = i;
                smallFile[i].length = f.tellg();
                f.close();
            }
        sort(smallFile, smallFile + fileNum,
             [](const SmallFile &f1, const SmallFile &f2) {
                 return f1.length > f2.length;
             });
        // for (i = 0; i < 100; i++) {
        //     printf("%d: file %d length= %d Bytes\n", i, smallFile[i].index,
        //            smallFile[i].length);
        //     fflush(stdout);
        // }
        do {
            MPI_Irecv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, READY_MSG,
                      MPI_COMM_WORLD, &pending_r);
            // MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, READY_MSG,
            //          MPI_COMM_WORLD, &status);
            ostringstream buf;
            length = 0;
            if (fileNo < fileNum) {
                string prefix = "";
                if (smallFile[fileNo].index < 10)
                    prefix = "0";
                path = "small_file/small_1" + prefix +
                       to_string(smallFile[fileNo].index) + ".txt";
                // path = "small_file/small_1" +
                //        *(smallFile[fileNo].index < 10 ? "0" : "") +
                //        to_string(smallFile[fileNo].index) + ".txt"; //
                //        个位数时仍有问题
                ifstream f(path);
                if (!f.is_open()) {
                    printf("File %d open fail\n", smallFile[fileNo].index);
                    fflush(stdout);
                }
                while (f.get(ch)) {
                    buf.put(ch);
                    length++;
                }
                f.close();
                // printf("length=%d, fileNo=%d, fileName=%d, trueLength=%d\n",
                //        length, fileNo, smallFile[fileNo].index,
                //        smallFile[fileNo].length);
                // fflush(stdout);
            }
            fileNo++;
            MPI_Wait(&pending_r, &status);
            source = status.MPI_SOURCE;
            if (length ||
                (!type && fileNo <= fileNum)) { // 如果没有排序则必须进入！
                MPI_Send(&length, 1, MPI_INT, source, LENGTH_MSG,
                         MPI_COMM_WORLD);
                if (length) // Bug: type=0,length=0时不发送
                    MPI_Send(buf.str().c_str(), length, MPI_CHAR, source,
                             TEXT_MSG, MPI_COMM_WORLD);
                numFile[source]++;
                // printf("send file%d to p%d\n", fileNo - 1, source);
                // fflush(stdout);
                sendNum++;
            } else { // 排序了的话，length=0就代表没文件了或者后面的文件都是空文件了，直接终止
                MPI_Isend(&length, 1, MPI_INT, source, STOP_MSG, MPI_COMM_WORLD,
                          &pending_s); // length=0
                // printf("Terminate p%d fileNo= %d\n", source, fileNo);
                // fflush(stdout);
                // MPI_Send(&length, 1, MPI_INT, source, STOP_MSG,
                // MPI_COMM_WORLD);
                numTerminated++;
            }
        } while (numTerminated < p - 1);
        // printf("out----------------\n");
        // 每接受到一个进程的结束信号就读取那个进程的结果文件（reduce）
        for (i = 1; i < p; i++) {
            MPI_Recv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, FINISH_MSG,
                     MPI_COMM_WORLD, &status);
            // 部分结果
            path = "small_file/p" + to_string(status.MPI_SOURCE) + ".txt";
            ifstream p_result(path);
            while (!p_result.eof()) {
                p_result >> word >> count;
                wordCount[word] += count;
            }
            p_result.close();
            remove(path.c_str());
        }
        vector<pair<string, int>> wordCountv;
        for (auto wc : wordCount) {
            wordCountv.push_back(wc);
        }
        sort(wordCountv.begin(), wordCountv.end(),
             [](const pair<string, int> &a, const pair<string, int> &b) {
                 return a.second > b.second;
             });
        ofstream o("small_file/final_result.txt");
        int totalNum = 0;
        for (auto a : wordCountv) {
            o << a.first << " " << a.second << "\n";
            totalNum += a.second;
        }
        o << "Total Number: " << totalNum;
        o.close();
        // for (i = 1; i < p; i++) {
        //     printf("p=%d, process file number: %d\n", i, numFile[i]);
        //     fflush(stdout);
        // }
        // printf("sendNum=%d\n", sendNum);
        // fflush(stdout);
    } else {
        MPI_Request pending;
        // int c = 0;
        while (true) {
            // MPI_Send(NULL, 0, MPI_INT, 0, READY_MSG, MPI_COMM_WORLD);
            MPI_Isend(NULL, 0, MPI_INT, 0, READY_MSG, MPI_COMM_WORLD, &pending);
            MPI_Recv(&length, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD,
                     &status);
            // c++;
            // printf("receive: %d\n", c);
            // fflush(stdout);
            if (status.MPI_TAG == STOP_MSG)
                break;
            // Bug: 不能直接跳过！！否则0号进程会卡在send那里,
            // 所以type=0且length=0时不发送
            if (!length) // 空文件，直接跳过
                continue;
            char buffer[length];
            MPI_Recv(buffer, length, MPI_CHAR, 0, TEXT_MSG, MPI_COMM_WORLD,
                     &status);
            string text(buffer, length);
            i = 0;
            // printf("%s\n", text.c_str());
            // fflush(stdout);
            while (i < length) {
                word = "";
                while (i < length) {
                    if (text[i] == ' ') { // 如果是行末的空格，则有可能有连字符
                        if (i + 1 < length && text[i + 1] == '\n') {
                            // printf("--------------%d------------\n", i);
                            // fflush(stdout);
                            if (i > 1 &&
                                text.substr(i - 2, 2) != "¬") // i 有可能小于2！
                                break; // 行末，单词结束
                            else
                                word = word.substr(0, word.length() -
                                                          2); // 删除连字符
                            i++;
                        } else
                            break; // 行内的空格，单词结束
                    } else if (iswalpha(text[i]))
                        word += tolower(text[i]);
                    else
                        break; // , . \n ' ' ? ! “ ”
                    i++;
                }
                i++;
                if (word != "")
                    wordCount[word]++;
            }
        }
        ofstream o("small_file/p" + to_string(id) + ".txt");
        for (auto a : wordCount)
            o << a.first << " " << a.second << "\n";
        o.close();
        // MPI_Send(NULL, 0, MPI_INT, 0, FINISH_MSG, MPI_COMM_WORLD);
        MPI_Isend(NULL, 0, MPI_INT, 0, FINISH_MSG, MPI_COMM_WORLD, &pending);
    }
}

// BigFile: 对一个大文本里的单词计数
// SmallFile: 对100个小文本的单词计数
int main(int argc, char **argv) {
    int p, id;
    double elapsed_time = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    if (p < 2 && !id) {
        printf("进程数太少！\n");
        fflush(stdout);
        MPI_Finalize();
        exit(1);
    }

    // MPI_Barrier 对结果的影响不大
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    elapsed_time = -MPI_Wtime();
    big_file_word_count_fixed();
    elapsed_time += MPI_Wtime();
    if (!id) {
        printf("p=%d, BigFile FixedLength Run Time: %5.7f s\n", p,
               elapsed_time);
        fflush(stdout);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    elapsed_time = -MPI_Wtime();
    big_file_word_count_avg();
    elapsed_time += MPI_Wtime();
    if (!id) {
        printf("p=%d, BigFile AvgLength Run Time: %5.7f s\n", p, elapsed_time);
        fflush(stdout);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    elapsed_time = -MPI_Wtime();
    small_file_word_count(0);
    elapsed_time += MPI_Wtime();
    if (!id) {
        printf("p=%d, SmallFile UnSort Run Time: %5.7f s\n", p, elapsed_time);
        fflush(stdout);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    elapsed_time = -MPI_Wtime();
    small_file_word_count(1);
    elapsed_time += MPI_Wtime();
    if (!id) {
        printf("p=%d, SmallFile Sorted Run Time: %5.7f s\n", p, elapsed_time);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}
