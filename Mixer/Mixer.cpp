
#include "MusicbrainzFetcher.h"
#include "SongNamesFetcher.h"
#include "SongNamesFetcherHandler.h"
#include "SoundcloudTask.h"
#include "HttpsRequest.h"
#include <boost\filesystem\fstream.hpp>

extern "C" {
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
}

using namespace boost::asio;
using namespace boost::asio::ip;

int main(int argc, char* argv[])
{
	io_service ios;
	// have a handler for this
	SongNamesFetcherHandler snfh(ios);


	



	// Get list of songs
	std::vector<Song> songs = snfh.FetchSongNames("SNSD"); // Works for Sia, BIGBANG, SNSD, One Direction
	for (Song s : songs) {
		std::cout << s.ToString() << std::endl;
	}
	// Fix list of songs
	
	// Download list of songs from Soundcloud, Youtube, etc...
	// have a handler to keep track of the count
	SongDownloadTask *sdt = new SoundcloudTask(ios);
	sdt->DownloadSong(songs[0]);

	// Output the files in an mp3
	//SongUtils::combineSongs("output/");

	// Make a webpage that allows users to enter in a name and get an mp3 in return

	
}






































/*
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>

const std::string domain = "en.wikipedia.org";
const std::string path = "/w/api.php";
const std::string service = "https";


class client
{
public:
	client(boost::asio::io_service& io_service, boost::asio::ssl::context& context, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
		: socket_(io_service, context)
	{
		socket_.set_verify_mode(boost::asio::ssl::context::verify_none);
		socket_.set_verify_callback(boost::bind(&client::verify_certificate, this, _1, _2));

		boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator, boost::bind(&client::handle_connect, this, boost::asio::placeholders::error));
	}

	bool verify_certificate(bool preverified, boost::asio::ssl::verify_context& ctx)
	{
		char subject_name[256];
		X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
		X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
		std::cout << "Verifying:\n" << subject_name << std::endl;

		return preverified;
	}

	void handle_connect(const boost::system::error_code& error)
	{
		if (!error) {
			std::cout << "Connection OK!" << std::endl;
			socket_.async_handshake(boost::asio::ssl::stream_base::client, boost::bind(&client::handle_handshake, this, boost::asio::placeholders::error));
		}
		else {
			std::cout << "Connect failed: " << error.message() << std::endl;
		}
	}

	void handle_handshake(const boost::system::error_code& error)
	{
		if (!error) {
			std::cout << "Sending request: " << std::endl;

			std::stringstream request_;

			request_ << "GET " << path << " HTTP/1.1\r\n";
			request_ << "Host: " << domain << "\r\n";
			request_ << "Accept-Encoding: *\r\n";
			request_ << "\r\n";

			std::cout << request_.str() << std::endl;

			boost::asio::async_write(socket_, boost::asio::buffer(request_.str()), boost::bind(&client::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}
		else {
			std::cout << "Handshake failed: " << error.message() << std::endl;
		}
	}

	void handle_write(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (!error) {
			std::cout << "Sending request OK!" << std::endl;
			boost::asio::async_read(socket_, boost::asio::buffer(reply_, bytes_transferred), boost::bind(&client::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}
		else {
			std::cout << "Write failed: " << error.message() << std::endl;
		}
	}

	void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (!error) {
			std::cout << "Reply: ";
			std::cout.write(reply_, bytes_transferred);
			std::cout << "\n";
			boost::asio::async_read(socket_, boost::asio::buffer(reply_, bytes_transferred), boost::bind(&client::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}
		else {
			std::cout << "Read failed: " << error.message() << std::endl;
		}
	}

private:
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
	char reply_[0x1 << 16];
};

int main(int argc, char* argv[])
{
	try {
		boost::asio::io_service io_service;

		boost::asio::ip::tcp::resolver resolver(io_service);
		boost::asio::ip::tcp::resolver::query query(domain, service);
		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

		boost::asio::ssl::context context(boost::asio::ssl::context::sslv23);
		context.load_verify_file("C:\\Users\\Richard\\Downloads\\cacert.pem");

		client c(io_service, context, iterator);

		io_service.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	std::cin.get();
	return 0;
}

*/