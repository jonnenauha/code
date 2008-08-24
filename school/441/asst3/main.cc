//****************************************************************************************************
//
//			CPSC 441 - Assignment 3
//
//		Simulate Network Layer Routing.
//
//							Copyright:	Ryan McDougall -- 2003
//
//****************************************************************************************************



//====================================================================================================
// Includes and Defines

#include	<iostream>
#include	<fstream>
#include	<algorithm>
#include 	<vector>
#include	<queue>

using namespace std;



//====================================================================================================
// Global Variables and Types

typedef	int	vertex;

template<class weight> class		Network;

int		glb_number_dropped	= 0;
int		glb_number_hops		= 0;
int		glb_total_calls		= 0;
float		glb_total_delay		= 0;

int		_NET_ROUTER		= 0;

const int	_NET_MAX_NODES		= 26;
const int	_NET_MAX_WEIGHT		= 1000;
const int	_NET_UNREACHABLE	= 1001;




//====================================================================================================
// Utility functions.

template < typename Type >
Type	maximum( Type op1, Type op2 )
{
	return	( op1 < op2 )? op2 : op1;
}

template < typename Type >
Type	minimum( Type op1, Type op2 )
{
	return	( op1 < op2 )? op1 : op2;
}




//====================================================================================================
// Object represents a weighted Network.

template < typename weight >
class	Network
{
//----------------------------------------------------------------------------------------------------
//
	public:
		Network( int num_nodes )
		 : _size( num_nodes )
		{
			_weight		= new weight*[ _size ];
			_capacity	= new int*[ _size ];
			_load		= new int*[ _size ];
			
			for( int i=0; i < _size; i++ )
			{
				_weight[i]	= new weight[ i+1 ];
				_capacity[i]	= new int[ i+1 ];
				_load[i]	= new int[ i+1 ];
				
				fill_n( _weight[i], i+1, _NET_UNREACHABLE );
				fill_n( _capacity[i], i+1, 0 );
				fill_n( _load[i], i+1, 0 );
				
				_weight[i][i] = 0;
			}
		}
		
		
		~Network()
		{
			for( int i=0; i < _size; i++ )
			{
				delete [] _weight[i];
				delete [] _capacity[i];
			}
			
			delete [] _weight;
			delete [] _capacity;
		}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

		void	connectionWeight( vertex i, vertex j, weight d )
		{
			if( check_bounds( i, j ) )
			{
				_weight[i][j]	= d;
			}
			else
			{}
		}
		
		
		weight	connectionWeight( vertex i, vertex j )		const
		{
			weight	result = _NET_UNREACHABLE;
			
			if( check_bounds( i, j ) )
			{
				if( _capacity[i][j] > _load[i][j] )
				{	
					result 	= _weight[i][j];
				}
				else
				{}		
				
			}
			else
			{}
			
			return 	result;
		}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//		

		void	connectionCapacity( vertex i, vertex j, int c )
		{
			if( check_bounds( i, j ) )
			{
				_capacity[i][j]	= c;
			}
			else
			{}
		}
		
		
		int	connectionCapacity( vertex i, vertex j )	const
		{
			int	result;
			
			if( check_bounds( i, j ) )
			{
				result 	= _capacity[i][j];
			}
			else
			{
				result	= 0;
			}
			
			return 	result;
		}
		
		
		int	connectionLoad( vertex i, vertex j )
		{
			int	result;
			
			if( check_bounds( i, j ) )
			{
				result 	= _load[i][j];
			}
			else
			{
				result	= 0;
			}
			
			return 	result;
		}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
		bool	checkConnection( vertex i, vertex j )		const
		{
			bool	success = false;
			
			if( check_bounds( i, j ) )
			{
				success	= ( _capacity[i][j] > _load[i][j] );
			}
			else
			{}
			
			return	success;
		}
		
		
		void	addConnection( vertex i, vertex j )
		{
			if( check_bounds( i, j ) )
			{
				_load[i][j] ++;
			}
			else
			{}
		}
		
		
		void	removeConnection( vertex i, vertex j )
		{
			if( check_bounds( i, j ) )
			{
				_load[i][j] --;
			}
			else
			{}			
		}
		

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

		bool	addRoute( vector<vertex>* route )
		{
			for( int i=0; i < route->size()-1; i++ )
			{
				addConnection( (*route)[i], (*route)[i+1] );
			}
			
			return	true;
		}
		
		
		bool	removeRoute( vector<vertex>* route )
		{
			for( int i=0; i < route->size()-1; i++ )
			{
				removeConnection( (*route)[i], (*route)[i+1] );
			}
			
			return	true;		
		}
		

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

		int	size()	const
		{
			return	_size;
		}
		

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

