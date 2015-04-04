#ifndef HAS_CLASS_HPP
#define HAS_CLASS_HPP
#include <type_traits>
template< typename T >
struct has_class
{
    template< typename TT >
    static std::true_type function( decltype( std::declval< TT >( ).~TT( ) ) * );
    template< typename ... >
    static std::false_type function( ... );
    constexpr static bool value = decltype( function< T >( nullptr ) )::value;
};

#endif // HAS_CLASS_HPP
