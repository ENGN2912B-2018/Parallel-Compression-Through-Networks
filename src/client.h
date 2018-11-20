#pragma once

#include <thread>              // for ???
#include <string>              // for string
#include <cstring>             // for strlen
#include <deque>               // for deque
#include <cstddef>             // for size_t
#include <boost/asio.hpp>      // for boost::asio
#include <boost/bind.hpp>

using std::deque;
using std::string;
using std::size_t;

using namespace boost::asio;
using boost::system::error_code;

typedef boost::asio::ip::tcp::socket boost_socket;
typedef boost::asio::ip::tcp::resolver::iterator resolver_iterator;


// TODO(la): depending on our later representation of message values we should
// change these. Or we could this templated Message class and require using that.
typedef char* message;
typedef char* response;
typedef deque<message> message_queue;


// TODO(la): namespaces?
// TODO(la): seperate openning socket from creation?
// TODO(la): currently this client has a 'read()' private member function. So,
// what this class does with a message is fixed. Could add a 'Handler/Listener'
// object that will take the data that we read. After reading from the buffer, we
// just pass the value along



// NOTE(la): this code is based of a Boost official "chat" tutorial
// however, I am working on changing this to fufill our needs


/// A class representing our
class Client {
  public:
    //= Constructor =============================================================
    /// Constructs Client with an io_service for writing and iterator for finding the endpoint
    Client(io_service& io_service, resolver_iterator endpoint_iterator);

    //= Operations =============================================================
    /// Sends a message to the server.
    void write(const message& msg);

    /// Closes the socket.
    void close();

  private:
    /// Performs the actual 'send message' operation.
    void do_write();

    /// Reads any messages the socket recieves.
    void do_read();

    /// Connects the socket to the server.
    void do_connect(resolver_iterator endpoint_iterator);

    //= Member Variables =============================================================
    response res_msg_;         // store responses from server
    boost_socket socket_;      // socket for sending & receiving messages
    message_queue queue_;      // queue for sending message
    io_service &io_service_;   // io_service for socket
};
