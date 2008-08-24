//****************************************************************************************************
//
//			CPSC 441 - Assignment 1
//
//		Simulate Data Link Layer.
//
//							Copyright:	Ryan McDougall -- 2003
//
//****************************************************************************************************

#ifndef __PHYSICAL_HH
#define __PHYSICAL_HH



//====================================================================================================
// Includes and Defines

#include <list>

#include "main.hh"
#include "protocol.hh"
#include "packet.hh"



//====================================================================================================
// Implements the Physical layer.

class 	PhysicalLayer	: public ProtocolLayer
{
	
//----------------------------------------------------------------------------------------------------
//
	public:
				PhysicalLayer();
				~PhysicalLayer();

		void		push( Packet& packet );
		Packet*		pop();
		
		void		stackLayer( ProtocolLayer& layer );
		void		attachNode( PhysicalLayer& node );

		void		transmit( Packet& packet );
		void		receive( Packet& packet );
		bool		ready();
		

//----------------------------------------------------------------------------------------------------
//			
	private:
		list<PhysicalLayer*>	_network;
		list<Packet*>		_message_queue;
};


#endif
