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
#define DECLARE_HELPER( TYPE, NAME_SEQ ) \
template< > \
struct helper< TYPE > \
{ \
	DECLARE_ALL_POSSIBLE_STATIC_VARIABLE( NAME_SEQ ) \
	DECLARE_ALL_POSSIBLE_MEMBER_VARIABLE( NAME_SEQ ) \
	DECLARE_ALL_POSSIBLE_STATIC_FUNCTION( NAME_SEQ ) \
	DECLARE_ALL_POSSIBLE_MEMBER_FUNCTION( NAME_SEQ ) \
	template< typename SELF, typename K > \
	static void \
	invoke_all_member_variable( SELF * t, const K & k ) \
	{ \
		INVOKE_ALL_MEMBER_VARIABLE( k, NAME_SEQ ) \
	} \
};
#define DECLARE_ALL_HELPER( R, DATA, ELEMENT ) DECLARE_HELPER( ELEMENT, DATA )
#define DECLARE_ALL( NAME_SEQ ) BOOST_PP_LIST_FOR_EACH( DECLARE_ALL_HELPER, NAME_SEQ, BOOST_PP_SEQ_TO_LIST( NAME_SEQ ) )
#endif //DECLARE_HPP
