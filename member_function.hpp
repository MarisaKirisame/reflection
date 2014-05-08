#ifndef MEMBER_FUNCTION_HPP
#define MEMBER_FUNCTION_HPP
#include "../misc/misc.hpp"
#include "reflection.hpp"
#define DECLARE_POSSIBLE_MEMBER_FUNCTION( NAME ) \
	template< typename T, typename SELF, typename ... R > \
	constexpr static bool has_member_function( \
		typename std::enable_if \
		< \
			std::is_same \
			< \
				T, \
				BOOST_PP_CAT( NAME, _tag ) \
			>::value && \
			std::is_member_function_pointer< decltype( & SELF::NAME ) >::value, \
			typename std::add_pointer \
			< \
				decltype( misc::construct< SELF * >( )->NAME( misc::construct< R >( ) ... ) ) \
			>::type \
		>::type ) \
	{ return true; } \
	template< typename T, typename SELF, typename ... R > \
	typename std::enable_if \
	< \
		std::is_same \
		< \
			T, \
			BOOST_PP_CAT( NAME, _tag ) \
		>::value, \
		decltype( misc::construct< SELF * >( )->NAME( misc::construct< R >( ) ... ) ) \
	>::type \
	call_member_function( const R & ...  r ) { return NAME( r ... ); } \
	template< typename T, typename SELF, typename ... R > \
	static typename std::enable_if \
	< \
		std::is_same \
		< \
			T, \
			BOOST_PP_CAT( NAME, _tag ) \
		>::value, \
		decltype( misc::construct< SELF * >( )->NAME( misc::construct< R >( ) ... ) ) \
	>::type \
	call_member_function( const SELF & t, const R & ...  r ) { return t->NAME( r ... ); }
#define HAS_MEMBER_FUNCTION( TYPE, NAME, ARGUMENT_TUPLE ) \
	has_member_function \
	< \
		TYPE, \
		BOOST_PP_CAT( NAME, _tag ) \
		EXPAND_TUPLE_ARGUMENT( ARGUMENT_TUPLE ) \
	>::value
#define MEMBER_FUNCTION_RETURN_TYPE( TYPE, NAME, ARGUMENT_TUPLE ) \
	member_function_return_type \
	< \
		TYPE, \
		BOOST_PP_CAT( NAME, _tag ) \
		EXPAND_TUPLE_ARGUMENT( ARGUMENT_TUPLE ) \
	>::type
#define CALL_MEMBER_FUNCTION( SELF, NAME, ARGUMENT_TUPLE ) \
	call_member_function \
	< \
		decltype( SELF ), \
		BOOST_PP_CAT( NAME, _tag ) \
		EXPAND_TUPLE_ARGUMENT( \
			BOOST_PP_SEQ_TO_TUPLE( \
				BOOST_PP_SEQ_TRANSFORM( \
					DECLTYPE_HELPER, \
					_, \
					BOOST_PP_TUPLE_TO_SEQ( ARGUMENT_TUPLE ) ) ) ) \
	>( )( SELF, BOOST_PP_TUPLE_ENUM( ARGUMENT_TUPLE ) )
template< typename TYPE, typename NAME, typename ... ARG >
struct has_member_function { static constexpr bool value = TYPE::template has_member_function< NAME, TYPE, ARG ... >( nullptr ); };
template< typename TYPE, typename NAME >
struct has_member_function< TYPE, NAME, void >
{ static constexpr bool value = TYPE::template has_member_function< NAME, TYPE >( nullptr ); };
template< typename TYPE, typename NAME, typename ... ARG >
struct member_function_return_type
{
	typedef decltype(
			misc::construct< TYPE * >( )->template call_member_function< NAME, TYPE >(
				misc::construct< ARG >( ) ... ) ) type;
};
template< typename TYPE, typename NAME >
struct member_function_return_type< TYPE, NAME, void >
{
	typedef decltype(
			misc::construct< TYPE * >( )->template call_member_function< NAME, TYPE >( ) ) type;
};
template< typename TYPE, typename NAME, typename ... ARG >
struct call_member_function
{
	decltype(
			TYPE::template call_member_function< NAME, TYPE >( misc::construct < TYPE >( ), misc::construct< ARG >( ) ... ) )
	operator ( )( const TYPE & t,const ARG & ... r )
	{ return TYPE::template call_member_function< NAME, TYPE >( t, r ... ); }
};
template< typename TYPE, typename NAME >
struct call_member_function< TYPE, NAME, void >
{
	decltype( TYPE::template call_member_function< NAME, TYPE >( ) )
	operator ( )( const TYPE & t )
	{ return TYPE::template call_member_function< NAME, TYPE >( t ); }
};
#endif //MEMBER_FUNCTION_HPP
