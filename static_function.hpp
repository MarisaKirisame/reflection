#ifndef STATIC_FUNCTION_HPP
#define STATIC_FUNCTION_HPP
#include "reflection.hpp"
#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/tuple/to_seq.hpp>
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#define DECLARE_POSSIBLE_STATIC_FUNCTION( NAME ) \
	template< typename T, typename SELF, typename ... R > \
	constexpr static bool has_static_function( \
		typename std::enable_if \
		< \
			std::is_same \
			< \
				T, \
				BOOST_PP_CAT( NAME, _tag ) \
			>::value, \
			typename std::add_pointer \
			< \
				decltype( SELF::NAME( std::declval< R >( ) ... ) ) \
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
		decltype( SELF::NAME( std::declval< R >( ) ... ) ) \
	>::type \
	static call_static_function( const R & ...  r ) { return SELF::NAME( r ... ); }
template< typename TTTYPE, typename NNNAME, typename ... AAARG >
struct static_function_return_type
{
	template< typename TTYPE, typename NNAME, typename ... AARG >
	struct inner
	{
		template< typename TYPE, typename NAME, typename ... ARG >
		static decltype(
				TYPE::template call_static_function< NAME, TYPE >(
					std::declval< ARG >( ) ... ) ) function( void * );
		template< typename ... >
		static no_existence function( ... );
	};
	template< typename TTYPE, typename NNAME >
	struct inner< TTYPE, NNAME, void >
	{
		template< typename TYPE, typename NAME, typename ... >
		static decltype(
				TYPE::template call_static_function< NAME, TYPE >( ) ) function( void * );
		template< typename ... >
		static no_existence function( ... );
	};
	typedef decltype( inner< TTTYPE, NNNAME, AAARG ... >::template function< TTTYPE, NNNAME, AAARG ... >( nullptr ) ) type;
};
template< typename TTYPE, typename NNAME, typename ... AARG >
struct has_static_function
{
	template< typename TTTYPE, typename NNNAME, typename ... AAARG >
	struct inner
	{
		template< typename TYPE, typename NAME, typename ... ARG >
		static constexpr bool function( typename std::enable_if< has_class< TYPE >::value >::type * ) { return TYPE::template has_static_function< NAME, TYPE, ARG ... >( nullptr ); }
		template< typename ... >
		static constexpr bool function( ... ) { return false; }
	};
	template< typename TTTYPE, typename NNNAME >
	struct inner< TTTYPE, NNNAME, void >
	{
		template< typename TYPE, typename NAME >
		static constexpr bool function( typename std::enable_if< has_class< TYPE >::value >::type * ) { return TYPE::template has_static_function< NAME, TYPE >( nullptr ); }
		template< typename ... >
		static constexpr bool function( ... ) { return false; }
	};
	constexpr static bool value = inner< TTYPE, NNAME, AARG ... >::template function< TTYPE, NNAME, AARG ... >( nullptr );
};
template< typename TYPE, typename NAME, typename ... ARG >
struct call_static_function
{
	decltype( TYPE::template call_static_function< NAME, TYPE >( std::declval< ARG >( ) ... ) )
	operator ( )( const ARG & ... r )
	{ return TYPE::template call_static_function< NAME, TYPE >( r ... ); }
};
template< typename TYPE, typename NAME >
struct call_static_function< TYPE, NAME, void >
{
	decltype( TYPE::template call_static_function< NAME, TYPE >( ) )
	operator ( )( )
	{ return TYPE::template call_static_function< NAME, TYPE >( ); }
};
#endif //STATIC_FUNCTION_HPP
