#include "timelen.h"

#define MINUTE 60
#define HOUR ( MINUTE * 60 )
#define DAY ( HOUR * 24 )
#define WEEK ( DAY * 7 )

void timelen_human_readable( std::ostream &stream, int seconds ) {
	if( seconds >= 2*WEEK ) {
		stream << seconds/WEEK << " weeks";
		return;
	}
	if( seconds >= 2*DAY ) {
		stream << seconds/DAY << " days";
		return;
	}
	if( seconds >= 2*HOUR ) {
		stream << seconds/HOUR << " hours";
		return;
	}
	if( seconds >= 2*MINUTE ) {
		stream << seconds/MINUTE << " minutes";
		return;
	}
	stream << seconds << " seconds";
}
