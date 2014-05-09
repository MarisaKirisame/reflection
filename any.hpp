#ifndef ANY_HPP
#define ANY_HPP
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <memory>
#include <get_typename.hpp>
#include <member_variable.hpp>
#define GET_MEMBER_VARIABLE_HELPER( R, DATA, ELEMENT ) \
	if ( BOOST_PP_CAT( DATA, _typename ) == GET_TYPENAME( ELEMENT ) ) \
	{ \
		k( member_variable< ELEMENT, TAG >( )( * static_cast< ELEMENT * >( BOOST_PP_CAT( DATA, _data ) ) ) ); \
	}
#define DECLARE_ANY( NAME, NAME_SEQ ) \
struct BOOST_PP_CAT( any, BOOST_PP_CAT( _, NAME ) ) \
{ \
	std::string BOOST_PP_CAT( NAME, _typename ); \
	void * BOOST_PP_CAT( NAME, _data ); \
	template< typename T > \
	BOOST_PP_CAT( any, BOOST_PP_CAT( _, NAME ) )( const T & t ) : \
		BOOST_PP_CAT( NAME, _typename )( GET_TYPENAME( T ) ), \
		BOOST_PP_CAT( NAME, _data )( new T( t ) ) { } \
	template< typename T > \
	BOOST_PP_CAT( any, BOOST_PP_CAT( _, NAME ) )( typename std::remove_reference< T >::type && t ) : \
		BOOST_PP_CAT( NAME, _typename )( GET_TYPENAME( T ) ), \
		BOOST_PP_CAT( NAME, _data )( new T( std::move( t ) ) ) { } \
	template< typename TAG, typename CPS > \
	void get_member_variable( CPS & k ) \
	{ \
		BOOST_PP_SEQ_FOR_EACH( GET_MEMBER_VARIABLE_HELPER, NAME, NAME_SEQ )	\
	} \
};

#endif // ANY_HPP
