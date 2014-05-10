#ifndef ANY_HPP
#define ANY_HPP
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <memory>
#include <get_typename.hpp>
#include <member_variable.hpp>
#define GET_MEMBER_VARIABLE_HELPER( R, DATA, ELEMENT ) \
	if ( BOOST_PP_CAT( DATA, _typename ) == get_typename< ELEMENT >( )( ) ) \
	{ \
		k( member_variable< ELEMENT, TAG >( )( * static_cast< ELEMENT * >( BOOST_PP_CAT( DATA, _data ) ) ) ); \
	}
#define DESTRUCTOR_HELPER( R, DATA, ELEMENT ) \
	if ( BOOST_PP_CAT( DATA, _typename ) == get_typename< ELEMENT >( )( ) ) \
	{ \
		k( member_variable< ELEMENT, TAG >( )( * static_cast< ELEMENT * >( BOOST_PP_CAT( DATA, _data ) ) ) ); \
	}
#define DECLARE_ANY( NAME, NAME_SEQ ) \
struct BOOST_PP_CAT( any_, NAME ) \
{ \
	std::string BOOST_PP_CAT( NAME, _typename ); \
	void * BOOST_PP_CAT( NAME, _data ); \
	template< typename T > \
	BOOST_PP_CAT( any_, NAME )( T t ) : \
		BOOST_PP_CAT( NAME, _typename )( get_typename< T >( )( ) ), \
		BOOST_PP_CAT( NAME, _data )( new T( std::forward< T >( t ) ) ) \
	{ \
		if ( BOOST_PP_CAT( NAME, _typename ).empty( ) )\
		{ \
			delete static_cast< typename std::remove_reference< T >::type * >( BOOST_PP_CAT( NAME, _data ) ); \
			throw std::runtime_error( "cannot determine type of argument." ); \
		} \
	} \
	~BOOST_PP_CAT( any_, NAME )( ) { }	\
	template< typename TAG, typename CPS > \
	void get_member_variable( CPS & k ) \
	{ \
		BOOST_PP_SEQ_FOR_EACH( GET_MEMBER_VARIABLE_HELPER, NAME, NAME_SEQ )	\
	} \
};

#endif // ANY_HPP
