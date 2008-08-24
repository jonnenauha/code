//****************************************************************************************************
//
//			CPSC 441 - Assignment 1
//
//		Simulate Physical Layer. Physical layer is a special layer that additional to
//	logical components, must have physical components.
//
//							Copyright:	Ryan McDougall -- 2003
//
//****************************************************************************************************


//====================================================================================================
//

#include <iostream>
#include "physical.hh"

using namespace std;



//====================================================================================================
// Constructor.

	PhysicalLayer::PhysicalLayer()
{
	_network.push_back( this );	// Loopback membership
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// DEstructor.

	PhysicalLayer::~PhysicalLayer()
{
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// This method transmits the packet physically, instead of pushing it lower.

void		PhysicalLayer::push( Packet& packet )
{
	transmit( packet );	// Physical transmission
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Remove from the physical buffer and pass up the stack.

Packet* 	PhysicalLayer::pop()
{
	Packet*	next	= _message_queue.front();

	_message_queue.pop_front();

	return	next;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Unused, nothing lower than physical

void		PhysicalLayer::stackLayer( ProtocolLayer& layer )
{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// This method physically connects the NICs

void		PhysicalLayer::attachNode( PhysicalLayer& node )
{
	_network.push_back( &node );
	node._network.push_back( this );
}



//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Physical reception

void		PhysicalLayer::receive( Packet& packet )
{
	_message_queue.push_back( &packet );
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Physical transmission

void		PhysicalLayer::transmit( Packet& packet )
{	
	Packet*				copy	= NULL;
	list<PhysicalLayer*>::iterator	node	= _network.begin();
	
	while( node != _network.end() )
	{
		copy = packet.clone();
		
		( *node )->receive( *copy );
	
		node++;
	}
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Is the medium ready

bool		PhysicalLayer::ready()
{
	return ( _message_queue.size() > 0 );
}
