#pragma once

#include "SongDownloadTask.h"
#include "Constants.h"
#include "HttpRequest.h"
#include "Utils.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "HttpsRequest.h"

class SoundcloudTask : public SongDownloadTask {
public:
	// remember that you cannot instantiate base class members with the initializer list
	// you can however, assign it in the body, but that is unnecessary and inefficient
	SoundcloudTask(boost::asio::io_service& ios) : SongDownloadTask(ios) { }
	virtual void DownloadSong(const Song& song) {
		Uri download_uri = GetDownloadUri(song);
		Utils::AppendSoundcloudClientId(download_uri);
		std::cout << download_uri.GetFullPath() << std::endl;
		https::GetBytes(ios_, download_uri, Constants::ROOT_PATH + song.GetTitle() + "_by_" + song.GetArtist()+".mp3");
	}
private:
	Uri GetDownloadUri(const Song& song) {
		
		// get title
		std::string song_title = Utils::FixStringQuery(song.GetTitle() + " " + song.GetArtist());

		// get json
		std::string song_json = http::GetPage(ios_, Utils::GetSoundcloudUri(Constants::Soundcloud::TRACKS, song_title));

		// Setup tree
		boost::property_tree::ptree pt;
		std::istringstream is(song_json);
		boost::property_tree::read_json(is, pt);

		unsigned int lower_song_length_limit = song.GetSongDuration() - Constants::SONG_DURATION_LIMIT_RADIUS;
		unsigned int higher_song_length_limit = song.GetSongDuration() + Constants::SONG_DURATION_LIMIT_RADIUS;
		typedef std::pair<std::string, std::string> strpair;
		strpair duration_from = strpair(Constants::Soundcloud::DURATION_FROM, std::to_string(lower_song_length_limit));
		strpair duration_to = strpair(Constants::Soundcloud::DURATION_TO, std::to_string(higher_song_length_limit));

		// Read objects
		for (auto i : pt) {
			if (IsValidNode(i.second, song)) {
				// return download uri
				Uri download_uri = Uri(ParseJsonNodeForDownloadUri(i.second));
				download_uri.AppendParam(duration_from);
				download_uri.AppendParam(duration_to);
				return download_uri;
			}
		}
		return Uri();
		//throw std::runtime_error("No suitable song found");
	}

	//TODO
	std::string ParseJsonNodeForDownloadUri(const boost::property_tree::ptree& node) {
		std::string download_uri = node.get<std::string>("stream_url");
		return download_uri;
	}

	//TODO(also implement DuplicateUtils and fix the list of songs to remove duplicates)
	bool IsValidNode(const boost::property_tree::ptree& node, const Song& song) {
		

		return true;
	}

};