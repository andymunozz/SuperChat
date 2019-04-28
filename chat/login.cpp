#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>

std::string User;
std::string Pass;



void Register()
{
    
    std::ifstream file;
    std::ofstream newUser;
    int n=500;
    char Username[n]; 
    char Password[n]; 
    char confirm[n];
    file.open("User.txt", std::ios::app);
    newUser.open("User.txt", std::ios::app);
    bool Uservalid = false;
    
     int height,width;
	height=20;
	width=49;

   while (!Uservalid)
    {
        
	initscr();
	WINDOW *win2= newwin(height,width,10,75);	
	refresh();
	box(win2,0,0);

	mvwprintw(win2,1,2,"Enter your Username: \n");
        wmove(win2,2,2);
	wgetnstr(win2,Username,n);
	//getline(std::cin, Username);
        

	mvwprintw(win2,3,2,"Enter your Password: \n");
	//std::cout << "Enter your Password: ";
	wmove(win2,4,2);
	wgetnstr(win2,Password,n);
	//getline(std::cin, Password);
        

	mvwprintw(win2,5,2,"Enter your Password_c: \n");
	//std::cout << "Confirm your Password: ";
        wmove(win2,6,2);
	wgetnstr(win2,confirm,n);
	//getline(std::cin, Password_c);
	
	wrefresh(win2);
	refresh();
	endwin();

        int m=0;
        int k=0;

        while (file >> User >> Pass)
        {
            m++;
            if (Username!=User)
                k++;
        }
        if (/*m == k &&*/ Password != confirm)
            Uservalid = true;

        else if (m != k)
            //mvwprintw(win2,7,2,"This Username already exist \n");
		std::cout << "This Username already exist." << std::endl;
        else
          //mvwprintw(win2,8,2,"The Passwords given do not match \n");  
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
    return 0;
}

int main()
{
    int loginattempts = 0;
    std::ifstream Userfile;
    Userfile.open("User.txt");
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
        else if (option == "1")
        {
            Register();
        }
        else if (option == "3")
        {
            return 1;
        }
    }
    return 0;
}
