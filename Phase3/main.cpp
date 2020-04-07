#include <iostream>

#include "os/os.h"

using namespace std;

int main()
{
	OS * start_os =  new OS;
	start_os->run();
	//delete start_os;
	
	return 0;
}
