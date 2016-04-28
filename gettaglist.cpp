#include <sys/types.h>
#include <sys/xattr.h>
#include <string.h>

#include "gettaglist.h"

std::list<std::string> getTagList( int fd ) {
	char tmp[XATTR_MAXBUFLEN];
	char *buf = tmp;
	int buflen = fgetxattr( fd, "user.tags", buf, XATTR_MAXBUFLEN );

	std::list<std::string> l;
	while( buflen > 0 ) {
		int taglen = strnlen( buf, buflen );
		if( taglen >= buflen )
			throw std::string( "unexpected end of tag list\n" );
		l.push_back( std::string( buf ) );
		taglen++;
		buflen -= taglen;
		buf += taglen;
	}
	return l;
}
