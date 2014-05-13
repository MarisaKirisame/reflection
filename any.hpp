#ifndef ANY_HPP
#define ANY_HPP
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <memory>
#include <get_typename.hpp>
#include <member_variable.hpp>
#include <static_variable.hpp>
#include <member_function.hpp>
#define GET_MEMBER_VARIABLE_HELPER( R, DATA, ELEMENT ) \
	if ( any_typename == get_typename< ELEMENT >( )( ) ) \
	{ \
		k( member_variable< ELEMENT, TAG >( )( * static_cast< ELEMENT * >( any_data->data ) ) ); \
		return;\
	}
#define HAS_MEMBER_VARIABLE_HELPER( R, DATA, ELEMENT ) \
	if ( any_typename == get_typename< ELEMENT >( )( ) ) \
	{ \
		return ::has_member_variable< ELEMENT, TAG >::value; \
	}
#define HAS_STATIC_VARIABLE_HELPER( R, DATA, ELEMENT ) \
	if ( any_typename == get_typename< ELEMENT >( )( ) ) \
	{ \
		return ::has_static_variable< ELEMENT, TAG >::value; \
	}
#define GET_STATIC_VARIABLE_HELPER( R, DATA, ELEMENT ) \
	if ( any_typename == get_typename< ELEMENT >( )( ) ) \
	{ \
		k( static_variable< ELEMENT, TAG >( )( ) ); \
		return;\
	}
#define CALL_MEMBER_FUNCTION_HELPER( R, DATA, ELEMENT ) \
	if ( any_typename == get_typename< ELEMENT >( )( ) )  \
	{ \
		call_member_function_delegate< ELEMENT, TAG, K, ARG ... >( )( static_cast< ELEMENT * >( any_data->data ), k, arg ... ); \
		return;\
	}
#define CALL_STATIC_FUNCTION_HELPER( R, DATA, ELEMENT ) \
	if ( any_typename == get_typename< ELEMENT >( )( ) )  \
	{ \
		call_static_function_delegate< ELEMENT, TAG, K, ARG ... >( )( k, arg ... ); \
		return;\
	}
#define HAS_STATIC_FUNCTION_HELPER( R, DATA, ELEMENT ) \
	if ( any_typename == get_typename< ELEMENT >( )( ) ) \
	{ \
		return ::has_static_function< ELEMENT, TAG, ARG ... >::value; \
	}
#define HAS_MEMBER_FUNCTION_HELPER( R, DATA, ELEMENT ) \
	if ( any_typename == get_typename< ELEMENT >( )( ) ) \
	{ \
		return ::has_member_function< ELEMENT, TAG, ARG ... >::value; \
	}
