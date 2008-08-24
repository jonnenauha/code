//
//
//

#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

const int MAXLINE = 80;

int	main( int argc, char** argv )
{
	if( argc == 2 )
	{
		char		type;
		char		string[MAXLINE];
		float		vertex;
		int		face;
		
		ifstream	infile( argv[1] );
		
		while( !infile.eof() )
		{
			infile >> type;
					
			switch( type )
			{
				case 'v':
					do
					{
						infile >> vertex;
					}
					while( isprint( infile.peek() ) );
					break;
					
				case 'f':
					do
					{
						infile >> face;
					}
					while( isprint( infile.peek() ) );
					break;
					
				case 'g':
					infile.ignore( MAXLINE, '\n' );
					break;
				
				case '#':
					infile.ignore( MAXLINE, '\n' );
					break;
					
				default:
					infile.ignore( MAXLINE, '\n' );
					break;
			}
		}
	}
	else
	{
		cout << "Couldnt Open File." << endl;
	}
	return	0;
}
