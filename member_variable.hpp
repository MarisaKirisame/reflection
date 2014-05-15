#ifndef MEMBER_VARIABLE_HPP
#define MEMBER_VARIABLE_HPP
#include "reflection.hpp"
#include <boost/preprocessor.hpp>
#define DECLARE_POSSIBLE_MEMBER_VARIABLE( NAME ) \
	template< typename SELF, typename TAG > \
	constexpr static bool has_member_variable( \
	typename std::enable_if \
	< \
		std::is_same \
		< \
			TAG, \
			::tag< NAME > \
		>::value && \
		std::is_member_object_pointer< decltype( & SELF::NAME ) >::value, \
		decltype( std::declval< SELF * >( )->NAME ) \
	>::type * ) \
	{ return true; } \
	template< typename SELF, typename TAG > \
	constexpr static typename \
	std::enable_if \
	< \
		std::is_same \
		< \
			TAG, \
			::tag< ::NAME > \
		>::value, \
		bool \
	>::type has_member_variable( ... ) { return false; } \
	template< typename SELF, typename TAG > \
	static typename std::add_lvalue_reference \
	< \
		typename enable_if_valid \
		< \
			typename std::enable_if \
			< \
				std::is_same \
				< \
					TAG, \
					::tag< NAME > \
				>::value \
			>::type, \
			decltype( std::declval< SELF * >( )->NAME ) \
		>::type \
	>::type get_member_variable( SELF * t ) { return t->NAME; } \
	template< typename SELF, typename TAG > \
	static typename enable_if_valid \
	< \
		typename std::enable_if \
		< \
			std::is_same \
			< \
				TAG, \
				::tag< ::NAME > \
			>::value \
		>::type, \
		decltype( std::declval< SELF * >( )->NAME ) \
	>::type member_variable_type( ) { return static_cast< SELF * >( nullptr )->NAME; } \
	template< typename SELF, typename TAG, typename K > \
	typename std::enable_if \
	< \
		std::is_same \
		< \
			TAG, \
			::tag< ::NAME > \
		>::value && has_member_variable< TAG, SELF >( nullptr ), \
	typename enable_if_valid< decltype( std::declval< SELF * >( )->NAME ) >::type \
	>::type invoke_member_variable( const K & k ) \
	{ \
		k( ::tag< ::NAME >( ), static_cast< SELF * >( this )->NAME ); \
	}
#define INVOKE_ALL_MEMBER_VARIABLE_HELPER( R, DATA, ELEMENT ) \
	invoke_member_variable \
	< \
		::tag< ::ELEMENT >, \
		typename std::remove_reference< typename std::remove_cv< decltype( * this ) >::type >::type, \
		typename std::remove_cv< decltype( DATA ) >::type \
	>( DATA );
#define INVOKE_ALL_MEMBER_VARIABLE( K, NAME_SEQ ) \
	BOOST_PP_SEQ_FOR_EACH( INVOKE_ALL_MEMBER_VARIABLE_HELPER, K, NAME_SEQ )
template< typename TYPE, typename TAG >
struct has_member_variable { constexpr static bool value = false; };
template< typename TYPE, typename TAG >
struct member_variable_type { typedef no_existence type; };
template< typename TTYPE, typename NNAME >
struct member_variable
{
	template< typename TYPE, typename NAME >
	static decltype( helper< TYPE >::template get_member_variable< TYPE, NAME >( nullptr ) )
	function(
			typename std::enable_if
			<
				has_class< TYPE >::value && \
				has_member_variable< TYPE, NAME >::value,
				TYPE
			>::type * t )
	{ return helper< TYPE >::template get_member_variable< TYPE, NAME >( t ); }
	template< typename TYPE, typename NAME >
	static no_existence function( ... ) { return no_existence( ); }
	decltype( function< TTYPE, NNAME >( std::declval< TTYPE * >( ) ) ) operator ( )( TTYPE * t )
	{ return function< TTYPE, NNAME >( t ); }
};
#define DECLARE_ALL_POSSIBLE_MEMBER_VARIABLE_HELPER( R, DATA, ELEMENT ) DECLARE_POSSIBLE_MEMBER_VARIABLE( ELEMENT )
#define DECLARE_ALL_POSSIBLE_MEMBER_VARIABLE( NAME_SEQ ) BOOST_PP_SEQ_FOR_EACH( DECLARE_ALL_POSSIBLE_MEMBER_VARIABLE_HELPER, _, NAME_SEQ )
#endif //MEMBER_VARIABLE_HPP