struct unable_to_determine_type : std::runtime_error
{
	unable_to_determine_type( ) : std::runtime_error( "Unable to determine the type." ) { }
};
#define DECLARE_ANY( NAME, NAME_SEQ ) \
struct BOOST_PP_CAT( any_, NAME ) \
{	\
	struct loop_tag{ };\
	std::string any_typename; \
	template< typename T > \
	BOOST_PP_CAT( any_, NAME )( const T & t ) : \
		any_typename( get_typename< T >( )( ) ), \
		any_data( new any_internal_implement< typename std::remove_reference< T >::type >( t ) ) \
	{ \
		if ( any_typename.empty( ) )\
		{ \
			delete static_cast< typename std::remove_reference< T >::type * >( any_data->data ); \
			throw unable_to_determine_type( ); \
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
				throw unable_to_determine_type( ); \
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
		throw unable_to_determine_type( ); \
	} \
	template< typename TAG > \
	bool has_member_variable( ) \
	{ \
		BOOST_PP_SEQ_FOR_EACH( HAS_MEMBER_VARIABLE_HELPER, NAME, NAME_SEQ )	\
		throw unable_to_determine_type( ); \
	} \
	template< typename TAG > \
	bool has_static_variable( ) \
	{ \
		BOOST_PP_SEQ_FOR_EACH( HAS_STATIC_VARIABLE_HELPER, NAME, NAME_SEQ )	\
		throw unable_to_determine_type( ); \
	} \
	template< typename TAG, typename CPS > \
	void get_static_variable( CPS & k ) \
	{ \
		BOOST_PP_SEQ_FOR_EACH( GET_STATIC_VARIABLE_HELPER, NAME, NAME_SEQ )	\
		throw unable_to_determine_type( ); \
	} \
	template< typename TAG, typename T, typename ... ARG > \
	void call_member_function( const T & t, const ARG & ... r ) { call_member_function_loop< TAG >( t, r ..., loop_tag( ) ); }\
	template< typename TAG, typename T, typename ... ARG > \
	void call_member_function_loop( const T & t, const ARG & ... r ) \
	{ call_member_function_loop< TAG >( r ..., t ); }\
	template< typename TAG, typename T, typename ... ARG > \
	void call_member_function_loop( const T & t, loop_tag, const ARG & ... r ) \
	{ call_member_function_inner< TAG >( t, r ... ); }\
	template< typename TAG, typename K, typename ... ARG > \
	void call_member_function_inner( const K & k, const ARG & ... arg ) \
	{\
		BOOST_PP_SEQ_FOR_EACH( CALL_MEMBER_FUNCTION_HELPER, NAME, NAME_SEQ )	\
		throw unable_to_determine_type( );\
	}\
	template< typename TT, typename TTAG, typename KK, typename ... AARG > \
	struct call_member_function_delegate \
	{	\
		struct inner \
		{	\
			template< typename T, typename TAG, typename K, typename ... ARG > \
			static void function( \
				typename std::enable_if\
			< \
				std::is_same< typename member_function_return_type< T, TAG, ARG ... >::type, void >::value && \
				has_member_function< T, TAG, ARG ... >::value,\
				T * \
			>::type t, \
			const K & k, const ARG & ... arg ) \
			{	\
				::call_member_function< T, TAG, ARG ... >( )( * t, arg ... );\
				k( ); \
			} \
			template< typename T, typename TAG, typename K, typename ... ARG > \
			static void function( \
				typename std::enable_if \
				< \
					( ! std::is_same< typename member_function_return_type< T, TAG, ARG ... >::type , void >::value ) && \
					has_member_function< T, TAG, ARG ... >::value, \
					T * \
			>::type t, \
			const K & k, const ARG & ... arg ){	k( ::call_member_function< T, TAG, ARG ... >( )( * t, arg ... ) ); } \
			template< typename T, typename TAG, typename K, typename ... ARG > \
			static void function( void *, const K & k, ... ) { k( no_existence( ) ); } \
		}; \
		void operator( )( TT * t, const KK & k, const AARG & ... arg ) \
		{\
			inner::function< TT, TTAG, KK, AARG ... >( t, k, arg ... );\
		}\
	};\
	template< typename TAG, typename T, typename ... ARG > \
	void call_static_function( const T & t, const ARG & ... r ) { call_static_function_loop< TAG >( t, r ..., loop_tag( ) ); }\
	template< typename TAG, typename T, typename ... ARG > \
	void call_static_function_loop( const T & t, const ARG & ... r ) \
	{ call_static_function_loop< TAG >( r ..., t ); }\
	template< typename TAG, typename T, typename ... ARG > \
	void call_static_function_loop( const T & t, loop_tag, const ARG & ... r ) \
	{ call_static_function_inner< TAG >( t, r ... ); }\
	template< typename TAG, typename K, typename ... ARG > \
	void call_static_function_inner( const K & k, const ARG & ... arg ) \
	{\
		BOOST_PP_SEQ_FOR_EACH( CALL_STATIC_FUNCTION_HELPER, NAME, NAME_SEQ )	\
		throw unable_to_determine_type( );\
	}\
	template< typename TT, typename TTAG, typename KK, typename ... AARG > \
	struct call_static_function_delegate \
	{	\
		struct inner \
		{	\
			template< typename T, typename TAG, typename K, typename ... ARG > \
			static void function( \
				typename std::enable_if\
			< \
				std::is_same< typename static_function_return_type< T, TAG, ARG ... >::type, void >::value && \
				has_member_function< T, TAG, ARG ... >::value,\
				const K &  \
			>::type k, \
			const ARG & ... arg ) \
			{	\
				::call_static_function< T, TAG, ARG ... >( )( arg ... );\
				k( ); \
			} \
			template< typename T, typename TAG, typename K, typename ... ARG > \
			static void function( \
				typename std::enable_if \
				< \
					( ! std::is_same< typename static_function_return_type< T, TAG, ARG ... >::type , void >::value ) && \
					has_static_function< T, TAG, ARG ... >::value, \
					const K & \
			>::type k, \
			const ARG & ... arg ){	k( ::call_static_function< T, TAG, ARG ... >( )( arg ... ) ); } \
			template< typename T, typename TAG, typename K, typename ... ARG > \
			static void function( const K & k, ... ) { k( no_existence( ) ); } \
		}; \
		void operator( )( const KK & k, const AARG & ... arg ) \
		{\
			inner::function< TT, TTAG, KK, AARG ... >( k, arg ... );\
		}\
	};\
	template< typename TAG, typename ... ARG > \
	bool has_static_function( ) \
	{ \
		BOOST_PP_SEQ_FOR_EACH( HAS_STATIC_FUNCTION_HELPER, NAME, NAME_SEQ )	\
		throw unable_to_determine_type( ); \
	} \
	template< typename TAG, typename ... ARG > \
	bool has_member_function( ) \
	{ \
		BOOST_PP_SEQ_FOR_EACH( HAS_MEMBER_FUNCTION_HELPER, NAME, NAME_SEQ )	\
		throw unable_to_determine_type( ); \
	} \
};
#endif // ANY_HPP
