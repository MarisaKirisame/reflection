#ifndef GET_TYPENAME_HPP
#define GET_TYPENAME_HPP
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <string>
#include <type_traits>
#define DECLARE_POSSIBLE_GET_TYPENAME( ELEMENT ) \
	template < typename SELF > \
	static constexpr \
	typename std::enable_if \
	< \
		std::is_same< BOOST_PP_CAT( ELEMENT, _tag ), typename SELF::tag >::value, \
		const char * \
	>::type get_typename( ) { return BOOST_PP_STRINGIZE( ELEMENT ); }
template< typename T >
struct get_typename { constexpr const char * operator( )( ){ return T::template get_typename< T >( ); } };
#define GET_TYPENAME( T ) get_typename< typename std::remove_reference< T >::type >( )( )
#endif //GET_TYPENAME_HPP
