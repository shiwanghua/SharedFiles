#include<iostream>
#include "../hFiles/OfflineJobScheduler.h"
#include "../hFiles/Util.h"

int main() {
	OfflineJobScheduler ojs;
	generator(ojs);
	ojs.schedule();
	ojs.outputSolution();

	return 0;
}

