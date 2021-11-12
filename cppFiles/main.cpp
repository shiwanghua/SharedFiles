#include<iostream>
#include "../hFiles/ResourceScheduler.h"
#include "../hFiles/Util.h"

int main() {
	ResourceScheduler rs(1);
	//generator(rs,1);
	rs.schedule();
	rs.outputSolutionFromBlock();
	rs.outputSolutionFromCore();

	return 0;
}

