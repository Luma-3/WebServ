/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReduceRule.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthony <anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 23:40:41 by jbrousse          #+#    #+#             */
/*   Updated: 2024/11/13 09:37:46 by anthony          ###   ########.fr       */
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

void TakeTo(std::stack< Token * > &dst, std::stack< IParserToken * > &stack,
			bool (*breaker)(const IParserToken &))
{
	while (!stack.empty()) {
		if (breaker(*stack.top())) {
			break;
		}
		dst.push(dynamic_cast< Token * >(stack.top()));
		stack.pop();
	}
	dst.push(dynamic_cast< Token * >(stack.top()));
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
	std::stack< IParserToken * >		tokens;
	const std::vector< IParserToken * > params;

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
		throw Parser::MissingParamException("listen");
	}
	if (_current->getParam("hostname") == NULL) {
		Param *param = new Param(
			"hostname", _current->getParam("listen")->getPair().first + ":" +
							_current->getParam("listen")->getPair().second);
		_current->addParam("hostname", param);
	}

	_servers.push_back(_current);
	_current = new VirtualServer();
}

void Parser::R2_Param()
{
	std::vector< Token * > tokens;

	for (size_t i = 0; i < 3; ++i) {
		tokens.push_back(dynamic_cast< Token * >(_parse_stack.top()));
		_parse_stack.pop();
	}

	const Terminal_Type term_type = tokens[2]->getTerminal();
	const std::string	value = tokens[1]->getKey();

	if (term_type == T_Hostname) {
		if (!IsHostname(value)) {
			throw Token::InvalidTokenException(
				"hostname not correctly formatted",
				LIME "\nExemple:" RESET " example.com", value,
				tokens[1]->getCol(), tokens[1]->getLine());
		}
	}
	else if (term_type == T_BodySize) {
		if (!IsBodySize(value)) {
			throw Token::InvalidTokenException(
				"body_size not correctly formatted",
				LIME "\nSize must be:" RESET
					 " a number and follow by 'k/K' or 'm/M' or 'g/G'",
				value, tokens[1]->getCol(), tokens[1]->getLine());
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
	std::stack< Token * > tokens;

	TakeTo(tokens, _parse_stack, Token::IsKey);

	const Terminal_Type term_type = tokens.top()->getTerminal();
	const std::string	key = tokens.top()->getKey();

	delete tokens.top();
	tokens.pop();

	const std::string value1 = tokens.top()->getKey();
	size_t			  tmp_line = tokens.top()->getLine();
	size_t			  tmp_col = tokens.top()->getCol();
	delete tokens.top();
	tokens.pop();

	std::string value2;

	if (term_type == T_Listen) {
		if (!IsHostname(value1) && !IsIP(value1)) {
			throw Token::InvalidTokenException(
				"hostname or IP address not correctly formatted",
				LIME "\nExemple:" RESET " example.com / 127.0.0.1 ", value1,
				tmp_col, tmp_line);
		}
		delete tokens.top();
		tokens.pop();

		value2 = tokens.top()->getKey();
		tmp_col = tokens.top()->getCol();
		tmp_line = tokens.top()->getLine();
		delete tokens.top();
		tokens.pop();

		if (!IsPort(value2)) {
			throw Token::InvalidTokenException("port not correctly formatted",
											   LIME "\nExemple:" RESET
													" 0 to 65535",
											   value2, tmp_col, tmp_line);
		}
	}
	else if (tokens.size() > 1) {
		value2 = tokens.top()->getKey();
		delete tokens.top();
		tokens.pop();
	}
	delete tokens.top();
	tokens.pop();

	if (term_type == T_Return) {
		if (!IsErrorCode(value1)) {
			throw Token::InvalidTokenException(
				"return not correctly formatted",
				LIME "\nExemple:" RESET
					 "1xx, 2xx, 3xx, 4xx, 5xx (x is a digit)",
				value1, tmp_col, tmp_line);
		}
	}
	Param *param = new Param(key, value1, value2);

	_parse_stack.push(param);
}

void Parser::R4_ErrorPage()
{
	std::stack< Token * > tokens;

	TakeTo(tokens, _parse_stack, Token::IsKey);

	std::string				   value;
	std::vector< std::string > errorCode;

	while (!tokens.empty()) {
		if (tokens.top()->getTerminal() == T_Digits) {
			if (!IsErrorCode(tokens.top()->getKey())) {
				throw Token::InvalidTokenException(
					"error code not correctly formatted",
					LIME "\nExemple:" RESET
						 " 1xx, 2xx, 3xx, 4xx, 5xx (x is a digit)",
					tokens.top()->getKey(), tokens.top()->getCol(),
					tokens.top()->getLine());
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

void Parser::R7_CGI()
{
	std::stack< Token * > tokens;

	std::string cgi_path;
	std::string cgi_extension;

	TakeTo(tokens, _parse_stack, Token::IsKey);

	while (!tokens.empty()) {
		if (tokens.top()->getTerminal() == T_Identifier) {
			cgi_path = tokens.top()->getKey();
		}
		else if (tokens.top()->getTerminal() == T_CGIExtension) {
			cgi_extension = tokens.top()->getKey();
		}
		delete tokens.top();
		tokens.pop();
	}

	Param *cgi = new Param(cgi_extension, cgi_path);

	_parse_stack.push(cgi);
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

	const std::string route = tokens.top()->getKey();
	delete tokens.top();
	tokens.pop();

	Location *location = new Location(route);

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