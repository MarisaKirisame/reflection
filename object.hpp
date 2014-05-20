#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <any.hpp>
#include <map>
#include <string>
#include "member_variable.hpp"
template< typename ANY, typename CRTP >
struct object_base : reflection_base< object_base< ANY, CRTP > >
{
	std::map< std::string, ANY > member_variable;
	static std::map< std::string, ANY > & static_variable( )
	{
		static std::map< std::string, ANY > ret;
		return ret;
	}
	template< typename TAG >
	bool has_member_variable( ) const
	{
		return
				::has_member_variable< CRTP, TAG >::value ||
				( member_variable.count( TAG::name( ) ) > 0 );
	}
	template< typename TAG >
	bool has_static_variable( ) const
	{
		return
				::has_static_variable< CRTP, TAG >::value ||
				( static_variable( ).count( TAG::name( ) ) > 0 );
	}
	struct has_member_variable_helper
	{
		const CRTP * that;
		bool & ret;
		template< typename TAG >
		void operator ( )( const TAG & ) const { ret = that->template has_member_variable< TAG >( ); }
		has_member_variable_helper( const CRTP * that, bool & ret ) : that( that ), ret( ret ) { }
	};
	struct has_static_variable_helper
	{
		const CRTP * that;
		bool & ret;
		template< typename TAG >
		void operator ( )( const TAG & ) const { ret = that->template has_static_variable< TAG >( ); }
		has_static_variable_helper( const CRTP * that, bool & ret ) : that( that ), ret( ret ) { }
	};
	bool has_member_variable( const std::string & str ) const
	{
		if ( member_variable.count( str ) > 0 ) { return true; }
		try
		{
			bool ret;
			string_to_tag( str, has_member_variable_helper( static_cast< const CRTP * >( this ), ret ) );
			return ret;
		}
		catch ( const unable_to_determine_type & )
		{ return false; }
	}
	bool has_static_variable( const std::string & str ) const
	{
		if ( static_variable( ).count( str ) > 0 ) { return true; }
		try
		{
			bool ret;
			string_to_tag( str, has_static_variable_helper( static_cast< const CRTP * >( this ), ret ) );
			return ret;
		}
		catch ( const unable_to_determine_type & )
		{ return false; }	}
	template< typename TAG, typename K >
	typename
	std::enable_if< ::has_member_variable< CRTP, TAG >::value >::type get_member_variable( const K & k )
	{ k( ::member_variable< CRTP, TAG >( )( static_cast< CRTP * >( this ) ) ); }
	template< typename TAG, typename K >
	typename
	std::enable_if< ::has_static_variable< CRTP, TAG >::value >::type get_static_variable( const K & k ) const
	{ k( ::static_variable< CRTP, TAG >( )( ) ); }
	template< typename TAG, typename K >
	typename
	std::enable_if< ! ::has_member_variable< CRTP, TAG >::value >::type get_member_variable( const K & k )
	{ get_member_variable( TAG::name( ), k ); }
	template< typename K >
	void get_member_variable( const std::string & str, const K & k )
	{
		auto i = member_variable.find( str );
		if ( i == member_variable.end( ) ) { k( no_existence( ) ); }
		else { i->type_restore( k ); }
	}
	template< typename K >
	void get_static_variable( const std::string & str, const K & k ) const
	{
		auto i = static_variable( ).find( str );
		if ( i == static_variable( ).end( ) ) { k( no_existence( ) ); }
		else { i->type_restore( k ); }
	}
	template< typename TAG, typename K >
	typename
	std::enable_if< ! ::has_static_variable< CRTP, TAG >::value >::type get_static_variable( const K & k ) const
	{ get_static_variable( TAG::name( ), k ); }
	bool add_member_variable( const std::string & str, const ANY & any )
	{ return member_variable.insert( std::make_pair( str, any ) ).second; }
	static bool add_static_variable( const std::string & str, const ANY & any )
	{ return static_variable( ).insert( std::make_pair( str, any ) ).second; }
	virtual ~object_base( ) { }
	template< typename TAG, typename K >
	void member_variable_type( const K & k ) const
	{

	}
	template< typename TAG, typename K >
	void static_variable_type( const K & k ) const
	{

	}
};
template< typename ANY >
struct object : object_base< ANY, object< ANY > >
{

};
#endif // OBJECT_HPP
