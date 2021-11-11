#include"../hFiles/Util.h"

// 随机生成器, 没考虑主机核数
// 测试的时候可以随便在这里输入一些数, 就可以生成一个新的用例
void generator(OfflineJobScheduler& ojs) {
	srand((int)time(0));
	ojs.numJob = 20, ojs.numHost = 1, ojs.St = 501.2, ojs.alpha = 0.06;
	int minCore = 5, maxCore = 5;
	int minBlock = 3, maxBlock = 12;
	int minSize = 50, maxSize = 150;
	double minSpeed = 10, maxSpeed = 100;
	ojs.hostCore.resize(ojs.numHost);
	ojs.jobBlock.resize(ojs.numJob);
	ojs.Sc.resize(ojs.numJob);
	ojs.dataSize.resize(ojs.numJob);
	ojs.location.resize(ojs.numJob);

	cout << "\n\n-----------Generator starts.--------------\n\n";

	cout << "numJob = " << ojs.numJob << ", numHost = " << ojs.numHost << ", St = " << ojs.St << ", alpha = " << ojs.alpha << "\n\n";

	cout << "hostCore:\n";
	for (int i = 0; i < ojs.numHost; i++) {
		ojs.hostCore[i] = rand() % (maxCore - minCore + 1) + minCore;
		cout << ojs.hostCore[i] << " ";
	}

	cout << "\n\njobBlockNumber:\n";
	for (int i = 0; i < ojs.numJob; i++) {
		ojs.jobBlock[i] = rand() % (maxBlock - minBlock + 1) + minBlock;
		cout << ojs.jobBlock[i] << " ";
	}

	cout << "\n\njobCalculatingSpeed:\n";
	for (int i = 0; i < ojs.numJob; i++)
	{
		ojs.Sc[i] = rand() % int(maxSpeed - minSpeed + 1) + minSpeed;
		cout << ojs.Sc[i] << " ";
	}

	cout << "\n\nblockDataSize:\n";
	for (int i = 0; i < ojs.numJob; i++)
	{
		ojs.dataSize[i].resize(ojs.jobBlock[i]);
		for (int j = 0; j < ojs.jobBlock[i]; j++) {
			ojs.dataSize[i][j] = rand() % (maxSize - minSize + 1) + minSize;
			cout << ojs.dataSize[i][j] << " ";
		}
		cout << endl;
	}

	cout << "\njobBlockInitialLocation:\n";
	for (int i = 0; i < ojs.numJob; i++)
	{
		ojs.location[i].resize(ojs.jobBlock[i]);
		for (int j = 0; j < ojs.jobBlock[i]; j++) {
			ojs.location[i][j] = rand() % ojs.numHost;
			cout << ojs.location[i][j] << " ";
		}
		cout << endl;
	}

	ojs.runLoc.resize(ojs.numJob);
	for (int i = 0; i < ojs.numJob; i++)
		ojs.runLoc[i].resize(ojs.jobBlock[i]);

	cout << "\n\n-----------Generator ends.--------------\n\n";
}