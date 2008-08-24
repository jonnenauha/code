//************************************************************************************************************************
//
//			CPSC 441 - Assignment 4
//
//							Copyright:	Ryan McDougall -- 2003
//
//************************************************************************************************************************



//========================================================================================================================
// Includes and Defines

	#ifndef		__INCLUDE_CC
	#include	"network.hh"
	#endif
	
	using namespace	std;



//========================================================================================================================
// Includes functionality for communicating over a network.

namespace	Network
{

//________________________________________________________________________________________________________________________
// URL handler class. TODO: Improve parsing capability.

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

		URL::URL( string& url )
		 : _url( url )
	{
		parse_url_string( _url );
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

		URL::URL( const string& protocol, const string& host, const string& port, const string& path )
		 : _protocol( protocol ), _host( host ), _port( port ), _path( path )
	{
		_url = _protocol + _host + _port + _path;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

		URL::URL( const URL& url )
		 : _protocol( url._protocol ), _host( url._host ), _port( url._port ), _path( url._path )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

		URL::~URL()
	{}


//------------------------------------------------------------------------------------------------------------------------
//

	void	URL::parse_url_string( string& url )
	{				
		string::iterator	prot_begin;
		string::iterator	prot_end;
		string::iterator	host_begin;
		string::iterator	host_end;
		string::iterator	port_begin;
		string::iterator	port_end;
		string::iterator	path_begin;
		string::iterator	path_end;
		
		
		prot_begin	= url.begin();
		prot_end	= find( prot_begin, url.end(), _SCHM_DELIM );
		
		host_begin	= prot_end +sizeof( _NETP_DELIM );
		host_end	= find( host_begin, url.end(), _PORT_DELIM );
		
		if( host_end == url.end() )
		{
			host_end	= find( host_begin, url.end(), _PATH_DELIM );
			
			port_begin 	= host_end;
			port_end 	= port_begin;
		}
		else
		{
			port_begin	= host_end +sizeof( _PORT_DELIM );
			port_end	= find( port_begin, url.end(), _PATH_DELIM );
		}
	
		path_begin	= port_end;
		path_end	= url.end();
		
		
		_protocol.assign( prot_begin, prot_end );
		_host.assign( host_begin, host_end );
		_port.assign( port_begin, port_end );
		_path.assign( path_begin, path_end );
	}
	
	
	
//________________________________________________________________________________________________________________________
// Network conversion class to convert from string to numerical form. TODO: Make useful.

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

		inet_address::inet_address( const char* address )
		: _address( 0 ), _port( 0 ), _string( address )
	{
		convert_string_to_address();
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
		
		inet_address::inet_address( string address )
		: _address( 0 ), _port( 0 ), _string( address )
	{
		convert_string_to_address();
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
			
		inet_address::inet_address( const in_addr_t address, const in_port_t port )
		 : _address( htonl( address ) ), _port( htons( port ) )
	{
		convert_address_to_string();
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
			
		inet_address::inet_address( const inet_address& inet_addr )
		 : _address( inet_addr._address ), _port( inet_addr._port ), 
		   _string( inet_addr._string )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
	
		inet_address::~inet_address()
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

	in_addr_t		inet_address::net_address()	const
	{
		return	_address;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
			
	in_port_t		inet_address::net_port()	const
	{
		return	_port;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

	string		inet_address::str_address()	const
	{
		return	_string;
	}
	

//------------------------------------------------------------------------------------------------------------------------
//

	void	inet_address::convert_string_to_address()
	{
		string::iterator port_begin	= find( _string.begin(), _string.end(), _PORT_DELIM );
		string::iterator port_end		= _string.end();
		
		int address	= 0;
		
				
		if( port_begin == port_end )
		{}
		else
		{
			string	port( port_begin+1, port_end );
			_string.erase( port_begin, port_end );
				
			_port	= htons( Utility::string_to_integer( port ) );
		}
				
		if( inet_pton( AF_INET, _string.c_str(), &address ) > 0 )
		{
			_address = htonl( address );

		}
		else
		{
			Error::printer( "Unable to convert address" );
		}
	}
		

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
	
	void	inet_address::convert_address_to_string()
	{
		string	new_string( Utility::integer_to_string( ntohl( _address ) ) );
		int		port	= ntohs( _port );
	
		if( port > 0 )
		{
			new_string += ':' + Utility::integer_to_string( port );
		}
		else
		{}
				
		_string	= new_string;
	}

}



//========================================================================================================================
// Includes socket related classes.

namespace	Socket
{

//________________________________________________________________________________________________________________________
// Socket option class allows encapsulation of sockopts. TODO: figure out how sockopts work!

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// 

	template < typename Type >
		option<Type>::option( int name )
		 : _name( name )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

	template < typename Type >
		option<Type>::option( int name, Utility::buffer<Type>& value )
		 : _name( name ), _value( value )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
	template < typename Type >
		option<Type>::option( const option<Type>& copy )
		 : _name( copy.name() ), _value( copy.value() )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

	template < typename Type >
		option<Type>::~option()
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
	template < typename Type >
	int	option<Type>::name()
	{
		return	_name;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

	template < typename Type >
	Utility::buffer<Type>&	option<Type>::value()
	{
		return	_value;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//	

	template < typename Type >
	const option<Type>&	option<Type>::operator=( Utility::buffer<Type>& rhs )
	{
		_value	= rhs;
		
		return	*this;
	}



//________________________________________________________________________________________________________________________
// Basic socket object supports creation, send and recv.

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Default constructor. Creates a socket with domain type, and protocol.

		basic::basic( int domain, int type, int protocol )
		 : _domain( domain ), _type( type ), _protocol( protocol )
	{
		_file_desc = socket( _domain, _type, _protocol );
		
		if( _file_desc < 0 )
		{
			Error::printer( "Unable to create socket" );
		}
		else
		{}
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Copy constructor for C style sockets.

		basic::basic( const int socket_file_desc, int domain, int type, int protocol )
		 : _file_desc( socket_file_desc ), _domain( domain ), _type( type ), _protocol( protocol )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Copy constructor.
			
		basic::basic( const basic& socket )
		 : _file_desc( socket._file_desc ), 
		   _domain( socket._domain ), _type( socket._type ), _protocol( socket._protocol )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Default destructor.
	
		basic::~basic()
	{
		::close( _file_desc );
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Assignment operator.

	basic&		basic::operator=( const basic& rhs )
	{		
		_file_desc	= rhs._file_desc;
		_domain		= rhs._domain;
		_type		= rhs._type;
		_protocol	= rhs._protocol;
		
		return	*this;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Accessing the socket fields. TODO: Should file_desc be public?

	int		basic::file_desc()
	{
		return	_file_desc;
	}
	
	int		basic::domain()
	{
		return	_domain;
	}
	
	int		basic::type()
	{
		return	_type;
	}
	
	int		basic::protocol()
	{
		return	_protocol;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Sets a socket option for the socket. TODO: getsockopt

	template < typename Type >
	void		basic::set_opt( option<Type>& opt )
	{
		if( ::setsockopt( _file_desc, SOL_SOCKET, opt.name(), opt.value().byte_data(), opt.value().byte_size() ) < 0 )
		{
			Error::printer( "Unable to set socket options" );
		}
		else
		{}
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Send information to the socket.

	template < typename Type >
	size_t		basic::send( Utility::buffer<Type>& buffer )
	{
		int 	flags = 0;
		
		size_t	bytes_writ = ::send( _file_desc, buffer.byte_data(), buffer.byte_size(), flags  );
		
		if( bytes_writ > 0 )
		{
			if( bytes_writ == buffer.byte_size() )
			{}
			else
			{
				Error::printer( "Problem sending" );
			}
		}
		else
		{
			Error::printer( "Unable to send" );
		}
		
		return	bytes_writ;
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Receive information from the socket.

	template < typename Type >
	size_t		basic::recv( Utility::buffer<Type>& buffer )
	{
		int	flags = 0;
		
		size_t	bytes_read = ::recv( _file_desc, buffer.byte_data(), buffer.byte_size(), flags  );
		
		if( bytes_read > 0 )
		{}
		else
		{
			Error::printer( "Unable to receive" );
		}
			
		return	bytes_read;
	}



//________________________________________________________________________________________________________________________
// Socket Server interface for all domains.

		server::server( int domain, int type, int protocol )
		 : basic( domain, type, protocol )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

		server::server( const server& srv )
		 : basic( srv )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

		server::~server()
	{}	



//________________________________________________________________________________________________________________________
// Socket Client interface for all domains.

		client::client( int client_file_desc, int domain, int type, int protocol )
		 : basic( client_file_desc, domain, type, protocol )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
		client::client( int domain, int type, int protocol )
		 : basic( domain, type, protocol )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//	
		
		client::client( const client& cli )
	 	: basic( cli )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
			
		client::~client()
	{}


	
	
//________________________________________________________________________________________________________________________
// Socket Server class for Internet domain. Defaults to STREAM.

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Constructor takes address from inet_address object.

		server_inet::server_inet( const Network::inet_address& inet, int type )
		 : server( AF_INET, type, 0 )
	{
		_address			= new sockaddr_in;
		_address->sin_family		= _domain;
		_address->sin_port		= inet.net_port();
		_address->sin_addr.s_addr	= inet.net_address();
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Constructor takes address as long/short pair.
			
		server_inet::server_inet( in_addr_t address, in_port_t port, int type )
		 : server( AF_INET, type, 0 )
	{
		_address			= new sockaddr_in;
		_address->sin_family		= _domain;
		_address->sin_port		= htons( port );
		_address->sin_addr.s_addr	= htonl( address );
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Copy constructor borrows basic socket copy constructor.
			
		server_inet::server_inet( const server_inet& server_inet )
		 : server( server_inet )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Default destructor.
		
		server_inet::~server_inet()
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Bind socket server to TCP/IP address.

	void		server_inet::bind()
	{
		if( ::bind( _file_desc, (sockaddr*)_address, sizeof( *_address ) ) < 0 )
		{
			 Error::printer( "Unable to bind" );
		}
		else
		{
			cout	<< "Socket Server bound to " 
				<< _address->sin_addr.s_addr << ":" << _address->sin_port << endl;
		}
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Listen for incoming connections.
			
	void		server_inet::listen( int backlog )
	{
		if( ::listen( _file_desc, backlog ) < 0 )
		{
			 Error::printer( "Unable to listen" );
		}
		else
		{}
	}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Accept socket connection from queue. Return basic socket for I/O.
			
	basic*	server_inet::accept()
	{
		int 		file_desc	= -1;
		socklen_t	address_size	= sizeof( *_address );
		basic*	socket		= NULL;
		
		file_desc = ::accept( _file_desc, (sockaddr*)_address, &address_size );
				
		if( file_desc < 0 )
		{
			 Error::printer( "Unable to accpet" );
		}
		else
		{
			socket = new basic( file_desc, _domain, _type, _protocol );
		}
				
		return	socket;
	}


//________________________________________________________________________________________________________________________
// Socket client interface for the Internet domain. Defaults to STREAM.

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Default constructor sets its _file_desc to -1 until connect.

		client_inet::client_inet()
		 : client( -1, AF_INET, SOCK_STREAM, 0 )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Default constructor sets its _file_desc to -1 until connect.

		client_inet::client_inet( int type )
		 : client( -1, AF_INET, type, 0 )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Copy constructor uses basic socket copy constructor.

		client_inet::client_inet( const client_inet& inet_cli )
		 : client( inet_cli )
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Default destructor.
			
		client_inet::~client_inet()
	{}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Connect to Socket Server by inet_address. 

	basic*	client_inet::connect( const Network::inet_address& inet )
	{
		basic*		socket		= new basic( _domain, _type, _protocol );
		sockaddr_in	addr;
		
		addr.sin_family		= AF_INET;
		addr.sin_port		= inet.net_port();
		addr.sin_addr.s_addr	= inet.net_address();


		_file_desc	= socket->file_desc();
				
		if( ::connect( _file_desc, (sockaddr*)&addr, sizeof( addr ) ) < 0 )
		{
			Error::printer( "Unable to connect" );
			
			delete	socket;
			socket	= NULL;
		}
		else
		{}
		
		return	socket;
	}



//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Connect to Socket Server by TCP/IP address. 

	basic*	client_inet::connect( in_addr_t address, in_port_t port )
	{
		basic*		socket		= new basic( _domain, _type, _protocol );
		sockaddr_in	addr;
				
		addr.sin_family		= AF_INET;
		addr.sin_port		= htonl( port );
		addr.sin_addr.s_addr	= htons( address );


		_file_desc	= socket->file_desc();
				
		if( ::connect( _file_desc, (sockaddr*)&addr, sizeof( addr ) ) < 0 )
		{
			Error::printer( "Unable to connect" );
			
			delete	socket;
			socket	= NULL;
		}
		else
		{}
		
		return	socket;		
	}

}
