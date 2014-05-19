#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP
#include <memory>
#include "declare.hpp"
#include "any.hpp"
#include "../misc/misc.hpp"
#include "object.hpp"
#define EXAMPLE_NAME_SEQ (data)(cache)(func)(function)(foo)(bar)(test)
DECLARE_NAMES( EXAMPLE_NAME_SEQ )
struct test : reflection_base< test >
{
	int data = 12450;
	double func( int ) { return 10; }
	double func( int, int ) { return 10; }
	static int function( ) { return 1; }
	static long cache;
	static decltype( & function ) bar( long, long time ) { if ( time == 0 ) { } return & function; }
};
DECLARE_ALL( EXAMPLE_NAME_SEQ )
long test::cache = 1230;
DECLARE_ANY( any_test, EXAMPLE_NAME_SEQ )
static_assert( has_member_function< test, tag< func >, long, long >::value, "" );
static_assert( ! has_member_function< test, tag< func >, void * >::value , "" );
static_assert( std::is_same< member_function_return_type< test, tag< func >, double >::type, double >::value, "" );
static_assert( ! has_static_function< test, tag< func >, void  >::value, "" );
static_assert( has_static_function< test, tag< function > >::value, "" );
static_assert( ! has_static_function< test, tag< func > >::value, "" );
static_assert( std::is_same< static_function_return_type< test, tag< function >, void >::type, int >::value, "" );
static_assert( ! has_member_function< test, tag< function >, void >::value, "" );
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
template< typename T, typename K >
void invoke_all_member_variable( T & t, const K & k )
{
	helper< T >::template invoke_all_member_variable< T, K >( & t, k );
}
void example( )
{
	test t;
	invoke_all_member_variable( t, misc::make_expansion( []( tag< data >, int i ){ std::cout << i << std::endl; }, [](...){} ) );
	any_test tr( t );
	auto ii = misc::make_expansion( [](int i){ std::cout << i << std::endl; }, [](...){ std::cout << "Hello World" << std::endl; } );
	tr.get_member_variable( "data", ii );
	t.get_member_variable( "data", ii );
	tr.get_static_variable( "cache", ii );
	t.get_static_variable( "cache", ii );
	assert( tr.has_member_variable( "data" ) );
	assert( t.has_member_variable( "data" ) );
	assert( ! tr.has_member_variable( "cache" ) );
	assert( ! t.has_member_variable( "cache" ) );
	assert( tr.has_static_variable( "cache" ) );
	assert( t.has_static_variable( "cache" ) );
	tr.call_member_function( "func", 1, ii );
	t.call_member_function( "func", 1, ii );
	tr.call_static_function( "function", ii );
	t.call_static_function( "function", ii );
	tr.call_member_function( "function", ii );
	t.call_member_function( "function", ii );
	assert( ! tr.has_static_function< int >( "func" ) );
	assert( ! t.has_static_function< int >( "func" ) );
	assert( tr.has_member_function< int >( "func" ) );
	assert( t.has_member_function< int >( "func" ) );
	tr.member_variable_type( "data", misc::make_expansion( [](tag<int>){ std::cout << "Test pass" << std::endl; },[](...){} ) );
	t.member_variable_type( "data", misc::make_expansion( [](tag<int>){ std::cout << "Test pass" << std::endl; },[](...){} ) );
	tr.static_variable_type( "cache", misc::make_expansion( [](tag<long>){ std::cout << "Test pass" << std::endl; },[](...){} ) );
	t.static_variable_type( "cache", misc::make_expansion( [](tag<long>){ std::cout << "Test pass" << std::endl; },[](...){} ) );
//	tr.member_function_return_type< tag< func >, int >( )(
//				misc::make_expansion( [](tag<double>){ std::cout << "Test pass" << std::endl; },[](...){} ) );
//	tr.static_function_return_type< tag< function > >( )(
//				misc::make_expansion( [](tag<int>){ std::cout << "Test pass" << std::endl; },[](...){} ) );
	t.member_function_return_type< int >( "func" )(
				misc::make_expansion( [](tag<double>){ std::cout << "Test pass" << std::endl; },[](...){} ) );
	t.static_function_return_type( "function" )(
				misc::make_expansion( [](tag<int>){ std::cout << "Test pass" << std::endl; },[](...){} ) );
	object< any_test > ob;
}
#endif //EXAMPLE_HPP
