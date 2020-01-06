#include <stdlib.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <thread>
#include <chrono>
#include <iostream>
using namespace boost;
using namespace std;
void writeToSocket(asio::ip::tcp::socket& sock, std::string& buf) {  
	std::size_t total_bytes_written = 0;
	while (total_bytes_written != buf.length()) {
		total_bytes_written += sock.write_some(
			asio::buffer(buf.c_str() +
			total_bytes_written,
			buf.length() - total_bytes_written));
	}
}

bool flag = true; 


void ClientThread(string clientName) 
{
	std::string raw_ip_address = "127.0.0.1"; 
	unsigned short port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address),port_num); 
	asio::io_service ios;
	asio::ip::tcp::socket sock(ios, ep.protocol()); 
	sock.connect(ep); 

	
	string strToServ = clientName;  
	cout << "To server: " << strToServ << endl; 
	writeToSocket(sock, strToServ); 
	
	boost::array<char, 256> buf; 
	size_t len = sock.read_some(boost::asio::buffer(buf), error); 
	buf[len] = 0; 
	cout  << "From server: " << buf.data(); 
	while (flag) 
	{
		switch (rand() % 3 + 1) 
		{
		case 1:	strToServ = "ping\n"; break;
		case 2:	strToServ = "clients\n"; break;
		case 3:	strToServ = "Hernауауцya\n"; break;
		}
		cout << "To server: " << strToServ ; 
		writeToSocket(sock, strToServ); 
		// Получаем ответ сервера
		size_t len = sock.read_some(boost::asio::buffer(buf), error); 
		buf[len] = 0; 
		cout << "From server: " << buf.data() ; 
		this_thread::sleep_for(chrono::seconds(1)); 
	}

	
	sock.shutdown(asio::socket_base::shutdown_send);
	sock.close();
	
}


void main()
{
	
	
	cout << "For start thread, press Enter!\n";
	cout << "For exit, press Enter!\n";
	cin.get(); 
	thread t1(ClientThread, "MyClient145");
	cout << "Thread1 Start Ok!!!!!\n";
	this_thread::sleep_for(chrono::seconds(5)); 
	thread t2(ClientThread, "FghghhhMyClient");
	cout << "Thread2 Start Ok!!!!!\n";
	cin.get(); 
	flag = false; 
	t1.join();
	t2.join();
	cout << "Client is end!\n";
	::system("pause");

}
