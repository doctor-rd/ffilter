#include <iostream>
#include <string>
#include <list>
#include <iomanip>
#include <sstream>

#include "lister.h"
#include "timelen.h"

using namespace std;

int main( int argc, char* const argv[] ) {
	list<string> tags;
	for( int i = 1; i < argc; i++ )
		tags.push_back( argv[i] );

	try {
		Lister ls( "." );
		ListerOpts opts;
		opts.tags = &tags;
		list<FileAttr> res = ls.doit( opts );
		int maxfnlen = 0;
		for( FileAttr &fa : res ) {
			int l = fa.name.length();
			if( l > maxfnlen )
				maxfnlen = l;
		}
		for( FileAttr &fa : res ) {
			cout << right << setw( 2 ) << fa.tags.size() << "  ";
			cout << left << setw( maxfnlen + 2 ) << fa.name;
			ostringstream age;
			timelen_human_readable( age, fa.age );
			cout << setw( 12 ) << age.str();
			cout << setw( 0 ) << "   ";
			for( string &i : fa.tags )
				cout << ' ' << i;
			cout << endl;
		}
	}
	catch( string c ) {
		cerr << c;
		return 1;
	}
	return 0;
}
