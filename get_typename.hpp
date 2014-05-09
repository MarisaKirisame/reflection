#ifndef GET_TYPENAME_HPP
#define GET_TYPENAME_HPP
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <string>
#include <type_traits>
#include <has_class.hpp>
template< typename TT >
struct get_typename
{
	constexpr const char * operator ( )( ... ) { return get_typename_inner::function< TT >( nullptr ); }
	struct get_typename_inner
	{
		template< typename T >
		static constexpr typename std::enable_if< HAS_CLASS( T ), const char * >::type function( T * ){ return T::get_typename( ); }
		template< typename ... >
		static constexpr const char *  function( ... ) { return "no exist"; }
	};
};
#define GET_TYPENAME( T ) get_typename< T >( )( nullptr )
#endif //GET_TYPENAME_HPP