		void	printWeight()		const
		{
			for( int i=0; i < _size; i++ )
			{
				cout << "Node " <<  i << ": ";
				
				for( int j=0; j <= i; j++ )
				{
					cout << _weight[i][j] << " ";
				}
				
				cout << endl;
			}
		}


		void	printCapacity()	const
		{
			for( int i=0; i < _size; i++ )
			{
				cout << "Node " <<  i << ": ";
							
				for( int j=0; j <= i; j++ )
				{
					cout << _capacity[i][j] << " ";
				}
				
				cout << endl;
			}
		}


		void	printLoad()	const
		{
			for( int i=0; i < _size; i++ )
			{
				cout << "Node " <<  i << ": ";
							
				for( int j=0; j <= i; j++ )
				{
					cout << _load[i][j] << " ";
				}
				
				cout << endl;
			}
		}
		



//----------------------------------------------------------------------------------------------------
//
	private:
		weight**	_weight;
		int**		_capacity;
		int**		_load;
		int		_size;


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//

		bool	check_bounds( vertex& i, vertex& j )	const
		{
			vertex 	t;
			
			bool	success = true;
			
			if( i < _size && j < _size )
			{
				if( j < i )
				{}
				else
				{
					t = j;
					j = i;
					i = t;
				}
			}
			else
			{
				cout << "Out of bounds: " << i << "," << j << endl;
				success = false;
			}
			
			return 	success;
		}
};




//====================================================================================================
// Functor represents a Route finding algorithm interface.


class	Router
{
//----------------------------------------------------------------------------------------------------
//
	public:
		Router( Network<int>& network ) : _network( &network )	{}
		virtual vector<vertex>*	operator()( vertex a, vertex b ) const = 0;
		
	protected:
		Network<int>*	_network;
};




//====================================================================================================
// Functor represents a SDPF Route finding algorithm.

template < typename weight >
class	BasicRouter	: public Router
{
//----------------------------------------------------------------------------------------------------
//
	public:
		BasicRouter( Network<int>& network ) : Router( network )
		{}
		
		virtual	vector<vertex>*	operator()( vertex begin, vertex end ) const
		{
			int			number_vertices = _network->size();
			
			vector<bool>		vertex_set( number_vertices );
			vector<weight> 		distance( number_vertices );
			vector<vertex>		predecessor( number_vertices );
			
			vector<vertex>*		route;
			
			vertex			next_vertex	 = 0;
			
			
			// Initialize.
			initialize( begin, vertex_set, distance, predecessor );


			// For all verticies calculate shortest path.
			while( number_vertices )
			{
			
				// Find the smallest known distance to begin.
				next_vertex = find_least_vertex( vertex_set, distance );

				
				// Update adjacent vertices' distance and predecessor
				update( next_vertex, distance, predecessor );
				
				
				// This vertex is done.
				vertex_set[ next_vertex ] = false;
				number_vertices --;
			}


			// Create the route from the predecessor list.
			route =	build_route( begin, end, predecessor );
			
			
			// Count the delay of the route.
			if( route )
			{
				for( int i=0; i < route->size()-1; i++ )
				{
					glb_total_delay	+= _network->
						connectionWeight( (*route)[i], (*route)[i+1] );
				}
			}
			
			
			return	route;
		}

//----------------------------------------------------------------------------------------------------
//
	protected:

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Initialize to appropriate values.

		virtual	void	initialize( const vertex begin, vector<bool>& vertex_set, 
					vector<weight>& distance, vector<vertex>& predecessor ) const
		{
			for( int i=0; i < predecessor.size(); i++ )
			{
				predecessor[i] = i;
			}
			
			fill( vertex_set.begin(), vertex_set.end(), true );
			fill( distance.begin(), distance.end(), _NET_UNREACHABLE );
			
			distance[ begin ] = 0;			
		}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Find the smallest distance in the vector set.

		virtual	vertex	find_least_vertex( const vector<bool>& vertex_set, 
						const vector<weight>& distance ) const
		{
			weight	smallest_weight = _NET_MAX_WEIGHT;
			vertex	smallest_vertex = 0;
			
			for( int i=0; i < distance.size(); i++ )
			{
				if( vertex_set[i] &&( distance[i] < smallest_weight ))
				{
					smallest_weight = distance[i];
					smallest_vertex	= i;
				}
			}
			
			return	smallest_vertex;
		}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Adjust distances and predecessors for each adjacent to smallest.

		virtual void	update( vertex v, vector<weight>& distance, 
						vector<vertex>& predecessor ) const
		{
			weight	w = 0;

			for( int u=0; u < distance.size(); u++ )
			{
				w = distance[ v ] + _network->connectionWeight( v, u );
						
				if( distance[u] > w )
				{
					distance[u] = w;
					predecessor[u] = v;
				}
				else
				{}
			}			
		}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Turn the predecessor list into a route.

