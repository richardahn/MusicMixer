#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>
#include "Uri.h"

class FailedFetchException : public std::runtime_error {
public:
	FailedFetchException(const Uri& uri) : uri_(uri), runtime_error("Failed to fetch") { }
	virtual const char* what() const throw()
	{
		cnvt.str("");
		cnvt << runtime_error::what() << ": " << "Could not fetch from "
			<< uri_.GetFullPath();
		return cnvt.str().c_str();
	}

	std::string getUri() const { return uri_.GetFullPath(); }

private:
	Uri uri_;
	static std::ostringstream cnvt;
};

std::ostringstream FailedFetchException::cnvt;