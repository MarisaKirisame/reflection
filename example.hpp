#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP
#include <memory>
#include "declare.hpp"
#include "any.hpp"
#include "../misc/misc.hpp"
#define EXAMPLE_NAME_SEQ (data)(cache)(func)(function)(foo)(bar)(test)
DECLARE_NAMES( EXAMPLE_NAME_SEQ )
struct test
{
	DECLARE_TYPE( test, EXAMPLE_NAME_SEQ );
	int data = 12450;
	double func( int ) const { return 10; }
	static int function( ) { return 1; }
	static int cache;
	static decltype( & function ) bar( long, long time ) { if ( time == 0 ) { } return & function; }
};
int test::cache = 0;
DECLARE_ANY( test, EXAMPLE_NAME_SEQ )
static_assert( has_member_function< test, func_tag, long >::value, "" );
static_assert( ! has_member_function< test, func_tag, void * >::value , "" );
static_assert( std::is_same< typename member_function_return_type< test, func_tag, double >::type, double >::value, "" );
static_assert( ! has_static_function< test, func_tag, void  >::value, "" );
static_assert( has_static_function< test, function_tag >::value, "" );
static_assert( ! has_static_function< test, func_tag >::value, "" );
static_assert( std::is_same< static_function_return_type< test, function_tag, void >::type, int >::value, "" );
static_assert( ! has_member_function< test, foo_tag, void >::value, "" );
static_assert( ! has_static_function< test, foo_tag >::value, "" );
static_assert( has_static_variable< test, cache_tag >::value, "" );
static_assert( ! has_static_variable< test, data_tag >::value, "" );
static_assert( has_static_variable< test, cache_tag >::value, "" );
static_assert( std::is_same< static_variable_type< test, cache_tag >::type, int >::value, "" );
static_assert( has_static_function< test, bar_tag, int, int >::value, "" );
static_assert(
		std::is_same
		<
			decltype( call_static_function< test, bar_tag, int, int >( )( 0, 1 )( ) ),
			int
		>::value, "" );
static_assert(
		std::is_same
		<
			decltype( call_member_function< test, func_tag, long >( )( std::declval< test >( ), ( 1 ) ) ),
			double
		>::value, "" );
static_assert( has_member_variable< test, data_tag >::value, "" );
static_assert( ! has_member_variable< test, cache_tag >::value, "" );
static_assert( std::is_same< member_variable_type< test, data_tag >::type, int >::value, "" );
#include <iostream>
void example( )
{
	test t;
	any_test tr( t );
	auto ii = misc::make_expansion( [](int i){ std::cout << i << std::endl; }, [](...){ std::cout << "Hello World" << std::endl; } );
	tr.get_member_variable< data_tag >( ii );
	tr.get_static_variable< cache_tag >( ii );
	assert( ! tr.has_member_variable< cache_tag >( ) );
	assert( tr.has_static_variable< cache_tag >( ) );
	tr.call_member_function< func_tag >( 1, ii );
	tr.call_static_function< function_tag >( ii );
	tr.call_member_function< function_tag >( ii );
}
#endif //EXAMPLE_HPP
