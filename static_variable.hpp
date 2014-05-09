#ifndef STATIC_VARIABLE_HPP
#define STATIC_VARIABLE_HPP
#include "reflection.hpp"
#define DECLARE_POSSIBLE_STATIC_VARIABLE( NAME ) \
template< typename T, typename SELF > \
constexpr static bool has_static_variable( \
	typename std::enable_if \
	< \
		std::is_same \
		< \
			T, \
			BOOST_PP_CAT( NAME, _tag ) \
		>::value && \
		! std::is_member_object_pointer< decltype( & SELF::NAME ) >::value \
	>::type * ) \
	{ return true; } \
	template< typename T, typename SELF > \
	static typename \
	std::add_lvalue_reference \
	< \
		typename enable_if_valid \
		< \
			typename std::enable_if \
			< \
				std::is_same \
				< \
					T, \
					BOOST_PP_CAT( NAME, _tag ) \
				>::value \
			>::type, \
			decltype( SELF::NAME ) \
		>::type \
	>::type get_static_variable( ) { return SELF::NAME; } \
	template< typename T, typename SELF > \
	static typename \
	enable_if_valid \
	< \
		typename std::enable_if \
		< \
			std::is_same \
			< \
				T, \
				BOOST_PP_CAT( NAME, _tag ) \
			>::value \
		>::type, \
		decltype( SELF::NAME ) \
	>::type get_static_variable_return_type( );
#define HAS_STATIC_VARIABLE( TYPE, NAME ) ( has_static_variable< TYPE, BOOST_PP_CAT( NAME, _tag ) >::value )
#define STATIC_VARIABLE_TYPE( TYPE, NAME ) typename static_variable_type< TYPE, BOOST_PP_CAT( NAME, _tag ) >::type
#define STATIC_VARIABLE( TYPE, NAME ) static_variable< TYPE, BOOST_PP_CAT( NAME, _tag ) >( )( )
template< typename TYPE, typename NAME >
struct static_variable
{
	decltype( TYPE::template get_static_variable< NAME, TYPE >( ) )
	operator ( )( ) { return TYPE::template get_static_variable< NAME, TYPE >( ); }
};
template< typename TYPE, typename NAME >
struct has_static_variable { static constexpr bool value = TYPE::template has_static_variable< NAME, TYPE >( nullptr ); };
template< typename TYPE, typename NAME >
struct static_variable_type { typedef decltype( TYPE::template get_static_variable_return_type< NAME, TYPE >( ) ) type; };
#endif //STATIC_VARIABLE_HPP
