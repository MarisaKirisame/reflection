#ifndef MEMBER_VARIABLE_HPP
#define MEMBER_VARIABLE_HPP
#include "reflection.hpp"
#include <boost/preprocessor.hpp>
#define INVOKE_ALL_MEMBER_VARIABLE_HELPER( R, DATA, ELEMENT ) \
	invoke_member_variable \
	< \
		typename std::remove_reference< typename std::remove_cv< decltype( * t ) >::type >::type, \
		::tag< ::ELEMENT >, \
		typename std::remove_cv< decltype( DATA ) >::type \
	>( t, DATA );
#define INVOKE_ALL_MEMBER_VARIABLE( K, NAME_SEQ ) \
	BOOST_PP_SEQ_FOR_EACH( INVOKE_ALL_MEMBER_VARIABLE_HELPER, K, NAME_SEQ )
#define DECLARE_POSSIBLE_MEMBER_VARIABLE( NAME, NAME_SEQ ) \
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
	static typename std::enable_if \
	< \
		std::is_same \
		< \
			TAG, \
			::tag< ::NAME > \
		>::value && has_member_variable< SELF, TAG >( nullptr ), \
	typename enable_if_valid< decltype( std::declval< SELF * >( )->NAME ) >::type \
	>::type invoke_member_variable( SELF * t, const K & k ) \
	{ \
		k( ::tag< ::NAME >( ), t->NAME ); \
	} \
	template< typename SELF, typename TAG, typename K > \
	static typename std::enable_if \
	< \
		std::is_same \
		< \
			TAG, \
			::tag< ::NAME > \
		>::value \
	>::type invoke_member_variable( ... ) { }
template< typename TYPE, typename TAG >
struct has_member_variable
{
	template< typename TTYPE, typename TTAG >
	constexpr static decltype( helper< TTYPE >::template has_member_variable< TTYPE, TTAG >( nullptr ) ) function( void * )
	{ return helper< TTYPE >::template has_member_variable< TTYPE, TTAG >( nullptr ); }
	template< typename ... >
	constexpr static bool function( ... ) { return false; }
	constexpr static bool value = function< TYPE, TAG >( nullptr );
};
template< typename TYPE, typename TAG >
struct member_variable_type_inner
{
	template< typename TTYPE, typename TTAG >
	static decltype( helper< TTYPE >::template member_variable_type< TTYPE, TTAG >( ) ) function( void * );
	template< typename ... >
	static no_existence function( ... );
	typedef decltype( function< TYPE, TAG >( nullptr ) ) type;
};
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
#define DECLARE_ALL_POSSIBLE_MEMBER_VARIABLE_HELPER( R, DATA, ELEMENT ) DECLARE_POSSIBLE_MEMBER_VARIABLE( ELEMENT, DATA )
#define DECLARE_ALL_POSSIBLE_MEMBER_VARIABLE( NAME_SEQ ) BOOST_PP_SEQ_FOR_EACH( DECLARE_ALL_POSSIBLE_MEMBER_VARIABLE_HELPER, NAME_SEQ, NAME_SEQ )
#endif //MEMBER_VARIABLE_HPP
