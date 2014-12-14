#ifndef STATIC_FUNCTION_HPP
#define STATIC_FUNCTION_HPP
#include "reflection.hpp"
#include <boost/preprocessor.hpp>
#define DECLARE_POSSIBLE_STATIC_FUNCTION( NAME ) \
    template< typename SELF, typename T, typename ... R > \
    constexpr static bool has_static_function( \
        typename std::enable_if \
        < \
            std::is_same \
            < \
                T, \
                ::tag< NAME > \
            >::value, \
            typename std::add_pointer \
            < \
                decltype( SELF::NAME( std::declval< R >( ) ... ) ) \
            >::type \
        >::type ) \
    { return true; } \
    template< typename SELF, typename T, typename ... R > \
    typename std::enable_if \
    < \
        std::is_same \
        < \
            T, \
            ::tag< ::NAME > \
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
                helper< TYPE >::template call_static_function< TYPE, NAME >(
                    std::declval< ARG >( ) ... ) ) function( void * );
        template< typename ... >
        static no_existence function( ... );
    };
    template< typename TTYPE, typename NNAME >
    struct inner< TTYPE, NNAME, void >
    {
        template< typename TYPE, typename NAME, typename ... >
        static decltype(
                helper< TYPE >::template call_static_function< TYPE, NAME >( ) ) function( void * );
        template< typename ... >
        static no_existence function( ... );
    };
    typedef decltype(
            inner
            <
                TTTYPE,
                NNNAME,
                AAARG ...
            >::template function< TTTYPE, NNNAME, AAARG ... >( nullptr ) ) type;
};
template< typename TYPE, typename TAG, typename ... AARG >
struct has_static_function
{
    template< typename, typename, typename ... >
    struct inner
    {
        template< typename TTYPE, typename TTAG, typename ... ARG >
        static constexpr typename
        std::enable_if
        <
            helper< TTYPE >::template has_static_function< TTYPE, TTAG, ARG ... >( nullptr ) || true,
            bool
        >::type function( typename std::enable_if< has_class< TTYPE >::value >::type * )
        { return helper< TTYPE >::template has_static_function< TTYPE, TAG, ARG ... >( nullptr ); }
        template< typename ... >
        static constexpr bool function( ... ) { return false; }
    };
    template< typename T1, typename T2 >
    struct inner< T1, T2, void >
    {
        template< typename TTYPE, typename TTAG >
        static constexpr bool function( typename std::enable_if< has_class< TYPE >::value >::type * )
        { return helper< TTYPE >::template has_static_function< TTYPE, TTAG >( nullptr ); }
        template< typename ... >
        static constexpr bool function( ... ) { return false; }
    };
    constexpr static bool value =
            inner< TYPE, TAG, AARG ... >::template function< TYPE, TAG, AARG ... >( nullptr );
};
template< typename TYPE, typename NAME, typename ... ARG >
struct call_static_function
{
    decltype( helper< TYPE >::template call_static_function< TYPE, NAME >( std::declval< ARG >( ) ... ) )
    operator ( )( const ARG & ... r )
    { return helper< TYPE >::template call_static_function< TYPE, NAME >( r ... ); }
};
template< typename TYPE, typename NAME >
struct call_static_function< TYPE, NAME, void >
{
    decltype( helper< TYPE >::template call_static_function< NAME, TYPE >( ) )
    operator ( )( )
    { return helper< TYPE >::template call_static_function< NAME, TYPE >( ); }
};
#define DECLARE_ALL_POSSIBLE_STATIC_FUNCTION_HELPER( R, DATA, ELEMENT ) \
    DECLARE_POSSIBLE_STATIC_FUNCTION( ELEMENT )
#define DECLARE_ALL_POSSIBLE_STATIC_FUNCTION( NAME_SEQ ) \
    BOOST_PP_SEQ_FOR_EACH( DECLARE_ALL_POSSIBLE_STATIC_FUNCTION_HELPER, _, NAME_SEQ )
#endif //STATIC_FUNCTION_HPP
