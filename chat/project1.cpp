#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <deque>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"

using namespace std; //consider getting rid of this
using asio::ip::tcp;
typedef std::deque<chat_message> chat_message_queue;
class Cipher
{
	public:
	//string  msg;//Need to change chart 
	//int offset;

	string modify(string msg, int offset)
	{
		string result = "";
		for(int i = 0; i< msg.length(); i++)
		{
			if(isupper(msg[i]))
			{
				result +=char(int(msg[i]+offset-65)%26+65);
			}
			else if(islower(msg[i]))
			{
				result += char(int(msg[i]+offset-97)%26+97);
			}
			else 
			{
				result += msg[i];
			}
		}
		return result;
	}
	string decrypt(string msg, int offset)
	{
		string result = "";
		for(int i = 0; i<msg.length(); i++)
		{
			if(isupper(msg[i]))
			{
				result +=char(int(msg[i]-offset+65)%26+65);
			}
			else if(islower(msg[i]))
			{
				result+= char(int(msg[i]-offset+111)%26+97);
			}
			else 
			{
				result += msg[i];
			}
		}
		return result;
	}
	
};
class special_privileges
{
	public:
	int request;
	int people_on_server;

	void request_chat_session(Client a, Client b)
	{
		open_private_chat(Client a, Client b);//Need to integrate with the private chat class
	}
	void requent_delete_admin()
	{
		int r;
		request++;
		if(people_on_server>0)
		{
			//Need to initialize random number generator with srand(time(0));
			r = (rand() % people_on_server) + 1;
		}
		if(request>=people_on_server/4)
		{
			administrator->current = client_list[r]; //Reassigns the admin to a random person from the list of people currently on the server

			cout<<"The old admin is out, the new admin is "<<administrator->name
		}
		
		
	}
	
};

class Base_privileges
{
	public:
	chat_client(asio::io_context& io_context,
	 const tcp::resolver::results_type& endpoints)
	 : io_context_(io_context),
	socket_(io_context)
	{
		do_connect(endpoints);
	}

	void write(const chat_messages& msg)
	{
		asio::post(io_context_,
		[this,msg]()
		{
			bool write_in_progress = !write_msgs_.empty();
			write_msgs_.push_back(msg);
			if(!write_in_progress)
			{
				do_write();
			}
		});
	}
	private:
  	void do_connect(const tcp::resolver::results_type& endpoints)
 	{
    		asio::async_connect(socket_, endpoints,
        	[this](std::error_code ec, tcp::endpoint)
        	{
          		if (!ec)
          		{
            		do_read_header();
          		}
      	});
  	}
	void do_read_header()
  	{
   		 asio::async_read(socket_,
        	asio::buffer(read_msg_.data(), chat_message::header_length),
        	[this](std::error_code ec, std::size_t /*length*/)
        	{
          		if (!ec && read_msg_.decode_header())
          		{
           			do_read_body();
          		}
          		else
          		{
            			socket_.close();
        		}
        	});
  	}

	void do_read_body()
	{
   		asio::async_read(socket_,
	        asio::buffer(read_msg_.body(), read_msg_.body_length()),
      	 	[this](std::error_code ec, std::size_t /*length*/)
       	 	{
          	if (!ec)
         	{
            		std::cout.write(read_msg_.body(), read_msg_.body_length());
          		std::cout << "\n";
          		do_read_header();
          	}
          	else
          	{
            		socket_.close();
        	}
        });
  

  void do_write()
  {
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
        }
          else
          {
            socket_.close();
          }
        });
 }

private:
  asio::io_context& io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};





class Chat_Client: public Base_privileges
{
	public:
	void close()
	{
		//Redundant, consider deleting, there is already a leave function in the chat_room class
		asio::post(io_context_, [this]() {socket_.close(); });
	}
	
	void invite_to_chat(Client a)
	{
		
	}
	
};


int main()
{
	Cipher c;
	int a;
	string line;
	cout<<"PLease enter what you want encrypted: ";
	getline(cin, line);
	cout<<"Enter offset: ";
	cin>>a;
	//c.msg = line;
	//c.offset = a;
	string new_line = c.modify(line, a);
	cout<<"Encrypted: "<<new_line<<" "<<a;
	cout<<"\n";

	return 0;

} 
