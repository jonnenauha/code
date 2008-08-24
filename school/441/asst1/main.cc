//****************************************************************************************************
//
//			CPSC 441 - Assignment 1
//
//		Simulate DataLink Layer. This module contains app wide utility functions, constants,
//	and simulation conrtol logic.
//
//							Copyright:	Ryan McDougall -- 2003
//
//****************************************************************************************************



//====================================================================================================
// Includes and Defines

#include <iostream>
#include "protocol.hh"
#include "packet.hh"
#include "datalink.hh"
#include "physical.hh"



//====================================================================================================
// Global Variables

bool		_GLB_ERR_MSG		= false;	// Control the Error in messages
bool		_GLB_ERR_DST		= false;	// Control the Error in destinations



//====================================================================================================
// These are type safe conveniece functions for operations on strings of bytes.

byte*	byte_string( const char* cstring, int length )
{
	if( length == 0 )
	{
		length	= strlen( cstring );
	}
	else
	{}
	
	byte*	bstring	= new byte[ length ];
	
	memset( bstring, 0, length );
	
	string_copy( cstring, bstring, length );
	
	return 	bstring;
}


//----------------------------------------------------------------------------------------------------
//

int	string_copy( const char* string, byte* dst, int length )
{
	int	i;
	
	for( i=0; ( i < length )&&( string[i] ); i++ )
	{
		*( dst+i ) = *( string+i );
	}
	
	return i;
}


//----------------------------------------------------------------------------------------------------
//
int	string_copy( const byte* string, char* dst, int length )
{
	int	i;
	
	for( i=0; i < length; i++ )
	{
		*( dst+i ) = *( string+i );
	}
	
	return i;
}



//----------------------------------------------------------------------------------------------------
//

int	byte_copy( const byte* src, byte* dst, int length )
{
	int	i;
	
	for( i=0; i < length; i++ )
	{
		*( dst+i ) = *( src+i );
	}
	
	return i;
}


//----------------------------------------------------------------------------------------------------
//

int	byte_compare( const byte* a, const byte* b, int length )
{
	int	i, difference = 0;
	
	for( i=0; i < length; i++ )
	{
		difference += *( a+i ) - *( b+i );
	}
	
	return difference;
}


//----------------------------------------------------------------------------------------------------
//

byte*	byte_concat( const byte* first, int length_1st, const byte* second, int length_2nd )
{
	int	i	= 0;
	byte*	cat	= new byte[ length_1st + length_2nd ];
	
	while( i < length_1st )
	{
		cat[i]	= first[i];
		i++;
	}
	
	while( i < length_1st + length_2nd )
	{
		cat[i]	= second[i];
		i++;
	}
	
	return 	cat;
}



//====================================================================================================
// Convenience class for interchangeing character and byte strings. DEPRICATED.

	String::String( const char* cstring, int size )
{
	if( size == 0 )
	{
		_size	= strlen( cstring );
	}
	else
	{
		_size 	= size;
	}
			
	_string	= new byte[ _size ];
		
	memset( _string, 0, _size );
			
	string_copy( cstring, _string, _size );
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

	String::String( const String& copy )
{
	_size	= copy._size;
		
	_string	= new byte[ _size ];
				
	memset( _string, 0, _size );
	
	byte_copy( copy._string, _string, _size );
}
		

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

	String::~String()
{
	delete [] _string;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

char*	String::getCharString()
{
	char*	cstring	= new char[ _size+1 ];
	
	string_copy( _string, cstring, _size );
	
	cstring[ _size ] = 0;
	
	return cstring;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

byte*	String::getByteString()
{
	return _string;
}



//====================================================================================================
// Main

int	main()
{
	// Create and Attach the Physical Layer.
	PhysicalLayer	client0_nic;
	PhysicalLayer	client1_nic;
	PhysicalLayer	server0_nic;
	
	server0_nic.attachNode( client0_nic );
	server0_nic.attachNode( client1_nic );
	client0_nic.attachNode( client1_nic );
	
	
	// Create and Stack the DataLink Layer.
	DataLinkLayer	client0_dll( byte_string( "NC0", _FRM_ADDR_SIZE ) );
	DataLinkLayer	client1_dll( byte_string( "NC1", _FRM_ADDR_SIZE ) );
	DataLinkLayer	server0_dll( byte_string( "NS0", _FRM_ADDR_SIZE ) );
	
	client0_dll.stackLayer( client0_nic );
	client1_dll.stackLayer( client1_nic );
	server0_dll.stackLayer( server0_nic );
	
	
	// Create the Network layer Messages.
	NetworkPacket	msg1;
	msg1.source( byte_string( "cli_0", _NET_ADDR_SIZE ) );
	msg1.destination( byte_string( "srv_0", _NET_ADDR_SIZE ) );
	msg1.message( byte_string( "Hello!!", _NET_DATA_SIZE ) );
	
	NetworkPacket	msg2;
	msg2.source( byte_string( "srv_0", _NET_ADDR_SIZE ) );
	msg2.destination( byte_string( "cli_0", _NET_ADDR_SIZE ) );
	msg2.message( byte_string( "I hear you.", _NET_DATA_SIZE ) );


	// Send and Receive the Messages.
	Packet*	ret	= NULL;
	
	
//________________________________________________________________________________
// PART A

	cout << endl << "\tPart A" << endl;
	
	
	client0_dll.push( msg1 );
	
	client0_dll.pop();
	client1_dll.pop();
	ret = server0_dll.pop();
	
	if( ret ) cout << "NS0 got: " << ret->message() << endl;
	
	
	server0_dll.push( msg2 );
	
	ret = client0_dll.pop();
	client1_dll.pop();
	server0_dll.pop();
	
	if( ret ) cout << "NC0 got: " << ret->message() << endl;
	
	
//________________________________________________________________________________
// PART B

	cout << endl << "\tPart B" << endl;
	
	
	client0_dll.push( msg1 );
	
	client0_dll.pop();
	
	_GLB_ERR_MSG	= true;
	client1_dll.pop();
	_GLB_ERR_MSG	= false;

	ret = server0_dll.pop();
	
	if( ret ) cout << "NS0 got: " << ret->message() << endl;
	
	
	server0_dll.push( msg2 );
	
	ret = client0_dll.pop();
	client1_dll.pop();
	server0_dll.pop();
	
	if( ret ) cout << "NC0 got: " << ret->message() << endl;
	
	
//________________________________________________________________________________
// PART C

	cout << endl << "\tPart C" << endl;
	
	
	client0_dll.push( msg1 );
	
	client0_dll.pop();
	
	_GLB_ERR_DST	= true;
	client1_dll.pop();
	_GLB_ERR_DST	= false;

	ret = server0_dll.pop();
	
	if( ret ) cout << "NS0 got: " << ret->message() << endl;
	
	
	server0_dll.push( msg2 );
	
	ret = client0_dll.pop();
	client1_dll.pop();
	server0_dll.pop();
	
	if( ret ) cout << "NC0 got: " << ret->message() << endl;	
	
//________________________________________________________________________________
// PART D

	cout << endl << "\tPart D" << endl;
	
	
	client0_dll.push( msg1 );
	
	client0_dll.pop();
	client1_dll.pop();
	ret = server0_dll.pop();
	
	if( ret ) cout << "NS0 got: " << ret->message() << endl;
	
	
	server0_dll.push( msg2 );

	_GLB_ERR_MSG	= true;
	ret = client0_dll.pop();
	_GLB_ERR_MSG	= false;
	
	client1_dll.pop();
	server0_dll.pop();
	
	if( ret ) cout << "NC0 got: " << ret->message() << endl;
		
		
	return 0;
}
