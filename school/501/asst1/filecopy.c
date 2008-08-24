//
//	501 Assignment 1
//
//
//						Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "filecopy.h"


extern int errno;



//--------------------------------------------------------------------------------
//
int	main( int argc, char** argv )
{
	int	errcode	= 0;
	
	char*	src_filename 	= NULL;
	char*	dst_filename	= NULL;
	char*	method		= NULL;
	
	
	if( argc != 4 )
	{
		usage_print();
		errcode = ER_BAD_ARGS;
	}
	else
	{
		src_filename 	= argv[1];
		dst_filename	= argv[2];
		method		= argv[3];		
		
		if( !strcmp( method, "syscall" ) )
		{
			errcode = filecopy_syscall( src_filename, dst_filename );
		}
		else if( !strcmp( method, "std-c" ) )
		{
			errcode = filecopy_stdc( src_filename, dst_filename );
		}
		else if( !strcmp( method, "formatted" ) )
		{
			errcode = filecopy_formatted( src_filename, dst_filename );
		}
		else if( !strcmp( method, "mmap" ) )
		{
			errcode = filecopy_mmap( src_filename, dst_filename );
		}
		else
		{
			usage_print();
			errcode = ER_BAD_ARGS;		
		}				
	}

	return errcode;
}



//--------------------------------------------------------------------------------
//
void	usage_print()
{
	printf( "Usage: filecopy <SOURCE_FILE> <DESTINATION_FILE> <METHOD>\n" );
	printf( "\t Where <METHOD> is one of: syscall | std-c | formatted | mmap | makedata\n" );
}



//--------------------------------------------------------------------------------
//
int	file_analyze( char* src, char* dst, int* src_size, int* block_size )
{
	int		errcode		= 0;
	struct stat	src_file_stats;
	struct stat	dst_file_stats;
	
	
	if( stat( src, &src_file_stats ) != -1 )
	{
		*src_size = src_file_stats.st_size;
		
		if( stat( dst, &dst_file_stats ) != -1 )
		{
			if( src_file_stats.st_ino != dst_file_stats.st_ino )
			{
				if( src_file_stats.st_blksize >= dst_file_stats.st_blksize )
				{
					*block_size = src_file_stats.st_blksize;
				}
				else
				{
					*block_size = dst_file_stats.st_blksize;
				}
				
				errcode = ER_NONE;
			}
			else
			{
				fprintf( stdout, "Warning: Source and Destination are the same.\n" );
				
				*block_size = 0;
				errcode = ER_NONE;
			}
			
		}
		else
		{
			fprintf( stdout, "Warning: Destination does not exist, and will be created.\n" );
			
			*block_size = src_file_stats.st_blksize;
			errcode = ER_NONE;
		}
	}
	else
	{
		errcode = ER_FILE_CMP;
		fprintf( stderr, "Err: %x: %s\n", errcode, strerror( errno ) );
	}
	
	fprintf( stdout, "Info: Source file size is : %i\n", *src_size );
	fprintf( stdout, "Info: Block size recommendation is : %i\n", *block_size );
	
	return errcode;	
}



//--------------------------------------------------------------------------------
//
int	filecopy_syscall( char* src, char* dst )
{
	int	errcode		= 0;

	int	file_size	= 0;
	int	block_size 	= 0;
	int	access_mode	= 0640;
	
	int	src_desc	= 0;
	int	dst_desc	= 0;
	int	bytes_read	= 0;
	int	bytes_written	= 0;
	
	char*	buffer		= NULL;


	errcode = file_analyze( src, dst, &file_size, &block_size );
	if( errcode == ER_NONE )
	{
		buffer = (char*) malloc( block_size );
		
		src_desc = open( src, O_RDONLY );
		dst_desc = open( dst, O_WRONLY | O_TRUNC | O_CREAT, access_mode );
		
		if(( src_desc != -1 )&&( dst_desc != -1 ))
		{

			while( bytes_read = read( src_desc, buffer, block_size ) )
			{
				bytes_written += write( dst_desc, buffer, block_size );
			}

		}
		else
		{
			errcode = ER_SYSCALL;
			fprintf( stderr, "Err: %x: %s\n", errcode, strerror( errno ) );			
		}
		
	}
	else
	{}
	
	fprintf( stdout, "Info: %i bytes written of %i.\n", bytes_written, file_size );
	
	close( src_desc );
	close( dst_desc );
	
	return errcode;
}



//--------------------------------------------------------------------------------
//
int	filecopy_stdc( char* src, char* dst )
{
	int	errcode		= 0;

	int	file_size	= 0;
	int	block_size 	= 0;
	
	FILE*	src_fp		= 0;
	FILE*	dst_fp		= 0;
	int	bytes_written	= 0;
	
	char*	buffer		= NULL;


	errcode = file_analyze( src, dst, &file_size, &block_size );
	if( errcode == ER_NONE )
	{
		buffer = (char*) malloc( block_size );
		
		src_fp = fopen( src, "r" );
		dst_fp = fopen( dst, "w" );
		
		if(( src_fp != NULL )&&( dst_fp != NULL ))
		{

			//setvbuf( src_fp, buffer, _IOFBF, block_size );
			//setvbuf( dst_fp, buffer, _IOFBF, block_size );
				
			while( fgets( buffer, block_size, src_fp ) != NULL )
			{
				bytes_written += fputs( buffer, dst_fp );
			}

		}
		else
		{
			errcode = ER_STD_C;
			fprintf( stderr, "Err: %x: %s\n", errcode, strerror( errno ) );			
		}
		
	}
	else
	{}
	
	fprintf( stdout, "Info: %i bytes written of %i.\n", bytes_written, file_size );
	
	fclose( src_fp );
	fclose( dst_fp );	
	
	return errcode;
}



