#ifndef ANY_HPP
#define ANY_HPP
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <memory>
#include <get_typename.hpp>
#include <member_variable.hpp>
#define GET_MEMBER_VARIABLE_HELPER( R, DATA, ELEMENT ) \
	if ( any_typename == get_typename< ELEMENT >( )( ) ) \
	{ \
		k( member_variable< ELEMENT, TAG >( )( * static_cast< ELEMENT * >( any_data->data ) ) ); \
		return;\
	}
#define DECLARE_ANY( NAME, NAME_SEQ ) \
struct BOOST_PP_CAT( any_, NAME ) \
{ \
	std::string any_typename; \
	template< typename T > \
	BOOST_PP_CAT( any_, NAME )( const T & t ) : \
		any_typename( get_typename< T >( )( ) ), \
		any_data( new any_internal_implement< typename std::remove_reference< T >::type >( t ) ) \
	{ \
		if ( any_typename.empty( ) )\
		{ \
			delete static_cast< typename std::remove_reference< T >::type * >( any_data->data ); \
			throw std::runtime_error( "cannot determine type of argument." ); \
		} \
	} \
	template< typename T > \
	BOOST_PP_CAT( any_, NAME )( T && t ) : \
		any_typename( get_typename< T >( )( ) ),\
		any_data( new any_internal_implement< typename std::remove_reference< T >::type >( t ) ) \
		{ \
			if ( any_typename.empty( ) )\
			{ \
				delete static_cast< typename std::remove_reference< T >::type * >( any_data->data ); \
				throw std::runtime_error( "cannot determine type of argument." ); \
			} \
		} \
	struct any_internal \
	{ \
		void * data;\
		virtual ~any_internal( ) { } \
		any_internal( void * d ) : data( d ) { }\
	}; \
	template< typename T > \
	struct any_internal_implement : any_internal \
	{	\
		any_internal_implement( T const & t ) : any_internal( new T( t ) ) { }\
		any_internal_implement( T && t ) : any_internal( new T( std::move< T >( t ) ) ) { } \
		~any_internal_implement( ) \
		{ delete static_cast< T * >( data ); }\
	};\
	any_internal * any_data; \
	~BOOST_PP_CAT( any_, NAME )( ) { delete any_data; }	\
	template< typename TAG, typename CPS > \
	void get_member_variable( CPS & k ) \
	{ \
		BOOST_PP_SEQ_FOR_EACH( GET_MEMBER_VARIABLE_HELPER, NAME, NAME_SEQ )	\
	} \
};

#endif // ANY_HPP
