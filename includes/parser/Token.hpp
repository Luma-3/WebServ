#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum Token_Type {
	None,
	Key,
	Value,
	Comma,
	Semi_Colon,
	Bracket,
	Colone,
	Equal
};

class Token
{
  private:
	enum Token_Type _type;
	std::string		_value;

  public:
	Token();
	Token(std::string value, enum Token_Type type);

	enum Token_Type getType();
	std::string		getValue();

	Token &operator=(const Token &src);
	virtual ~Token();
};

#endif // TOKEN_HPP