#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

# include "../webserv.h"

# ifdef __APPLE__
#  define NOEXCEPT_SPEC _NOEXCEPT
# else
#  define NOEXCEPT_SPEC noexcept
# endif

class BaseException : public std::exception
{
	protected:
		std::string			message;
	public:
		explicit			BaseException(const std::string& msg);
		virtual const char*	what() const NOEXCEPT_SPEC override;
};

class SocketCreationFailedException : public BaseException
{
	public:
		explicit SocketCreationFailedException(const std::string& server_name);
};

class SetSocketOptionFailedException : public BaseException
{
	public:
		explicit SetSocketOptionFailedException(const std::string& server_name);
};

class InvalidPortException : public BaseException
{
	public:
		explicit InvalidPortException(const std::string& server_name, int port);
};

class BindFailedException : public BaseException
{
	public:
		explicit BindFailedException(const std::string& server_name, int port);
};

class ListenFailedException : public BaseException
{
	public:
		explicit ListenFailedException(const std::string& server_name);
};

class OpenFailedException : public BaseException
{
	public:
		explicit OpenFailedException(const std::string& server_name, const std::string& file_path);
};

class PollFailedException : public BaseException
{
	public:
		explicit PollFailedException(void)
		: BaseException("Poll failed") {}
};

class InterruptSignal : public BaseException
{
	public:
		explicit InterruptSignal(void)
		: BaseException("Kill Signal received, shutting down!") {}
};

#endif /*EXCEPTIONS_HPP*/