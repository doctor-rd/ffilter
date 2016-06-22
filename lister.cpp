#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#include "lister.h"
#include "gettaglist.h"

static bool contains_sorted( std::list<std::string> a, std::list<std::string> b ) {
	std::list<std::string>::iterator j = b.begin();
	for( std::string &i : a ) {
		while( true ) {
			if( j == b.end() )
				return false;
			if( i == *j )
				break;
			j++;
		}
	}
	return true;
}

ListerOpts::ListerOpts() {
	tags = 0;
	minage = -1;
	maxage = -1;
}

Lister::Lister( const char *dirname ) {
	d = opendir( dirname );
}

Lister::~Lister() {
	closedir( d );
}

std::list<FileAttr> Lister::doit( ListerOpts &opts ) {
	std::list<std::string> tmp_tags;
	if( opts.tags != 0 ) {
		tmp_tags = *opts.tags;
		tmp_tags.sort();
	}
	int tmp_minage = opts.minage;
	int tmp_maxage = opts.maxage;
	if( tmp_maxage < 0 )
		tmp_maxage = INT_MAX;

	std::list<FileAttr> res;
	std::lock_guard<std::mutex> lg( m );
	rewinddir( d );
	struct dirent *de;
	while( ( de = readdir( d ) ) != NULL ) {
		char *name = de->d_name;
		if( *name == '.' )
			continue;
		int fd = openat( dirfd( d ), name, O_RDONLY );
		if( fd == -1 )
			continue;
		struct stat s;
		fstat( fd, &s );
		if( S_ISREG( s.st_mode ) ) {
			FileAttr fa;

			struct timespec tp;
			clock_gettime( CLOCK_REALTIME, &tp );
			fa.age = tp.tv_sec - s.st_mtime;
			if( tmp_minage > fa.age )
				goto stop;
			if( tmp_maxage < fa.age )
				goto stop;

			fa.tags = getTagList( fd );
			if( !contains_sorted( tmp_tags, fa.tags ) )
				goto stop;

			fa.name = std::string( name );
			res.push_back( fa );
		}
		stop:
		close( fd );
	}
	return res;
}
