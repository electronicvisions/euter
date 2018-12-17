#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <iostream>

namespace euter {

/* Not-supported-on-hardware exception */
class NotSupportedOnHW : public std::exception {
public:
    NotSupportedOnHW() :
	    std::exception(),  mMsg()
	{}

    NotSupportedOnHW(std::string const & msg) :
	    std::exception(), mMsg(msg)
	{}

    virtual ~NotSupportedOnHW() throw() {}

    virtual char const * what() const throw() { return mMsg.c_str(); }
private:
    const std::string mMsg;
};

/// throw to signal that the feature is not implemented in our pynn version
class NotImplemented : public std::exception {
public:
    NotImplemented() :
		std::exception(),
		mMsg("need more PhD-stundents (aka unimplemented feature, sry)")
	{}
	NotImplemented(const std::string & file, const std::string & func, size_t line) :
		std::exception(),
	    mMsg(format(file, func, line))
	{
	}

    virtual ~NotImplemented() throw() {}

    virtual char const * what() const throw() { return mMsg.c_str(); }

private:
	std::string format(const std::string & file, const std::string & func, size_t line)
	{
		std::stringstream s;
		s << "need more PhD-stundents (aka unimplemented feature, in file '"
		  << func << " in '" << file << ":" << line << "'";
		return s.str();
	}

    const std::string mMsg;
};

class InvalidDimensions : public std::exception
{
public:
    InvalidDimensions(std::string message)
    {
        this->message = message;
    }

    ~InvalidDimensions() throw() {}

    const char* what() const throw() { return this->message.c_str(); }
private:
    std::string message;
};

} // namespace euter

#define NOT_IMPLEMENTED() do { throw euter::NotImplemented( __FILE__, __PRETTY_FUNCTION__, __LINE__ ); } while(0)
