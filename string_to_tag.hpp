#ifndef STRING_TO_TAG_HPP
#define STRING_TO_TAG_HPP
#include <string>
#include <map>
#include <boost/preprocessor.hpp>
#include <functional>
struct unable_to_determine_type : std::runtime_error
{ unable_to_determine_type( ) : std::runtime_error( "Unable to determine the type." ) { } };
#define STRING_TO_TAG_BASE_HANDLER_HELPER( R, DATA, ELEMENT ) \
    virtual void string_to_tag_handle( const tag< ELEMENT > & ) const = 0;
#define STRING_TO_TAG_HANDLER_HELPER( R, DATA, ELEMENT ) \
    void string_to_tag_handle( const tag< ELEMENT > & e ) const { k( e ); }
#define STRING_TO_TAG_HANDLER( NAME_SEQ ) \
    struct string_to_tag_base_handler \
    { \
        virtual ~string_to_tag_base_handler( ) { } \
        BOOST_PP_SEQ_FOR_EACH( STRING_TO_TAG_BASE_HANDLER_HELPER, _, NAME_SEQ ) \
    }; \
    template< typename K > \
    struct string_to_tag_handler : string_to_tag_base_handler \
    { \
        const K & k; \
        BOOST_PP_SEQ_FOR_EACH( STRING_TO_TAG_HANDLER_HELPER, _, NAME_SEQ ) \
        string_to_tag_handler( const K & k ) : k( k ) { } \
    };

#define STRING_TO_TAG_HELPER( R, DATA, ELEMENT ) \
    tem.insert( std::make_pair( \
        std::string( BOOST_PP_STRINGIZE( ELEMENT ) ), \
        std::function< void( const string_to_tag_base_handler & ) >( \
            []( const string_to_tag_base_handler & x ) \
            { x.string_to_tag_handle( tag< ELEMENT >( ) ); } ) ) );
#define DECLARE_STRING_TO_TAG( NAME_SEQ ) \
    STRING_TO_TAG_HANDLER( NAME_SEQ ) \
    static \
        std::map \
        < \
            std::string, \
            std::function< void( const string_to_tag_base_handler & ) > \
        > & reflection_map( ) \
    { \
        static std::map \
        < \
            std::string, \
            std::function< void( const string_to_tag_base_handler & ) > \
        > ret_map( []( ) \
            { \
                std::map \
                < \
                    std::string, \
                    std::function< void( const string_to_tag_base_handler & ) > \
                > tem; \
                BOOST_PP_SEQ_FOR_EACH( STRING_TO_TAG_HELPER, _, NAME_SEQ ) \
                return tem; \
            }( ) ); \
        return ret_map; \
    } \
    template< typename K > \
    void string_to_tag( const std::string & str, const K & k ) \
    { \
        auto it = reflection_map( ).find( str ); \
        if ( it == reflection_map( ).end( ) ) \
        { throw unable_to_determine_type( ); } \
        it->second( string_to_tag_handler< K >( k ) ); \
    }
#endif // STRING_TO_TAG_HPP
