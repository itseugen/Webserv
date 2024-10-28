#include "utils.h"


std::vector<std::string>	tokenize(const std::string& str, const std::string& delim)
{
	std::vector<std::string>	tokens;
	size_t	start = 0;
	size_t	end = str.find(delim);

	while (end != std::string::npos)
	{
		tokens.push_back(str.substr(start, end - start));
		start = end + 1;
		end = str.find(delim, start);
	}

	tokens.push_back(str.substr(start));

	return (tokens);
}

bool ends_with(const std::string& str, const std::string& suffix)
{
	return str.size() >= suffix.size() && 
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string clean_file_path(const std::string& url)
{
	size_t query_pos = url.find('?');
	if (query_pos != std::string::npos)
	{
		return url.substr(0, query_pos); // Return the part before the '?'
	}
	return url; // Return the original URL if no '?' is found
}