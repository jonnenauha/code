//****************************************************************************************************
//
//			CPSC 441 - Assignment 1
//
//		Simulate Transmission Packets. Implements Data Link and Network packets.
//
//							Copyright:	Ryan McDougall -- 2003
//
//****************************************************************************************************



//====================================================================================================
//

#include <iostream>
#include <cstring>
#include "packet.hh"



//====================================================================================================
// Allocate memory for the network packet.

	NetworkPacket::NetworkPacket()
{
	_size		= _NET_SIZE;
	_packet		= new byte[ _size ];
	
	memset( _packet, 0, _size );
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//  Byte copy contructor.

	NetworkPacket::NetworkPacket( byte* packet )
{
	_size		= _NET_SIZE;
	_packet		= packet;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Packet copy contructor.

	NetworkPacket::NetworkPacket( const NetworkPacket& copy )
{
	_size		= _NET_SIZE;
	_packet		= new byte[ _size ];

	memset( _packet, 0, _size );
	
	byte_copy( copy._packet, _packet, _size );
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Deconstructor.

	NetworkPacket::~NetworkPacket()
{
	delete [] _packet;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Copy byte string to the source field.

void	NetworkPacket::source( byte* src )
{
	byte_copy( src, _packet+_NET_SRC_OFFSET, _NET_ADDR_SIZE );	
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Return a point to the source field.

byte*	NetworkPacket::source()
{
	return	_packet+_NET_SRC_OFFSET;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Copy byte string to the destination field.

void	NetworkPacket::destination( byte* dst )
{
	byte_copy( dst, _packet+_NET_DST_OFFSET, _NET_ADDR_SIZE );
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Return a point to the destination field.

byte*	NetworkPacket::destination()
{
	return 	_packet+_NET_DST_OFFSET;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Copy byte string to the message field.

void	NetworkPacket::message( byte* msg )
{
	byte_copy( msg, _packet+_NET_DATA_OFFSET, _NET_DATA_SIZE );		
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Return a point to the message field.

byte*	NetworkPacket::message()
{
	return 	_packet+_NET_DATA_OFFSET;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Return size.

int	NetworkPacket::size()
{
	return	_size;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Return the byte string representation of the packet.

byte*	NetworkPacket::serialize()
{
	return	_packet;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Clone the packet.

Packet*	NetworkPacket::clone()
{
	return new NetworkPacket( *this );
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Return true if there is an error.

bool	NetworkPacket::error()
{}



//====================================================================================================
//

	DataLinkPacket::DataLinkPacket()
{
	_size		= _FRM_SIZE;
	_packet		= new byte[ _size ];
	
	memset( _packet, 0, _size );
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

	DataLinkPacket::DataLinkPacket( const DataLinkPacket& copy )
{
	_size		= _FRM_SIZE;
	_packet		= new byte[ _size ];

	memset( _packet, 0, _size );
	
	byte_copy( copy._packet, _packet, _size );
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

	DataLinkPacket::~DataLinkPacket()
{
	delete [] _packet;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Copy byte string to the source field.

void	DataLinkPacket::source( byte* src )
{
	byte_copy( src, _packet+_FRM_SRC_OFFSET, _FRM_ADDR_SIZE );
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Return a point to the source field.

byte*	DataLinkPacket::source()
{
	return 	_packet+_FRM_SRC_OFFSET;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Copy byte string to the source field.

void	DataLinkPacket::destination( byte* dst )
{
	byte_copy( dst, _packet+_FRM_DST_OFFSET, _FRM_ADDR_SIZE );	
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

byte*	DataLinkPacket::destination()
{
	return 	_packet+_FRM_DST_OFFSET;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Copy byte string to the source field.

void	DataLinkPacket::message( byte* msg )
{
	byte_copy( msg, _packet+_FRM_DATA_OFFSET, _FRM_DATA_SIZE );	
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Return a point to the source field.

byte*	DataLinkPacket::message()
{	
	return 	_packet+_FRM_DATA_OFFSET;	
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Return size.

int	DataLinkPacket::size()
{
	return 	_size;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Return the byte string representation of the packet.

byte*	DataLinkPacket::serialize()
{
	CRC32	chksum = checksum();

	byte*	str = (byte*)( &chksum );

	byte_copy( str, _packet+_FRM_CRC_OFFSET, _FRM_CRC_SIZE );
	
	return 	_packet;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Clone the packet.

Packet*	DataLinkPacket::clone()
{
	return new DataLinkPacket( *this );
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Return true if there is an error.

bool	DataLinkPacket::error()
{
	CRC32	chksum	= checksum();
	
	return ( byte_compare( (byte*)(&chksum), _packet+_FRM_CRC_OFFSET, _FRM_CRC_SIZE ) );
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Compute the check sum.

CRC32	DataLinkPacket::checksum()
{
	CRC32	checksum	= 0;
	
	for( int i=0; i < _FRM_CRC_OFFSET; i++ )
	{
		checksum += *( _packet+i );
	}
	
	return 	checksum;
}
