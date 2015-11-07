#pragma once


#include <boost/asio/io_service.hpp>
#include "SongNamesFetcher.h"
#include "MusicbrainzFetcher.h"

/**
* This class is a handler for the song list fetchers
*/
class SongNamesFetcherHandler {
public:
	/**
	* Constructor
	* 
	* @param ios object necessary for asynchronous io services
	*/
	SongNamesFetcherHandler(boost::asio::io_service& ios) : ios_(ios), sf_(new MusicbrainzFetcher(ios_)) { }

	/**
	* Deconstructor
	* 
	* Safely deletes the fetcher pointer
	*/
	~SongNamesFetcherHandler() { delete sf_; }

	// Fetches songs for a given artist
	/// <param name="artist">Artist from which I will pick my songs</param>
	/// <returns>Returns a list of songs</returns>
	std::vector<Song> FetchSongNames(const std::string& artist) {
		try {
			return sf_->FetchSongNames(artist);
		}
		catch (std::exception) {
			// TODO add exception
			std::cout << "FAILED TO FETCH" << std::endl;
		}
	}

private:
	// Can't assign/copy in adherance to non-copyable io_service
	SongNamesFetcherHandler& operator=(const SongNamesFetcherHandler& snfh);
	SongNamesFetcherHandler(const SongNamesFetcherHandler& snfh);

	boost::asio::io_service &ios_;
	SongNamesFetcher *sf_;


};