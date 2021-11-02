#include "../hFiles/OfflineJobScheduler.h"

OfflineJobScheduler::OfflineJobScheduler() {
	freopen("../input/case1.txt", "r", stdin);
	cin >> numJob >> numHost >> numCore >> St >> A >> B;

	Sc.resize(numJob);
	for (int i = 0; i < numJob; i++)
		cin >> Sc[i];

	jobBlock.resize(numJob);
	for (int i = 0; i < numJob; i++)
		cin >> jobBlock[i];

	dataSize.resize(numJob);
	for (int i = 0; i < numJob; i++) {
		dataSize[i].resize(jobBlock[i]);
		for (int j = 0; j < jobBlock[i]; j++)
			cin >> dataSize[i][j];
	}

	location.resize(numJob);
	for (int i = 0; i < numJob; i++) {
		location[i].resize(jobBlock[i], vector<int>(2));
		for (int j = 0; j < jobBlock[i]; j++)
			cin >> location[i][j][0] >> location[i][j][1];
	}

	runLoc.resize(numJob);
	for (int i = 0; i < numJob; i++)
		runLoc[i].resize(jobBlock[i]);

	finishTime.resize(numJob);
}

void OfflineJobScheduler::schedule() {
	for (int i = 0; i < numJob; i++) {
		for (int j = 0; j < jobBlock[i]; j++) {
			runLoc[i][j] = make_tuple(i, i, j);
		}
	}
}

void OfflineJobScheduler::outputSolution() {
	cout << "Solution of Teaching Assistant:\n\n";
	for (int i = 0; i < numJob; i++) {
		cout << "Job" << i << " finishes at time " << finishTime[i] << ": \n";
		for (int j = 0; j < jobBlock[i]; j++) {
			cout << "\tBlock" << j << ": H" << get<0>(runLoc[i][j]) << ", C" << get<1>(runLoc[i][j]) << ", R" << get<2>(runLoc[i][j]) << " \n";
		}
		cout << "\n";
	}

	cout << "The maximum finish time: " << *max_element(finishTime.begin(), finishTime.end()) << "\n";
	cout << "The total response time: " << accumulate(finishTime.begin(), finishTime.end(), 0) << "\n";
}
