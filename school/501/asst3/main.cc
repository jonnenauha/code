//
//		CPSC 501 -- Assignment 3
//
//	Neural Net
//
//				Copyright:	Ryan McDougall -- 2002
//
//--------------------------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>
#include "random.h"

using namespace std;



//================================================================================
// System Parameters

const int	NUMINPUTS	= 4;	// Number of input items
const int	NUMOUTPUTS	= 3;	// Number of output items
float 		LEARNRATE	= 0.25;	// Parameter of the Network
int		NUMTRAIN	= 5;	// Number of times same data is retrained
int		NUMLAYERS	= 2;	// Number of hidden layers in the Network
int		NUMNODES	= 24;	// Number of nodes in a hidden layer



//================================================================================
// System Function Parameters

typedef	vector<double>::iterator real_iter;
typedef	double (*ActivationFunction)( double );
typedef	double (*ActivationFunctionDerivative)( double );
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
double 	linear_activefunc( double x )
{	
	return x;
}
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
double	logistic_activefunc( double x )
{
	return ( 1.0/( 1.0 + exp( -1.0 * x ) ) );
}
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
double 	del_linear_activefunc( double x )
{	
	return 1.0;
}
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
double	del_logistic_activefunc( double x )
{
	double	a = logistic_activefunc( x );
	return ( a * ( 1.0 - a ) );
}



//================================================================================
// One layer of a Neural Network

class	NeuralLayer
{
//--------------------------------------------------------------------------------
//
	public:
		NeuralLayer( int number_nodes )
			: _num_nodes( number_nodes ), _num_outputs( number_nodes ),
			  _activefunc( &logistic_activefunc ),
			  _del_activefunc( &del_logistic_activefunc ),
			  _prev_layer( NULL ), _connected( false )
		{
			_matrix		= new double*[ _num_nodes ];
			_outputs	= new double[ _num_outputs ];
			_raw_outputs	= new double[ _num_outputs ];
		}
		
