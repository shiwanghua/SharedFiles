#include"../hFiles/Util.h"

// ���������
// ���Ե�ʱ������������������һЩ��, �Ϳ�������һ���µ�����
void generator(ResourceScheduler& rs, int taskType) {
	srand((int)time(0));
	rs.numJob = 20, rs.numHost = 1, rs.alpha = 0.06;
	if (taskType == 2)rs.St = 500;
	int minCore = 20, maxCore = 20;
	int minBlock = 200, maxBlock = 1000;
	int minSize = 50, maxSize = 150;
	double minSpeed = 10, maxSpeed = 100;
	rs.hostCore.resize(rs.numHost);
	rs.jobBlock.resize(rs.numJob);
	rs.Sc.resize(rs.numJob);
	rs.dataSize.resize(rs.numJob);
	rs.location.resize(rs.numJob);

	cout << "\n\n-----------Generator starts.--------------\n\n";

	cout << "numJob = " << rs.numJob << ", numHost = " << rs.numHost << ", St = " << rs.St << ", alpha = " << rs.alpha << "\n\n";

	cout << "hostCore:\n";
	for (int i = 0; i < rs.numHost; i++) {
		rs.hostCore[i] = rand() % (maxCore - minCore + 1) + minCore;
		cout << rs.hostCore[i] << " ";
	}

	cout << "\n\njobBlockNumber:\n";
	for (int i = 0; i < rs.numJob; i++) {
		rs.jobBlock[i] = rand() % (maxBlock - minBlock + 1) + minBlock;
		cout << rs.jobBlock[i] << " ";
	}

	cout << "\n\njobCalculatingSpeed:\n";
	for (int i = 0; i < rs.numJob; i++)
	{
		rs.Sc[i] = rand() % int(maxSpeed - minSpeed + 1) + minSpeed;
		cout << rs.Sc[i] << " ";
	}

	cout << "\n\nblockDataSize:\n";
	for (int i = 0; i < rs.numJob; i++)
	{
		rs.dataSize[i].resize(rs.jobBlock[i]);
		for (int j = 0; j < rs.jobBlock[i]; j++) {
			rs.dataSize[i][j] = rand() % (maxSize - minSize + 1) + minSize;
			cout << rs.dataSize[i][j] << " ";
		}
		cout << endl;
	}

	cout << "\njobBlockInitialLocation:\n";
	for (int i = 0; i < rs.numJob; i++)
	{
		rs.location[i].resize(rs.jobBlock[i]);
		for (int j = 0; j < rs.jobBlock[i]; j++) {
			rs.location[i][j] = rand() % rs.numHost;
			cout << rs.location[i][j] << " ";
		}
		cout << endl;
	}

	rs.finishTime.resize(rs.numJob, 0);
	rs.jobCore.resize(rs.numJob);

	rs.runLoc.resize(rs.numJob);
	for (int i = 0; i < rs.numJob; i++)
		rs.runLoc[i].resize(rs.jobBlock[i]);

	rs.hostCoreTask.resize(rs.numHost);
	for (int i = 0; i < rs.numHost; i++)
		rs.hostCoreTask[i].resize(rs.hostCore[i]);

	rs.hostCoreFinishTime.resize(rs.numHost);
	for (int i = 0; i < rs.numHost; i++)
		rs.hostCoreFinishTime[i].resize(rs.hostCore[i], 0);

	cout << "\n\n-----------Generator ends.--------------\n\n";
}