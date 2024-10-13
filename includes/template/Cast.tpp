
#ifndef CAST_TPP
#define CAST_TPP

template < typename T > T *D_Cast(IParserToken *token)
{
	T *cast = dynamic_cast< T * >(token);
	if (!cast) {
		throw std::runtime_error("Invalid cast");
	}
	return cast;
};

#endif // CAST_TPP
