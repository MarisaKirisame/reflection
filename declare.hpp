#ifndef DECLARE_HPP
#define DECLARE_HPP
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include "member_function.hpp"
#include "static_function.hpp"
#include "member_variable.hpp"
#include "static_variable.hpp"
template< typename >
struct tag{ };
#define DECLARE_NAME( R, DATA, NAME ) \
	struct NAME; \
	template< > \
	struct tag< NAME > \
	{ \
		static constexpr const char * name( ) \
		{ \
			return BOOST_PP_STRINGIZE( NAME ); \
		} \
	};
#define DECLARE_NAMES( NAME_SEQ ) BOOST_PP_SEQ_FOR_EACH( DECLARE_NAME, _, NAME_SEQ )
#define DECLARE_TYPE( TYPE, NAME_SEQ ) \
	template< typename ... > \
	void invoke_member_variable( ... ) { } \
	template< typename K > \
	void invoke_all_member_variable( const K & k ) \
	{ \
		INVOKE_ALL_MEMBER_VARIABLE( k, NAME_SEQ );\
	} \
	DECLARE_ALL_POSSIBLE( NAME_SEQ )
#define DECLARE_INTERFACE( TYPE ) \
	template< typename TAG > \
	struct has_static_variable< TYPE, TAG > \
	{ constexpr static bool value = helper< TYPE >::template has_static_variable< TYPE, TAG >( nullptr ); }; \
	template< typename TAG > \
	struct static_variable_type< TYPE, TAG > \
	{ typedef decltype( helper< TYPE >::template static_variable_type< TYPE, TAG >( ) ) type; }; \
	template< typename TAG > \
	struct static_variable< TYPE, TAG > \
	{ \
		decltype( helper< TYPE >::template static_variable< TYPE, TAG >( ) ) \
		operator ( )( ){ return helper< TYPE >::template static_variable< TYPE, TAG >( ); } \
	}; \
	template< typename TAG > \
	struct has_member_variable< TYPE, TAG > \
	{ constexpr static bool value = helper< TYPE >::template has_member_variable< TYPE, TAG >( nullptr ); }; \
	template< typename TAG > \
	struct member_variable_type< TYPE, TAG > \
	{ typedef decltype( helper< TYPE >::template member_variable_type< TYPE, TAG >( ) ) type; }; \
	template< typename TTAG, typename ... AARG > \
	struct has_member_function< TYPE, TTAG, AARG ... > \
	{ \
		template< typename, typename, typename ... > \
		struct inner \
		{ \
			template< typename TTYPE, typename TAG, typename ... ARG > \
			static constexpr typename \
			std::enable_if \
			< \
				helper< TTYPE >::template has_member_function< TTYPE, TAG, ARG ... >( nullptr ) || true, \
				bool \
			>::type function( typename std::enable_if< has_class< TTYPE >::value >::type * ) \
			{ return helper< TTYPE >::template has_member_function< TTYPE, TAG, ARG ... >( nullptr ); } \
			template< typename ... > \
			static constexpr bool function( ... ) \
			{ return false; } \
		}; \
		template< typename T1, typename T2 > \
		struct inner< T1, T2, void > \
		{ \
			template< typename TTYPE, typename TAG, typename ... ARG > \
			static constexpr typename \
			std::enable_if \
			< \
				helper< TTYPE >::template has_member_function< TYPE, TAG >( nullptr ) || true, \
				bool \
			>::type function( typename std::enable_if< has_class< TTYPE >::value >::type * ) \
			{ return helper< TTYPE >::template has_member_function< TYPE, TAG >( nullptr ); } \
			template< typename TYPE, typename NAME, typename ... ARG > \
			static constexpr bool function( ... ) \
			{ return false; } \
		}; \
		constexpr static bool value = inner< TYPE, TTAG, AARG ... >::template function< TYPE, TTAG, AARG ... >( nullptr );\
	}; \
	template< typename TAG, typename ... AARG > \
	struct has_static_function< TYPE, TAG, AARG ... > \
	{ \
		template< typename, typename, typename ... > \
		struct inner \
		{ \
			template< typename TTYPE, typename TTAG, typename ... ARG > \
			static constexpr typename \
			std::enable_if \
			< \
				helper< TTYPE >::template has_static_function< TTYPE, TTAG, ARG ... >( nullptr ) || true, \
				bool \
			>::type function( typename std::enable_if< has_class< TTYPE >::value >::type * ) \
			{ return helper< TTYPE >::template has_static_function< TTYPE, TAG, ARG ... >( nullptr ); } \
			template< typename ... > \
			static constexpr bool function( ... ) { return false; } \
		}; \
		template< typename T1, typename T2 > \
		struct inner< T1, T2, void > \
		{ \
			template< typename TTYPE, typename TTAG > \
			static constexpr bool function( typename std::enable_if< has_class< TYPE >::value >::type * ) \
			{ return helper< TTYPE >::template has_static_function< TTYPE, TTAG >( nullptr ); } \
			template< typename ... > \
			static constexpr bool function( ... ) { return false; } \
		}; \
		constexpr static bool value = inner< TYPE, TAG, AARG ... >::template function< TYPE, TAG, AARG ... >( nullptr ); \
	}; \
	template< typename TAG, typename ... ARG > \
	struct member_function_return_type< TYPE, TAG, ARG ... > \
	{ \
		template< typename, typename, typename ... > \
		struct member_function_return_type_inner \
		{ \
			template< typename TTYPE, typename TTAG, typename ... AARG > \
			static decltype( \
						helper< TTYPE >::template call_member_function< TTYPE, TTAG >( \
							std::declval< TTYPE >( ),\
							std::declval< AARG >( ) ... ) ) function( void * ); \
			template< typename ... > \
			static no_existence function( ... ); \
		}; \
		template< typename T1, typename T2 > \
		struct member_function_return_type_inner< T1, T2, void > \
		{ \
			template< typename TTYPE, typename TTAG > \
			static decltype( \
					helper< TTYPE >::template call_member_function< TYPE, TTAG >( std::declval< TTYPE >( ) ) ) function( ); \
			template< typename ... > \
			static no_existence function( ... ); \
		}; \
		typedef decltype( member_function_return_type_inner< TYPE, TAG, ARG ... >::template function< TYPE, TAG, ARG ... >( nullptr ) ) type; \
	}; \
	template< typename TAG, typename ... ARG > \
	struct call_member_function< TYPE, TAG, ARG ... > \
	{ \
		decltype( \
				helper< TYPE >::template call_member_function \
				< \
					TYPE, \
					TAG \
				>( std::declval< typename std::remove_reference< decltype( std::declval< TYPE >( ) ) >::type >( ), std::declval< ARG >( ) ... ) ) \
		operator ( )( const TYPE & t, const ARG & ... r ) \
		{ return helper< TYPE >::template call_member_function< TYPE, TAG, ARG ... >( t, r ... ); } \
	}; \
	template< typename TAG > \
	struct call_member_function< TYPE, TAG, void > \
	{ \
		decltype( helper< TYPE >::template call_member_function< TYPE, TAG >( ) ) \
		operator ( )( const TYPE & t ) \
		{ return helper< TYPE >::template call_member_function< TYPE, TAG >( t ); } \
	};
#endif //DECLARE_HPP
