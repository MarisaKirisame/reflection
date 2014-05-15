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
#endif //DECLARE_HPP
