#include <ncurses.h>
#include <string>
#include <iostream>
#include "chat_message.hpp"
#include "login.h"
using namespace std;



void lobby()
{
	int height,width;
	height=10;
	width=49;

	initscr();
	WINDOW *win4= newwin(height,width,10,75);	
	refresh();
	box(win4,0,0);
	
	
	mvwprintw(win4,1,7,"Lobby");
	wrefresh(win4);

	
	getch();
	refresh();
	endwin();	

}


void sign_up()
{
	int height,width;
	char* name=new char[100];
	char* ID=new char[100];
	//Login login1;
	height=10;
	width=49;

	initscr();
	WINDOW *win2= newwin(height,width,10,75);	
	refresh();
		
	box(win2,0,0);
	
	mvwprintw(win2,1,2,"Hello please enter your name and ID\n");	
	
	Register();

	wrefresh(win2);

	refresh();
	endwin();
	lobby();
	

}


void login()
{
	int height,width;
	height=10;
	width=49;

	initscr();
	WINDOW *win3= newwin(height,width,10,75);	
	refresh();

	box(win3,0,0);
	
	
	mvwprintw(win3,1,5,"Login");
	wrefresh(win3);

	
	getch();
	refresh();
	endwin();
	lobby();
}


void start_up_window()
{
	int y,x,cho;
	x=y=5;

	initscr();
	
	timeout(-1);

	int height=20;
	int width=50;
	string choices[2]={"1.SignUp","2.Login"};
	int choice,count,i;
	int highlight=0;
	

	WINDOW *win= newwin(height,width,y,x);
	refresh();

	box(win,0,0);
	keypad(win,true);  
	   

	   wattron(win,A_BLINK);//Change the text inside the box
	   wattron(win,COLOR_PAIR(1));//Change the text inside the window
	   
	   mvwprintw(win,1,15,"Welcome to SuperChat");
	   wattroff(win,A_BLINK);
	   mvwprintw(win,2,1,"Please enter 1 to signup or 2 to login");
	   move(4,4);
	   wattroff(win,COLOR_PAIR(1));
	   wrefresh(win);

	
	for(int i=0;i<2;i++){
	
		mvwprintw(win,i+3,1,choices[i].c_str());	
	}	
	
	
	choice=wgetch(win);
	endwin();

	if(choice=='1')
	sign_up();
	
	else if(choice=='2')
	login();

	else
	cout<<"i dont got it\n";
}


int main(int argc, char **argv)
{
	//Login login1;
	start_up_window();
	//login1.start_up();
	return 0;
	
}