		virtual	vector<vertex>*	build_route( const vertex begin, const vertex end, 
							const vector<vertex>& predecessor ) const
		{
		
			vertex		next_vertex	= end;
			vector<vertex>*	route		= new vector<vertex>;
			
			while( next_vertex != begin )
			{
				// If there is a cycle then there is no route.
				
				if( next_vertex == predecessor[ next_vertex ] )
				{
					delete	route;
					return	NULL;
				}
				else
				{
					// Add to the route. Find next.
					
					route->push_back( next_vertex );
					next_vertex = predecessor[ next_vertex ];
				}
			}
			
			route->push_back( begin );
			reverse( route->begin(), route->end() );
			
			/*cout << "Route: ";
			for( int i=0; i < route->size(); i++ )
				cout << (*route)[i] << " ";
			cout << endl;*/
	
			return	route;			
		}
};




//====================================================================================================
// Functor represents a SHPF Route finding algorithm.

template < typename weight >
class	HopRouter	: public BasicRouter<weight>
{
//----------------------------------------------------------------------------------------------------
//
	public:
		HopRouter( Network<int>& network ) : BasicRouter<weight>( network )
		{}

//----------------------------------------------------------------------------------------------------
//
	private:
		
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Adjust distances and predecessors for each adjacent to smallest.

		virtual void	update( vertex v, vector<weight>& distance, 
						vector<vertex>& predecessor ) const
		{
			for( int u=0; u < distance.size(); u++ )
			{	
				if( _network->checkConnection( u, v ) )
				{
					if( distance[u] > distance[ v ] + 1 )
					{
						distance[u] = distance[ v ] + 1;
						predecessor[u] = v;
					}
					else
					{}
				}
				else
				{}
			}			
		}
};




//====================================================================================================
// Functor represents a LLC Route finding algorithm.

template < typename weight >
class	LLCRouter	: public BasicRouter<weight>
{
//----------------------------------------------------------------------------------------------------
//
	public:
		LLCRouter( Network<int>& network ) : BasicRouter<weight>( network )
		{}

//----------------------------------------------------------------------------------------------------
//
	private:
		
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Adjust distances and predecessors for each adjacent to smallest.

		virtual void	update( vertex v, vector<weight>& distance, 
						vector<vertex>& predecessor ) const
		{
			weight	load_ratio;
			
			for( int u=0; u < distance.size(); u++ )
			{	
				if( _network->checkConnection( u, v ) )
				{
				
					load_ratio =  (weight)_network->connectionLoad( u, v )
						/ (weight)_network->connectionCapacity( u, v );
						
					if( distance[u] > maximum( distance[v], load_ratio ) )
					{
						distance[u] = maximum( distance[v], load_ratio );
						predecessor[u] = v;
					}
					else
					{}
				}
				else
				{}
			}			
		}
};




//====================================================================================================
// Functor represents a MFC Route finding algorithm.

template < typename weight >
class	MFCRouter	: public BasicRouter<weight>
{
//----------------------------------------------------------------------------------------------------
//
	public:
		MFCRouter( Network<int>& network ) : BasicRouter<weight>( network )
		{}

//----------------------------------------------------------------------------------------------------
//
	private:
		
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// Adjust distances and predecessors for each adjacent to smallest.

		virtual void	update( vertex v, vector<weight>& distance, 
						vector<vertex>& predecessor ) const
		{
			weight	available;
			
			for( int u=0; u < distance.size(); u++ )
			{	
				if( _network->checkConnection( u, v ) )
				{
				
					available = 1.0/(weight)(_network->connectionCapacity( u, v )
								- _network->connectionLoad( u, v ));
						
					if( distance[u] > maximum( distance[v], available ) )
					{
						distance[u] = maximum( distance[v], available );
						predecessor[u] = v;
					}
					else
					{}
				}
				else
				{}
			}			
		}
};




//====================================================================================================
// Network Event class.

class	NetworkEvent
{
	public:
		int			source;
		int			destination;
		float			begin_time;
		float			end_time;
		
		vector<vertex>*		route;
		
		void	print()
		{
			cout 	<< "Event at " << begin_time << ":" << end_time << " : " 
				<< source << " -> " << destination << endl;
		}
};



//====================================================================================================
// Network Event Comparator ( by start time ) and Priority Queue definition.

class	NetworkEventStartTimeComparator
{
	public:
		bool	operator()( const NetworkEvent* op1, const NetworkEvent* op2 )
		{
			return	( op1->begin_time > op2->begin_time );
		}
};

typedef	priority_queue< NetworkEvent*, vector<NetworkEvent*>, NetworkEventStartTimeComparator >	
	NetworkEventStartQueue;


//----------------------------------------------------------------------------------------------------
// Network Event Comparator ( by stop time ) and Priority Queue definition.

class	NetworkEventStopTimeComparator
{
	public:
		bool	operator()( const NetworkEvent* op1, const NetworkEvent* op2 )
		{
			return	( op1->end_time > op2->end_time );
		}
};

