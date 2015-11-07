#pragma once
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <array>
#include <string>
#include <iterator>
#include "Song.h"

// Abstract
class SongNamesFetcher {
public:
	// Constructors
	SongNamesFetcher(boost::asio::io_service& ios) : ios_(ios) { }

	// Methods
	// FetchArtistNames
	virtual std::vector<Song> FetchSongNames(const std::string&) = 0;
protected:
	boost::asio::io_service& ios_;
};
