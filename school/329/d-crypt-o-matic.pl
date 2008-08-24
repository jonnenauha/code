#!/usr/bin/perl
#
#	A program with various features to Crypt-Analyze simple ciphers.
#
#			copyright: Ryan McDougall -- 2003
#
#=======================================================================================================================



#=======================================================================================================================
#	Constants and Defines

use Getopt::Std;

use constant PLAINALPHABET	=> "abcdefghijklmnopqrstuvwxyz";
use constant CIPHERALPHABET	=> "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
use constant NUMCOLUMNS		=> 480;
use constant MAXTABLE		=> 52;
use constant ENGLISH_PHI	=> 0.0661;
use constant RANDOM_PHI 	=> 0.0385;

my	$cipher_alphabet;
my	$plain_alphabet;



#=======================================================================================================================
#	Subroutines

#-----------------------------------------------------------------------------------------------------------------------
#	Converts characters to and from integer coded representation.

sub	char_code( $ )
{
	return	( ord( @_[0] ) - ord(A) );
}

sub	code_char( $ )
{
	return	chr( ( @_[0] % length($plain_alphabet) ) + ord(A) );
}


#-----------------------------------------------------------------------------------------------------------------------
#	Reads the ciphertext from a file into a list.

sub	read_ciphertext( $ )
{
	my	( $filename ) = @_;
	my	@ciphertext_list;

	if( -f $filename )
	{
		open( INFILE, "<$filename" ) || die "$!, couldnt open $filename";
		
		@ciphertext_list = <INFILE>;
		
		close INFILE;
	}

	return	( digest_ciphertext( \@ciphertext_list ) );
}



#-----------------------------------------------------------------------------------------------------------------------
#	Takes an array of text, read directly from a file, and process it into one big string.

sub	digest_ciphertext( \@ )
{
	my	( $ciphertext_list ) = @_;
	my	$ciphertext;
		
	# Join the list into one big uppercase string
	$ciphertext = join( '', @$ciphertext_list );
	$ciphertext = uc( $ciphertext );
	
	# Clean up spaces
	$re = $cipher_alphabet;
	$re =~ s/([\^\[\]])/\\$1/g;
	$ciphertext =~ s/([^$re])//g;

	return	( $ciphertext );
}
	


#-----------------------------------------------------------------------------------------------------------------------
#	Takes a string of text representing the cipher alphabet and creates a mapping to the plain alphabet.

