#ifndef MEMBER_FUNCTION_HPP
#define MEMBER_FUNCTION_HPP
#include "reflection.hpp"
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <type_traits>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>
#define DECLARE_POSSIBLE_MEMBER_FUNCTION( NAME ) \
	template< typename T, typename SELF, typename ... R > \
	constexpr static bool has_member_function( \
		typename std::enable_if \
		< \
			std::is_same \
			< \
				T, \
				::tag< NAME > \
			>::value && \
			std::is_member_function_pointer< decltype( & SELF::NAME ) >::value, \
			typename std::add_pointer \
			< \
				decltype( std::declval< SELF * >( )->NAME( std::declval< R >( ) ... ) ) \
			>::type \
		>::type ) \
	{ return true; } \
	template< typename T, typename SELF, typename ... R > \
	typename std::enable_if \
	< \
		std::is_same \
		< \
			T, \
			::tag< NAME > \
		>::value, \
		decltype( std::declval< SELF * >( )->NAME( std::declval< R >( ) ... ) ) \
	>::type \
	call_member_function( const R & ...  r ) { return NAME( r ... ); } \
	template< typename T, typename SELF, typename ... R > \
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
template< typename TTYPE, typename NNAME, typename ... AARG >
struct has_member_function
{
	template< typename TTTYPE, typename NNNAME, typename ... AAARG >
	struct has_member_function_inner
	{
		template< typename TYPE, typename NAME, typename ... ARG >
		static constexpr bool function( typename std::enable_if< has_class< TYPE >::value >::type * )
		{ return TYPE::template has_member_function< NAME, TYPE, ARG ... >( nullptr ); }
		template< typename TYPE, typename NAME, typename ... ARG >
		static constexpr bool function( ... )
		{ return false; }
	};
	template< typename TTTYPE, typename NNNAME >
	struct has_member_function_inner< TTTYPE, NNNAME, void >
	{
		template< typename TYPE, typename NAME, typename ... ARG >
		static constexpr bool function( typename std::enable_if< has_class< TYPE >::value >::type * )
		{ return TYPE::template has_member_function< NAME, TYPE >( nullptr ); }
		template< typename TYPE, typename NAME, typename ... ARG >
		static constexpr bool function( ... )
		{ return false; }
	};
	constexpr static bool value = has_member_function_inner< TTYPE, NNAME, AARG ... >::template function< TTYPE, NNAME, AARG ... >( nullptr );
};
template< typename TTYPE, typename NNAME, typename ... AARG >
struct member_function_return_type
{
	template< typename TTTYPE, typename NNNAME, typename ... AAARG >
	struct member_function_return_type_inner
	{
		template< typename TYPE, typename NAME, typename ... ARG >
		static decltype(
					std::declval< TYPE * >( )->template call_member_function< NAME, TYPE >(
						std::declval< ARG >( ) ... ) ) function( void * );
		template< typename TYPE, typename NAME, typename ... ARG >
		static no_existence function( ... );
	};
	template< typename TTTYPE, typename NNNAME >
	struct member_function_return_type_inner< TTTYPE, NNNAME, void >
	{
		template< typename TYPE, typename NAME >
		static decltype(
				std::declval< TYPE * >( )->template call_member_function< NAME, TYPE >( ) ) function( );
	};
	typedef decltype( member_function_return_type_inner< TTYPE, NNAME, AARG ... >::template function< TTYPE, NNAME, AARG ... >( nullptr ) ) type;
};
template< typename TYPE, typename NAME, typename ... ARG >
struct call_member_function
{
	decltype(
			TYPE::template call_member_function \
			< \
				NAME , \
				TYPE \
			>( std::declval< typename std::remove_reference< decltype( std::declval< TYPE >( ) ) >::type >( ), std::declval< ARG >( ) ... ) )
	operator ( )( const TYPE & t,const ARG & ... r )
	{ return std::remove_reference< TYPE >::type::template call_member_function< NAME, typename std::remove_reference< TYPE >::type >( t, r ... ); }
};
template< typename TYPE, typename NAME >
struct call_member_function< TYPE, NAME, void >
{
	decltype( TYPE::template call_member_function< NAME, TYPE >( ) )
	operator ( )( const TYPE & t )
	{ return TYPE::template call_member_function< NAME, TYPE >( t ); }
};
#endif //MEMBER_FUNCTION_HPP
