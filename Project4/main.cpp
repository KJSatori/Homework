#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "Experiment.h"
#include "Computing.h"
using namespace std;


int main()
{
	srand((unsigned int)time(0));
	
	DoExperiment(10000);

	return 0;
}