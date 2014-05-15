#ifndef STATIC_VARIABLE_HPP
#define STATIC_VARIABLE_HPP
#include <boost/preprocessor.hpp>
#include "reflection.hpp"
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
				>::value \
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
template< typename T, typename TAG >
struct static_variable { no_existence operator( )( ... ) { return no_existence( ); } };
template< typename T, typename TAG >
struct has_static_variable { constexpr static bool value = false; };
template< typename T, typename TAG >
struct static_variable_type { typedef no_existence type; };
#endif //STATIC_VARIABLE_HPP
