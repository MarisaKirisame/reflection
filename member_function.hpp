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
            ! ::has_static_function< SELF, TAG, ARG ... >::value, \
            typename std::add_pointer \
            < \
                decltype( std::declval< SELF * >( )->NAME( std::declval< ARG >( ) ... ) ) \
            >::type \
        >::type ) \
    { return true; } \
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
    call_member_function( SELF & t, const R & ...  r ) { return t.NAME( r ... ); } \
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
template< typename TYPE, typename TTAG, typename ... AARG >
struct has_member_function
{
    template< typename, typename, typename ... >
    struct inner
    {
        template< typename TTYPE, typename TAG, typename ... ARG >
        static constexpr typename
        std::enable_if
        <
            helper< TTYPE >::template has_member_function< TTYPE, TAG, ARG ... >( nullptr ) || true,
            bool
        >::type function( typename std::enable_if< has_class< TTYPE >::value >::type * )
        { return helper< TTYPE >::template has_member_function< TTYPE, TAG, ARG ... >( nullptr ); }
        template< typename ... >
        static constexpr bool function( ... )
        { return false; }
    };
    template< typename T1, typename T2 >
    struct inner< T1, T2, void >
    {
        template< typename TTYPE, typename TAG, typename ... ARG >
        static constexpr typename
        std::enable_if
        <
            helper< TTYPE >::template has_member_function< TYPE, TAG >( nullptr ) || true,
            bool
        >::type function( typename std::enable_if< has_class< TTYPE >::value >::type * )
        { return helper< TTYPE >::template has_member_function< TYPE, TAG >( nullptr ); }
        template< typename ... >
        static constexpr bool function( ... )
        { return false; }
    };
    constexpr static bool value =
            inner< TYPE, TTAG, AARG ... >::template function< TYPE, TTAG, AARG ... >( nullptr );
};
template< typename TYPE, typename TAG, typename ... ARG >
struct member_function_return_type
{
    template< typename, typename, typename ... >
    struct member_function_return_type_inner
    {
        template< typename TTYPE, typename TTAG, typename ... AARG >
        static decltype(
                    helper< TTYPE >::template call_member_function< TTYPE, TTAG >(
                        std::declval< TTYPE >( ),
                        std::declval< AARG >( ) ... ) ) function( void * );
        template< typename ... >
        static no_existence function( ... );
    };
    template< typename T1, typename T2 >
    struct member_function_return_type_inner< T1, T2, void >
    {
        template< typename TTYPE, typename TTAG >
        static decltype(
                helper< TTYPE >::template call_member_function< TYPE, TTAG >(
                    std::declval< TTYPE >( ) ) ) function( );
        template< typename ... >
        static no_existence function( ... );
    };
    typedef decltype( member_function_return_type_inner< TYPE, TAG, ARG ... >::template
                      function< TYPE, TAG, ARG ... >( nullptr ) ) type;
};
template< typename TYPE, typename TAG, typename ... ARG >
struct call_member_function
{
    decltype(
            helper< TYPE >::template call_member_function
            <
                TYPE,
                TAG
            >( std::declval
               <
                    typename std::remove_reference< decltype( std::declval< TYPE >( ) ) >::type >( ),
                    std::declval< ARG >( ) ... ) )
    operator ( )( TYPE & t, const ARG & ... r ) const
    { return helper< TYPE >::template call_member_function< TYPE, TAG, ARG ... >( t, r ... ); }
    decltype(
            helper< TYPE >::template call_member_function
            <
                TYPE,
                TAG
            >( std::declval
               <
                    typename std::remove_reference< decltype( std::declval< TYPE >( ) ) >::type >( ),
                    std::declval< ARG >( ) ... ) )
    operator ( )( const TYPE & t, const ARG & ... r ) const
    { return helper< TYPE >::template call_member_function< TYPE, TAG, ARG ... >( t, r ... ); }
};
template< typename TYPE, typename TAG >
struct call_member_function< TYPE, TAG, void >
{
    decltype( helper< TYPE >::template call_member_function< TYPE, TAG >( ) )
    operator ( )( const TYPE & t ) const
    { return helper< TYPE >::template call_member_function< TYPE, TAG >( t ); }
    decltype( helper< TYPE >::template call_member_function< TYPE, TAG >( ) )
    operator ( )( TYPE & t ) const
    { return helper< TYPE >::template call_member_function< TYPE, TAG >( t ); }
};
#define DECLARE_ALL_POSSIBLE_MEMBER_FUNCTION_HELPER( R, DATA, ELEMENT ) \
    DECLARE_POSSIBLE_MEMBER_FUNCTION( ELEMENT )
#define DECLARE_ALL_POSSIBLE_MEMBER_FUNCTION( NAME_SEQ ) \
    BOOST_PP_SEQ_FOR_EACH( DECLARE_ALL_POSSIBLE_MEMBER_FUNCTION_HELPER, _, NAME_SEQ );
#endif //MEMBER_FUNCTION_HPP