typedef	priority_queue< NetworkEvent*, vector<NetworkEvent*>, NetworkEventStopTimeComparator >	
	NetworkEventStopQueue;



//====================================================================================================
// Main Simulation Logic

int	main( int argc, char** argv )
{
	if( argc != 4 )
	{
		cout	<< "uasge: ./netsim <TOPOLOGY> <WORKLOAD> <ROUTER>" << endl
			<< "\twhere ROUTER = 1 (SDPF) | 2 (SHPF) | 3 (LLP) | 4 (MFC)" << endl;
			
		return -1;
	}
	else
	{
		Network<int>		network( _NET_MAX_NODES );
		Router*			router;
		
		BasicRouter<int>	basic_router( network );
		HopRouter<int>		hop_router( network );
		LLCRouter<float>	llc_router( network );
		MFCRouter<float>	mfc_router( network );
		
		NetworkEventStartQueue	pending_event;	
		NetworkEventStopQueue	current_event;
		NetworkEvent*		event;

		ifstream		topology_file( argv[1] );
		ifstream		workload_file( argv[2] );
			
		char			node1, node2;
		int			delay, capacity;
		float			time, duration;
		
		_NET_ROUTER		= argv[3][0] - 48;
		
		
		
		if( topology_file && workload_file )
		{
			
			while( !topology_file.eof() )
			{
				// Read in topology parameters.
				
				topology_file >> node1;
				topology_file >> node2;
				topology_file >> delay;
				topology_file >> capacity;
				
				
				// Readjust ASCII charcters to integers.
				
				node1 -= 'A';
				node2 -= 'A';
			
								
				// Set the links of the network.

				if(( node1 >= 0 )&&( node2 >= 0 ))
				{ 
					network.connectionWeight( node1, node2, delay );
					network.connectionCapacity( node1, node2, capacity );
				}
				else
				{}
			}
			
			
			
			while( !workload_file.eof() )
			{
				// Read in workload parameters.
				
				workload_file >> time;
				workload_file >> node1;
				workload_file >> node2;
				workload_file >> duration;
				
				
				// Readjust ASCII charcters to integers.
				
				node1 -= 'A';
				node2 -= 'A';				
				
				
				// Create a new Event
				
				if(( node1 >= 0 )&&( node2 >= 0 ))
				{
					event			= new NetworkEvent;
					event->source		= node1;
					event->destination	= node2;
					event->begin_time	= time;
					event->end_time		= time + duration;
					event->route		= NULL;
				
					pending_event.push( event );
				}
				
				// Count the calls.
				glb_total_calls ++;
			}
			
			
			// Close files.
			
			topology_file.close();
			workload_file.close();


			
			// Main processing Loop.
			
			while( !pending_event.empty() )
			{
				// An event has stopped. Remove and destroy.
	
				if( !current_event.empty() &&
					( current_event.top()->end_time < pending_event.top()->begin_time ))
				{
					event	= current_event.top();
					current_event.pop();
					
					network.removeRoute( event->route );
					
					delete	event;
				}
				
				// An Event has started. Move to current.
				else
				{
					event	= pending_event.top();	
					pending_event.pop();
					
					// Dynamically switch between routing algorithms.
					
					switch( _NET_ROUTER )
					{
						case 0:
						case 1:
							router = &basic_router;
							break;
							
						case 2:
							router = &hop_router;
							break;
							
						case 3:
							router = &llc_router;
							break;
							
						case 4:
							router = &mfc_router;
							break;
						
						default:
							cerr << "Invalid router." << endl;
							return	-3;
							break;
					}
					
					event->route = (*router)( event->source, event->destination );
					
					
					// If the route is valid.
						
					if( event->route )
					{
						network.addRoute( event->route );
						current_event.push( event );
						
						glb_number_hops += event->route->size();
					}
					else
					{
						glb_number_dropped ++;
						delete event;
					}
				}
			}
			
			glb_total_calls --;
			
			cout	<< glb_total_calls;
			
			cout	<< "\t" << glb_total_calls - glb_number_dropped 
				<< " ( " << 100.0*(1.0-((float)glb_number_dropped/(float)glb_total_calls))
				<< " % ) ";
				
			cout	<< "\t" << glb_number_dropped 
				<< " ( " << 100.0*((float)glb_number_dropped/(float)glb_total_calls)
				<< " % ) ";
				
			cout	<< "\t" << (float)glb_number_hops /((float)glb_total_calls-glb_number_dropped);
			
			cout	<< "\t" << ((float)glb_total_delay/(float)(glb_total_calls-glb_number_dropped))
				<< endl;
		}
		else
		{
			cerr << "Couldnt open files." << endl;
			return -2;
		}
	}
	
	return 0;
}
