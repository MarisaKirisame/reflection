#ifndef ANY_HPP
#define ANY_HPP
#include <boost/mpl/vector.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <memory>
#include <member_variable.hpp>
#include <static_variable.hpp>
#include <member_function.hpp>
#include <string_to_tag.hpp>
#include <reflection_base.hpp>
#define DECLARE_ANY( NAME, NAME_SEQ ) \
struct NAME : reflection_base< NAME > \
{	\
	using reflection_base::has_member_variable;	\
	using reflection_base::has_static_variable; \
	using reflection_base::has_static_function; \
	using reflection_base::has_member_function; \
	using reflection_base::get_member_variable; \
	using reflection_base::get_static_variable; \
	using reflection_base::call_static_function; \
	using reflection_base::static_variable_type; \
	using reflection_base::static_function_return_type; \
	using reflection_base::member_function_return_type; \
	using reflection_base::call_member_function; \
	using reflection_base::member_variable_type; \
	struct loop_tag{ }; \
	std::string any_typename; \
	template< typename T > \
	NAME( const T & t ) : \
		any_typename( tag< T >::name( ) ), \
		any_data( new any_internal_implement< typename std::remove_reference< T >::type >( t ) ) \
	{ \
		if ( any_typename.empty( ) )\
		{ \
			delete static_cast< typename std::remove_reference< T >::type * >( any_data->data ); \
			throw unable_to_determine_type( ); \
		} \
	} \
	template< typename T > \
	NAME( T && t ) : \
		any_typename( tag< typename std::remove_reference< T >::type >::name( ) ),\
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
	}; \
	any_internal * any_data; \
	~NAME( ) { delete any_data; }	\
	template< typename TAG, typename CPS > \
	struct get_member_variable_helper \
	{ \
		const CPS & k; \
		const NAME * that; \
		template< typename T > \
		void operator ( )( const tag< T > & ) const\
		{ k( member_variable< T, TAG >( )( static_cast< T * >( that->any_data->data ) ) ); } \
		get_member_variable_helper( const NAME * that, const CPS & k ) : k( k ), that( that ) { } \
	}; \
	template< typename TAG, typename CPS > \
	void get_member_variable( const CPS & k ) \
	{ string_to_tag( any_typename, get_member_variable_helper< TAG, CPS >( this, k ) ); } \
	template< typename TAG > \
	struct has_member_variable_helper \
	{ \
		bool & store_to; \
		template< typename T > \
		void operator ( )( const tag< T > & ) const\
		{ store_to = ::has_member_variable< T, TAG >::value; } \
		has_member_variable_helper( bool & store_to ) : store_to( store_to ) { } \
	}; \
	template< typename TAG > \
	bool has_member_variable( ) const \
	{ \
		bool ret; \
		string_to_tag( any_typename, has_member_variable_helper< TAG >( ret ) ); \
		return ret;\
	} \
	template< typename TAG > \
	struct has_static_variable_helper \
	{ \
		bool & store_to; \
		template< typename T > \
		void operator ( )( const tag< T > & ) const\
		{ store_to = ::has_static_variable< T, TAG >::value; } \
		has_static_variable_helper( bool & store_to ) : store_to( store_to ) { } \
	}; \
	template< typename TAG > \
	bool has_static_variable( ) const \
	{ \
		bool ret; \
		string_to_tag( any_typename, has_static_variable_helper< TAG >( ret ) ); \
		return ret; \
	} \
	template< typename TAG, typename CPS > \
	struct get_static_variable_helper \
	{ \
		const CPS & k; \
		template< typename T > \
		void operator ( )( const tag< T > & ) const  \
		{ k( static_variable< T, TAG >( )( ) ); } \
		get_static_variable_helper( const CPS & k ) : k( k ) { } \
	}; \
	template< typename TAG, typename CPS > \
	void get_static_variable( const CPS & k ) const \
	{ string_to_tag( any_typename, get_static_variable_helper< TAG, CPS >( k ) ); } \
	template< typename TAG, typename ... ARG > \
	void call_member_function( const ARG & ... r ) { call_member_function_loop< TAG >( r ..., loop_tag( ) ); } \
	template< typename TAG, typename T, typename ... ARG > \
	void call_member_function_loop( const T & t, const ARG & ... r ) \
	{ call_member_function_loop< TAG >( r ..., t ); } \
	template< typename TAG, typename T, typename ... ARG > \
	void call_member_function_loop( const T & t, loop_tag, const ARG & ... r ) \
	{ call_member_function_inner< TAG >( t, r ... ); } \
	template< typename TAG, typename K, typename ... ARG > \
	void call_member_function_inner( const K & k, const ARG & ... arg ) \
	{ string_to_tag( any_typename, call_member_function_delegate< TAG, K, ARG ... >( this, k, arg ... ) ); } \
	template< typename TTAG, typename KK, typename ... AARG > \
	struct call_member_function_delegate \
	{	\
		struct inner \
		{	\
			template< typename T, typename TAG, typename K, typename ... ARG > \
			static void function( \
				typename std::enable_if\
			< \
				std::is_same< typename member_function_return_type< T, TAG, ARG ... >::type, void >::value && \
				::has_member_function< T, TAG, ARG ... >::value,\
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
					::has_member_function< T, TAG, ARG ... >::value, \
					T * \
			>::type t, \
			const K & k, const ARG & ... arg ){	k( ::call_member_function< T, TAG, ARG ... >( )( * t, arg ... ) ); } \
			template< typename T, typename TAG, typename K, typename ... ARG > \
			static void function( void *, const K & k, ... ) { k( no_existence( ) ); } \
		}; \
		NAME * that; \
		const KK & k; \
		std::tuple< typename std::add_pointer< const AARG >::type ... > data; \
		call_member_function_delegate( NAME * that, const KK & k, const AARG & ... a ) : that( that ), k( k ), data( std::addressof( a ) ... ) { }\
		template< int i, typename T, typename ... ARG > \
		typename std::enable_if< i == std::tuple_size< decltype( data ) >::value >::type \
		func( const ARG & ... a ) const { inner::template function< T, TTAG, KK, AARG ... >( static_cast< T * >( that->any_data->data ), k, a ... ); } \
		template< int i, typename T, typename ... ARG > \
		typename std::enable_if< i != std::tuple_size< decltype( data ) >::value >::type \
		func( const ARG & ... a ) const { func< i + 1, T >( a ..., * std::get< i >( data ) ); } \
		template< typename T > \
		void operator( )( const tag< T > & ) const\
		{ func< 0, T >( ); } \
	}; \
	template< typename TAG, typename ... ARG > \
	void call_static_function( const ARG & ... r ) const { call_static_function_loop< TAG >( r ..., loop_tag( ) ); } \
	template< typename TAG, typename T, typename ... ARG > \
	void call_static_function_loop( const T & t, const ARG & ... r ) const \
	{ call_static_function_loop< TAG >( r ..., t ); } \
	template< typename TAG, typename T, typename ... ARG > \
	void call_static_function_loop( const T & t, loop_tag, const ARG & ... r ) const \
	{ call_static_function_inner< TAG >( t, r ... ); } \
	template< typename TAG, typename K, typename ... ARG > \
	void call_static_function_inner( const K & k, const ARG & ... arg ) const \
	{ string_to_tag( any_typename, call_static_function_delegate< TAG, K, ARG ... >( k, arg ... ) );	} \
	template< typename TTAG, typename KK, typename ... AARG > \
	struct call_static_function_delegate \
	{	\
		struct inner \
		{	\
			template< typename T, typename TAG, typename K, typename ... ARG > \
			static void function( \
				typename std::enable_if\
			< \
				std::is_same< typename static_function_return_type< T, TAG, ARG ... >::type, void >::value && \
				::has_member_function< T, TAG, ARG ... >::value,\
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
					::has_static_function< T, TAG, ARG ... >::value, \
					const K & \
			>::type k, \
			const ARG & ... arg ){	k( ::call_static_function< T, TAG, ARG ... >( )( arg ... ) ); } \
			template< typename T, typename TAG, typename K, typename ... ARG > \
			static void function( const K & k, ... ) { k( no_existence( ) ); } \
		}; \
		const KK & k; \
		std::tuple< typename std::add_pointer< const AARG >::type ... > data; \
		template< int i, typename T, typename ... ARG > \
		typename std::enable_if< i == std::tuple_size< decltype( data ) >::value >::type \
		func( const ARG & ... a ) const { inner::template function< T, TTAG, KK, AARG ... >( k, a ... ); } \
		template< int i, typename T, typename ... ARG > \
		typename std::enable_if< i != std::tuple_size< decltype( data ) >::value >::type \
		func( const ARG & ... a ) const { func< i + 1, T >( a ..., * std::get< i >( data ) ); } \
		template< typename T > \
		void operator( )( const tag< T > & ) const \
		{ func< 0, T >( ); } \
		call_static_function_delegate( const KK & k, const AARG & ... a ) : k( k ), data( std::addressof( a ) ... ) { } \
	}; \
	template< typename TAG, typename ... ARG > \
	struct has_static_function_helper \
	{ \
		bool & store_to; \
		template< typename T > \
		void operator ( )( const tag< T > & ) const\
		{ store_to = ::has_static_function< T, TAG, ARG ... >::value; } \
		has_static_function_helper( bool & store_to ) : store_to( store_to ) { } \
	}; \
	template< typename TAG, typename ... ARG > \
	bool has_static_function( ) \
	{ \
		bool ret; \
		string_to_tag( any_typename, has_static_function_helper< TAG, ARG ... >( ret ) ); \
		return ret; \
	} \
	template< typename TAG, typename ... ARG > \
	struct has_member_function_helper \
	{ \
		bool & store_to; \
		template< typename T > \
		void operator ( )( const tag< T > & ) const\
		{ store_to = ::has_member_function< T, TAG, ARG ... >::value; } \
		has_member_function_helper( bool & store_to ) : store_to( store_to ) { } \
	}; \
	template< typename TAG, typename ... ARG > \
	bool has_member_function( ) const \
	{ \
		bool ret; \
		string_to_tag( any_typename, has_member_function_helper< TAG, ARG ... >( ret ) ); \
		return ret; \
	} \
	template< typename TAG,  typename CPS > \
	struct member_variable_type_helper \
	{ \
		const CPS & k; \
		template< typename T > \
		void operator ( )( const tag< T > & ) const\
		{	k( tag< typename ::member_variable_type< T, TAG >::type >( ) ); } \
		member_variable_type_helper( const CPS & k ) : k( k ) { } \
	}; \
	template< typename TAG, typename K > \
	void member_variable_type( const K & k ) const \
	{ string_to_tag( any_typename, member_variable_type_helper< TAG, K >( k ) ); } \
	template< typename TAG,  typename CPS > \
	struct static_variable_type_helper \
	{ \
		const CPS & k; \
		template< typename T > \
		void operator ( )( const tag< T > & ) const\
		{	k( tag< typename ::static_variable_type< T, TAG >::type >( ) ); } \
		static_variable_type_helper( const CPS & k ) : k( k ) { } \
	}; \
	template< typename TAG, typename K > \
	void static_variable_type( const K & k ) const \
	{ string_to_tag( any_typename, static_variable_type_helper< TAG, K >( k ) ); } \
	template< typename TAG, typename ... ARG > \
	struct member_function_return_type_delegate \
	{ \
		NAME * that; \
		template< typename T > \
		void operator( )( const T & t ) { that->member_function_return_type_inner< TAG, T, ARG ... >( t ); } \
		member_function_return_type_delegate( NAME * that ) : that( that ) { } \
	}; \
	template< typename TAG,  typename CPS, typename ... ARG > \
	struct static_function_return_type_helper \
	{ \
		const CPS & k; \
		template< typename T > \
		void operator ( )( const tag< T > & ) const\
		{	k( ::tag< typename ::static_function_return_type< T, TAG, ARG ... >::type >( ) ); } \
		static_function_return_type_helper( const CPS & k ) : k( k ) { } \
	}; \
	template< typename TAG, typename K, typename ... ARG > \
	void static_function_return_type_inner( const K & k ) const \
	{ string_to_tag( any_typename, static_function_return_type_helper< TAG, K, ARG ... >( k ) ); } \
	template< typename TAG, typename ... ARG > \
	struct static_function_return_type_delegate \
	{ \
		NAME * that;\
		template< typename T > \
		void operator( )( const T & t ) { that->static_function_return_type_inner< TAG, T, ARG ... >( t ); } \
		static_function_return_type_delegate( NAME * that ) : that( that ) { } \
	}; \
	template< typename TAG, typename ... ARG > \
	static_function_return_type_delegate< TAG, ARG ... > static_function_return_type( ) const \
	{ return static_function_return_type_delegate< TAG, ARG ... >( this ); } \
	template< typename TAG,  typename CPS, typename ... ARG > \
	struct member_function_return_type_helper \
	{ \
		const CPS & k; \
		template< typename T > \
		void operator ( )( const tag< T > & ) const\
		{	k( ::tag< typename ::member_function_return_type< T, TAG, ARG ... >::type >( ) ); } \
		member_function_return_type_helper( const CPS & k ) : k( k ) { } \
	}; \
	template< typename TAG, typename K, typename ... ARG > \
	void member_function_return_type_inner( const K & k ) const \
	{ string_to_tag( any_typename, member_function_return_type_helper< TAG, K, ARG ... >( k ) ); } \
	template< typename TAG, typename ... ARG > \
	member_function_return_type_delegate< TAG, ARG ... > member_function_return_type( ) \
	{ return member_function_return_type_delegate< TAG, ARG ... >( this ); } \
};
#endif // ANY_HPP