sub	map_decipher_alphabet( $$ )
{
	my	( $plain_alphabet, $cipher_alphabet ) = @_;
	my	%decipher_map;
		
	# Format cipher alphabet properly
	$cipher_alphabet = uc( $cipher_alphabet );
	@cipher_alphabet = split( //, $cipher_alphabet );
	

	# Format plain alphabet properly	
	$plain_alphabet = lc( $plain_alphabet );
	@plain_alphabet	= split( //, $plain_alphabet );
	
	
	# Map the two alphabets together
	for( my $i=0; $i<length($cipher_alphabet); $i ++ )
	{
		$decipher_map{ $cipher_alphabet[ $i ] } = $plain_alphabet[ $i ];
	}

	return	( \%decipher_map );
}



#-----------------------------------------------------------------------------------------------------------------------
#	Takes a string of text representing the plain alphabet and creates a mapping to the cipher alphabet.

sub	map_encipher_alphabet( $$ )
{
	my	( $plain_alphabet, $cipher_alphabet ) = @_;
	my	%encipher_map;
		
	# Format cipher alphabet properly
	$cipher_alphabet = uc( $cipher_alphabet );
	@cipher_alphabet = split( //, $cipher_alphabet );
	

	# Format plain alphabet properly	
	$plain_alphabet = lc( $plain_alphabet );
	@plain_alphabet	= split( //, $plain_alphabet );
	

	# Map the two alphabets together
	for( my $i=0; $i<length($plain_alphabet); $i ++ )
	{
		$encipher_map{ $plain_alphabet[ $i ] } = $cipher_alphabet[ $i ];
	}

	return	( \%encipher_map );
}



#-----------------------------------------------------------------------------------------------------------------------
#	Calculates the frequency table

sub	calc_frequency( $$ )
{
	my	( $ciphertext, $graph_length ) = @_;
	my	%frequency_table;
	my	$ngraph;


	# Count ngraph frequencies
	for( my $i; $i < length($ciphertext); $i ++ )
	{
		$ngraph	= substr( $ciphertext, $i, $graph_length );
		$frequency_table{ $ngraph } ++;
	}
	
	return	( \%frequency_table );
}

#-----------------------------------------------------------------------------------------------------------------------
#

sub	calc_phi_statistic( $ )
{
	my	( $ciphertext ) = @_;
	my	$sum = 0;
	
	
	# Get monographic frequencies
	my	$frequency = calc_frequency( $ciphertext, 1 );
	
	# Convert to list for processing
	@key_list	= keys %$frequency;

	# Calculate
	foreach	$key	( @key_list )
	{
		$sum	+= ( $$frequency{ $key } * ( $$frequency{ $key } - 1 ) );
	}

	return	( $sum / ( length($ciphertext) * ( length($ciphertext) - 1 ) ) );
}



#-----------------------------------------------------------------------------------------------------------------------
#	Calculates the Index of Coincidence

sub	calc_coincidence( $$ )
{
	my	( $ciphertext, $offset_index ) = @_;
	my	$count = 0;


	# Shift the string by the given index.
	$ciphertext_header	= substr( $ciphertext, 0, $offset_index ); 
	$ciphertext_remainder	= substr( $ciphertext, $offset_index ); 
	$ciphertext_shifted	= $ciphertext_remainder.$ciphertext_header;
	
	# Convert the string to a list for computation
	@ciphertext		= split( //, $ciphertext );
	@ciphertext_shifted	= split( //, $ciphertext_shifted );

	# Count the coincidences
	foreach $i ( 0 .. $#ciphertext )
	{	
    		if( $ciphertext[ $i ] eq $ciphertext_shifted[ $i ] ) 
		{
			$count ++;
		}
	}

	return	( $count /  ($#ciphertext+1) );
}



#-----------------------------------------------------------------------------------------------------------------------
#	Prints the frequency table

sub	print_frequency( \% )
{
	my	( $frequency_table ) = @_;
	my	$total	= 0;
	my	$max	= 0;
	my	$table	= 0;
	
	# Sort the table by frequency
	@list	= sort	{ $$frequency_table{$b} <=> $$frequency_table{$a} }	keys %$frequency_table;
	
	# Count total number of elements and find the max
	foreach	$count	( values %$frequency_table )
	{
		$total	+= $count;
		
		if( $count > $max )
		{
			$max	= $count;
		}
	}
	
	# Print a pretty graph
	foreach	$key	( @list )
	{
		# Dont print over-large tables
		if( $table > MAXTABLE )
		{
			return;
		}
		else
		{
			$table ++;
		}
		
		my	$frequency	= $$frequency_table{$key} / $total;
		my	$length		= int( NUMCOLUMNS * $frequency );

		printf	("[ $key : %2f ]", $frequency*100.0 );
		
		for( my $i=0; $i < $length; $i ++ )
		{
			print '#';
		}
	
		print	"\n";
	}
}



#-----------------------------------------------------------------------------------------------------------------------
#	Partially or toally decrypts any cipher given an alphabet mapping from cipher alphabet to plain

sub	decrypt_substitution_cipher( $\% )
{
	my	( $ciphertext, $decipher_alphabet ) = @_;

	# Split into list for processing
	@ciphertext	= split( //, $ciphertext );
	
	# Perform the decrypt
	for( my $i=0; $i < length($ciphertext); $i ++ )
	{
		if( $$decipher_alphabet{ $ciphertext[ $i ] } )
		{
			$ciphertext[ $i ] = $$decipher_alphabet{ $ciphertext[ $i ] };
		}
	}
	
	# Reconsitute the string
	$deciphertext	= join( '', @ciphertext );

	return	( $deciphertext );
}


#-----------------------------------------------------------------------------------------------------------------------
#	Decrypts as Monoaplhabetic Shift Cipher

sub	decrypt_shift_cipher( $$$$ )
{
	my	( $ciphertext, $shift, $start, $offset ) = @_;
	my	$deciphertext;
	
	# Split into list for processing
	@ciphertext	= split( //, $ciphertext );
	
	# Perform the shift
	for( my $i=$start; $i < length($ciphertext); $i += $offset )
	{
		$ciphertext[ $i ] = lc( code_char( char_code($ciphertext[$i]) - $shift ) );
	}
	
	# Reconsitute the string
	$deciphertext	= join( '', @ciphertext );

	return	( $deciphertext );
}



#-----------------------------------------------------------------------------------------------------------------------
#	Decrypts as Vigenere Cipher

sub	decrypt_vigenere_cipher( $$ )
{
	my	( $ciphertext, $key ) = @_;
	my	$deciphertext = $ciphertext;
	my	$next = 0;

	# Process deciphered text
	$deciphertext =~ tr/A-Z//cd;

	# Split into list
	@key	= split( //, $key );
	
	# Perform the cyclic shifts
	foreach $shift ( @key )
	{
		$deciphertext	= decrypt_shift_cipher( $deciphertext, char_code($shift), $next, length($key) );
		$next ++;
	}
	
	return	( $deciphertext );
}



#-----------------------------------------------------------------------------------------------------------------------
#	Decrypts a Hill Cipher. NxN matrix is given as columns, then rows. Doesnt work.

sub	decrypt_hill_cipher( $\@ )
{
	my	( $ciphertext, $matrix ) = @_;
	my	$deciphertext	= "";
	my	$size		= 4;
	
	
	# Split into list for processing
	@ciphertext	= split( //, $ciphertext );
	
	for( my $i=0; $i < length($ciphertext); $i += $size )
	{
		$a	= lc( code_char( ( char_code($ciphertext[$i]) * $$matrix[0] ) + ( char_code($ciphertext[$i+1]) * $$matrix[1] ) ) );
		$b	= lc( code_char( ( char_code($ciphertext[$i]) * $$matrix[2] ) + ( char_code($ciphertext[$i+1]) * $$matrix[3] ) ) );
		$c	= lc( code_char( ( char_code($ciphertext[$i+2]) * $$matrix[0] ) + ( char_code($ciphertext[$i+3]) * $$matrix[1] ) ) );
		$d	= lc( code_char( ( char_code($ciphertext[$i+2]) * $$matrix[2] ) + ( char_code($ciphertext[$i+3]) * $$matrix[3] ) ) );
		
		$deciphertext	= $deciphertext . $a . $b . $c . $d;
	}
		
	
	return	( $deciphertext );
}



#=======================================================================================================================
#	Main entry point.

#-----------------------------------------------------------------------------------------------------------------------
#	Get command line options

my	%cmdline_options;	getopts( "f:i:k:p:c:m:d", \%cmdline_options);


#-----------------------------------------------------------------------------------------------------------------------
#	Main variables

my	$keyword;
my	$cipher_method;

my	$decipher_map;
my	$encipher_map;

my	$frequency_table;



#-----------------------------------------------------------------------------------------------------------------------
#	Build Alphabets and Mappings

if( $cmdline_options{'p'} || $cmdline_options{'c'} )
{
	$plain_alphabet = $cmdline_options{'p'};
	$cipher_alphabet = $cmdline_options{'c'};
}


# If no alphabets are supplied, use defaults

if( $plain_alphabet )
{}
else
{
	$plain_alphabet =  PLAINALPHABET;
}

if( $cipher_alphabet )
{}
else
{
	$cipher_alphabet =  CIPHERALPHABET;
}

# Build and display alphabets

$encipher_map	= map_encipher_alphabet( $plain_alphabet, $cipher_alphabet );
$decipher_map	= map_decipher_alphabet( $plain_alphabet, $cipher_alphabet );

print	"Plain Alphabet: $plain_alphabet\n";
print	"Cipher Alphabet: $cipher_alphabet\n";



#-----------------------------------------------------------------------------------------------------------------------
#	Read the Cipher Text file.

my	$ciphertext		= read_ciphertext( @ARGV[0] );



#	Perform desired analysis.
#-----------------------------------------------------------------------------------------------------------------------
#	Frequency Analysis

if( $cmdline_options{'f'} )
{
	print	"\nFrequency Analysis\n\n";
	
	$frequency_table = calc_frequency( $ciphertext, $cmdline_options{'f'} );
	print_frequency( %$frequency_table );
}


#-----------------------------------------------------------------------------------------------------------------------
#	Index of Coincidence

if( $cmdline_options{'i'} )
{
	printf	( "\nPhi Statistic: %2f\n", calc_phi_statistic( $ciphertext ) );
	print	"English: 0.0661\nRandom: 0.0385\n";

	print	"\nIndex of Coincidence\n\n";
	for( my $i=1; $i <= $cmdline_options{'i'}; $i ++ )
	{
		printf	( "[ $i : %2f ]\n", calc_coincidence( $ciphertext, $i ) );
	}
}


#-----------------------------------------------------------------------------------------------------------------------
#	Possble keyword.

if( $cmdline_options{'k'} )
{
	$keyword = $cmdline_options{'k'};
	
	print	"\nKeyword: $keyword\n";
}


#-----------------------------------------------------------------------------------------------------------------------
#	Document the cipher method.

if( $cmdline_options{'m'} )
{
	$cipher_method = $cmdline_options{'m'};
	print	"\nMethod: $cipher_method\n";
}


#-----------------------------------------------------------------------------------------------------------------------
#	Attempt to decrypt.
	
if( $cmdline_options{'d'} )
{
	print	"\nAttemping to decrypt ";
	
	if( $cipher_method eq 'vigenere' )
	{
		print	"$cipher_method cipher.\n\n";
		print	decrypt_vigenere_cipher( $ciphertext, $keyword );
		print	"\n";
	}
	elsif( $cipher_method eq 'shift' )
	{
		print	"$cipher_method cipher.\n\n";
		print	decrypt_shift_cipher( $ciphertext, $keyword, 0, 1 );
		print	"\n";
	}
	elsif( $cipher_method eq 'substitution' )
	{
		print	"$cipher_method cipher.\n\n";
		print	decrypt_substitution_cipher( $ciphertext, %$decipher_map );
		print	"\n";
	}
	elsif( $cipher_method eq 'hill' )
	{
		my	@matrix	= split( /,/, $keyword );
		
		print	"$cipher_method cipher.\n\n";
		print	decrypt_hill_cipher( $ciphertext, @matrix );
		print	"\n";
	}
	else
	{
		print	"$cipher_method. Method is unknown.\n";
	}
}
