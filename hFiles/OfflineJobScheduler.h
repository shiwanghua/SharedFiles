#pragma once
#include<vector>
#include<algorithm>
#include<iostream>
#include<iomanip>
#include<numeric>
#include<ctime>
#include<set>
using namespace std;

class OfflineJobScheduler {
public:
	int numJob; // No. 0 ~ numJob-1
	int numHost;// No. 0 ~ numHost-1
	double St;  // Speed of Transimision 
	double alpha;   // G(e)=1-alpha(e-1) alpha>0, e is the number of cores allocated to a single job
	vector<int> hostCore;              // The number of cores for each host
	vector<int> jobBlock;              // The number of blocks for each job
	vector<double> Sc;                    // Speed of calculation for each job
	vector<vector<double>> dataSize;      // Job-> block number-> block size
	vector<vector<int>> location;         // Job-> block number-> block location (host number)

	vector<double> finishTime;                 // The finish time of each job 
	vector<int> jobCore;                       // The number of cores allocated to each job.
	vector<vector<tuple<int, int, int>>> runLoc; // Run block on which core: Job number->block number->(hostID, coreID,rank), rank=1 means that block is the first task running on that core of that host

	OfflineJobScheduler();
	void schedule();
	void outputSolution();
	void visualization(); // An optional fuction.
	double g(int);
};