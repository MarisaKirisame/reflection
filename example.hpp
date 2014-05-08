#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP
#include "declare.hpp"
#define EXAMPLE_NAME_SEQ (data)(cache)(func)(function)(foo)(bar)(test)
DECLARE_NAMES( EXAMPLE_NAME_SEQ )
struct test
{
	DECLARE_TYPE( test, EXAMPLE_NAME_SEQ );
	int data = 12450;
	double func( int ) { return 0; }
	static int function( ) { return 1; }
	static int cache;
	static decltype( & function ) bar( long, long time ) { if ( time == 0 ) { } return & function; }
};
static_assert( HAS_MEMBER_FUNCTION( test, func, ( long ) ), "" );
static_assert( ! HAS_MEMBER_FUNCTION( test, func, ( void * ) ), "" );
static_assert( std::is_same< MEMBER_FUNCTION_RETURN_TYPE( test, func, ( double ) ), double >::value, "" );
static_assert( ! HAS_STATIC_FUNCTION( test, func, ( void ) ), "" );
static_assert( HAS_STATIC_FUNCTION( test, function, ( ) ), "" );
static_assert( ! HAS_STATIC_FUNCTION( test, func, ( ) ), "" );
static_assert( std::is_same< STATIC_FUNCTION_RETURN_TYPE( test, function, ( void ) ), int >::value, "" );
static_assert( ! HAS_MEMBER_FUNCTION( test, foo, ( void ) ), "" );
static_assert( ! HAS_STATIC_FUNCTION( test, foo, ( ) ), "" );
static_assert( HAS_STATIC_VARIABLE( test, cache ), "" );
static_assert( ! HAS_STATIC_VARIABLE( test, data ), "" );
static_assert( HAS_STATIC_VARIABLE( test, cache ), "" );
static_assert( std::is_same< STATIC_VARIABLE_TYPE( test, cache ), int >::value, "" );
static_assert( HAS_STATIC_FUNCTION( test, bar, ( int, int ) ), "" );
static_assert(
		std::is_same
		<
			decltype( CALL_STATIC_FUNCTION( test, bar, ( 0, 1 ) )( ) ),
			int
		>::value, "" );
static_assert(
		std::is_same
		<
			decltype( CALL_MEMBER_FUNCTION( misc::construct< test >( ), func, ( 1 ) ) ),
			double
		>::value, "" );
static_assert( HAS_MEMBER_VARIABLE( test, data ), "" );
static_assert( ! HAS_MEMBER_VARIABLE( test, cache ), "" );
static_assert( std::is_same< MEMBER_VARIABLE_TYPE( test, data ), int >::value, "" );
#endif //EXAMPLE_HPP
