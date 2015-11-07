#pragma once

#include <string>
#include <list>
#include "Uri.h"

namespace UriUtils {

	Uri createUri(const std::string& scheme, const std::string& host, 
		const std::string& path, const std::string& entity, 
		const std::list< std::pair<std::string, std::string> >& qparams) 
	{
		return Uri(scheme, host, path, entity, qparams);
	}
}