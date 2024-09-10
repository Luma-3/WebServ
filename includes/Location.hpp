
#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <map>
#include <string>
#include <vector>

class Locations
{
  private:
	std::string						   _route;
	std::string						   _root;
	std::string						   _index;
	bool							   _autoindex;
	std::vector<std::string>		   _methods;
	std::map<std::string, std::string> _error_pages;
	std::map<std::string, std::string> _return;

  public:
	Locations();
	~Locations();
};

#endif // LOCATION_HPP
