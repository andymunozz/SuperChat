//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "chat_message.hpp"
#include <fstream>
#include <string>
#include <ncurses.h>

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;



class Login{

public:


std::string User;
std::string Pass;

void Register()
{
    std::ifstream file;
    std::ofstream newUser;
    std::string Username, Password, Password_confirm;
    file.open("Users.txt", std::ios::app);
    newUser.open("Users.txt", std::ios::app);
    bool Uservalid = false;
    while (!Uservalid)
    {
        std::cout << "\nEnter your Username: ";
        getline(std::cin, Username);
        std::cout << "Enter your Password: ";
        getline(std::cin, Password);
        std::cout << "Confirm your Password: ";
        getline(std::cin, Password_confirm);

        int m=0;
        int k=0;

        while (file >> User >> Pass)
        {
            m++;
            if (Username!=User)
                k++;
        }
        if (m == k && Password == Password_confirm)
            Uservalid = true;

        else if (m != k)
            std::cout << "This Username already exist." << std::endl;
        else
            std::cout << "The Passwords given do not match." << std::endl;
    }
    newUser << Username << " " << Password << std::endl;;
    file.close();
    newUser.close();
}

int User_Login (std::string User, std::string Pass)
{
    std::ifstream file;
    std::string Username, Password;
    int n = 0;
    file.open("Users.txt");
    if (file.is_open())
    {
        while (!file.eof())
        {
            file >> Username >> Password;
            n++;
            if (User == Username && Pass == Password)
                return n;
        }
    }
    else
    {
        std::cout << "file not open" << std::endl;
    }
    //return;
}

void login()
{
	std::ifstream Userfile;
	Userfile.open("Users.txt");
	std::string User;
    	std::string Pass;
	int loginattempts = 0;


 while (User_Login(User, Pass) == 0)
            {
                loginattempts++;
                std::cout << "\nUsername: ";
                std::cin >> User;
                std::cout << "Password: ";
                std::cin >> Pass;
                if (User_Login(User, Pass)!=0)
                    std::cout << "\nWelcome " << User << "." << std::endl;
                else if (loginattempts == 3)
                {
                    std::cout << "Maximum login attempts exceeded." << std::endl;
                    break;
                }
                else
                {
                    std::cout << "Invalid Username/Password combination" << std::endl;
                }
            }
            Userfile.close();


}


void main2()
{
    int loginattempts = 0;
    std::ifstream Userfile;
    Userfile.open("Users.txt");
    std::string option;
    if (!Userfile.is_open())
    {
        std::cout << "file not found" << std::endl;
    }
    else
    {
        std::cout << "Welcome to the SuperChat" << std::endl;
        std::cout << "Please, register before trying to login" << std::endl;
        std::cout << "Enter one of the following option: " << std::endl;
        std::cout << "\n1. Register \n2. Login \n3. Exit\n" << "Your choice: ";
        getline(std::cin, option);

        if (option == "2")
        {
           login();
        }
        else if (option == "1")
        {
            Register();
        }
        else if (option == "3")
        {
            return;
        }
    }
    return;

}

};




class chat_client
{
public:
  chat_client(asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    do_connect(endpoints);
  }

  void write(const chat_message& msg)
  {
    asio::post(io_context_,
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
  }

  void close()
  {
    asio::post(io_context_, [this]() { socket_.close(); });
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
  }

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

int main(int argc, char* argv[])
{
  Login login1;
  login1.main2();

try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    chat_client c(io_context, endpoints);

    std::thread t([&io_context](){ io_context.run(); });

    char line[chat_message::max_body_length + 1];
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
      chat_message msg;
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);
    }

    c.close();
    t.join();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
