#ifndef TOKEN_HPP
#define TOKEN_HPP

enum Token_Type {
	BlockValue
};

class Token
{
  private:
	enum Token_Type _type;

  public:
	Token();
	enum Token_Type getType();
	virtual ~Token();
};

#endif // TOKEN_HPP