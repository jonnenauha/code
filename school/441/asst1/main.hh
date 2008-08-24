//****************************************************************************************************
//
//			CPSC 441 - Assignment 1
//
//		Simulate DataLink Layer. Useful constants and defines.
//
//							Copyright:	Ryan McDougall -- 2003
//
//****************************************************************************************************

#ifndef __MAIN_HH
#define __MAIN_HH



//====================================================================================================
// Includes and Defines

typedef	unsigned char	byte;
typedef	unsigned long	CRC32;

typedef byte*		MACAddress;

extern bool	_GLB_ERR_MSG;				// Global Message Error "Switch"
extern bool	_GLB_ERR_DST;				// Global Message Error "Switch"

const int	_NET_SIZE		= 180;		// Network Packet sizes
const int	_NET_ADDR_SIZE		= 20;
const int	_NET_DATA_SIZE		= 140;

const int	_NET_DST_OFFSET		= 0;		// Network Packet offsets
const int	_NET_SRC_OFFSET		= 20;
const int	_NET_DATA_OFFSET	= 40;


const int	_FRM_SIZE		= 200;		// Data Link Packet sizes
const int	_FRM_ADDR_SIZE		= 6;
const int 	_FRM_DATA_SIZE		= 180;
const int	_FRM_CRC_SIZE		= 4;
const int	_FRM_TYPE_SIZE		= 4;

const int 	_FRM_TYPE		= 0;

const int 	_FRM_DST_OFFSET		= 0;		// Data Link Packet offsets
const int	_FRM_SRC_OFFSET		= 6;
const int	_FRM_TYPE_OFFSET	= 12;
const int	_FRM_DATA_OFFSET	= 16;
const int	_FRM_CRC_OFFSET		= 196;



//====================================================================================================
//

byte*	byte_string( const char* cstring, int length = 0 );
int	string_copy( const char* string, byte* dst, int length );
int	string_copy( const byte* string, char* dst, int length );
int	byte_copy( const byte* src, byte* dst, int length );
int	byte_compare( const byte* a, const byte* b, int length );
byte*	byte_concat( const byte* first, int length_1st, const byte* second, int length_2nd );



//====================================================================================================
// Utility string class. Depricated.

class	String
{

//----------------------------------------------------------------------------------------------------
//
	public:
			String( const char* cstring, int size = 0 );
			String( const String& copy );
			~String();
			
		char*	getCharString();
		byte*	getByteString();

//----------------------------------------------------------------------------------------------------
//
	private:
		byte*	_string;
		int	_size;
};


#endif
