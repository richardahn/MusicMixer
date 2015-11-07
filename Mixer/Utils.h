#pragma once

#include "Uri.h"
#include "UriUtils.h"
#include "Constants.h"
#include <string>

namespace Utils {

	// replace spaces with underscores
	std::string FixStringQuery(const std::string& query) {
		std::string fixed_query;
		for (std::string::const_iterator i = query.begin(); i != query.end(); ++i) {
			if (*i == ' ') {
				fixed_query += "%20"; // encoded space character
			}
			else {
				fixed_query += *i;
			}
		}
		return fixed_query;
	}

	
	Uri GetSoundcloudUri(const std::string& entity, const std::string& query) {
		typedef std::pair<std::string, std::string> strpair;
		std::list<strpair> qparams;
		qparams.push_back(strpair(Constants::Soundcloud::CLIENT_ID, Constants::Soundcloud::CLIENT_ID_VALUE));
		qparams.push_back(strpair(Constants::Soundcloud::QUERY, query));

		return UriUtils::createUri(Constants::Soundcloud::API_SCHEME, Constants::Soundcloud::API_HOST,
			Constants::Soundcloud::API_PATH, entity, qparams);
	}

	void AppendSoundcloudClientId(Uri& uri) {
		typedef std::pair<std::string, std::string> strpair;
		std::list< std::pair<std::string, std::string> > params = uri.GetParams();
		params.push_back(strpair(Constants::Soundcloud::CLIENT_ID, Constants::Soundcloud::CLIENT_ID_VALUE));
		uri.SetParams(params);
	}
	

	// Format: path/entity?queryname=queryvalue...
	Uri GetMusicbrainzUri(const std::string& entity, const std::string& query) {
		typedef std::pair<std::string, std::string> strpair;

		// Determine the GET parameters depending upon the entity request
			// Maybe instead of the chained if's, just push_back(strpair(entity,query))
		std::list<strpair> qparams;
		if (entity == Constants::Musicbrainz::ARTIST) {
			qparams.push_back(strpair("query", query));
			qparams.push_back(strpair("limit", std::to_string(Constants::Musicbrainz::MAX_RESULTS)));
		}
		else if (entity == Constants::Musicbrainz::RECORDING) {
			qparams.push_back(strpair("artist", query)); // query == mbid
			qparams.push_back(strpair("limit", std::to_string(Constants::Musicbrainz::MAX_RESULTS)));
		}

		return UriUtils::createUri(Constants::Musicbrainz::API_SCHEME,
			Constants::Musicbrainz::API_HOST, Constants::Musicbrainz::API_PATH,
			entity, qparams);


	}
}