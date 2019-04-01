#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <direct.h>
#include <string>
#define MAX_PATH 260

using namespace std;


class Directory
{
public:
	static string PATH;
	
	Directory();
	void setDirectory(string path);
	~Directory() = default;

private:
	void setDefaultDirectory();
};
#endif