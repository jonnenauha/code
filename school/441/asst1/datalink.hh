//****************************************************************************************************
//
//			CPSC 441 - Assignment 1
//
//		Simulate Data Link Layer. This module simulates a datalink layer protocol.
//
//							Copyright:	Ryan McDougall -- 2003
//
//****************************************************************************************************

#ifndef __DATALINK_HH
#define __DATALINK_HH



//====================================================================================================
// Includes and Defines

#include <map>
#include "main.hh"
#include "physical.hh"
#include "packet.hh"



//====================================================================================================
// Byte string less than operator/ functor for the STL map.

class	LessThan
{
	public:
		bool	operator() ( const byte* first, const byte* second )
		{
			return ( byte_compare( first, second, _NET_ADDR_SIZE ) < 0 );
		}
};


//====================================================================================================
// Implementation of the Datalink Layer

class 	DataLinkLayer	: public ProtocolLayer
{
	
//----------------------------------------------------------------------------------------------------
//
	public:
				DataLinkLayer( MACAddress source );
				~DataLinkLayer();

		void		push( Packet& packet );
		Packet*		pop();
		
		void		stackLayer( ProtocolLayer& layer );
		

//----------------------------------------------------------------------------------------------------
//			
	private:
	
		MACAddress			_src;		// Source addr is known at manufacture
		PhysicalLayer*			_nic;		// The Network Interface Card
		
		map< byte*, byte*, LessThan >	_table;		// Implementation of ARP via table
		
		list< Packet* >			_message_queue;	// Internal buffer
};


#endif
