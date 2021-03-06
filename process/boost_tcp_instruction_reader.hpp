////////////////////////////////////////////////////////////////////////////////
// 
//  This file is part of Functional Simulation Engine.
//
//  Functional Simulation Engine is free software: you can redistribute it
//  and/or modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation, either version 3 of the License,
//  or (at your option) any later version.
//
//  Functional Simulation Engine is distributed in the hope that it will be
//  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
//  Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with Functional Simulation Engine.
//  If not, see <https://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __LIBPROCESS_BOOST_SOCKET_HPP__
#define __LIBPROCESS_BOOST_SOCKET_HPP__

#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include <list>
#include <string>
#include <functional>
#include <memory>

namespace kms {

namespace process {

typedef std::function< bool( std::string ) > validate_n_add_func_t;

class tcp_connection
	: public std::enable_shared_from_this<tcp_connection>
{
public:
	void start();

private:
	void do_read();
	void do_write( const std::string & message );

public:
	template< typename P >
	tcp_connection( boost::asio::ip::tcp::socket, const P & );

private:
	boost::asio::ip::tcp::socket _socket;
	std::string _message;

private:
	validate_n_add_func_t _validate_and_add;

private:
	enum{ max_data_length=1024 };
	char _data[ max_data_length ];
};


class tcp_server {
public:
	template< typename P >
	tcp_server( boost::asio::io_service & io, uint32_t port, const P & v_n_add );

private:
	void do_accept();

private:
	boost::asio::ip::tcp::acceptor _acceptor;
	boost::asio::ip::tcp::socket _socket;

private:
	validate_n_add_func_t _validate_and_add;
};


class BoostTcpInstructionReader {
public:
	typedef int address_t;

public:
	BoostTcpInstructionReader( uint32_t port );

	~BoostTcpInstructionReader();

	bool validate_and_add( std::string );

	std::list< std::string > get_instructions();

private:
	boost::asio::io_service _io;
	tcp_server _srv;

	std::list< std::string > _instr;

	boost::mutex _mtx;
	boost::thread _run_thread;
};

}	//	namespace process

}	//	namespace kms

#endif
