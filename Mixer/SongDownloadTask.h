#pragma once

#include "Song.h"
#include <boost/asio/io_service.hpp>

class SongDownloadTask {
public:
	SongDownloadTask(boost::asio::io_service& ios) : ios_(ios) { }
	virtual void DownloadSong(const Song&) = 0; // returns filePathName
protected:
	boost::asio::io_service& ios_;
};
