#ifndef STRING_TO_TAG_HPP
#define STRING_TO_TAG_HPP
#include <string>
#include <map>
struct unable_to_determine_type : std::runtime_error
{ unable_to_determine_type( ) : std::runtime_error( "Unable to determine the type." ) { } };
#define STRING_TO_TAG_HELPER( R, DATA, ELEMENT ) \
    if ( str == tag< ELEMENT >::name( ) ) \
    { \
        k( tag< ELEMENT >( ) );\
        return; \
    }
#define DECLARE_STRING_TO_TAG( NAME_SEQ ) \
    template< typename K > \
    void string_to_tag( const std::string & str, const K & k ) \
    { \
        BOOST_PP_SEQ_FOR_EACH( STRING_TO_TAG_HELPER, _, NAME_SEQ )	\
        throw unable_to_determine_type( ); \
    }
#endif // STRING_TO_TAG_HPP
