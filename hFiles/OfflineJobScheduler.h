#pragma once
#include<vector>
#include<algorithm>
#include<iostream>
#include<numeric>
using namespace std;

class OfflineJobScheduler {
private:
	int numJob; // No. 0 ~ numJob-1
	int numHost;// No. 0 ~ numHost-1
	int numCore;// Number of cores on each host
	double St;  // Speed of Transimision 
	int A, B;   // G(n)=A-Bn, A>0, B>0, n is the number of cores allocated to a single job
	vector<double> Sc;                    // Speed of calculation for each job
	vector<double> jobBlock;              // The number of blcok for each job
	vector<vector<double>> dataSize;      // Job->block number->block size
	vector<vector<vector<int>>> location; // Job->block number->host list of block location

	vector<double> finishTime;                 // The finish time of each job 
	vector<vector<tuple<int,int,int>>> runLoc; // Run block on which core: Job->block number->(hostID, coreID,rank), rank=1 means that block is the first task running on that core of that host
public:
	OfflineJobScheduler();
	void schedule();
	void outputSolution();
};