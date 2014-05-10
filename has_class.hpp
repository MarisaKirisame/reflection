#ifndef HAS_CLASS_HPP
#define HAS_CLASS_HPP
template< typename TYPE >
struct has_class
{
	template< typename ... >
	constexpr static bool has_class_inner( ... ) { return false; }
	template< typename T >
	constexpr static bool has_class_inner( typename T::tag * ) { return true; }
	constexpr static bool value = has_class_inner< TYPE >( nullptr );
	constexpr bool operator( )( ... ) { return value; }
};
#endif // HAS_CLASS_HPP
