#pragma once

#include <string>
#include <list>
#include <iostream>

/**
	A Uri is a resource identifier whose purpose 
	is to separate these five properties to make
	a proper http request:
	-Scheme (http, https, etc...)
	-Host	(www.google.com, etc...)
	-Path	(/ws/2/, etc...)
	-Entity (artist, recording, etc...) 
	-Params	(?query=thing1&limit=thing2, etc...)
*/
class Uri {
private:
	std::string scheme_;
	std::string host_;
	std::string path_;
	std::string entity_;
	std::list< std::pair<std::string, std::string> > params_;

	// Helper for constructor
	void ParseUri(const std::string& uri) {

		// find :// to get scheme (exclude ://)
		// if not found, assume http
		size_t scheme_divider_pos = uri.find("://");
		if (scheme_divider_pos != std::string::npos) {
			SetScheme(uri.substr(0, scheme_divider_pos));
		}
		else {
			SetScheme("http");
		}

		// find / to get host (exclude /)
		// if not found, then everything else is the host(and path, entity, everything is empty)
		size_t host_start = (scheme_divider_pos == std::string::npos) ? 0 : scheme_divider_pos + 3;
		size_t divider_pos = uri.find("/", host_start);
		if (divider_pos != std::string::npos) {
			SetHost(uri.substr(host_start, divider_pos - host_start));
		}
		else {
			SetHost(uri.substr(host_start, uri.size() - host_start));
			return;
		}
		// find last / to get path(include last /)
		size_t last_divider_pos = uri.rfind("/");
		if (last_divider_pos != std::string::npos) {
			SetPath(uri.substr(divider_pos, last_divider_pos - divider_pos + 1));
		}
		// find ? to get entity (exclude ?)
		// if not found, everything else is the entity
		size_t question_divider_pos = uri.find("?");
		if (question_divider_pos != std::string::npos) {
			SetEntity(uri.substr(last_divider_pos + 1, question_divider_pos - last_divider_pos - 1));
		}
		else {
			SetEntity(uri.substr(last_divider_pos + 1, uri.size() - last_divider_pos - 1));
			return;
		}

		// while find & to get params (exclude &)
		// if not found, everything else is the param
		// if found, set new
		typedef std::pair< std::string, std::string > strpair;
		std::list<strpair> params;

		size_t param_start = question_divider_pos + 1;
		size_t amp_divider_pos = uri.find("&");
		while (amp_divider_pos != std::string::npos) {
			params.push_back(ParseParam(uri.substr(param_start, amp_divider_pos - param_start)));
			param_start = amp_divider_pos + 1;
			amp_divider_pos = uri.find("&", param_start);
		}
		params.push_back(ParseParam(uri.substr(param_start, uri.size() - param_start)));
		SetParams(params);
	}

	std::pair<std::string, std::string> ParseParam(const std::string& param) {
		size_t equal_pos = param.find("=");
		std::string name = param.substr(0, equal_pos);
		std::string value = param.substr(equal_pos + 1, param.size() - equal_pos - 1);
		return std::pair<std::string, std::string>(name, value);
	}

public:
	// Constructors
	Uri() { }
	Uri(const std::string& uri) { ParseUri(uri); }
	Uri(const std::string& scheme, const std::string& host, const std::string& path, 
		const std::string& entity, const std::list< std::pair<std::string, std::string> >& params) 
		: scheme_(scheme), host_(host), entity_(entity), path_(path), params_(params) { }
	
	// Setters
	void SetScheme(const std::string& scheme) { scheme_ = scheme; }
	void SetHost(const std::string& host) { host_ = host; }
	void SetPath(const std::string& root_path) { path_ = root_path; }
	void SetEntity(const std::string& entity) { entity_ = entity; }
	void SetParams(const std::list< std::pair<std::string,std::string> >& params) { params_ = params; }

	void AppendParam(const std::pair<std::string, std::string>& param) {
		params_.push_back(param);
	}

	// Getters
	std::string GetScheme() const { return scheme_; }
	std::string GetHost() const { return host_; }
	std::list< std::pair<std::string, std::string> > GetParams() const { return params_; }
	// Get path necessary for http request
	std::string GetPath() const { 
		typedef std::list< std::pair<std::string, std::string> >::const_iterator list_iter;
		
		std::string full_path = path_ + entity_;
		list_iter i = params_.begin();
		if (!params_.empty()) {
			full_path += "?";
			full_path += (i->first) + "=" + (i->second);
			++i;
			while (i != params_.end()) {
				full_path += "&" + (i->first) + "=" + (i->second);
				++i;
			}
		}
		
		return full_path;
	}
	std::string GetFullPath() const {
		return scheme_ + "://" + host_ + GetPath();
	}

};