#include<iostream>
#include<fstream>
#include<string.h>
#include<conio.h>
#include "realestate.h"
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
	cout<<"\t\t\t\t___________________________________________________\n\n";
	cout<<"\t\t\t\t             Welcome to the Login Page             \n\n";
	cout<<"\t\t\t\t_____________          MENU           _____________\n\n";
	cout<<"                                                         \n";
	cout<<"\t\t\t\t| Press 1 to LOGIN                           |"<<endl;
	cout<<"\t\t\t\t| Press 2 to REGISTER                        |"<<endl;
	cout<<"\t\t\t\t| Press 3 if you forgot your Password        |"<<endl;
	cout<<"\t\t\t\t| Press 4 to EXIT                            |"<<endl;
	cout<<"\n\t\t\t\t\t Please enter your choice : ";
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
			cout<<"\t\t\t Thank You! \n\n";
			break;
		default:
			system("cls");
			cout<<"\t\t\t Please select from the options given above \n"<<endl;
			main();
			
	}

}

void UserAccount::login()
{
	int count=0;
	string userID = "", password = "", id, pass;
	char ch;
	
	system("cls");
	cout<<"\t\t\t Please enter the username and password : "<<endl;
	
	// Masking USERNAME input with '*'
	cout<<"\t\t\t USERNAME ";
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
	cout<<"\t\t\t PASSWORD ";
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
		cout << "\nYour LOGIN is successfull! \nThanks for logging in!\n";
		system("pause");
		system("cls");
		realEstateMenu();
	}
	else{
		cout<<"\n LOGIN ERROR \n Please check your username and password\n";
		system("pause");
		main();
	}
}
void UserAccount::registration()
{
	string ruserID, rpassword, rid, rpass;
	system("cls");
	cout<<"\t\t\t Enter the Username : ";
	cin>>ruserID;
	cout<<"\t\t\t Enter the Password : ";
	cin>>rpassword;
	
	ofstream f1("records.txt", ios::app);
	f1<<ruserID<<' '<<rpassword<<endl;
	system("cls");
	cout<<"\n\t\t\t Registration is successfull! \n";
	main();
	
	
}
void UserAccount::forgot()
{
	int option;
	system("cls");
	cout<<"\t\t\t You forgot the password? Now worries \n";
	cout<<"Press 1 to search your id by username "<<endl;
	cout<<"Press 2 to go back to the main menu "<<endl;
	cin>>option;
	switch(option)
	{
		
		case 1:
		{
			int count=0;
			string suserID,sID,spass;
			cout<<"\n\t\t\t Enter the username which you remembered : ";
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
			 	cout<<"\n\n Your account is found! \n";
			 	cout<<"\n\n Your Password is : "<<spass;
			 	main();
			 }
			 else{
			 	cout<<"\n\t Sorry! your account is not found! \n";
			 	main();
			 }
			 break;
		}
		case 2:
			{
				main();
			}
			default:
				cout<<"\t\t\t Wrong choice ! Please try again "<<endl;
				forgot();
				
	}
}
