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
	constexpr const char * operator ( )( ... ) { return get_typename_inner::function< typename std::remove_reference< typename std::remove_cv< TT >::type >::type >( nullptr ); }
	struct get_typename_inner
	{
		template< typename T >
		static constexpr typename std::enable_if< has_class< T >::value, const char * >::type function( T * ){ return T::get_typename( ); }
		template< typename ... >
		static constexpr const char *  function( ... ) { return ""; }
	};
};
#endif //GET_TYPENAME_HPP
