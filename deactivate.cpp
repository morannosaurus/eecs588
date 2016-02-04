//Deactivate the kernel module.
#include <unistd.h>
#include <sys/syscall.h>
#include "secrets.h"
#include <string>
#include <iostream>
using namespace std;
int main() {
	string arg = secret_api_deactivate;
	if (syscall(SYS_mkdir, arg.c_str()) == 72) {
		cout << "Module deactivated!" << endl;
	}
	else {
		cout << "Module deactivation failed." << endl;
	}
}
