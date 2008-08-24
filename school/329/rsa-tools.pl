#!/usr/bin/perl
#
#	A program with various features to Crypt-Analyze simple ciphers.
#
#			copyright:	Ryan McDougall -- 2003
#					Devon Smibert
#					Josh Thorp
#
#=======================================================================================================================



#=======================================================================================================================
#	Constants and Defines

use Getopt::Std;

use integer;
use constant INTSIZE => 32;



#=======================================================================================================================
#	Subroutines

#-----------------------------------------------------------------------------------------------------------------------
# Computes the Extended Euclidean Algorithm. Returns a vector representing the result.

sub	extended_euclid( $$ )
{
	my	( $p, $q ) = @_;
	
	# Build the representative matrix in three vectors
	my	@top	= ( 1, 0, $p );
	my	@bottom	= ( 0, 1, $q );
	my	@result = ( 0, 0 , 1 );
	my	$quotient;
	
	until( $result[2] == 0 )
	{
		# Find how many times "bottom" goes into "top"
		$quotient = int( $top[2] / $bottom[2] );
		
		# Reduce the "top" vector
		$result[0] = $top[0] - $quotient * $bottom[0];
		$result[1] = $top[1] - $quotient * $bottom[1];
		$result[2] = $top[2] - $quotient * $bottom[2];
		
		# Move the vectors "down"
		@top	= @bottom;
		@bottom	= @result;
	}
	
	# Our result is the last non-zero vector
	@result = @top;
	
	return	\@result;
}



#-----------------------------------------------------------------------------------------------------------------------
# Use extended Euclidean algorithm to find the greatest common divisor.

sub	gcd( $$ )
{
	my	$result	= extended_euclid( $_[0], $_[1] );
	
	return	$$result[2];
}



#-----------------------------------------------------------------------------------------------------------------------
# Calculates repeated squares. ex: x^(2^y)

sub	calc_repeat_squares( $$$ )
{
	my	( $base, $repetitions, $modulus ) = @_;
	my	$result = $base;
	
	for( my $i=0; $i < $repetitions; $i ++ )
	{
		$result *= $result;
		$result %= $modulus;
	}
	
	return	$result;
}



#-----------------------------------------------------------------------------------------------------------------------
# Calculates exponents with a modulus. Method is Repeated Square and Multiply.

sub	calc_exp_mod( $$$ )
{
	my	( $base, $exponent, $modulus ) = @_;
	my	$result = 1;
	
	for( my $i=0; $i < INTSIZE; $i ++ )
	{
		if( ( $exponent >> $i ) & 1 )
		{
			$result	= $result * calc_repeat_squares( $base, $i, $modulus );
			$result %= $modulus;
		}
	}
	
	return	$result;
}



#-----------------------------------------------------------------------------------------------------------------------
# Function that provides the Pollard-rho algorithm with "random" numbers.

sub	pollard_rho_function( $ )
{
	return	( $_[0] * $_[0] + 1 );
}



#-----------------------------------------------------------------------------------------------------------------------
# Pollard-rho algorithm for factoring large numbers. u ~ Xi and v ~ X2i

sub	factor_pollard_rho( $ )
{
	my	( $number ) = @_;
	my	$seed	= int( rand( $number ) );	# Seed value for randomization
	my	$bound	= 10 * sqrt( $number );		# Provide and expected bound on the search
	my	$factor;
	
	# Initialize the start values.
	my	$u = $seed;
	my	$v = pollard_rho_function( $u ) % $number;
	
	# Perform the optimized search.
	for( my $i=0; $i < $bound; $i ++ )
	{
		$factor = gcd( $v - $u, $number );
		last if(( $factor > 1 )&&( $factor < $number ));
		
		$u = pollard_rho_function( $u ) % $number;
		$v = pollard_rho_function( pollard_rho_function( $v ) ) % $number;
	}
	
	return	$factor;
}



#=======================================================================================================================
#	Main entry point.

$result = calc_exp_mod( $ARGV[0], $ARGV[1], $ARGV[2] );
print "$result\n";

