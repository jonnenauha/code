//****************************************************************************************************
//
//			CPSC 441 - Assignment 1
//
//		Simulate Transmission Packets. Defines the interface to all Packets.
//
//							Copyright:	Ryan McDougall -- 2003
//
//****************************************************************************************************

#ifndef __PACKET_HH
#define __PACKET_HH


//====================================================================================================
//

#include "main.hh"


//====================================================================================================
// A general interface to a packet.

class	Packet
{
	public:
		virtual void		source( byte* src )		= 0;
		virtual byte*		source()			= 0;
		
		virtual void		destination( byte* dst )	= 0;
		virtual byte*		destination()			= 0;
		
		virtual void		message( byte* msg )		= 0;
		virtual	byte*		message()			= 0;
		
		virtual byte*		serialize()			= 0;
		virtual Packet*		clone()				= 0;
		
		virtual	bool		error()				= 0;
		
		virtual int		size()				= 0;
};


//====================================================================================================
// A Network level packet.

class	NetworkPacket	: public Packet
{

//----------------------------------------------------------------------------------------------------
//
	public:
				NetworkPacket();
				NetworkPacket( byte* packet );
				NetworkPacket( const NetworkPacket& copy );
		virtual		~NetworkPacket();
				
		void		source( byte* src );
		byte*		source();
		
		void		destination( byte* dst );
		byte*		destination();
		
		void		message( byte* msg );
		byte*		message();
		
		byte*		serialize();
		Packet*		clone();

		bool		error();
		
		int		size();		


//----------------------------------------------------------------------------------------------------
//
	private:
		
		byte*		_packet;
		int		_size;
};


//====================================================================================================
// A Data Link level packet.

class	DataLinkPacket	: public Packet
{

//----------------------------------------------------------------------------------------------------
//
	public:				
				DataLinkPacket();
				DataLinkPacket( byte* packet );
				DataLinkPacket( const DataLinkPacket& copy );
		virtual		~DataLinkPacket();
				
		void		source( byte* src );
		byte*		source();
		
		void		destination( byte* dst );
		byte*		destination();
		
		void		message( byte* msg );
		byte*		message();
		
		byte*		serialize();
		Packet*		clone();
		
		bool		error();
		
		int		size();		

//----------------------------------------------------------------------------------------------------
//
	private:
		
		byte*		_packet;
		int		_size;
		
		CRC32		checksum();
};



#endif
