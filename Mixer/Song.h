#pragma once

#include <string>

class Song {
public:
	Song(const std::string& id, const std::string& title, const std::string& artist, 
		unsigned int song_duration) : id_(id), title_(title), artist_(artist),
		song_duration_(song_duration) { }
	Song() : song_duration_(0) { }

	std::string GetId() const { return id_; }
	std::string GetTitle() const { return title_; }
	std::string GetArtist() const { return artist_; }
	unsigned int GetSongDuration() const { return song_duration_; }
	std::string ToString() const { return "Title: " + title_ + " by " + artist_; }

private:
	std::string id_;
	std::string title_;
	std::string artist_;
	unsigned int song_duration_;
};