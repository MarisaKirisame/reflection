#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP
#include <memory>
#include "declare.hpp"
#include "any.hpp"
#include "../misc/misc.hpp"
#include "object.hpp"
#define EXAMPLE_NAME_SEQ (data)(cache)(func)(function)(foo)(bar)(test)
DECLARE_NAMES( EXAMPLE_NAME_SEQ )
struct test
{
	int data = 12450;
	double func( int ) const { return 10; }
	static int function( ) { return 1; }
	static long cache;
	static decltype( & function ) bar( long, long time ) { if ( time == 0 ) { } return & function; }
};
template< >
struct helper< test >
{
	DECLARE_ALL_POSSIBLE_STATIC_VARIABLE( EXAMPLE_NAME_SEQ )
	DECLARE_ALL_POSSIBLE_MEMBER_VARIABLE( EXAMPLE_NAME_SEQ )
	DECLARE_ALL_POSSIBLE_STATIC_FUNCTION( EXAMPLE_NAME_SEQ )
	DECLARE_ALL_POSSIBLE_MEMBER_FUNCTION( EXAMPLE_NAME_SEQ )
};
DECLARE_INTERFACE( test )
long test::cache = 1230;
DECLARE_ANY( any_test, EXAMPLE_NAME_SEQ )
static_assert( has_member_function< test, tag< func >, long >::value, "" );
static_assert( ! has_member_function< test, tag< func >, void * >::value , "" );
static_assert( std::is_same< member_function_return_type< test, tag< func >, double >::type, double >::value, "" );
static_assert( ! has_static_function< test, tag< func >, void  >::value, "" );
static_assert( has_static_function< test, tag< function > >::value, "" );
static_assert( ! has_static_function< test, tag< func > >::value, "" );
static_assert( std::is_same< static_function_return_type< test, tag< function >, void >::type, int >::value, "" );
static_assert( ! has_member_function< test, tag< foo >, void >::value, "" );
static_assert( ! has_static_function< test, tag< foo > >::value, "" );
static_assert( has_static_variable< test, tag< cache > >::value, "" );
static_assert( ! has_static_variable< test, tag< data > >::value, "" );
static_assert( has_static_variable< test, tag< cache > >::value, "" );
static_assert( std::is_same< static_variable_type< test, tag< cache > >::type, long >::value, "" );
static_assert( has_static_function< test, tag< bar >, int, int >::value, "" );
static_assert(
		std::is_same
		<
			decltype( call_static_function< test, tag< bar >, int, int >( )( 0, 1 )( ) ),
			int
		>::value, "" );
static_assert(
		std::is_same
		<
			decltype( call_member_function< test, tag< func >, long >( )( std::declval< test >( ), ( 1 ) ) ),
			double
		>::value, "" );
static_assert( has_class< test >::value, "" );
static_assert( has_member_variable< test, tag< data > >::value, "" );
static_assert( ! has_member_variable< test, tag< cache > >::value, "" );
static_assert( std::is_same< member_variable_type< test, tag< data > >::type, int >::value, "" );
#include <iostream>
void example( )
{
	test t;
	//t.invoke_all_member_variable( misc::make_expansion( []( tag< data >, int i ){ std::cout << i; }, [](...){} ) );
	any_test tr( t );
	auto ii = misc::make_expansion( [](int i){ std::cout << i << std::endl; }, [](...){ std::cout << "Hello World" << std::endl; } );
	tr.get_member_variable< tag< data > >( ii );
	tr.get_static_variable< tag< cache > >( ii );
	assert( tr.has_member_variable< tag< data > >( ) );
	assert( ! tr.has_member_variable< tag< cache > >( ) );
	assert( tr.has_static_variable< tag< cache > >( ) );
	tr.call_member_function< tag< func > >( 1, ii );
	tr.call_static_function< tag< function > >( ii );
	tr.call_member_function< tag< function > >( ii );
	assert( ( ! tr.has_static_function< tag< func >, int >( ) ) );
	assert( ( tr.has_member_function< tag< func >, int >( ) ) );
	tr.member_variable_type< tag< data > >( misc::make_expansion( [](tag<int>){ std::cout << "Test pass" << std::endl; },[](...){} ) );
	tr.static_variable_type< tag< cache > >( misc::make_expansion( [](tag<long>){ std::cout << "Test pass" << std::endl; },[](...){} ) );
	tr.member_function_return_type< tag< func >, int >( )(
				misc::make_expansion( [](tag<double>){ std::cout << "Test pass" << std::endl; },[](...){} ) );
	tr.static_function_return_type< tag< function > >( )(
				misc::make_expansion( [](tag<int>){ std::cout << "Test pass" << std::endl; },[](...){} ) );
	object< any_test > ob;
}
#endif //EXAMPLE_HPP
