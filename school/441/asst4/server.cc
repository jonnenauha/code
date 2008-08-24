//************************************************************************************************************************
//
//			CPSC 441 - Assignment 4
//
//		Simple Server.
//
//							Copyright:	Ryan McDougall -- 2003
//
//************************************************************************************************************************



//========================================================================================================================
// Includes and Defines

	#include	<iostream>
	#include	<string>
	#include	<signal.h>
	#include	"utility.hh"
	#include	"network.hh"

	using namespace	std;



//========================================================================================================================
// Global Variables and Types

const int	BUF_SIZE	= 3;
const int	NUM_TRIES	= 3;
const int	NUM_SEQUENCES	= 5;

typedef	int(*sequence)(int);



//========================================================================================================================
//

int	sequence_0( int n )
{
	return	static_cast<int>( ::pow( 2.0, n ) );
}

int	sequence_1( int n )
{
	return	( n & 1 )? -n : n;
}

int	sequence_2( int n )
{
	int m	= sequence_0( n );
	return	( n & 1 )? -m : m;
}

int	sequence_3( int n )
{
	return	n*n;;
}

int	sequence_4( int n )
{
	return	sequence_0(n) + sequence_3(n);
}



//========================================================================================================================
// Main Simulation Logic

int	main( int argc, char** argv )
{
	// Main variables.
	
	Utility::buffer<int>	recv_buffer( 1 );
	Utility::buffer<int>	send_buffer( 1 );
	Utility::buffer<int>	init_send_buffer( BUF_SIZE );
	
	Network::inet_address	server_address( "127.0.0.1:6969" );

	Socket::server_inet	socket_server( server_address );
	Socket::basic*		client_socket = NULL;
	
	sequence		sequence_list[ NUM_SEQUENCES ];
	sequence		current_sequence = NULL;
	
	
	// Set up the sequence list
	
	sequence_list[0]	= &sequence_0;
	sequence_list[1]	= &sequence_1;
	sequence_list[2]	= &sequence_2;
	sequence_list[3]	= &sequence_3;
	sequence_list[4]	= &sequence_4;
	
	
	// This mess sets the Socket option that allows rebinding to the same address/port.
	
	Utility::buffer<int>	reuse_value( 1 );
	Socket::option<int>	reuse_opt( SO_REUSEADDR, reuse_value );

	reuse_value[0] = 1;
	socket_server.set_opt( reuse_opt );


	// Ignore child exit signal.
	
	signal( SIGCHLD, SIG_IGN );

	
	// Standard Socket setup.
	
	socket_server.bind();
	socket_server.listen();

	while( client_socket = socket_server.accept() )
	{
	
	// Fork the child process.
	
	if( fork() == 0 )
	{

		// Main Socket loop.
		
		Utility::buffer<int>	score_buffer( NUM_TRIES );
		
	
		for( int try_number=0; try_number < NUM_TRIES; try_number++ )
		{
			// Initialize.
			
			int			sequence_number	= 0;
		
		
			// Choose a new sequence.
		
			current_sequence	= sequence_list[ Utility::random_long( 0, NUM_SEQUENCES ) ];
		
		
			// Send the initial data to client.
			
			init_send_buffer[0]	= current_sequence( sequence_number + 0 );
			init_send_buffer[1]	= current_sequence( sequence_number + 1 );
			init_send_buffer[2]	= current_sequence( sequence_number + 2 );
			sequence_number	+= 3;
		
			client_socket->send( init_send_buffer );
		
		
			// Check the result from client.
		
			client_socket->recv( recv_buffer );
		
			if( recv_buffer[0] == current_sequence( sequence_number ) )
			{
				score_buffer[ try_number ] ++;
				sequence_number ++;
			}
			else
			{}
		
		
			// Send second data to client.
		
			send_buffer[0]	= current_sequence( sequence_number );
			sequence_number ++;
		
			client_socket->send( send_buffer );
		
		
			// Check the result from client.
		
			client_socket->recv( recv_buffer );

			if( recv_buffer[0] == current_sequence( sequence_number ) )
			{
				score_buffer[ try_number ] ++;
				sequence_number ++;
			}
			else
			{}


			// Send final result to client.
		
			send_buffer[0]	= current_sequence( sequence_number );
			
			client_socket->send( send_buffer );
			client_socket->send( score_buffer );
		}
	}
	else
	{}
	}
	
	// Clean up socket.
	
	delete	client_socket;
	
	
	return 0;
}
