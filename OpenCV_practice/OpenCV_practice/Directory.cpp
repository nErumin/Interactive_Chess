#include "Directory.h"
#include <iostream>

string Directory::PATH = "";

Directory::Directory() {
	setDefaultDirectory();
}

void Directory::setDirectory(string path) {
	PATH = path;
	cout << "Set path : " << PATH << endl;
}

void Directory::setDefaultDirectory() {
	char strBuffer[MAX_PATH] = { 0, };
	char *pstrBuffer = NULL;
	PATH = getcwd(strBuffer, MAX_PATH);
}