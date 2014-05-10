#ifndef MEMBER_VARIABLE_HPP
#define MEMBER_VARIABLE_HPP
#include "reflection.hpp"
#include "has_class.hpp"
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
template< typename TYPE, typename NAME >
struct has_member_variable { static constexpr bool value = TYPE::template has_member_variable< NAME, TYPE >( nullptr ); };
template< typename TYPE, typename NAME >
struct member_variable_type { typedef decltype( std::declval< TYPE * >( )->template get_member_variable_return_type< NAME, TYPE >( ) ) type; };
struct no_existence{ };
template< typename TTYPE, typename NNAME >
struct member_variable
{
	struct member_variable_inner
	{
		template< typename TYPE, typename NAME >
		static decltype( std::declval< TYPE * >( )->template get_member_variable< NAME, TYPE >( ) )
		function( \
				typename std::enable_if \
				< \
					has_class< TYPE >::value && \
					has_member_variable< TYPE, NAME >::value, \
					TYPE \
				>::type * t ) \
		{ return t->template get_member_variable< NAME, TYPE >( ); }
		template< typename TYPE, typename NAME >
		static no_existence function( ... ) { return no_existence( ); }
	};
	decltype( member_variable_inner::function< TTYPE, NNAME >( std::declval< TTYPE * >( ) ) ) operator ( )( TTYPE & t )
	{ return member_variable_inner::function< TTYPE, NNAME >( & t ); }
};
#endif //MEMBER_VARIABLE_HPP
