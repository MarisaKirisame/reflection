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
template< typename TTYPE, typename NNAME >
struct static_variable
{
	struct inner
	{
		template< typename TYPE, typename NAME >
		static decltype( TYPE::template get_static_variable< NAME, TYPE >( ) )
		function( void * ) { return TYPE::template get_static_variable< NAME, TYPE >( ); }
		template< typename TYPE, typename NAME >
		static no_existence function( ... ) { return no_existence( ); }
	};
	decltype( inner::function< TTYPE, NNAME >( nullptr ) ) operator( )( ) { return inner::function< TTYPE, NNAME >( nullptr ); }
};
template< typename TTYPE, typename NNAME >
struct has_static_variable
{
	struct inner
	{
		template< typename TYPE, typename NAME >
		constexpr static bool function( typename std::enable_if< has_class< TYPE >::value >::type * )
		{ return TYPE::template has_static_variable< NAME, TYPE >( nullptr ); }
		template< typename TYPE, typename NAME >
		constexpr static bool function( ... ) { return false; }
	};
	static constexpr bool value = inner::function< TTYPE, NNAME >( nullptr );
};
template< typename TYPE, typename NAME >
struct static_variable_type { typedef decltype( TYPE::template get_static_variable_return_type< NAME, TYPE >( ) ) type; };
#endif //STATIC_VARIABLE_HPP
