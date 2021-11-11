#include "../hFiles/OfflineJobScheduler.h"

OfflineJobScheduler::OfflineJobScheduler() {
	freopen("../input/task1_case1.txt", "r", stdin);
	cin >> numJob >> numHost >> St >> alpha;

	hostCore.resize(numHost);
	for (int i = 0; i < numHost; i++)
		cin >> hostCore[i];

	jobBlock.resize(numJob);
	for (int i = 0; i < numJob; i++)
		cin >> jobBlock[i];

	Sc.resize(numJob);
	for (int i = 0; i < numJob; i++)
		cin >> Sc[i];

	dataSize.resize(numJob);
	for (int i = 0; i < numJob; i++) {
		dataSize[i].resize(jobBlock[i]);
		for (int j = 0; j < jobBlock[i]; j++)
			cin >> dataSize[i][j];
	}

	location.resize(numJob);
	for (int i = 0; i < numJob; i++) {
		location[i].resize(jobBlock[i]);
		for (int j = 0; j < jobBlock[i]; j++)
			cin >> location[i][j];
	}

	runLoc.resize(numJob);
	for (int i = 0; i < numJob; i++)
		runLoc[i].resize(jobBlock[i]);

	finishTime.resize(numJob,0);
	jobCore.resize(numJob);
}

void OfflineJobScheduler::schedule() {

	vector<vector<int>> hostCoreBlock(numHost);
	for (int i = 0; i < numHost; i++)
		hostCoreBlock[i].resize(hostCore[i], 0);

	for (int i = 0; i < numJob; i++) {
		set<pair<int, int>> allocatedJobCore;
		for (int j = 0; j < jobBlock[i]; j++) {
			int hid = rand() % numHost;
			int cid = rand() % hostCore[hid];
			allocatedJobCore.insert({ hid,cid });
			runLoc[i][j] = make_tuple(hid, cid, hostCoreBlock[hid][cid]++);
		}
		/*for (int j = 0; j < jobBlock[i]; j++) 
			finishTime[i]+=*/
		finishTime[i] = rand() % 200;
		jobCore[i] = allocatedJobCore.size();
	}
}

void OfflineJobScheduler::outputSolution() {
	cout << "Task2 Solution of Teaching Assistant:\n\n";
	for (int i = 0; i < numJob; i++) {
		double speed = g(jobCore[i]);
		cout << "Job" << i << " obtains " << jobCore[i] << " cores (speed=" << speed << ") and finishes at time " << finishTime[i] << ": \n";
		for (int j = 0; j < jobBlock[i]; j++) {
			cout << "\tBlock" << j << ": H" << get<0>(runLoc[i][j]) << ", C" << get<1>(runLoc[i][j]) << ", R" << get<2>(runLoc[i][j]) << " (time=" << fixed << setprecision(2) << dataSize[i][j] / speed << ")" << " \n";
		}
		cout << "\n";
	}

	cout << "The maximum finish time: " << *max_element(finishTime.begin(), finishTime.end()) << "\n";
	cout << "The total response time: " << accumulate(finishTime.begin(), finishTime.end(), 0.0) << "\n";
}

void OfflineJobScheduler::visualization() {

}

double OfflineJobScheduler::g(int e) {
	return 1 - alpha * (1 - e);
}