//--------------------------------------------------------------------------------
//
int	filecopy_formatted( char* src, char* dst )
{
	int	errcode		= 0;

	int	file_size	= 0;
	int	block_size 	= 0;
	
	FILE*	src_fp		= 0;
	FILE*	dst_fp		= 0;
	int	bytes_written	= 0;
	
	char	buffer		= '\0';


	errcode = file_analyze( src, dst, &file_size, &block_size );
	if( errcode == ER_NONE )
	{

		src_fp = fopen( src, "r" );
		dst_fp = fopen( dst, "w" );
		
		if(( src_fp != NULL )&&( dst_fp != NULL ))
		{

			while( fscanf( src_fp, "%c", &buffer) != EOF )
			{
				bytes_written += fprintf( dst_fp, "%c", buffer );
			}
		}
		else
		{
			errcode = ER_FORMAT;
			fprintf( stderr, "Err: %x: %s\n", errcode, strerror( errno ) );			
		}
		
	}
	else
	{}
	
	fprintf( stdout, "Info: %i bytes written of %i.\n", bytes_written, file_size );
	
	fclose( src_fp );
	fclose( dst_fp );
	
	return errcode;
}



//--------------------------------------------------------------------------------
//
int	filecopy_mmap( char* src, char* dst )
{
	int	errcode		= 0;

	int	file_size	= 0;
	int	block_size 	= 0;
	int	access_mode	= 0640;
	
	int	src_desc	= 0;
	int	dst_desc	= 0;
	int	bytes_read	= 0;
	int	bytes_written	= 0;
	
	char*	src_buffer	= NULL;
	char*	dst_buffer	= NULL;


	errcode = file_analyze( src, dst, &file_size, &block_size );
	if( errcode == ER_NONE )
	{		
		src_desc = open( src, O_RDONLY );
		dst_desc = open( dst, O_RDWR | O_TRUNC | O_CREAT, access_mode );
		
		if(( src_desc != -1 )&&( dst_desc != -1 ))
		{
			// Set the destination file size
			//lseek( dst_desc, file_size-1, SEEK_SET );
			//write( dst_desc, '0', 1 );
			ftruncate( dst_desc, file_size );
				
				
			src_buffer = mmap(	
						0,		// Allow system to choose start addr
						file_size,
						PROT_READ | PROT_WRITE,
						MAP_PRIVATE,
						src_desc,
						0		// Offset from begining of file
						);
						
						
			dst_buffer = mmap(	
						0,		// Allow system to choose start addr
						file_size,
						PROT_READ | PROT_WRITE,
						MAP_SHARED,
						dst_desc,
						0		// Offset from begining of file
						);
				
			if(( src_buffer != MAP_FAILED )&&( dst_buffer != MAP_FAILED ))
			{
				if( !madvise( src_buffer, file_size, MADV_SEQUENTIAL | MADV_WILLNEED ) )
				{}
				else
				{
					fprintf( stderr, "Warning: Unable to advise src memory: %i\n", strerror( errno ) );
				}
				
				if( !madvise( dst_buffer, file_size, MADV_SEQUENTIAL | MADV_WILLNEED ) )
				{}
				else
				{
					fprintf( stderr, "Warning: Unable to advise dst memory: %i\n", strerror( errno ) );
				}
							
				if( memcpy( dst_buffer, src_buffer, file_size ) )
				{}
				else
				{
					errcode = ER_MMAP_COPY;
					fprintf( stderr, "Err: %x: %s\n", errcode, strerror( errno ) );
				}
				
				if( !munmap( src_buffer, file_size ) )
				{}
				else
				{
					errcode = ER_MMAP;
					fprintf( stderr, "Err: Unmap src: %x: %s\n", errcode, strerror( errno ) );						
				}
				
				if( !munmap( dst_buffer, file_size ) )
				{}
				else
				{
					errcode = ER_MMAP;
					fprintf( stderr, "Err: Unmap dst: %x: %s\n", errcode, strerror( errno ) );					
				}
			}
			else
			{
				errcode = ER_MMAP;
				fprintf( stderr, "Err: %x: %s. src: %i dst: % i.\n", errcode, strerror( errno ), src_buffer, dst_buffer );
			}				
		}
		else
		{
			errcode = ER_MMAP_OPEN;
			fprintf( stderr, "Err: %x: %s\n", errcode, strerror( errno ) );			
		}
		
	}
	else
	{}
	
	//fprintf( stdout, "Info: %i bytes written of %i.\n", bytes_written, file_size );
	
	close( src_desc );
	close( dst_desc );
	
	return errcode;
}
