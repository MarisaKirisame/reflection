#ifndef MEMBER_FUNCTION_HPP
#define MEMBER_FUNCTION_HPP
#include "reflection.hpp"
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <type_traits>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>
#include <has_class.hpp>
#define DECLARE_POSSIBLE_MEMBER_FUNCTION( NAME ) \
	template< typename SELF, typename TAG, typename ... ARG > \
	constexpr static bool has_member_function( \
		typename std::enable_if \
		< \
			std::is_same \
			< \
				TAG, \
				::tag< NAME > \
			>::value && \
			std::is_member_function_pointer< decltype( & SELF::NAME ) >::value, \
			typename std::add_pointer \
			< \
				decltype( std::declval< SELF * >( )->NAME( std::declval< ARG >( ) ... ) ) \
			>::type \
		>::type ) \
	{ return true; } \
	template< typename SELF, typename TAG, typename ... ARG > \
	typename std::enable_if \
	< \
		std::is_same \
		< \
			TAG, \
			::tag< NAME > \
		>::value, \
		decltype( std::declval< SELF * >( )->NAME( std::declval< ARG >( ) ... ) ) \
	>::type \
	call_member_function( const ARG & ...  r ) { return NAME( r ... ); } \
	template< typename SELF, typename T, typename ... R > \
	static typename std::enable_if \
	< \
		std::is_same \
		< \
			T, \
			::tag< NAME > \
		>::value, \
		decltype( std::declval< SELF * >( )->NAME( std::declval< R >( ) ... ) ) \
	>::type \
	call_member_function( const SELF & t, const R & ...  r ) { return t.NAME( r ... ); }
template< typename TYPE, typename TAG, typename ... ARG >
struct has_member_function { constexpr static bool value = false; };
template< typename TYPE, typename TAG, typename ... ARG >
struct member_function_return_type{ typedef no_existence type; };
template< typename TYPE, typename TAG, typename ... ARG >
struct call_member_function { void operator( )( ... ); };
#define DECLARE_ALL_POSSIBLE_MEMBER_FUNCTION_HELPER( R, DATA, ELEMENT ) DECLARE_POSSIBLE_MEMBER_FUNCTION( ELEMENT )
#define DECLARE_ALL_POSSIBLE_MEMBER_FUNCTION( NAME_SEQ ) BOOST_PP_SEQ_FOR_EACH( DECLARE_ALL_POSSIBLE_MEMBER_FUNCTION_HELPER, _, NAME_SEQ );
#endif //MEMBER_FUNCTION_HPP
