#ifndef REFLECTION_BASE_HPP
#define REFLECTION_BASE_HPP
#include <string>
#include "static_variable.hpp"
#include "member_variable.hpp"
#include "static_function.hpp"
#include "member_function.hpp"
#include "string_to_tag.hpp"
template< typename CRTP >
struct reflection_base
{
	template< typename TAG,  typename CPS >
	struct static_variable_type_helper
	{
		const CPS & k;
		template< typename T >
		void operator ( )( const tag< T > & ) const
		{	k( tag< typename ::static_variable_type< T, TAG >::type >( ) ); }
		static_variable_type_helper( const CPS & k ) : k( k ) { }
	};
	template< typename TAG, typename K >
	void static_variable_type( const K & k ) const
	{ k( tag< typename ::static_variable_type< CRTP, TAG >::type >( ) ); }
	template< typename TAG,  typename CPS >
	struct member_variable_type_helper
	{
		const CPS & k;
		template< typename T >
		void operator ( )( const tag< T > & ) const
		{	k( tag< typename ::member_variable_type< T, TAG >::type >( ) ); }
		member_variable_type_helper( const CPS & k ) : k( k ) { }
	};
	template< typename TAG, typename K >
	void member_variable_type( const K & k ) const
	{ k( tag< typename ::member_variable_type< CRTP, TAG >::type >( ) ); }
	struct has_member_variable_string_helper
	{
		bool & store_to;
		const CRTP * that;
		template< typename T >
		void operator ( )( const T & ) const
		{ store_to = that->template has_member_variable< T >( ); }
		has_member_variable_string_helper( const CRTP * that, bool & store_to ) : store_to( store_to ), that( that ) { }
	};
	bool has_member_variable( const std::string & str ) const
	{
		bool ret;
		string_to_tag( str, has_member_variable_string_helper( static_cast< const CRTP * >( this ), ret ) );
		return ret;
	}
	struct has_static_variable_string_helper
	{
		bool & store_to;
		const CRTP * that;
		template< typename T >
		void operator ( )( const T & ) const
		{ store_to = that->template has_static_variable< T >( ); }
		has_static_variable_string_helper( const CRTP * that, bool & store_to ) : store_to( store_to ), that( that ) { }
	};
	bool has_static_variable( const std::string tag ) const
	{
		bool ret;
		string_to_tag( tag, has_static_variable_string_helper( static_cast< const CRTP * >( this ), ret ) );
		return ret;
	}
	template< typename ... ARG >
	struct has_static_function_string_helper
	{
		bool & store_to;
		const CRTP * that;
		template< typename T >
		void operator ( )( const T & ) const
		{ store_to = that->has_static_function< T, ARG ... >( ); }
		has_static_function_string_helper( const CRTP * that, bool & store_to ) : store_to( store_to ), that( that ) { }
	};
	template< typename ... ARG >
	bool has_static_function( const std::string & tag ) const
	{
		bool ret;
		string_to_tag( tag, has_static_function_string_helper< ARG ... >( static_cast< const CRTP * >( this ), ret ) );
		return ret;
	}
	template< typename ... ARG >
	struct has_member_function_string_helper
	{
		bool & store_to;
		const CRTP * that;
		template< typename T >
		void operator ( )( const T & ) const
		{ store_to = that->template has_member_function< T, ARG ... >( ); }
		has_member_function_string_helper( const CRTP * that, bool & store_to ) : store_to( store_to ), that( that ) { }
	};
	template< typename CPS >
	struct get_static_variable_string_helper
	{
		const CPS & k;
		const CRTP * that;
		get_static_variable_string_helper( const CRTP * that, const CPS & k ) : k( k ), that( that ) { }
		template< typename TAG >
		void operator ( )( const TAG & ) const
		{ that->template get_static_variable< TAG, CPS >( k ); }
	};
	template< typename ... ARG >
	struct call_static_function_string_helper
	{
		const CRTP * that;
		std::tuple< typename std::add_pointer< const ARG >::type ... > data;
		call_static_function_string_helper( const CRTP * that, const ARG & ... a ) : that( that ), data( std::addressof( a ) ... ) { }
		template< typename TAG >
		void operator ( )( const TAG & ) const { func< 0, TAG >( ); }
		template< int i, typename T, typename ... AARG >
		typename std::enable_if< i == std::tuple_size< decltype( data ) >::value >::type
		func( const AARG & ... a ) const { that->call_static_function< T, ARG ... >( a ... ); }
		template< int i, typename T, typename ... AARG >
		typename std::enable_if< i != std::tuple_size< decltype( data ) >::value >::type
		func( const AARG & ... a ) const { func< i + 1, T >( a ..., * std::get< i >( data ) ); }
	};
	template< typename ... ARG >
	void call_static_function( const std::string & str, const ARG & ... r ) const
	{ string_to_tag( str, call_static_function_string_helper< ARG ... >( static_cast< const CRTP * >( this ), r ... ) ); }
	template< typename CPS >
	void get_static_variable( const std::string & tag, const CPS & k ) const
	{ string_to_tag( tag, get_static_variable_string_helper< CPS >( static_cast< const CRTP * >( this ), k ) ); }
	template< typename TAG, typename ... ARG >
	struct has_member_function_helper
	{
		bool & store_to;
		template< typename T >
		void operator ( )( const tag< T > & ) const
		{ store_to = ::has_member_function< T, TAG, ARG ... >::value; }
		has_member_function_helper( bool & store_to ) : store_to( store_to ) { }
	};
	template< typename ... ARG >
	bool has_member_function( const std::string & tag ) const
	{
		bool ret;
		string_to_tag( tag, has_member_function_string_helper< ARG ... >( static_cast< const CRTP * >( this ), ret ) );
		return ret;
	}
	template< typename CPS >
	struct get_member_variable_string_helper
	{
		const CPS & k;
		CRTP * that;
		get_member_variable_string_helper( CRTP * that, const CPS & k ) : k( k ), that( that ) { }
		template< typename T >
		void operator ( )( const T & ) const { that->template get_member_variable< T, CPS >( k ); }
	};
	template< typename CPS >
	void get_member_variable( const std::string & tag, CPS & k )
	{ string_to_tag( tag, get_member_variable_string_helper< CPS >( static_cast< CRTP * >( this ), k ) ); }
	template< typename CPS >
	struct static_variable_type_string_helper
	{
		const CPS & k;
		const CRTP * that;
		static_variable_type_string_helper( const CRTP * that, const CPS & k ) : k( k ), that( that ) { }
		template< typename T >
		void operator ( )( const T & ) const { that->template static_variable_type< T, CPS >( k ); }
	};
	template< typename K >
	void static_variable_type( const std::string & tag, const K & k ) const
	{ string_to_tag( tag, static_variable_type_string_helper< K >( static_cast< const CRTP * >( this ), k ) ); }
	template< typename ... ARG >
	struct static_function_return_type_string_delegate
	{
		const CRTP * that;
		std::string tag;
		template< typename T >
		struct inner
		{
			const CRTP * that;
			const T & t;
			template< typename TAG >
			void operator( )( const TAG & ) const { that->template static_function_return_type< TAG, ARG ... >( )( t ); }
			inner( const CRTP * that, const T & t ) : that( that ), t( t ) { }
		};
		template< typename T >
		void operator( )( const T & t ) const { string_to_tag( tag, inner< T >( that, t ) ); }
		static_function_return_type_string_delegate( const CRTP * that, const std::string & tag ) : that( that ), tag( tag ) { }
	};
	template< typename ... ARG >
	static_function_return_type_string_delegate< ARG ... > static_function_return_type( const std::string & tag ) const
	{ return static_function_return_type_string_delegate< ARG ... >( static_cast< const CRTP * >( this ), tag ); }
	template< typename ... ARG >
	struct member_function_return_type_string_delegate
	{
		const CRTP * that;
		std::string tag;
		template< typename K >
		struct inner
		{
			const CRTP * that;
			const K & t;
			template< typename TAG >
			void operator( )( const TAG & ) const { that->template member_function_return_type< TAG, ARG ... >( )( t ); }
			inner( const CRTP * that, const K & t ) : that( that ), t( t ) { }
		};
		template< typename T >
		void operator( )( const T & t ) const { string_to_tag( tag, inner< T >( that, t ) ); }
		member_function_return_type_string_delegate( const CRTP * that, const std::string & tag ) : that( that ), tag( tag ) { }
	};
	template< typename ... ARG >
	member_function_return_type_string_delegate< ARG ... > member_function_return_type( const std::string & tag ) const
	{ return member_function_return_type_string_delegate< ARG ... >( static_cast< const CRTP * >( this ), tag ); }
	template< typename ... ARG >
	void call_member_function( const std::string & tag, const ARG & ... r )
	{ string_to_tag( tag, call_member_function_string_helper< ARG ... >( static_cast< CRTP * >( this ), r ... ) ); }
	template< typename ... ARG >
	struct call_member_function_string_helper
	{
		CRTP * that;
		std::tuple< typename std::add_pointer< const ARG >::type ... > data;
		call_member_function_string_helper( CRTP * that, const ARG & ... a ) : that( that ), data( std::addressof( a ) ... ) { }
		template< typename TAG >
		void operator ( )( const TAG & ) const { func< 0, TAG >( ); }
		template< int i, typename T, typename ... AARG >
		typename std::enable_if< i == std::tuple_size< decltype( data ) >::value >::type
		func( const AARG & ... a ) const { that->call_member_function< T, ARG ... >( a ... ); }
		template< int i, typename T, typename ... AARG >
		typename std::enable_if< i != std::tuple_size< decltype( data ) >::value >::type
		func( const AARG & ... a ) const { func< i + 1, T >( a ..., * std::get< i >( data ) ); }
	};
	template< typename CPS >
	struct member_variable_type_string_helper
	{
		const CPS & k;
		const CRTP * that;
		member_variable_type_string_helper( const CRTP * that, const CPS & k ) : k( k ), that( that ) { }
		template< typename T >
		void operator ( )( const T & ) const { that->template member_variable_type< T, CPS >( k ); }
	};
	struct loop_tag{ };
	template< typename K >
	void member_variable_type( const std::string & tag, const K & k ) const
	{ string_to_tag( tag, member_variable_type_string_helper< K >( static_cast< const CRTP * >( this ), k ) ); }
	template< typename TAG, typename ... ARG >
	void call_member_function( const ARG & ... r ) { call_member_function_loop< TAG >( r ..., loop_tag( ) ); }
	template< typename TAG, typename T, typename ... ARG >
	void call_member_function_loop( const T & t, const ARG & ... r )
	{ call_member_function_loop< TAG >( r ..., t ); }
	template< typename TAG, typename T, typename ... ARG >
	void call_member_function_loop( const T & t, loop_tag, const ARG & ... r )
	{ call_member_function_inner< TAG >( t, r ... ); }
	template< typename TAG, typename K, typename ... ARG >
	typename
	std::enable_if
	<
		( ! std::is_same< typename member_function_return_type< CRTP, TAG, ARG ... >::type , void >::value ) &&
		::has_member_function< CRTP, TAG, ARG ... >::value
	>::type call_member_function_inner( const K & k, const ARG & ... arg )
	{ k( ::call_member_function< CRTP, TAG, ARG ... >( )( * static_cast< CRTP * >( this ), arg ... ) ); }
	template< typename TAG, typename K, typename ... ARG >
	typename
	std::enable_if
	<
		( std::is_same< typename member_function_return_type< CRTP, TAG, ARG ... >::type , void >::value ) &&
		::has_member_function< CRTP, TAG, ARG ... >::value
	>::type call_member_function_inner( const K & k, const ARG & ... arg )
	{
		::call_member_function< CRTP, TAG, ARG ... >( )( * static_cast< CRTP * >( this ), arg ... );
		k( );
	}
	template< typename TAG, typename ... ARG >
	void call_static_function( const ARG & ... r ) const { call_static_function_loop< TAG >( r ..., loop_tag( ) ); }
	template< typename TAG, typename T, typename ... ARG >
	void call_static_function_loop( const T & t, const ARG & ... r ) const
	{ call_static_function_loop< TAG >( r ..., t ); }
	template< typename TAG, typename T, typename ... ARG >
	void call_static_function_loop( const T & t, loop_tag, const ARG & ... r ) const
	{ call_static_function_inner< TAG >( t, r ... ); }
	template< typename TAG, typename K, typename ... ARG >
	typename std::enable_if
	<
		std::is_same< typename static_function_return_type< CRTP, TAG, ARG ... >::type, void >::value &&
		::has_static_function< CRTP, TAG, ARG ... >::value
	>::type call_static_function_inner( const K & k, const ARG & ... arg ) const
	{
		::call_static_function< CRTP, TAG, ARG ... >( )( arg ... );
		k( );
	}
	template< typename TAG, typename K, typename ... ARG >
	typename std::enable_if
	<
		( ! std::is_same< typename static_function_return_type< CRTP, TAG, ARG ... >::type, void >::value ) &&
		::has_static_function< CRTP, TAG, ARG ... >::value
	>::type call_static_function_inner( const K & k, const ARG & ... arg ) const
	{ k( ::call_static_function< CRTP, TAG, ARG ... >( )( arg ... ) ); }
	template< typename TAG, typename K, typename ... ARG >
	typename std::enable_if< ! ::has_static_function< CRTP, TAG, ARG ... >::value >::type
	call_static_function_inner( const K & k, ... ) const
	{ k( no_existence( ) ); }
	template< typename TAG, typename K, typename ... ARG >
	typename std::enable_if< ! ::has_member_function< CRTP, TAG, ARG ... >::value >::type
	call_member_function_inner( const K & k, ... ) const
	{ k( no_existence( ) ); }
	template< typename TAG, typename ... ARG >
	struct member_function_return_type_delegate
	{
		const CRTP * that;
		template< typename K >
		void operator( )( const K & k ) const { that->template member_function_return_type_inner< TAG, K, ARG ... >( k ); }
		member_function_return_type_delegate( const CRTP * that ) : that( that ) { }
	};
	template< typename TAG, typename ... ARG >
	member_function_return_type_delegate< TAG, ARG ... > member_function_return_type( ) const
	{ return member_function_return_type_delegate< TAG, ARG ... >( static_cast< const CRTP * >( this ) ); }
	template< typename TAG, typename K, typename ... ARG >
	void member_function_return_type_inner( const K & k ) const { k( tag< typename ::member_function_return_type< CRTP, TAG, ARG ... >::type >( ) ); }
	template< typename TAG, typename K, typename ... ARG >
	void static_function_return_type_inner( const K & k ) const { k( tag< typename ::static_function_return_type< CRTP, TAG, ARG ... >::type >( ) ); }
	template< typename TAG, typename ... ARG >
	struct static_function_return_type_delegate
	{
		const CRTP * that;
		template< typename K >
		void operator( )( const K & k ) const { that->template static_function_return_type_inner< TAG, K, ARG ... >( k ); }
		static_function_return_type_delegate( const CRTP * that ) : that( that ) { }
	};
	template< typename TAG, typename ... ARG >
	static_function_return_type_delegate< TAG, ARG ... > static_function_return_type( ) const
	{ return static_function_return_type_delegate< TAG, ARG ... >( static_cast< const CRTP * >( this ) ); }
	virtual ~reflection_base( ) { }
	template< typename TAG, typename CPS >
	void get_member_variable( const CPS & k )
	{ k( ::member_variable< CRTP, TAG >( )( static_cast< CRTP * >( this ) ) ); }
	template< typename TAG, typename CPS >
	void get_static_variable( const CPS & k ) const
	{ k( ::static_variable< CRTP, TAG >( )( ) ); }
	template< typename TAG >
	constexpr bool has_member_variable( ) const { return ::has_member_variable< CRTP, TAG >::value; }
	template< typename TAG >
	constexpr bool has_static_variable( ) const { return ::has_static_variable< CRTP, TAG >::value; }
	template< typename TAG, typename ... ARG >
	constexpr bool has_member_function( ) const { return ::has_member_function< CRTP, TAG, ARG ... >::value; }
	template< typename TAG, typename ... ARG >
	constexpr bool has_static_function( ) const { return ::has_static_function< CRTP, TAG, ARG ... >::value; }
};
#endif // REFLECTION_BASE_HPP
