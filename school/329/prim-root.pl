#!/usr/bin/perl

use constant INTSIZE => 32;

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
# Calculates Euler phi-value

sub	calc_euler_phi( $ )
{}



#-----------------------------------------------------------------------------------------------------------------------
# 

my	$result;
my	$root		= $ARGV[0];
my	$modulus 	= $ARGV[1];
my	$euler_phi	= $ARGV[2];
my	$is_prim_root	= 1;

for( my $i=1; $i < $euler_phi; $i ++ )
{
	$result	= calc_exp_mod( $root, $i, $modulus );
	print	"$root^$i mod$modulus = $result\n";
	
	if( $result == 1 )
	{
		$is_prim_root = 0;
		break;
	}
}

if( $is_prim_root )
{
	print	"$root is a primitive root mod $modulus, where phi($modulus)=$euler_phi.\n";
}
else
{
	print	"$root is NOT a primitive root mod $modulus, where phi($modulus)=$euler_phi.\n";
}
