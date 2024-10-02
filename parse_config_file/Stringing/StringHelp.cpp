
#include "StringHelp.hpp"

//	checks if position is inside a comment, comment being anything betweedd '#' and the next '\n'
//	in files ending with .conf, vscode also marks anything after ';' to the end of the line as a comment (makes it green)
//		but here it only considers '#'
bool		StringHelp::is_comment(std::string str, size_t pos)
{
	size_t hash = str.rfind('#', pos);
	if (hash != -1)
	{
		size_t newl = str.find('\n', hash);
		if (pos < newl)
			return true;
	}
	return false;
}


//	setting ignore means that anything found inside ignore will be ignored and the next result will be searched for
size_t		StringHelp::find_ignore(std::string str, size_t pos, char c, char ignore)
{
	size_t p = std::string::npos;
	bool isSQ = false, isDQ = false;
	do
	{
		p = str.find(c, pos);
		if ((ignore & FIND_IGNORE_COMMENT) && is_comment(str, p))
			pos = p + 1;
		else if (ignore & (FIND_IGNORE_SINGLE_QUOTE | FIND_IGNORE_DOUBLE_QUOTE))
		{
			StringDataTracker::trackQuotes(str, pos, p, isSQ, isDQ);
			if ((ignore & FIND_IGNORE_SINGLE_QUOTE) && isSQ)
				pos = p + 1;
			else if ((ignore & FIND_IGNORE_DOUBLE_QUOTE) && isDQ)
				pos = p + 1;
			else
				break;
		}
		else
			break;
	}
	while (p != std::string::npos);
	return (p);
}
size_t		StringHelp::find_ignore(std::string str, size_t pos, std::string s, char ignore)
{
	size_t p = std::string::npos;
	bool isSQ = false, isDQ = false;
	do
	{
		p = str.find(s, pos);
		if ((ignore & FIND_IGNORE_COMMENT) && is_comment(str, p))
			pos = p + 1;
		else if (ignore & FIND_IGNORE_QUOTE)
		{
			StringDataTracker::trackQuotes(str, pos, p, isSQ, isDQ);
			if ((ignore & FIND_IGNORE_SINGLE_QUOTE) && isSQ)
				pos = p + 1;
			else if ((ignore & FIND_IGNORE_DOUBLE_QUOTE) && isDQ)
				pos = p + 1;
			else
				break;
		}
		else
			break;

	}
	while (p != std::string::npos);
	return (p);
}



std::string	StringHelp::cut(std::string str, size_t from, size_t to)
{
	return str.substr(from, to - from);
}
std::string	StringHelp::cut(std::string str, size_t from)
{
	return str.substr(from);
}





bool			StringHelp::is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}
bool			StringHelp::only_whitespace(std::string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (!is_whitespace(str[i]))
			return (false);
	}
	return (true);
}
std::string		StringHelp::trim_whitespace(std::string str)
{
	std::string trim = "";
	bool last_was_white = false;
	for (int i = 0; i < str.length(); i++)
	{
		if (is_whitespace(str[i]))
		{
			if (!last_was_white)
			{
				trim += ' ';
				last_was_white = true;
			}
		}
		else
		{
			last_was_white = false;
			trim += str[i];
		}
	}
	return (trim);
}



std::string		StringHelp::remove_comments(std::string str)
{
	std::string trim;
	size_t	p1, p2;

	p1 = 0;
	p2 = find_ignore(str, p1, '#');
	while (p2 != std::string::npos)
	{
		trim += cut(str, p1, p2);
		p1 = find_ignore(str, p2, '\n');
		p2 = find_ignore(str, p1, '#');
	}
	trim += cut(str, p1);

	return (trim);
}
