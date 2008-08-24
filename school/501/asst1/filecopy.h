//
//	501 Assignment 1
//
//
//						Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------


#define ER_NONE		0x0000

#define	ER_BAD_ARGS	0x1001
#define ER_FILE_CMP	0x1002

#define ER_SYSCALL	0x2000
#define ER_STD_C	0x3000
#define ER_FORMAT	0x4000

#define ER_MMAP		0x5000
#define ER_MMAP_OPEN	0x5001
#define ER_MMAP_COPY	0x5002


void	usage_print();
int	file_analyze( char* src, char* dst, int* src_size, int* block_size );
int	filecopy_syscall( char* src, char* dest );
int	filecopy_stdc( char* src, char* dest );
int	filecopy_formatted( char* src, char* dest );
int	filecopy_mmap( char* src, char* dest );

