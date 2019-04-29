#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>

std::string User;
std::string Pass;

int User_Login (std::string User, std::string Pass)
{
    std::ifstream file;
    std::string Username, Password;
    int n = 0;
    file.open("User.txt");
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

void login()
{
	int m=500;
	char User2[m];
    	char Pass2[m];
	int loginattempts = 0;

	int height,width;
	height=10;
	width=49;

	std::ifstream Userfile;
    	Userfile.open("User.txt");

	while (User_Login(User2, Pass2) == 0)
            {
                loginattempts++;
                initscr();
		WINDOW *win3= newwin(height,width,10,45);	
		refresh();
		box(win3,0,0);

		mvwprintw(win3,1,2,"Login!! \n");
		mvwprintw(win3,2,2,"Enter your Username: \n");
        	wmove(win3,3,2);
		wgetnstr(win3,User2,m);
		mvwprintw(win3,4,2,"Enter your Password: \n");
        	

		wattron(win3,A_INVIS);
		wmove(win3,5,2);
		wattroff(win3,A_INVIS);
		wgetnstr(win3,Pass2,m);
		
		
		if (User_Login(User2, Pass2)!=0)
                    mvwprintw(win3,6,2,"Welcome: \n");
                else if (loginattempts == 3)
                {
                    mvwprintw(win3,7,2,"Maximum login attempts exceeded. \n");
                    wrefresh(win3);
		    getch();
		    refresh();
		    endwin();
		    break;
                }
                else
                {
                    mvwprintw(win3,8,2,"Invalid Username/Password combination \n");
                }
            	wrefresh(win3);
		getch();
		refresh();
		endwin();
	}
            Userfile.close();	


}


void Register()
{
    int n=500;
    std::ifstream file;
    std::ofstream newUser;
    char Username[n]; 
    char Password[n]; 
    char confirm[n];
    file.open("User.txt", std::ios::app);
    newUser.open("User.txt", std::ios::app);
    bool Uservalid = false;
    
     int height,width;
	height=10;
	width=49;

   while (!Uservalid)
    {
        
	initscr();
	WINDOW *win2= newwin(height,width,10,45);	
	refresh();
	box(win2,0,0);

	mvwprintw(win2,1,2,"Register!! \n");
	mvwprintw(win2,2,2,"Enter your Username: \n");
        wmove(win2,3,2);
	wgetnstr(win2,Username,n);
	//getline(std::cin, Username);
        

	mvwprintw(win2,4,2,"Enter your Password: \n");
	wmove(win2,5,2);
	
	wattron(win2,A_INVIS);
	wgetnstr(win2,Password,n);
        wattroff(win2,A_INVIS);

	mvwprintw(win2,6,2,"Enter your Password_c: \n");
        wmove(win2,7,2);
	
	wattron(win2,A_INVIS);
	wgetnstr(win2,confirm,n);
	wattroff(win2,A_INVIS);
	
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
    login();
}

int main()
{
    //int loginattempts = 0;
    std::ifstream Userfile;
    Userfile.open("User.txt");
    //std::string option;
    int option;
    if (!Userfile.is_open())
    {
        std::cout << "file not found" << std::endl;
    }
    else
    {
        int height,width;
	height=10;
	width=49;

	initscr();
	WINDOW *win= newwin(height,width,10,45);	
	refresh();
	box(win,0,0);
	
	wattron(win,A_BLINK);
	mvwprintw(win,1,1,"Welcome to the SuperChat");
	wattroff(win,A_BLINK);
	
	mvwprintw(win,2,1,"Please, register before trying to login");
	mvwprintw(win,3,1,"Enter one of the following option:");
	mvwprintw(win,4,1,"1. Register");
	mvwprintw(win,5,1,"2. Login");
	mvwprintw(win,6,1,"3. Exit");
	wmove(win,7,1);
	option=wgetch(win);

	getch();
	refresh();
	endwin();

        if (option == '2')
        {
	   login();
        }
        else if (option == '1')
        {
            Register();
        }
        else if (option == '3')
        {
            return 1;
        }
    }
    	
	return 0;
}
