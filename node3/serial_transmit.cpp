#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/serial_port_base.hpp>
#include <boost/asio/streambuf.hpp>

#define BAUD_RATE 115200
using namespace std;
using namespace boost::asio;

io_service io;
serial_port *sp_ptr;
boost::asio::streambuf buf;

void serial_init(std::string port){
    serial_port sp(io, port);
    sp_ptr = &sp;
    sp.set_option(serial_port_base::baud_rate(BAUD_RATE));
    sp.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));
    sp.set_option(serial_port_base::parity(serial_port_base::parity::none));
    sp.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
    sp.set_option(serial_port_base::character_size(8));
    boost::asio::streambuf buf;
}

int serial_send(std::string msg){
    boost::asio::write(*sp_ptr, buffer(msg));
    string data = buffer_cast<const char*>(buf.data());
    cout << data << endl;
    return 0;
}