		~NeuralLayer()
		{
			delete [] _raw_outputs;
			delete [] _outputs;
			
			for( int i=0; i < _num_nodes; i++ )
			{
				delete [] _matrix[i];
			}
			
			delete [] _matrix;
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
		int	size()	const
		{
			return _num_outputs;
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
		double*	outputs()
		{
			return _outputs;
		}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
		void	inputs( double* input_data )
		{
			assert( _connected );

			_inputs		= input_data;
			_prev_layer	= NULL;
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
		void	setActivationFunction( ActivationFunction f )
		{	
			_activefunc = f;
		}
		
	
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
		void	connect( NeuralLayer* layer )
		{	
			_connected	= true;
			_prev_layer	= layer;
			_num_inputs	= layer->size();
			_num_weights	= _num_inputs;
			
			
			for( int i=0; i < _num_nodes; i++ )
			{
				_matrix[i] = new double[ _num_weights ];
				
				for( int j=0; j < _num_weights; j++ )
				{
					_matrix[i][j] = random_double( -0.5, 0.5 );
				}
			}
			
			cout 	<< "[NeuralLayer]: " << _num_nodes << " by " 
				<< _num_weights << " matrix created." << endl;
			
		}


		void	connect( int size )
		{
			_connected	= true;
			_prev_layer	= NULL;
			_num_inputs	= size;
			_num_weights	= _num_inputs;
			
			
			for( int i=0; i < _num_nodes; i++ )
			{			
				_matrix[i] = new double[ _num_weights ];
				
				for( int j=0; j < _num_weights; j++ )
				{
					_matrix[i][j] = random_double( -0.5, 0.5 );
				}			
			}
			
			cout 	<< "[NeuralLayer]: " << _num_nodes << " by " 
				<< _num_weights << " matrix created." << endl;
		}		


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
		const double*	compute()
		{
			if( _prev_layer != NULL )
			{
				_inputs = _prev_layer->compute();
			}
			else
			{}	// We are the First layer.
			
			assert( _inputs != NULL );
			
			
			// This is a matrix multiplication
			for( int i=0; i < _num_nodes; i++ )
			{
				_raw_outputs[i] = 0.0;
				
				for( int j=0; j < _num_weights; j++ )
				{
					_raw_outputs[i] += _matrix[i][j] * _inputs[j];
				}
				
				_outputs[i] = _activefunc( _raw_outputs[i] );
			}
			
			return _outputs;
		}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
		void		train( double* right_ans )
		{
			int node, weight;
			double* err_vector	= new double[ _num_nodes ];
			double* train_vector	= new double[ _num_inputs ];


			for( node=0; node < _num_nodes; node++ )
			{
				// This represents the amount of error for each node.
				err_vector[node] = ( right_ans[node] - _outputs[node] )
							* _del_activefunc( _raw_outputs[node] );


				// Adjust each node's weights by above.
				for( weight=0; weight < _num_inputs; weight++ )
				{
					_matrix[node][weight] += err_vector[node] * _inputs[weight]
									* LEARNRATE;
				}
			}


			// Compute a "training" vector which describes the amount
			// of error each previous node is "responsible" for, using
			// above error vector and the transpose of the _matrix.
			for( int j=0; j < _num_inputs; j++ )
			{
				train_vector[j] = 0.0;
				
				for( int i=0; i < _num_nodes; i++ )
				{
					train_vector[j] += err_vector[i] * _matrix[i][j];
				}
			}
			
			// Clean up
			delete [] err_vector;


			// Propagate the "responsiblity" backwards.
			_prev_layer->back_propagate( train_vector );
		}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//		
		void		back_propagate( double* propagate_vector )
		{
			int node, weight;
			double* err_vector	= new double[ _num_nodes ];
			double* train_vector 	= new double[ _num_inputs ];


			for( node=0; node < _num_nodes; node++ )
			{
				// This represents the amount of error for each node.
				err_vector[node] = _del_activefunc( _raw_outputs[node] ) 
							* propagate_vector[node];

				
				// Adjust each node's weights by above.
				for( weight=0; weight < _num_weights; weight++ )
				{
					_matrix[node][weight] += err_vector[node] * _inputs[weight]
									* LEARNRATE;
				}
			}
			
			
			// Compute a "training" vector which describes the amount
			// of error each previous is "responsible" for, using
			// above error vector and the transpose of the _matrix.
			for( int j=0; j < _num_weights; j++ )
			{
				train_vector[j] = 0.0;

				for( int i=0; i < _num_nodes; i++ )
				{
					train_vector[j] += err_vector[i] * _matrix[i][j];
				}
			}
			
			// Clean up
			delete [] err_vector;
			delete [] propagate_vector;
			

			// Propagate the "responsiblity" backwards.
			if( _prev_layer != NULL )
			{	
				_prev_layer->back_propagate( train_vector );	
			}
			else
			{
				// No more layers to propagate
				delete [] train_vector;
			}
		}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
		void 	printOutputs()
		{
			cout << "[NeuralLayer Output]: ";
			for( int i=0; i < _num_outputs; i++ )
			{
				cout << _outputs[i] << ", ";
			}
			cout << endl;
		}
		
		void	printMatrix()
		{
			cout << "[NeuralLayer Matrix]: " << endl;
			for( int i=0; i < _num_nodes; i++ )
			{
				cout << "| ";
				for( int j=0; j < _num_weights; j++ )
				{
					cout << _matrix[i][j] << ", ";
				}
				cout << "|" << endl;
			}
		}


//--------------------------------------------------------------------------------
//
	private:
		int				_num_nodes;
		int				_num_weights;	
		double**			_matrix;

		int				_num_inputs;
		const double*			_inputs;

		int				_num_outputs;
		double*				_raw_outputs;
		double*				_outputs;
		
		NeuralLayer*			_prev_layer;
		
		bool				_connected;

		ActivationFunction		_activefunc;
		ActivationFunctionDerivative	_del_activefunc;
};



//================================================================================
// Utility class for the properties of an Iris

class	IrisProperties
{
	public:	
		double		data_vector[4];
		int		type;


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//		
		void	print()
		{
			cout 	<< "[Iris]: " << type
				<< " " << data_vector[0] << " " << data_vector[1]
				<< " " << data_vector[2] << " " << data_vector[3]
				<< " " << endl;
		}	
};



//================================================================================
// Utility function for providing the correct answer to a Neural Network

void		set_type_vector( const IrisProperties& iris, double vector[3] )
{
	for( int i=0; i < 3; i++ )
	{
		if( iris.type == i )
		{
			vector[i] = 1.0;
		}
		else
		{
			vector[i] = 0.0;
		}
	}
}		



//================================================================================
// Utility function for providing the classification given an Nerual Net output

int		get_class( const double vector[3] )
{
	int max = 0;
	for( int i=0; i < 3; i++ )
	{
		if( vector[i] > vector[max] ) max = i;
	} 
	
	return max;
}



//================================================================================
// Utility function to read Iris data from a file

void	read_data( char* filename, vector<IrisProperties>* data )
{
	ifstream	file( filename );
	IrisProperties*	iris = NULL;
	
	if( file )
	{
		while( !file.eof() )
		{
			iris	= new IrisProperties;
			
			file >> iris->data_vector[0];
			file >> iris->data_vector[1];
			file >> iris->data_vector[2];
			file >> iris->data_vector[3];
			file >> iris->type;
			
			if(( iris->type >= 0 )&&( iris->type <= 2 ))
			{
				data->push_back( *iris );
			}
			else
			{}
		}
	}
	else
	{	cerr << "Cant open file." << endl;	}
}



//================================================================================

// Main function

void		main( int argc, char** argv )
{
	// Standard Input Parameters.
	
	cin >>	LEARNRATE;
	cin >>	NUMTRAIN;
	cin >>	NUMLAYERS;
	cin >>	NUMNODES;
	
	
	
	// Main Section.
	
	NeuralLayer		output_layer( NUMOUTPUTS );
	NeuralLayer		input_layer( NUMNODES );
	NeuralLayer*		next;
	NeuralLayer*		previous;
	
	vector<IrisProperties>			iris_data;
	
	int	num_type_correct[3]	= { 0,0,0 };
	int	num_type[3]		= { 0,0,0 };
	int	num_correct		= 0;
	int	num_total		= 0;
		
	int	given_type 		= 0;
	int	correct_type		= 0;
		
	double			correct_vector[3];
	
	
	
	// Read in data file and randomly shuffle 
	// [ with Uniform Distribution ]
	
	read_data( argv[1], &iris_data );
	random_shuffle( iris_data.begin(), iris_data.end() );
	
	
	
	// Connect up all Layers.
	
	input_layer.connect( NUMINPUTS );
	previous = &input_layer;
	
	for( int l=0; l < NUMLAYERS-2; l++ )
	{
		next = new NeuralLayer( NUMNODES );
		next->connect( previous );
		previous = next;
	}
	
	output_layer.connect( previous );
	
	
	
	// Train over the first part of Data.
	
	int partition_begin = int( iris_data.size() * 0.9 );


	for( int t=0; t < NUMTRAIN; t++ )
	{
		// Reset variables per Training
		
		num_type_correct[0] = num_type_correct[1] = num_type_correct[2] = 0;
		num_type[0] = num_type[1] = num_type[2] = 0;
		num_correct = num_total	= 0;
		given_type = correct_type = 0;
		
		
		// Compute value and Train on result
		
		for( int i=0; i < partition_begin; i++ )
		{
			// Compute
			
			input_layer.inputs( iris_data[i].data_vector );
			output_layer.compute();
			
			
			// Train
			
			set_type_vector( iris_data[i], correct_vector );
			output_layer.train( correct_vector );
			
			
			// Count Correct classifications for Statistics
			
			given_type 	= get_class( output_layer.outputs() );
			correct_type	= get_class( correct_vector );
			
			num_type[ correct_type ]++;
			num_total++;
			
			if( given_type == correct_type )
			{
				num_type_correct[ correct_type ]++;
				num_correct++;
			}
		}
		
		
		// Print Statistics
		
		cout << "Trial " << t << " --- " << endl;
		cout << "Total: " 
			<< 100.0 * num_correct / num_total
			<< "% correct." << endl;

		for( int i=0; i < 3; i++ )
		{
			cout << "Type " << i << ": " 
				<< 100.0 * num_type_correct[i] / num_type[i]
				<< "% correct." << endl;
		}
	}
	
	
	
	// Test our network on Remaining Data
	
	num_type_correct[0] = num_type_correct[1] = num_type_correct[2] = 0;
	num_type[0] = num_type[1] = num_type[2] = 0;
	num_correct = num_total	= 0;
	given_type = correct_type = 0;
			
	
	for( int i=partition_begin; i < iris_data.size(); i++ )
	{
		// Compute
		
		input_layer.inputs( iris_data[i].data_vector );
		output_layer.compute();


		// Count Correct classifications for Statistics
		
		given_type 	= get_class( output_layer.outputs() );
		correct_type	= get_class( correct_vector );
		
		//cout << "g: " << given_type << endl;
		cout << "c: " << correct_type << endl;
			
		num_type[ correct_type ]++;
		num_total++;
			
		if( given_type == correct_type )
		{
			num_type_correct[ correct_type ]++;
			num_correct++;
		}		
	}
	
	
	// Print Statistics
		
	cout << "Test " << " --- " << endl;
	cout << "Total: " 
		<< 100.0 * num_correct / num_total
		<< "% correct." << endl;

	for( int i=0; i < 3; i++ )
	{
		cout << "Type " << i << ": " 
			<< 100.0 * num_type_correct[i] / num_type[i]
			<< "% correct." << endl;
	}
}

