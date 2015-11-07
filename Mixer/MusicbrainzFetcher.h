#pragma once

#include "MusicbrainzFetcher.h"
#include "Utils.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/array.hpp>
#include "pugixml.hpp"
#include "HttpRequest.h"
#include "Song.h"
#include "SongNamesFetcher.h"

/*
	Issue: if i have a cpp file for a separate implementation, i get a LNK2005 error, already defined
*/
class MusicbrainzFetcher : public SongNamesFetcher {

public:
	// Constructor
	MusicbrainzFetcher(boost::asio::io_service& ios) : SongNamesFetcher(ios) { }

	// Methods
	virtual std::vector<Song> FetchSongNames(const std::string& artist) {
		// Get songs in xml
		std::vector<Song> songs = GetSongs(artist);
		return songs;
	}
private:
	// maybe have artist here
	std::string GetArtistMbid(const std::string& artist) {
		// Get artist xml
		std::string artist_xml = http::GetPage(ios_, Utils::GetMusicbrainzUri(Constants::Musicbrainz::ARTIST, artist));

		// Attach xml to the dom
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string(artist_xml.c_str());

		// Locate artist id where ext:score="100"(a.k.a best match)
		pugi::xml_node artists_list_node = doc.child("metadata").child("artist-list");
		std::string mbid = artists_list_node.find_child_by_attribute("artist", "ext:score", "100").attribute("id").value();

		return mbid;
	}
	std::vector<Song> GetSongs(const std::string& artist) {
		// Get artist(and make sure artist has underscores and no spaces)
		std::string artist_mbid = GetArtistMbid(Utils::FixStringQuery(artist));

		// Create the dom for the songs xml
		std::string songs_xml = http::GetPage(ios_, Utils::GetMusicbrainzUri(Constants::Musicbrainz::RECORDING, artist_mbid));
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string(songs_xml.c_str());

		// Find the list of songs in the xml
		pugi::xml_node songs_list_node = doc.child("metadata").child("recording-list");

		// For each song in the xml, parse the necessary values into a Song object
		std::vector<Song> songs;
		pugi::xml_node song_node = songs_list_node.first_child();

		while (song_node) {
			// Optionally, I can use a for loop and loop through attributes
			std::string id = song_node.attribute("id").value();
			std::string title = song_node.child("title").child_value();
			std::string length = "0";
			if (song_node.child("length")) {
				std::string length = song_node.child("length").child_value(); // milliseconds
			}

			songs.push_back(Song(id, title, artist, std::stoul(length)));
			song_node = song_node.next_sibling();
		}

		return songs;
	}

};