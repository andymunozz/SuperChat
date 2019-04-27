#include <iostream>
#include <fstream>
#include <string>

std::string User;
std::string Pass;

void Register()
{
    std::ifstream file;
    std::ofstream newUser;
    std::string Username, Password, Password_c;
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
        getline(std::cin, Password_c);

        int m=0;
        int k=0;

        while (file >> User >> Pass)
        {
            m++;
            if (Username!=User)
                k++;
        }
        if (m == k && Password == Password_c)
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
    return 0;
}

int main()
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
