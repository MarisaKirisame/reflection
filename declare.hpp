#ifndef DECLARE_HPP
#define DECLARE_HPP
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include "member_function.hpp"
#include "static_function.hpp"
#include "member_variable.hpp"
#include "static_variable.hpp"
#include "get_typename.hpp"
#define DECLARE_NAME( NAME ) struct BOOST_PP_CAT( NAME, _tag ){ }
#define DECLARE_NAMES_HELPER( R, DATA, ELEMENT ) DECLARE_NAME( ELEMENT );
#define DECLARE_NAMES( NAME_SEQ ) BOOST_PP_SEQ_FOR_EACH( DECLARE_NAMES_HELPER, _, NAME_SEQ )
#define DECLARE_TYPE( TYPE, NAME_SEQ ) \
	typedef BOOST_PP_CAT( TYPE, _tag ) tag; \
	template< typename ... > \
	constexpr static bool has_member_function( ... ) { return false; } \
	template< typename ... > \
	constexpr static bool has_static_function( ... ) { return false; } \
	template< typename ... > \
	constexpr static bool has_member_variable( ... ) { return false; } \
	template< typename ... > \
	constexpr static bool has_static_variable( ... ) { return false; } \
	template< typename ... > \
	void invoke_member_variable( ... ) { } \
	template< typename K > \
	void invoke_all_member_variable( const K & k ) \
	{\
		INVOKE_ALL_MEMBER_VARIABLE( k, NAME_SEQ );\
	}\
	DECLARE_ALL_POSSIBLE( NAME_SEQ )
#define DECLARE_POSSIBLE( NAME ) \
	DECLARE_POSSIBLE_MEMBER_FUNCTION( NAME ); \
	DECLARE_POSSIBLE_MEMBER_VARIABLE( NAME ); \
	DECLARE_POSSIBLE_STATIC_FUNCTION( NAME ); \
	DECLARE_POSSIBLE_STATIC_VARIABLE( NAME ); \
	DECLARE_POSSIBLE_GET_TYPENAME( NAME )
#define DECLARE_ALL_POSSIBLE_HELPER( R, DATA, ELEMENT ) DECLARE_POSSIBLE( ELEMENT );
#define DECLARE_ALL_POSSIBLE( NAME_SEQ ) BOOST_PP_SEQ_FOR_EACH( DECLARE_ALL_POSSIBLE_HELPER, _, NAME_SEQ )
#endif //DECLARE_HPP
