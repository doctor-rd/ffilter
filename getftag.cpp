#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#include <iostream>
#include <iterator>
#include <algorithm>

#include "gettaglist.h"

using namespace std;

void usage( char* name ) {
	printf( "Usage: %s [OPTIONS] FILENAME\n"
	"\t-h\n\t\tThis\n"
	"\t-n INDEX\n\t\tPrint the raw tag at the position INDEX.\n"
	, name );
}

bool printable( string &tag ) {
	for( char i : tag )
		if( isgraph( i ) == 0 )
			return false;
	return true;
}

int main( int argc, char* const argv[] ) {
	int n = -1;
	char* filename;
	int opt;
	int fd;

	if( argc < 2 ) {
		fprintf( stderr, "Type\n# %s -h\nfor help.\n", argv[0] );
		return 1;
	}
	while( ( opt = getopt( argc, argv, "hn:" ) ) != -1 ) {
		switch( opt ) {
		case 'h':
			usage( argv[0] );
			return 1;
		case 'n':
			n = atoi( optarg );
			break;
		default:
			return 1;
		}
	}
	if( optind >= argc ) {
		fprintf( stderr, "filename missing\n" );
		return 1;
	}
	filename = argv[optind];

	fd = open( filename, O_RDONLY );
	if( fd == -1 ) {
		perror( filename );
		return 1;
	}
	try {
		list<string> l = getTagList( fd );
		if( n >= 0 ) {
			if( n >= l.size() ) {
				cerr << "index out of range\n";
				return 1;
			}
			list<string>::iterator it = l.begin();
			advance( it, n );
			cout << *it;
		}
		else {
			if( !is_sorted( l.begin(), l.end() ) )
				cerr << "Warning: the tags are not sorted\n";
			int count = 0;
			for( string &i : l ) {
				cout << "tag[" << count++ << "] ";
				if( printable( i ) )
					cout << "= " << i;
				else
					cout << "contains special characters or whitespaces";
				cout << endl;
			}
		}
	}
	catch( string c ) {
		cerr << c;
		return 1;
	}
	return 0;
}
