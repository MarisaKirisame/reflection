#ifndef REFLECTION_HPP
#define REFLECTION_HPP
#include "is_empty_tuple.hpp"
#include "get_typename.hpp"
#include <type_traits>
#define EXPAND_TUPLE_ARGUMENT_HELPER( R, DATA, ELEMENT ) , ELEMENT
#define EAT_ARG( ... )
#define EXPAND_TUPLE_ARGUMENT( ARGUMENT_TUPLE ) \
	BOOST_PP_IIF( \
		IS_EMPTY_TUPLE( ARGUMENT_TUPLE ), \
		EAT_ARG, \
		BOOST_PP_SEQ_FOR_EACH )\
		( \
				EXPAND_TUPLE_ARGUMENT_HELPER, \
				_, \
				BOOST_PP_TUPLE_TO_SEQ( ARGUMENT_TUPLE ) )
#define DECLTYPE_HELPER( D, DATA, ELEMENT ) decltype( ELEMENT )
template< typename T, typename RET = void >
struct enable_if_valid { typedef RET type; };
struct ignore { ignore( ... ) { } };
struct no_existence{ };
#endif //REFLECTION_HPP
