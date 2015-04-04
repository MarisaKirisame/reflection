#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP
#include <vector>
#include <memory>
#include "declare.hpp"
#include "any.hpp"
#include "../cpp_common/expansion.hpp"
#include "object.hpp"
#include <cassert>
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#define EXAMPLE_NAME_SEQ (data)(cache)(func)(function)(foo)(bar)(test)(any_test)(evil)
DECLARE_NAMES( EXAMPLE_NAME_SEQ )
struct test : reflection_base< test >
{
    int data = 12450;
    double func( int ) { return 10; }
    double func( int, int ) { return 10; }
    static int function( ) { return 1; }
    static long cache;
    static decltype( & function ) bar( long, long )
    { return & function; }
    struct bad { auto operator & ( ) const { return & test::data; } };
    static bad evil;
};

DECLARE_ALL( EXAMPLE_NAME_SEQ )
test::bad test::evil = test::bad( );
long test::cache = 1230;
DECLARE_ANY( any_test, EXAMPLE_NAME_SEQ )
static_assert( has_member_function< test, tag< func >, long, long >::value );
static_assert( ! has_member_function< test, tag< func >, void * >::value );
static_assert( std::is_same<
                member_function_return_type< test, tag< func >, double >::type,
                double >::value );
static_assert( ! has_static_function< test, tag< func >, void  >::value );
static_assert( has_static_function< test, tag< function > >::value );
static_assert( ! has_static_function< test, tag< func > >::value );
static_assert( std::is_same< static_function_return_type< test, tag< function >, void >::type, int >::value );
static_assert( ! has_member_function< test, tag< function >, void >::value );
static_assert( ! has_static_function< test, tag< foo > >::value );
static_assert( has_static_variable< test, tag< cache > >::value );
static_assert( ! has_static_variable< test, tag< data > >::value );
static_assert( has_static_variable< test, tag< cache > >::value );
static_assert( std::is_same< static_variable_type< test, tag< cache > >::type, long >::value );
static_assert( has_static_function< test, tag< bar >, int, int >::value );
static_assert(
        std::is_same
        <
            decltype( call_static_function< test, tag< bar >, int, int >( )( 0, 1 )( ) ),
            int
        >::value );
static_assert(
        std::is_same
        <
            decltype( call_member_function< test, tag< func >, long >( )( std::declval< test >( ), ( 1 ) ) ),
            double
        >::value );
static_assert( has_class< test >::value );
static_assert( has_member_variable< test, tag< data > >::value );
static_assert( ! has_member_variable< test, tag< cache > >::value );
static_assert( std::is_same< member_variable_type< test, tag< data > >::type, int >::value );
static_assert( std::is_same< static_variable_type< test, tag< evil > >::type, test::bad >::value );
#include <iostream>
BOOST_AUTO_TEST_CASE( compiletime_reflection_test )
{
    BOOST_CHECK_EQUAL((static_variable< test, tag< cache > >( )( )), 1230);
    auto d = call_static_function< test, tag< bar >, int, int >( )( 0, 1 )( );
    BOOST_CHECK_EQUAL( d, 1 );
}
BOOST_AUTO_TEST_CASE( runtime_reflection_test )
{
    using common::make_expansion;
    auto f = [](...){ BOOST_CHECK( false ); };
    auto ii = make_expansion(
        [](int i){ std::cout << i << std::endl; },
        [](...){ std::cout << "Hello World" << std::endl; } );
    test t;
    invoke_all_member_variable(
        t,
        make_expansion( []( tag< data >, int i ){ std::cout << i << std::endl; }, [](...){} ) );
    t.get_member_variable( "data",
                            make_expansion( []( int i ){ BOOST_CHECK_EQUAL( i, 12450 ); }, f ) );
    t.get_static_variable( "cache", make_expansion( []( int i ){ BOOST_CHECK_EQUAL( i, 1230 ); }, f ) );
    BOOST_CHECK( t.has_member_variable( "data" ) );
    BOOST_CHECK( ! t.has_member_variable( "cache" ) );
    BOOST_CHECK( t.has_static_variable( "cache" ) );
    t.call_member_function( "func", 1, ii );
    t.call_static_function( "function", ii );
    t.call_member_function( "function", ii );
    BOOST_CHECK( ! t.has_static_function< int >( "func" ) );
    BOOST_CHECK( t.has_member_function< int >( "func" ) );
    t.member_variable_type(
        "data",
        make_expansion( [](tag<int>){ }, f ) );
    t.static_variable_type(
        "cache",
        make_expansion( [](tag<long>){ }, f ) );
    t.member_function_return_type< int >( "func" )(
        make_expansion( [](tag<double>){ }, f ) );
    t.static_function_return_type( "function" )(
        make_expansion( [](tag<int>){ }, f ) );
}
BOOST_AUTO_TEST_CASE( any_reflection_test )
{
    using common::make_expansion;
    auto f = [](...){ BOOST_CHECK( false ); };
    any_test tr( []( ){ test t; return t; }( ) );
    auto ii = make_expansion(
        [](int i){ std::cout << i << std::endl; },
        [](...){ std::cout << "Hello World" << std::endl; } );
    tr.member_variable_type(
        "data",
        make_expansion( [](tag<int>){ }, f ) );
    tr.get_member_variable( "data", ii );
    tr.get_static_variable( "cache", ii );
    BOOST_CHECK( tr.has_member_variable( "data" ) );
    BOOST_CHECK( ! tr.has_member_variable( "cache" ) );
    BOOST_CHECK( tr.has_static_variable( "cache" ) );
    tr.call_member_function( "func", 1, ii );
    tr.call_static_function( "function", ii );
    tr.call_member_function( "function", ii );
    BOOST_CHECK( ! tr.has_static_function< int >( "func" ) );
    BOOST_CHECK( tr.has_member_function< int >( "func" ) );
    tr.static_variable_type(
        "cache",
        make_expansion( [](tag<long>){ }, f ) );
    tr.member_function_return_type< tag< func >, int >( )(
        make_expansion( [](tag<double>){ }, f ) );
    tr.static_function_return_type< tag< function > >( )(
        make_expansion( [](tag<int>){ }, f ) );
}
BOOST_AUTO_TEST_CASE( object_reflection_test )
{
    auto f = [](...){ BOOST_CHECK( false ); };
    object< any_test > ob;
    any_test tr( []( ){ test t; return t; }( ) );
    ob.add_member_variable( "mem", tr );
    BOOST_CHECK( ob.has_member_variable( "mem" ) );
    BOOST_CHECK( ! ob.has_member_variable( "noexist" ) );
    ob.member_variable_type( "mem", common::make_expansion( []( const tag< test > & ){ }, f ) );
}
#endif //EXAMPLE_HPP
