#ifndef IS_EMPTY_TUPLE_HPP
#define IS_EMPTY_TUPLE_HPP
#include <boost/preprocessor/logical/or.hpp>
#include <boost/preprocessor/logical/not.hpp>
#define HAS_COMMA_HELPER_HELPER(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define HAS_COMMA_HELPER(...) HAS_COMMA_HELPER_HELPER(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define EAT_PARENTHESIS(...) ,
#define IS_EMPTY_TUPLE(...) IS_EMPTY_TUPLE_HELPER( BOOST_PP_TUPLE_ENUM( __VA_ARGS__ ) )
#define IS_EMPTY_TUPLE_HELPER(...) \
	BOOST_PP_NOT( \
		BOOST_PP_OR( \
			BOOST_PP_OR( \
				BOOST_PP_OR( \
					HAS_COMMA_HELPER( __VA_ARGS__  ), \
					HAS_COMMA_HELPER( EAT_PARENTHESIS __VA_ARGS__  ) ), \
				HAS_COMMA_HELPER( __VA_ARGS__  ( ) ) ), \
			BOOST_PP_NOT( HAS_COMMA_HELPER( EAT_PARENTHESIS __VA_ARGS__  ( ) ) ) ) )
#define LIMIT_IS_EMPTY_TUPLE 15
#endif //IS_EMPTY_TUPLE_HPP
