#include<iostream>
#include<fstream>
#include<string.h>
#include<conio.h>
#include "realestate.h"

#define RESET  "\033[0m"
#define RED    "\033[31m"      /* Red */
#define GREEN  "\033[32m"      /* Green */
#define YELLOW "\033[33m"      /* Yellow */
#define CYAN   "\033[36m"      /* Cyan */
#define BOLD   "\033[1m"        /* Bold */
#define  C "                                      "//used for underlined text formatting

using namespace std;

class UserAccount{
	public:
		void login();
		void registration();
		void forgot();
};

UserAccount account;
void login(){
	account.login();
}
void registration(){
	account.registration();
}
void forgot(){
	account.forgot();
}

int main()
{
	int c;
	cout<<C<<BOLD<<YELLOW<<"___________________________________________________\n"<<RESET<<endl;
	cout<<C<<BOLD<<YELLOW<<"            Welcome to the Login Page             \n"<<RESET<<endl;
	cout<<C<<BOLD<<YELLOW<<"____________          MENU           _____________\n"<<RESET<<endl;
	cout<<C<<"                                                         "<<RESET<<endl;
	cout<<C<<BOLD<<CYAN<<"| Press 1 to LOGIN                           |"<<RESET<<endl;
	cout<<C<<BOLD<<CYAN<<"| Press 2 to REGISTER                        |"<<RESET<<endl;
	cout<<C<<BOLD<<CYAN<<"| Press 3 if you forgot your Password        |"<<RESET<<endl;
	cout<<C<<BOLD<<CYAN<<"| Press 4 to EXIT                            |"<<RESET<<endl;
	cout<<C<<CYAN<<"Please enter out choice : "<<RESET<<endl;
	cin>>c;
	cout<<endl;
	
	switch(c)
	{
		case 1:
			login();
			break;
		
		case 2:
			registration();
			break;
			
		case 3:
			forgot();
			break;
			
		case 4:
			cout<<C<<GREEN<<"Thank You! "<<RESET<<endl;
			break;
		default:
			system("cls");
			cout<<C"Please select from the options given above "<<RESET<<endl;
			main();
			
	}

}

void UserAccount::login()
{
	int count=0;
	string userID = "", password = "", id, pass;
	char ch;
	
	system("cls");
	cout<<C<<"Please enter the username and password : "<<RESET<<endl;
	
	// Masking USERNAME input with '*'
	cout<<C<<"USERNAME ";
	while((ch = _getch()) != '\r') { // '\r' is the Enter key
		if(ch == '\b') { // Handle Backspace key
			if(!userID.empty()) {
				userID.pop_back();
				cout << "\b \b"; // Erase the last '*'
			}
		}
		else if(ch >= 32 && ch <= 126) { // Printable characters
			userID.push_back(ch);
			cout << '*';
		}
	}
	cout << endl;

	// Masking PASSWORD input with '*'
	cout<<C<<"PASSWORD ";
	while((ch = _getch()) != '\r') { 
		if(ch == '\b') { 
			if(!password.empty()) {
				password.pop_back();
				cout << "\b \b"; 
			}
		}
		else if(ch >= 32 && ch <= 126) { 
			password.push_back(ch);
			cout << '*';
		}
	}
	cout << endl;
	
	ifstream input("records.txt");
	
	while(input>>id>>pass)
	{
		if(id==userID && pass==password)
		{
			count=1;
			system("cls");
		}
	}
	input.close();
	
	if(count==1)
	{
		cout << C<<GREEN<<"Your LOGIN is successfull! "<<RESET<<endl<<C<<GREEN<<"Thanks for logging in!"<<RESET<<endl;
		system("pause");
		system("cls");
		realEstateMenu();
	}
	else{
		cout<<C<<RED<<"LOGIN ERROR"<<RESET<<endl<<C<<RED<<"Please check your username and password"<<RESET<<endl;
		system("pause");
		main();
	}
}
void UserAccount::registration()
{
	string ruserID, rpassword, rid, rpass;
	system("cls");
	cout<<C<<"Enter the Username : "<<RESET<<endl;
	cin>>ruserID;
	cout<<C<<"Enter the Password : "<<RESET<<endl;
	cin>>rpassword;
	
	ofstream f1("records.txt", ios::app);
	f1<<ruserID<<' '<<rpassword<<endl;
	system("cls");
	cout<<C<<GREEN<<"Registration is successfull! "<<RESET<<endl;
	main();
	
	
}
void UserAccount::forgot()
{
	int option;
	system("cls");
	cout<<C<<"You forgot the password? Now worries "<<RESET<<endl;
	cout<<C<<"Press 1 to search your id by username "<<RESET<<endl;
	cout<<C<<"Press 2 to go back to the main menu "<<RESET<<endl;
	cin>>option;
	switch(option)
	{
		
		case 1:
		{
			int count=0;
			string suserID,sID,spass;
			cout<<C<<"Enter the username which you remembered : "<<RESET<<endl;
			cin>>suserID;
			
			ifstream f2("records.txt");
			while(f2>>sID>>spass)
			{
				if(sID==suserID)
				{
					count=1;
				}
			 } 
			 f2.close();
			 if(count==1)
			 {
			 	cout<<C<<GREEN<<"Your account is found! "<<RESET<<endl;
			 	cout<<C<<"Your Password is : "<<spass<<RESET<<endl;
			 	main();
			 }
			 else{
			 	cout<<C<<RED<<"Sorry! your account is not found!"<<RESET<<endl;
			 	main();
			 }
			 break;
		}
		case 2:
			{
				main();
			}
			default:
				cout<<C<<RED<<"Wrong choice! Please try again "<<RESET<<endl;
				forgot();
				
	}
}
