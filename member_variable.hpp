#ifndef MEMBER_VARIABLE_HPP
#define MEMBER_VARIABLE_HPP
#include "reflection.hpp"
#define DECLARE_POSSIBLE_MEMBER_VARIABLE( NAME ) \
template< typename T, typename SELF > \
constexpr static bool has_member_variable( \
	typename std::enable_if \
	< \
		std::is_same \
		< \
			T, \
			BOOST_PP_CAT( NAME, _tag ) \
		>::value && \
		std::is_member_object_pointer< decltype( & SELF::NAME ) >::value, \
		decltype( std::declval< SELF * >( )->NAME ) \
	>::type * ) \
	{ return true; } \
	template< typename T, typename SELF > \
	typename std::add_lvalue_reference \
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
			decltype( std::declval< SELF * >( )->NAME ) \
		>::type \
	>::type get_member_variable( ) { return static_cast< SELF * >( this )->NAME; } \
	template< typename T, typename SELF > \
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
		decltype( std::declval< SELF * >( )->NAME ) \
	>::type get_member_variable_return_type( ) { return static_cast< SELF * >( this )->NAME; } \
	template< typename T, typename SELF, typename K > \
	typename std::enable_if \
	< \
		std::is_same \
		< \
			T, \
			BOOST_PP_CAT( NAME, _tag ) \
		>::value && has_member_variable< T, SELF >( nullptr ), \
	typename enable_if_valid< decltype( std::declval< SELF * >( )->NAME ) >::type \
	>::type invoke_member_variable( const K & k ) \
	{ \
		k( BOOST_PP_CAT( NAME, _tag )( ), static_cast< SELF * >( this )->NAME ); \
	}
#define INVOKE_ALL_MEMBER_VARIABLE_HELPER( R, DATA, ELEMENT ) \
	invoke_member_variable \
	< \
		BOOST_PP_CAT( ELEMENT, _tag ), \
		typename std::remove_reference< typename std::remove_cv< decltype( * this ) >::type >::type, \
		typename std::remove_cv< decltype( DATA ) >::type \
	>( DATA );
#define INVOKE_ALL_MEMBER_VARIABLE( K, NAME_SEQ ) \
	BOOST_PP_SEQ_FOR_EACH( INVOKE_ALL_MEMBER_VARIABLE_HELPER, K, NAME_SEQ )
#define HAS_MEMBER_VARIABLE( TYPE, NAME ) ( has_member_variable< TYPE, BOOST_PP_CAT( NAME, _tag ) >::value )
#define MEMBER_VARIABLE_TYPE( TYPE, NAME ) typename member_variable_type< TYPE, BOOST_PP_CAT( NAME, _tag ) >::type
#define MEMBER_VARIABLE( SELF, NAME ) ( member_variable< decltype( SELF ), BOOST_PP_CAT( NAME, _tag ) >( )( SELF ) )
template< typename TYPE, typename NAME >
struct has_member_variable { static constexpr bool value = TYPE::template has_member_variable< NAME, TYPE >( nullptr ); };
template< typename TYPE, typename NAME >
struct member_variable_type { typedef decltype( std::declval< TYPE * >( )->template get_member_variable_return_type< NAME, TYPE >( ) ) type; };
template< typename TYPE, typename NAME >
struct member_variable
{
	decltype( std::declval< TYPE * >( )->template get_member_variable< NAME, TYPE >( ) )
	operator ( )( TYPE & t ) { return t.template get_member_variable< NAME, TYPE >( ); }
};
#endif //MEMBER_VARIABLE_HPP
