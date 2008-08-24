//************************************************************************************************************************
//
//			CPSC 441 - Assignment 4
//
//							Copyright:	Ryan McDougall -- 2003
//
//************************************************************************************************************************

#ifndef __NETWORK_HH
#define __NETWORK_HH



//========================================================================================================================
//
	#include	<iostream>
	#include	<string>
	#include	<algorithm>
	
	#include	<unistd.h>
	#include	<sys/types.h>
	#include	<sys/socket.h>
	#include	<netinet/in.h>
	#include	<arpa/inet.h>
	
	#include	"utility.hh"
	
	using namespace	std;



//========================================================================================================================
//

namespace Network
{

//________________________________________________________________________________________________________________________
//
	const int 	_QUAD_SIZE	= 8;
	const char	_QUAD_DELIM	= '.';
	const char	_SCHM_DELIM	= ':';
	const char	_PORT_DELIM	= ':';
	const char	_PATH_DELIM	= '/';
	const char	_NETP_DELIM[3]	= "//";



//________________________________________________________________________________________________________________________
//
	class	URL
	{
		public:
				URL( string& url );
				URL( const string& protocol, const string& host, const string& port, const string& path );
				URL( const URL& url );
			
			virtual	~URL();
			
			
		protected:
			
			string	_url;
			string	_protocol;
			string	_host;
			string	_path;
			string	_port;
			
			
			void	parse_url_string( string& url );
	};



//________________________________________________________________________________________________________________________
//

	class	inet_address
	{
		public:
				inet_address( const char* address );
				inet_address( string address );
				inet_address( const in_addr_t address, const in_port_t port = 0 );
				inet_address( const inet_address& inet_addr );
			
			virtual	~inet_address();
			
			
			in_addr_t	net_address()	const;
			in_port_t	net_port()	const;
			string		str_address()	const;
			

		private:
			in_addr_t	_address;
			in_port_t	_port;
			string		_string;


			void	convert_string_to_address();
			void	convert_address_to_string();
	};


}



//========================================================================================================================
//

namespace	Socket
{

//________________________________________________________________________________________________________________________
//

	template < typename Type >
	class	option
	{
		public:
				option( int name );
				option( int name, Utility::buffer<Type>& value );
				option( const option<Type>& copy );
				
			virtual	~option();
				
				
			int			name();
			Utility::buffer<Type>&	value();
			
			const option<Type>&	operator=( Utility::buffer<Type>& rhs );
			
			
		private:
			int			_name;
			Utility::buffer<Type>	_value;
			
				option();
	};



//________________________________________________________________________________________________________________________
//
	class	basic
	{
		public:
				basic( int domain, int type, int protocol );
				basic( const int socket_file_desc, int domain, int type, int protocol );
				basic( const basic& socket );
			
			virtual	~basic();
			
			
			int		file_desc();
			int		domain();
			int		type();
			int		protocol();
			
			basic&	operator=( const basic& rhs );

			template < typename Type >
			void		set_opt( option<Type>& opt );

			template < typename Type >
			size_t		send( Utility::buffer<Type>& buffer );
			
			template < typename Type >
			size_t		recv( Utility::buffer<Type>& buffer );


		protected:
		
			int		_file_desc;
			int		_domain;
			int		_type;
			int		_protocol;
			
				basic();
	};

	

//________________________________________________________________________________________________________________________
//
	class	server		: public basic
	{
		public:
				server( int domain, int type, int protocol );
				server( const server& srv );
			
			virtual	~server();
			

			virtual	void		bind()				= 0;
			virtual	void		listen( int backlog = 5 )	= 0;
			virtual	basic*		accept()			= 0;
	};



//________________________________________________________________________________________________________________________
//	
	class	client		: public basic
	{
		public:
				client( int client_file_desc, int domain, int type, int protocol );
				client( int domain, int type, int protocol );
				client( const client& cli );
			
			virtual	~client();


			virtual basic*	connect( in_addr_t address, in_port_t port )		= 0;
			virtual basic*	connect( const Network::inet_address& inet )		= 0;
	
	};


	
//________________________________________________________________________________________________________________________
//
	class	server_inet	: public server
	{
		public:
				server_inet( const Network::inet_address& inet, int type = SOCK_STREAM );
				server_inet( in_addr_t address, in_port_t port, int type = SOCK_STREAM );
				server_inet( const server_inet& inet_srv );
				
			virtual	~server_inet();


			virtual	void		bind();
			virtual	void		listen( int backlog = 5 );
			virtual	basic*		accept();


		protected:
			sockaddr_in*		_address;
			
				server_inet();
	};



//________________________________________________________________________________________________________________________
//
	class	client_inet	: public client
	{
		public:
				client_inet();
				client_inet( int type );
				client_inet( const client_inet& inet_srv );
			
			virtual	~client_inet();

			virtual basic*	connect( const Network::inet_address& inet );
			virtual basic*	connect( in_addr_t address, in_port_t port );
	};
}



//========================================================================================================================
// Template hack.

#ifdef		__INCLUDE_CC
#include	"network.cc"
#endif


#endif
