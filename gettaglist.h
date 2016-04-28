#ifndef GETTAGLIST_H
#define GETTAGLIST_H

#include <string>
#include <list>

#define XATTR_MAXBUFLEN 1024

std::list<std::string> getTagList( int fd );

#endif
