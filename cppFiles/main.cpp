#include<iostream>
#include "../hFiles/ResourceScheduler.h"
#include "../hFiles/Util.h"

int main() {
	int taskType=2;
	int caseID=1;
	ResourceScheduler rs(taskType,caseID);
	generator(rs,taskType);
	rs.schedule();
	rs.outputSolutionFromBlock();
	rs.outputSolutionFromCore();

	return 0;
}

