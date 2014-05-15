#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <any.hpp>
#include <map>
#include <string>
#include "member_variable.hpp"
template< typename ANY, typename CRTP >
struct object_base
{
	std::map< std::string, ANY > member_variable;
	static std::map< std::string, ANY > & static_variable( )
	{
		static std::map< std::string, ANY > ret;
		return ret;
	}
	template< typename TAG >
	bool has_member_variable( )
	{
		return
				::has_member_variable< CRTP, TAG >::value ||
				( member_variable.count( TAG::name( ) ) > 0 );
	}
	template< typename TAG >
	bool has_static_variable( )
	{
		return
				has_static_variable< CRTP, TAG >( ) ||
				( static_variable( ).count( TAG::name( ) ) > 0 );
	}
	virtual ~object_base( ) { }
};
template< typename ANY >
struct object : object_base< ANY, object< ANY > >
{

};
#endif // OBJECT_HPP
