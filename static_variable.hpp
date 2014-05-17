#ifndef STATIC_VARIABLE_HPP
#define STATIC_VARIABLE_HPP
#include <boost/preprocessor.hpp>
#include "reflection.hpp"
#include "has_class.hpp"
#define DECLARE_POSSIBLE_STATIC_VARIABLE( NAME ) \
	template< typename SELF, typename TAG > \
	constexpr static bool has_static_variable( \
	typename std::enable_if \
	< \
		std::is_same \
		< \
			TAG, \
			::tag< ::NAME > \
		>::value && \
		! std::is_member_object_pointer< decltype( & SELF::NAME ) >::value \
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
	>::type has_static_variable( ... ) { return false; } \
	template< typename SELF, typename TAG > \
	static typename \
	std::add_lvalue_reference \
	< \
		typename enable_if_valid \
		< \
			typename std::enable_if \
			< \
				std::is_same \
				< \
					TAG, \
					::tag< ::NAME > \
				>::value && \
				::has_static_variable< SELF, TAG >::value \
			>::type, \
			decltype( SELF::NAME ) \
		>::type \
	>::type static_variable( ) { return SELF::NAME; } \
	template< typename SELF, typename TAG > \
	static typename \
	enable_if_valid \
	< \
		typename std::enable_if \
		< \
			std::is_same \
			< \
				TAG, \
				::tag< ::NAME > \
			>::value \
		>::type, \
		decltype( SELF::NAME ) \
	>::type static_variable_type( );
#define DECLARE_ALL_POSSIBLE_STATIC_VARIABLE_HELPER( R, TYPE, NAME ) DECLARE_POSSIBLE_STATIC_VARIABLE( NAME )
#define DECLARE_ALL_POSSIBLE_STATIC_VARIABLE( NAME_SEQ ) BOOST_PP_SEQ_FOR_EACH( DECLARE_ALL_POSSIBLE_STATIC_VARIABLE_HELPER, _, NAME_SEQ )
template< typename TYPE, typename TAG >
struct has_static_variable
{
	template< typename TTYPE, typename TTAG >
	constexpr static
	decltype( helper< TTYPE >::template has_static_variable< TTYPE, TTAG >( nullptr ) )
	function( void * )
	{ return helper< TTYPE >::template has_static_variable< TTYPE, TTAG >( nullptr ); }
	template< typename ... >
	constexpr static bool function( ... ) { return false; }
	constexpr static bool value = function< TYPE, TAG >( nullptr );
};
template< typename TYPE, typename TAG >
struct static_variable_type
{
	template< typename TTYPE, typename TTAG >
	static decltype( helper< TTYPE >::template static_variable_type< TTYPE, TTAG >( ) ) function( void * );
	template< typename ... >
	static no_existence function( ... );
	typedef decltype( function< TYPE, TAG >( nullptr ) ) type;
};
template< typename TYPE, typename TAG >
struct static_variable
{
	template< typename TTYPE, typename TTAG >
	static decltype( helper< TTYPE >::template static_variable< TTYPE, TTAG >( ) ) function( void * )
	{ return helper< TTYPE >::template static_variable< TTYPE, TTAG >( ); }
	template< typename ... >
	static no_existence function( ... ) { return no_existence( ); }
	decltype( function< TYPE, TAG >( nullptr ) )
	operator ( )( ) { return function< TYPE, TAG >( nullptr ); }
};
#endif //STATIC_VARIABLE_HPP
