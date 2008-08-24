//****************************************************************************************************
//
//			CPSC 441 - Assignment 1
//
//		Simulate Data Link Layer. This module simulates a datalink layer protocol.
//
//							Copyright:	Ryan McDougall -- 2003
//
//****************************************************************************************************


//====================================================================================================
//

#include <iostream>
#include "main.hh"
#include "packet.hh"
#include "datalink.hh"

using namespace std;



//====================================================================================================
// Construct the protocol. Seed the DDL address resolution table.

	DataLinkLayer::DataLinkLayer( MACAddress source )
{
	_src	= source;
	_nic	= NULL;
	
	// Hack the address resolution table for the simulation.
	_table[ byte_string( "cli_0", _NET_ADDR_SIZE ) ] = byte_string( "NC0", _FRM_ADDR_SIZE );
	_table[ byte_string( "cli_1", _NET_ADDR_SIZE ) ] = byte_string( "NC1", _FRM_ADDR_SIZE );
	_table[ byte_string( "srv_0", _NET_ADDR_SIZE ) ] = byte_string( "NS0", _FRM_ADDR_SIZE );
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

	DataLinkLayer::~DataLinkLayer()
{
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// This method frames the higher level packet and pushes it to the lower.

void		DataLinkLayer::push( Packet& packet )
{
	// Build frame.
	DataLinkPacket	frame;
	
	frame.destination( _table[ packet.destination() ] );
	frame.source( _src );
	frame.message( packet.serialize() );
	frame.serialize();
	
	// Send to NIC.
	_nic->push( frame );
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// This method removes the framing and passes the packet up.

Packet* 	DataLinkLayer::pop()
{
	// Get First Frame from NIC.
	Packet*		frame;
	NetworkPacket* 	packet;
	
	
	// Pull packets from the _nic to the DLL buffer.
	while( _nic->ready() )
	{
		frame	= _nic->pop();
		
		// Simulation element. Add some error to message.
		if( _GLB_ERR_MSG )
		{	
			(*( frame->message() ) )++;
		}
		if( _GLB_ERR_DST )
		{
			(*( frame->destination() ) )++;
		}
			
			
		// Drop packets not addressed to us.
		if( byte_compare( _src, frame->destination(), _FRM_ADDR_SIZE ) )
		{	
			cout 	<< _src << ": Dropping frame: " << frame->source() << " -> "
				<< frame->destination() << " : " << frame->message() << endl;
		}
		else
		{
			// Check for transmittion errors.
			if( frame->error() )
			{
				// Error detected in CRC
				cout	<< _src << ": Error in frame: " << frame->source() << " -> "
					<< frame->destination() << " : " << frame->message() << endl;
			}
			else
			{
			
				// No error.
				_message_queue.push_back( frame );
			}
		}
	}

	
	if( _message_queue.size() > 0 )
	{
		// Decompose Frame.	
		frame	= _message_queue.front();
		packet	= new NetworkPacket( frame->message() );
		
		// Clean up.
		delete	frame;
		_message_queue.pop_front();	
	}
	else
	{
		// No new message.
		packet	= NULL;
	}
	
	
	return 	packet;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// This method allows the layer to be stackable.

void		DataLinkLayer::stackLayer( ProtocolLayer& lower )
{
	_nic	=  dynamic_cast<PhysicalLayer*>( &lower );
}
