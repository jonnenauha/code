//************************************************************************************************************************
//
//			CPSC 441 - Assignment 4
//
//		Simple Client.
//
//							Copyright:	Ryan McDougall -- 2003
//
//************************************************************************************************************************



//========================================================================================================================
// Includes and Defines

	#include	<iostream>
	#include	<string>
	#include	"utility.hh"
	#include	"network.hh"

	using namespace	std;



//========================================================================================================================
// Global Variables and Types

const int	BUF_SIZE	= 3;
const int	NUM_TRIES	= 3;



//========================================================================================================================
// Main Simulation Logic

int	main( int argc, char** argv )
{
	// Main variables.
	Utility::buffer<int>	score_buffer( NUM_TRIES );
	Utility::buffer<int>	send_buffer( 1 );
	Utility::buffer<int>	recv_buffer( 1 );
	Utility::buffer<int>	init_recv_buffer( BUF_SIZE );
	
	Network::inet_address	server_address( "127.0.0.1:6969" );
	
	Socket::client_inet	socket_client;
	Socket::basic*		server_socket = NULL;
	
	
	// Connect to the Server.
	
	server_socket = socket_client.connect( server_address );
	
	
	// Main loop.
	
	if( server_socket )
	{
		int	sequence_number	= 0;
		
		for( int try_number = 0; try_number < NUM_TRIES; try_number++ )
		{
			cout << "The initial sequence is: ";
			server_socket->recv( init_recv_buffer );
			init_recv_buffer.print();
			
			cout << "Type your guess then press enter: ";
			cin >> send_buffer[0];
			server_socket->send( send_buffer );
			
			
			cout << "The next in the sequence is: ";
			server_socket->recv( recv_buffer );
			recv_buffer.print();

			cout << "Type your guess then press enter: ";
			cin >> send_buffer[0];
			server_socket->send( send_buffer );
			
			
			cout << "The next in the sequence is: ";
			server_socket->recv( recv_buffer );
			recv_buffer.print();
			
			cout << "Your score is: ";
			server_socket->recv( score_buffer );
			score_buffer.print();
		}
	}

	
	// Clean up Socket.
	
	delete	server_socket;
	
	
	return 0;
}
