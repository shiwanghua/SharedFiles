#include<iostream>
#include "../hFiles/OfflineJobScheduler.h"

int main() {
	OfflineJobScheduler ojs;
	ojs.schedule();
	ojs.outputSolution();
	return 0;
}