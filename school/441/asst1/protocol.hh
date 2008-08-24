//****************************************************************************************************
//
//			CPSC 441 - Assignment 1
//
//		Simulate Protocol Stack. Implements an abstract interface to all protocol layers, and
//	an contrete class to composit those layers.
//
//							Copyright:	Ryan McDougall -- 2003
//
//****************************************************************************************************

#ifndef __PROTOCOL_HH
#define __PROTOCOL_HH


//====================================================================================================
//
#include <vector>
#include "main.hh"
#include "packet.hh"

using namespace std;


//====================================================================================================
// A general Protocol layer.

class	ProtocolLayer
{
	public:
		virtual	void		stackLayer( ProtocolLayer& layer )	= 0;
		virtual	void		push( Packet& packet )			= 0;
		virtual	Packet*		pop()					= 0;
};


//====================================================================================================
// A Protocol stack object.

class	ProtocolStack
{
	public:
				ProtocolStack();
				~ProtocolStack();
			
		void		stackLayer( ProtocolLayer& layer );
		
		void		send( byte* message );
		const byte*	receive();
	
	
	private:
		vector<ProtocolLayer*>	_stack;

};


#endif
