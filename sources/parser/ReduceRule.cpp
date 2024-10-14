/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReduceRule.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrousse <jbrousse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 23:40:41 by jbrousse          #+#    #+#             */
/*   Updated: 2024/10/14 11:05:32 by jbrousse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stack>

#include "parser/Parser.hpp"

using parser::Parser;

void TakeTo(std::stack< IParserToken * > &dst,
			std::stack< IParserToken * > &stack,
			bool (*breaker)(const IParserToken &))
{
	while (!stack.empty()) {
		if (breaker(*stack.top())) {
			break;
		}
		dst.push(stack.top());
		stack.pop();
	}
	dst.push(stack.top());
	stack.pop();
}

bool R1Breaker(const IParserToken &token)
{
	return (token.getTerminal() == T_Server);
}

bool R6Breaker(const IParserToken &token)
{
	return (token.getTerminal() == T_Location);
}

void Parser::R1_Server()
{
	std::stack< IParserToken * >  tokens;
	std::vector< IParserToken * > params;

	TakeTo(tokens, _parse_stack, R1Breaker);

	while (!tokens.empty()) {
		if (tokens.top()->getType() != TOKEN) {
			_current->addParam(tokens.top()->getKey(),
							   D_Cast< Param >(tokens.top()));
		}
		else {
			delete tokens.top();
		}
		tokens.pop();
	}
	if (_current->getParam("listen") == NULL) {
		throw Token::MissingParamException("listen");
	}
	if (_current->getParam("hostname") == NULL) {
		Param *param = new Param(
			"hostname", _current->getParam("listen")->getPair().first + ":" +
							_current->getParam("listen")->getPair().second);
		_current->addParam("hostname", param);
	}

	_servers.push_back(_current);
	_current = new VirtualServer;
}

void Parser::R2_Param()
{
	std::vector< IParserToken * > tokens;

	for (size_t i = 0; i < 3; ++i) {
		tokens.push_back(_parse_stack.top());
		_parse_stack.pop();
	}

	Terminal_Type term_type = tokens[2]->getTerminal();
	std::string	  value = tokens[1]->getKey();

	if (term_type == T_Hostname) {
		if (!IsHostname(value)) {
			throw Token::InvalidTokenException();
		}
	}
	else if (term_type == T_BodySize) {
		if (!IsBodySize(value)) {
			throw Token::InvalidTokenException();
		}
	}

	Param *param = new Param(tokens[2]->getKey(), value);

	for (size_t i = 0; i < tokens.size(); ++i) {
		delete tokens[i];
	}
	_parse_stack.push(param);
}

void Parser::R3_DoubleParam()
{
	std::stack< IParserToken * > tokens;

	TakeTo(tokens, _parse_stack, Token::IsKey);

	Terminal_Type term_type = tokens.top()->getTerminal();
	std::string	  key = tokens.top()->getKey();

	delete tokens.top();
	tokens.pop();

	std::string value1 = tokens.top()->getKey();
	delete tokens.top();
	tokens.pop();

	std::string value2;

	if (term_type == T_Listen) {
		if (!IsHostname(value1) && !IsIP(value1)) {
			throw Token::InvalidTokenException();
		}
		delete tokens.top();
		tokens.pop();

		value2 = tokens.top()->getKey();
		delete tokens.top();
		tokens.pop();

		if (!IsPort(value2)) {
			throw Token::InvalidTokenException();
		}
	}
	else {
		value2 = tokens.top()->getKey();
		delete tokens.top();
		tokens.pop();
	}
	delete tokens.top();
	tokens.pop();

	if (term_type == T_Return) {
		if (!IsErrorCode(value1)) {
			throw Token::InvalidTokenException();
		}
	}

	Param *param = new Param(key, value1, value2);

	_parse_stack.push(param);
}

void Parser::R4_ErrorPage()
{
	std::stack< IParserToken * > tokens;

	TakeTo(tokens, _parse_stack, Token::IsKey);

	std::string				   value;
	std::vector< std::string > errorCode;

	while (!tokens.empty()) {
		if (tokens.top()->getTerminal() == T_Digits) {
			if (!IsErrorCode(tokens.top()->getKey())) {
				throw Token::InvalidTokenException();
			}
			errorCode.push_back(tokens.top()->getKey());
		}
		else if (tokens.top()->getTerminal() == T_Identifier) {
			value = tokens.top()->getKey();
		}
		delete tokens.top();
		tokens.pop();
	}

	std::vector< std::string >::const_iterator it = errorCode.begin();

	while (it != errorCode.end()) {
		Param *param = new Param(*it, value);
		_parse_stack.push(param);
		++it;
	}
}

void Parser::R5_DenyMethod()
{
	std::stack< IParserToken * > tokens;
	std::vector< std::string >	 method;

	TakeTo(tokens, _parse_stack, Token::IsKey);

	while (!tokens.empty()) {
		if (tokens.top()->getTerminal() == T_Method) {
			method.push_back(tokens.top()->getKey());
		}
		delete tokens.top();
		tokens.pop();
	}

	Param *deniedMethod = new Param("deny_method", method);
	_parse_stack.push(deniedMethod);
}

void Parser::R6_Location()
{
	std::stack< IParserToken * > tokens;

	TakeTo(tokens, _parse_stack, R6Breaker);
	delete tokens.top();
	tokens.pop();

	std::string route = tokens.top()->getKey();
	delete tokens.top();
	tokens.pop();

	Location *location = new Location();

	while (!tokens.empty()) {
		if (tokens.top()->getType() != TOKEN) {
			location->addParam(tokens.top()->getKey(),
							   D_Cast< Param >(tokens.top()));
		}
		else {
			delete tokens.top();
		}
		tokens.pop();
	}
	_current->addLocation(route, location);
}