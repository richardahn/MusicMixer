#pragma once

#include <string>

namespace Constants {

	const std::string ROOT_PATH = "C:/Users/Richard/Desktop/";
	const int SONG_DURATION_LIMIT_RADIUS = 5000; // in milliseconds

	namespace Musicbrainz {
		const std::string API_SCHEME = "http";
		const std::string API_HOST = "musicbrainz.org";
		const std::string API_PATH = "/ws/2/";
		const std::string ARTIST = "artist";
		const std::string RECORDING = "recording";
		const std::string API_SEARCH;
		const unsigned int MAX_RESULTS = 10;
		const unsigned int PER_PAGE = 5; // also can be used as offset

	}

	namespace Soundcloud {
		const std::string API_SCHEME = "http";
		const std::string API_HOST = "api.soundcloud.com";
		const std::string API_PATH = "/";
		const std::string TRACKS = "tracks";
		const std::string CLIENT_ID = "client_id";
		const std::string QUERY = "q";
		const std::string CLIENT_ID_VALUE = "0da2f9b333a9144c0496d8c7969abeff"; // Unique client id for me only
		const std::string DURATION_FROM = "duration[from]";
		const std::string DURATION_TO = "duration[to]";
		const int MAX_RESULTS = 10;
		const int PER_PAGE = 5;
	}

	namespace Youtube {
		const std::string API_HOST = "www.googleapis.com";
		const std::string API_SCHEME = "https";
		const int MAX_RESULTS = 1000;
		const std::string SAFE_SEARCH = "none";
		const std::string TYPE = "video";
		const std::string KIND_VIDEO = "youtube#video";
		const std::string VIDEO_URL = "https://www.youtube.com/watch?v=%s";
		const int PER_PAGE = 50;
		const std::string API_SEARCH = "youtube/v3/search";
		const std::string API_TOKEN = "";

	}

	namespace Backend {
		const std::string API_HOST = "youtube-dl-api.appspot.com";
		const std::string API_SCHEME = "http";
	}


}