#ifndef LISTER_H
#define LISTER_H

#include <sys/types.h>
#include <dirent.h>

#include <string>
#include <list>
#include <mutex>

struct FileAttr {
	std::string name;
	std::list<std::string> tags;
	int age;
};

class ListerOpts {
public:
	ListerOpts();
	std::list<std::string> *tags;
	int minage;
	int maxage;
};

class Lister {
public:
	Lister( const char *dirname );
	~Lister();
	std::list<FileAttr> doit( ListerOpts &opts );
private:
	DIR *d;
	std::mutex m;
};

#endif
