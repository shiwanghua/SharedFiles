#include "../hFiles/ResourceScheduler.h"

ResourceScheduler::ResourceScheduler(int tasktype, int caseID) {
	taskType = tasktype;
	string filePath = "../input/task" + to_string(taskType) + "_case" + to_string(caseID) + ".txt";
	freopen(filePath.c_str(), "r", stdin);
	cin >> numJob >> numHost >> alpha;
	if (taskType == 2)
		cin >> St;

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

	jobFinishTime.resize(numJob, 0);
	jobCore.resize(numJob);

	runLoc.resize(numJob);
	for (int i = 0; i < numJob; i++)
		runLoc[i].resize(jobBlock[i]);

	hostCoreTask.resize(numHost);
	for (int i = 0; i < numHost; i++)
		hostCoreTask[i].resize(hostCore[i]);

	hostCoreFinishTime.resize(numHost);
	for (int i = 0; i < numHost; i++)
		hostCoreFinishTime[i].resize(hostCore[i], 0);
}

void ResourceScheduler::schedule() {
	// 以下代码是为了测试，不是合理方案

	vector<vector<int>> hostCoreBlock(numHost);
	for (int i = 0; i < numHost; i++)
		hostCoreBlock[i].resize(hostCore[i], 0);

	int hid, cid; // hostId, coreId
	for (int i = 0; i < numJob; i++) {
		set<pair<int, int>> allocatedJobCore;
		double jobDataSize = 0.0;
		for (int j = 0; j < jobBlock[i]; j++) {
			jobDataSize += dataSize[i][j];
			if (g(allocatedJobCore.size() + 1) < 0) { // 再加新核导致速度为负
				set<pair<int,int>>::const_iterator position(allocatedJobCore.begin());
				advance(position, rand() % allocatedJobCore.size()); // 随机取一个已分配核计算当前数据块
				hid = position->first;
				cid = position->second;
			}
			else {
				hid = rand() % numHost;
				cid = rand() % hostCore[hid];
				allocatedJobCore.insert({ hid,cid });
			}
			runLoc[i][j] = make_tuple(hid, cid, ++hostCoreBlock[hid][cid]); // rank 从1开始
		}

		jobFinishTime[i] = jobDataSize / (Sc[i] * g(allocatedJobCore.size()));
		jobCore[i] = allocatedJobCore.size();
	}

	for (int i = 0; i < numHost; i++) {
		for (int j = 0; j < hostCore[i]; j++) {
			int numTask = rand() % 10 + 1;
			hostCoreTask[i][j].resize(numTask);
			for (int k = 0; k < numTask; k++) {
				int jid = rand() % numJob;
				int bid = rand() % jobBlock[jid];
				int endTime = hostCoreFinishTime[i][j] + rand() % 100 + 1;
				hostCoreTask[i][j][k] = make_tuple(jid, bid, hostCoreFinishTime[i][j], endTime);
				hostCoreFinishTime[i][j] = endTime;
			}
		}
	}
}

// 以数据块的视角打印解决方案
void ResourceScheduler::outputSolutionFromBlock() {
	cout << "\nTask" << taskType << " Solution (Block Perspective) of Teaching Assistant:\n\n";
	for (int i = 0; i < numJob; i++) {
		double speed = g(jobCore[i]) * Sc[i];
		cout << "Job" << i << " obtains " << jobCore[i] << " cores (speed=" << speed << ") and finishes at time " << jobFinishTime[i] << ": \n";
		for (int j = 0; j < jobBlock[i]; j++) {
			cout << "\tBlock" << j << ": H" << get<0>(runLoc[i][j]) << ", C" << get<1>(runLoc[i][j]) << ", R" << get<2>(runLoc[i][j]) << " (time=" << fixed << setprecision(2) << dataSize[i][j] / speed << ")" << " \n";
		}
		cout << "\n";
	}

	cout << "The maximum finish time: " << *max_element(jobFinishTime.begin(), jobFinishTime.end()) << "\n";
	cout << "The sum of response time: " << accumulate(jobFinishTime.begin(), jobFinishTime.end(), 0.0) << "\n\n";
}

// 以核的视角打印解决方案
void ResourceScheduler::outputSolutionFromCore() {
	cout << "\nTask" << taskType << " Solution (Core Perspective) of Teaching Assistant:\n\n";
	double maxHostTime = 0, totalRunningTime = 0.0;
	for (int i = 0; i < numHost; i++) {
		double hostTime = *max_element(hostCoreFinishTime[i].begin(), hostCoreFinishTime[i].end());
		maxHostTime = max(hostTime, maxHostTime);
		totalRunningTime += accumulate(hostCoreFinishTime[i].begin(), hostCoreFinishTime[i].end(), 0.0);
		cout << "Host" << i << " finishes at time " << hostTime << ":\n\n";
		for (int j = 0; j < hostCore[i]; j++) {
			cout << "\tCore" << j << " has " << hostCoreTask[i][j].size() << " tasks and finishes at time " << hostCoreFinishTime[i][j] << ":\n";
			for (int k = 0; k < hostCoreTask[i][j].size(); k++) {
				cout << "\t\tJ" << setw(2) << setfill('0') << get<0>(hostCoreTask[i][j][k]) << ", B" << setw(2) << setfill('0') << get<1>(hostCoreTask[i][j][k]) << ", runTime " << fixed << setprecision(1) << setw(5) << setfill('0') << get<2>(hostCoreTask[i][j][k]) << " to " << fixed << setprecision(1) << setw(5) << setfill('0') << get<3>(hostCoreTask[i][j][k]) << "\n";
			}
			cout << "\n";
		}
		cout << "\n\n";
	}
	cout << "The maximum finish time of hosts: " << maxHostTime << "\n";
	cout << "The total efficacious running time: " << totalRunningTime << "\n";
	cout << "Utilization rate: " << totalRunningTime / accumulate(hostCore.begin(), hostCore.end(), 0.0) / maxHostTime << "\n\n";
}

