#include"../hFiles/Util.h"

// Random generator
// For testing, you can modify the range of the next parameters and get a new test case.
void generator(ResourceScheduler& rs, int taskType,int caseID) {
	srand((int)time(0));
	rs.numJob = 15, rs.numHost = 5, rs.alpha = 0.075;
	if (taskType == 2) rs.St = 400;
	int minCore = 4, maxCore = 12;      // The number of cores in a host.
	int minBlock = 20, maxBlock = 80;   // The number of blocks in a job.
	int minSize = 10, maxSize = 100;     // The size of a block in a job.
	double minSpeed = 5, maxSpeed = 40;  // Calculating speed.
	rs.hostCore.resize(rs.numHost);
	rs.jobBlock.resize(rs.numJob);
	rs.Sc.resize(rs.numJob);
	rs.dataSize.resize(rs.numJob);
	rs.location.resize(rs.numJob);

	cout << "\n\n-----------Generator starts.--------------\n\n";

	cout << "numJob = " << rs.numJob << ", numHost = " << rs.numHost << ", alpha = " << rs.alpha;
	if (taskType == 2) cout << ", St = " << rs.St;
	cout<< "\n\n";

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

	rs.jobFinishTime.resize(rs.numJob, 0);
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

	string fileName = "../input/task" + to_string(taskType) + "_case" + to_string(caseID) + ".txt";
	string text = to_string(rs.numJob)+" "+to_string(rs.numHost)+" "+to_string(rs.alpha);
	if (rs.taskType == 2)text += " " + to_string(rs.St);
	text += "\n";
	
	for (int i = 0; i < rs.numHost; i++) 
		text += to_string(rs.hostCore[i]) + " ";
	text += "\n";
	
	for (int i = 0; i < rs.numJob; i++) 
		text += to_string(rs.jobBlock[i]) + " ";
	text += "\n";
	
	for (int i = 0; i < rs.numJob; i++)
		text += to_string(rs.Sc[i]) + " ";
    text += "\n";
	
	for (int i = 0; i < rs.numJob; i++)
	{
		for (int j = 0; j < rs.jobBlock[i]; j++) 
			text += to_string(rs.dataSize[i][j]) + " ";
		text += "\n";
	}

	for (int i = 0; i < rs.numJob; i++)
	{
		for (int j = 0; j < rs.jobBlock[i]; j++)
			text += to_string(rs.location[i][j]) + " ";
		text += "\n";
	}
	WriteData(fileName, text);

	cout << "\n\n-----------Generator ends.--------------\n\n";
}

void WriteData(string fileName, string text) // Save $text into file $fileName
{
//	ifstream ifs(fileName); // 读文件
//	if (!ifs)
//		cout << "Create file failed!" << endl;
//	string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>())); // old file content
//	ifs.close();

	ofstream fileStream;
	fileStream.open(fileName.c_str(), ios::binary | ios::ate); //  插入到文件末尾
	fileStream.seekp(0, ios::beg);
	fileStream << text << endl; // << content; // 插入到文件头部(保留原文件内容)
	fileStream << flush;
	fileStream.close();
}