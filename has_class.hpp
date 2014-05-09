#ifndef HAS_CLASS_HPP
#define HAS_CLASS_HPP
template< typename ... >
constexpr bool has_class( ... ) { return false; }
template< typename T >
constexpr bool has_class( typename T::tag * ) { return true; }
#define HAS_CLASS( CLASS ) ( has_class< CLASS >( nullptr ) )
#endif // HAS_CLASS_HPP