// 从数据块的视角进行验证
void ResourceScheduler::validFromBlock() {

	// 1. 验证jobFinishTime: "jobFinishTime的和" 应该小于 "依次执行每个job, 所有数据块在各自初始所在主机的同一个核上直接运行情况下, job完成时间的和"
	double actualTime = accumulate(jobFinishTime.begin(), jobFinishTime.end(), 0.0);
	double maxRunningTime = 0.0;
	vector<unordered_set<int>> jobInitLocSet(numJob); // 每个job初始分散在哪几个主机上
	vector<int> jobTotalSize(numJob, 0); // 每个job所有数据块大小之和
	_for(i, 0, numJob) {
		_for(j, 0, location[i].size()) {
			jobInitLocSet[i].insert(location[i][j]);
			jobTotalSize[i] += dataSize[i][j];
		}
		maxRunningTime += maxRunningTime + (double)jobTotalSize[i] / (Sc[i]) / g(jobInitLocSet[i].size());
	}
	assert(maxRunningTime >= actualTime);

	// 2. 验证jobCore: 分配给每个Job的核数应该在 [1,总核数] 内
	int numCore = accumulate(hostCore.begin(), hostCore.end(), 0);
	_for(i, 0, numJob)
		assert(0 < jobCore[i] && jobCore[i] <= numCore);

	// 3. 验证runLoc: 每个核内的数据块运行次序不能相同, 必须是[1,...,n]的一个全排列, n是这个核上被调度的数据块个数
	//    顺便把从块视角的答案转化为从核视角的答案

	// 计算每个核被分配了多少数据块
	vector<vector<int>> hostCoreBlock(numHost);
	_for(i, 0, numHost)
		hostCoreBlock[i].resize(hostCore[i], 0);

	_for(i, 0, numJob) {
		_for(j, 0, jobBlock[i]) {
			int h = get<0>(runLoc[i][j]);
			int c = get<1>(runLoc[i][j]);
			if (h < 0 || h >= numHost || c < 0 || c >= hostCore[h])
				cerr << "Error: Job" << i << " block " << j << " should not be allocated in host " << h << " core " << c << "\n";
			hostCoreBlock[h][c]++;
		}
	}

	// 初始化主机-核的任务列表长度
	_for(i, 0, numHost)
		_for(j, 0, hostCore[i])
		hostCoreTask[i][j].resize(hostCoreBlock[i][j], make_tuple(-1, -1, -1, -1));

	// 尝试将每个数据块分配到主机-核的任务列表中
	_for(i, 0, numJob) {
		_for(j, 0, jobBlock[i]) {
			int h = get<0>(runLoc[i][j]);
			int c = get<1>(runLoc[i][j]);
			int r = get<2>(runLoc[i][j]);

			if (h < 0 || h >= numHost || c<0 || c >= hostCore[h] || r <= 0 || r>hostCoreBlock[h][c])
				cerr << "Error: Host " << h << " core " << c << " rank " << r << " should not be allocated by job " << i << " block " << j << "\n";
			else if (get<0>(hostCoreTask[h][c][r - 1]) != -1) // 注意 r 需要减一
				cerr << "Error: Host " << h << " core " << c << " rank " << r << " is already allocated by job " << get<0>(hostCoreTask[h][c][r - 1])
				<< " block " << get<1>(hostCoreTask[h][c][r - 1]) << ": " << get<2>(hostCoreTask[h][c][r - 1]) << " ~ " << get<3>(hostCoreTask[h][c][r - 1])
				<< " when allocate job " << i << " block " << j << "\n";

			hostCoreTask[h][c][r - 1] = make_tuple(i, j, -1.0, -1.0);
		}
	}

	// 计算主机-核上的运行情况, 可以把传输列表transferList也算出来

	// 初始化hostCoreFinishTime, 逐步模拟核当前运行到了哪个时间
	hostCoreFinishTime.resize(numHost);
	for (int i = 0; i < numHost; i++)
		hostCoreFinishTime[i].resize(hostCore[i], 0.0);

	int blockFinished = 0;
	int numTotalBlock = accumulate(jobBlock.begin(), jobBlock.end(), 0);
	vector<double> jobStartTime(numJob, 0.0); // 记录每个Job的开始时

	while (blockFinished < numTotalBlock) {
		// 1. 找最短运行时间的核, 尝试调度这个核上的下一个块对应的Job的所有数据块
		//    判断并记录需要传输的块到transferMap中

		// 2. 找最大的 这个job的 每个数据块所调度的核上完成上一个别的job数据块的结束时间, 
		//    将这个时间作为当前Job的起始时间jobStartTime

		// 3. 模拟完成当前Job的所有数据块
		//    更新hostCoreTask、hostCoreFinishTime、blockFinished

	}

}

void ResourceScheduler::validFromCore() {
	// 1. hostCoreTask上是否包含所有Job的所有数据块而且无重复, 得到runLoc


	// 2. hostCoreTask上所有数据块的运行时间不能有重叠, 得到jobStartTime和Job的运行顺序


	// 3. 根据每个Job的核数, hostCoreTask上每个数据块的运行时间应该在合理长度


}

void ResourceScheduler::visualization() {

}

double ResourceScheduler::g(int e) {
	return 1 - alpha * (e - 1);
}
