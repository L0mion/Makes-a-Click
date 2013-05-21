#define _CRT_SECURE_NO_DEPRECATE //Because fuck security.

//#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
//#  include <fcntl.h>
//#  include <io.h>
//#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
//#else
//#  define SET_BINARY_MODE(file)
//#endif

#define CHUNK_SIZE 16384

#include <stdio.h>
#include <assert.h>

#include <zlib.h>

#include "WriterZlib.h"

WriterZlib::WriterZlib() {

}
WriterZlib::~WriterZlib() {
	//Do nothing.
}

bool WriterZlib::init() {
	bool sucessfulCompress = false;

	//SET_BINARY_MODE(stdin);
	//SET_BINARY_MODE(stdout);
	
	FILE* source = NULL;
	source = fopen("ToCompress.txt", "r");
	if( source!=NULL ) {
		FILE* target = NULL;
		target = fopen("Compressed.txt", "w");
		if( target!=NULL ) {

			//SET_BINARY_MODE(source);
			//SET_BINARY_MODE(target);

			int result = def( source, target, Z_DEFAULT_COMPRESSION ); //0?
		}
	}

	return sucessfulCompress;
}

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
	int result;
	int flush; //This may be a bit dangerous.
	unsigned int have;
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
				strm.next_in = in;

				/* run the zlib deflate-function on input until output buffer not full, 
				finish compression if all source has been read */
				do {
					strm.avail_out = CHUNK_SIZE;
					strm.next_out = out;
					result = deflate( &strm, flush );
					if( result!=Z_STREAM_ERROR ) {
						have = CHUNK_SIZE - strm.avail_out;
						if( fwrite(out, 1, have, target) != have || ferror(target) ) {
							(void)deflateEnd( &strm );
							result = Z_ERRNO;
						}
					}
				} while( strm.avail_out==0 && result==Z_OK );
				//assert( strm.avail_in == 0 );
			}
		} while( flush!=Z_FINISH && result==Z_OK );
		//assert( result==Z_STREAM_END );
	}

	(void)deflateEnd( &strm );

	return result;
}