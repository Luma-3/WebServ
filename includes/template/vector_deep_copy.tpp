
#include <vector>

template < typename T >
std::vector< T * > vector_deep_copy(const std::vector< T * > &src)
{
	std::vector< T * > dst;
	for (typename std::vector< T * >::const_iterator it = src.begin();
		 it != src.end(); ++it)
		dst.push_back(*it);
	return dst;
}

template < typename T >
std::vector< T > vector_deep_copy(const std::vector< T > &src)
{
	std::vector< T > dst;
	for (typename std::vector< T >::const_iterator it = src.begin();
		 it != src.end(); ++it)
		dst.push_back(T(*it));
	return dst;
}