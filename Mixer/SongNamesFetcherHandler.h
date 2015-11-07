#pragma once


#include <boost/asio/io_service.hpp>
#include "SongNamesFetcher.h"
#include "MusicbrainzFetcher.h"

class SongNamesFetcherHandler {
public:
	SongNamesFetcherHandler(boost::asio::io_service& ios) : ios_(ios), sf_(new MusicbrainzFetcher(ios_)) { }
	~SongNamesFetcherHandler() { delete sf_; }

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