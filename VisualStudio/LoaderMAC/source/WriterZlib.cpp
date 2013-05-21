#include <stdio.h>

#include <zlib.h>

#include "WriterZlib.h"

WriterZlib::WriterZlib() {

}
WriterZlib::~WriterZlib() {
	//Do nothing.
}

bool WriterZlib::init() {
	bool sucessfulCompress = false;
	
	FILE* toCompress = NULL;
	toCompress = fopen("ToCompress.txt", "r");
	if( toCompress!=NULL ) {
		FILE* target = NULL;
		target = fopen("Compressed.txt", "w");
		if( target!=NULL ) {
			int result = def( toCompress, target, 0 ); //0?
		}
	}

	return sucessfulCompress;
}

//#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
//#  include <fcntl.h>
//#  include <io.h>
//#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
//#else
//#  define SET_BINARY_MODE(file)
//#endif

#define CHUNK_SIZE 16384

/*
C-style function compresses data from source into target until EOF.

Return values:
	Z_OK			- Success.
	Z_MEM_ERROR		- Memory could not be allocated.
	Z_STREAM_ERROR	- Invalid compression level passed to function.
	Z_VERSION ERROR	- Header and lib of zlib does not match.
	Z_ERRNO			- Error reading/writing files.
*/
int WriterZlib::def( FILE* source, FILE* target, int compLvl ) {
	int result	= 0;
	int flush	= 0; //This may be a bit dangerous.
	unsigned int have = 0;
	z_stream strm;
	unsigned char in [CHUNK_SIZE];
	unsigned char out[CHUNK_SIZE];

	/*Allocate zlib-deflate state, whatever the hell that means.*/
	strm.zalloc	= Z_NULL;
	strm.zfree	= Z_NULL;
	strm.opaque	= Z_NULL;
	result = deflateInit( &strm, compLvl );
	
	/*If zlib initialized correctly, begin compressing source into target until EOF.*/
	if( result==Z_OK ) {
		do {
			strm.avail_in = fread( in, 1, CHUNK_SIZE, source );
			if( ferror( source ) ) {
				(void)deflateEnd( &strm );
				result = Z_ERRNO;
			} else {
				flush = feof( source ) ? Z_FINISH : Z_NO_FLUSH;

				/* run the zlib deflate-function on input until output buffer not full, finish compression if all source has been read */
				
			}
		} while ( flush!=Z_FINISH && result==Z_OK );
	}
}