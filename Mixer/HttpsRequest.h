#pragma once


#include <boost\asio\ip\tcp.hpp>
#include <boost\asio.hpp>
#include "Uri.h"
#include <boost\asio\ssl.hpp>
#include <boost\bind.hpp>
#include <boost\filesystem\fstream.hpp>
#include <boost\filesystem\path.hpp>
#include <fstream>
#include <string>




namespace https
{
	
	void GetBytes(boost::asio::io_service& ios, const Uri& uri) {
		using namespace boost::asio;
		// (1) Make a connection
		ip::tcp::resolver::query query(uri.GetHost(), uri.GetScheme());
		ip::tcp::resolver resolver(ios);
		ip::tcp::resolver::iterator enditer = resolver.resolve(query);

		ssl::context ctx(ios, ssl::context::sslv23);
		// (2) Send cryptographic info
			// Send the SSL version number, cipher settings, and misc. info
		// make client
		ssl::stream<ip::tcp::socket> socket(ios, ctx);
		try {
		connect(socket.lowest_layer(), enditer);
		socket.lowest_layer().set_option(ip::tcp::no_delay(true));
		socket.set_verify_mode(ssl::context::verify_none);	
		socket.handshake(ssl::stream<ip::tcp::socket>::client);
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
			std::cout << std::endl;
		}

		// Read and write as normal
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		//std::ostringstream request_stream;
		request_stream << "GET " << uri.GetPath() << " HTTP/1.1\r\n"
			<< "Host: " << uri.GetHost() << "\r\n"
			<< "Accept: " << "*/*\r\n" 
			<< "Connection: close\r\n\r\n";
		// Send the request
		write(socket, request);
		streambuf response;
		// get first line
		read_until(socket, response, "\r\n");
		// use a stream to read it
		std::istream response_stream(&response);
		// Read in status 
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		// check status
		if (http_version.substr(0, 5) != "HTTP/")
			throw std::runtime_error("Bad version of HTTP");

		read_until(socket, response, "\r\n");
		std::string header;
		while (getline(response_stream, header) && header != "\r") {
			// if redirect
			if (header.substr(0, 8) == "Location") {
				std::cout << header << std::endl;

			}
		}

		boost::system::error_code error;
		while (read(socket, response, transfer_at_least(1), error))
			std::cout << std::string(std::istreambuf_iterator<char>(&response), std::istreambuf_iterator<char>()) << std::endl;

		if (error != boost::asio::error::eof)
			throw std::runtime_error("Error not getting end-of-file");
	}
	
	void GetBytes(boost::asio::io_service& ios, const Uri& uri, const std::string& file_loc) {
		using namespace boost::asio;
		// (1) Make a connection
		ip::tcp::resolver::query query(uri.GetHost(), uri.GetScheme());
		ip::tcp::resolver resolver(ios);
		ip::tcp::resolver::iterator enditer = resolver.resolve(query);

		ssl::context ctx(ios, ssl::context::sslv23);
		ssl::stream<ip::tcp::socket> socket(ios, ctx);
		try {
			connect(socket.lowest_layer(), enditer);
			socket.lowest_layer().set_option(ip::tcp::no_delay(true));
			socket.set_verify_mode(ssl::context::verify_none);
			socket.handshake(ssl::stream<ip::tcp::socket>::client);
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
			std::cout << std::endl;
		}

		// Read and write as normal

		// WRITE
		streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << uri.GetPath() << " HTTP/1.1\r\n"
			<< "Host: " << uri.GetHost() << "\r\n"
			<< "Accept: */*\r\n"
			<< "Connection: close\r\n\r\n";
		write(socket, request);
		// READ
		boost::filesystem::ofstream song_ostream;
		song_ostream.open(file_loc, std::ios::binary);

		boost::system::error_code error;
		boost::array<char, 2048> response;

		streambuf response_header;
		std::istream response_header_stream(&response_header);
		read_until(socket, response_header, "\r\n\r\n");
		std::string header;
		while (getline(response_header_stream, header)) {	
			if (header.substr(0, 8) == "Location") {
				std::string redirect_loc = header.substr(10, header.size() - 11);
				return GetBytes(ios, Uri(redirect_loc), file_loc);
			}
		}

		size_t total = 0;
		while (std::size_t s = socket.read_some(buffer(response), error)) {
			total += s;
			//std::cout << std::string(response.begin(), response.end());
			song_ostream.write(response.data(), s);
		}
		std::cout << total << std::endl;


		if (error != boost::asio::error::eof)
			throw boost::system::system_error(error);
		song_ostream.close();
	}

}
