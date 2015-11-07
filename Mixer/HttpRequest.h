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
#include <string>
#include "Uri.h"


namespace http
{
/*
	Stuff learned:
	-You must use io_service for the resolver and the socket
	-You resolve a query to get its TCP endpoint(which is an address:port)
	-You connect the socket to an endpoint iterator
	-You use the socket when reading/writing
		-write(socket, streambuf)
		-read_until(socket, streambuf, "\r\n")
	-To send sentences, you create a streambuf and encapsulate it
	within an istream or ostream object and then read/write to the i/ostream.
	The streambuf will then be filled up with data, that you can write/read_until
	-std::getline() takes an istream and a string to put the line in.
*/
// How to do wide characters?
	std::string GetPage(boost::asio::io_service& ios, const Uri& uri)
	{
		std::string xml_result;
		try {
			// Resolve the query
			boost::asio::ip::tcp::resolver::query query(uri.GetHost(), uri.GetScheme());
			boost::asio::ip::tcp::resolver resolver(ios);
			boost::asio::ip::tcp::resolver::iterator enditer = resolver.resolve(query);

			// Make a connection
			boost::asio::ip::tcp::socket socket(ios);
			boost::asio::connect(socket, enditer);

			// Request
				// Form the request
			boost::asio::streambuf request;
			std::ostream request_stream(&request);
			//std::ostringstream request_stream;
			request_stream << "GET " << uri.GetPath() << " HTTP/1.0\r\n"
				<< "Host: " << uri.GetHost() << "\r\n"
				<< "Accept: " << "*/*\r\n"
				<< "Connection: close\r\n\r\n";
			// Send the request
			boost::asio::write(socket, request);

			// Response
				/*
				// READ RESPONSE USING BUFFER
				boost::system::error_code error;
				boost::array<char, 512> response;
				while (std::size_t s = socket.read_some(buffer(response), error))
				result += std::string(response.begin(), response.begin() + s);
				if (error != boost::asio::error::eof)
				throw boost::system::system_error(error);
				*/
			boost::asio::streambuf response;
			// get first line
			boost::asio::read_until(socket, response, "\r\n");
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
			if (status_code != 200)
				throw std::runtime_error("Received status error " + status_code);
			// get the rest of the header(header will not be included, 
			// so I can just ignore this)
			boost::asio::read_until(socket, response, "\r\n");
			std::string header;
			while (getline(response_stream, header) && header != "\r") { }

			boost::system::error_code error;
			while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
				xml_result += std::string(std::istreambuf_iterator<char>(&response), std::istreambuf_iterator<char>());

			if (error != boost::asio::error::eof)
				throw std::runtime_error("Error not getting end-of-file");

		}
		catch (std::exception e) {
			throw std::exception("An error occurred");
		}
		
		return xml_result;
	}

	//byte[] GetBytes() { }
}