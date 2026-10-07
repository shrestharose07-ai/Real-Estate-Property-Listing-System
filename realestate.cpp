#include<iostream> //used for input and output operation
#include<string> //used to work with text/string variables
#include<fstream> //used for reading and writing file
#include<limits> //used to handle input stream limits
#include<cstdlib> //used for exit() function 
#include "realestate.h"
using namespace std;
/*Maximum capacity of the property list
(Global constant to prevent accidental modification)*/
const int MAX_PROPERTIES=50; 
//Custom Exception for Missing Property
class PropertyNotFoundException{
	string message;
	public: 
	//constructor to set error message
	PropertyNotFoundException(string msg){
		message=msg;
	}
	string getMessage() const{
		return message;
	}
};
//Abstract Base Class:Represent a generic Property
class Property{ 
	protected:
		int id; //Property identification number
		string address; //Property location/address
		double price; //Property cost 
		double area; //Property area in square feet
		bool isSold;// Status flag: true if sold or false if available
		static int totalListings; /* Static  member variable :Shared across all property 
		instances to track total listings created*/
	public:
		Property()
		{
			id=0;
			address="";
			area=0.0;
			price=0.0;
			isSold=false;		
		}
		Property(int i,string addr,double a,double pc)
		{
			id=i;
			address=addr;
			area=a;
			price=pc;
			isSold=false;
			totalListings++;//Increment static count whenever a new property is created
		}
		//virtual destructor for clean memory deallocation
		virtual ~Property(){}
		virtual double calculateTax() const=0;//for tax
		virtual double estimatedValue() const=0; //estimated value 
		
		virtual void displayDetails()const{
			cout<<"ID:"<<id<<endl;
			cout<<"Address:"<<address<<endl;
			cout<<"Area:" <<area<<"sq.ft"<<endl;
			cout<<"Price: Rs."<<price<<endl;
			cout<<"Status:"<<(isSold ?"SOLD":"AVAILABLE")<<endl;
		}
		
		int getID() const{
			return id; //returns property id
		}
		double getArea() const{
			return area; //returns area
		}
		double getPrice() const{
			return price; //returns asking price
		}
		bool getIsSold() const{
			return isSold; //returns true or false status
		}
		void setSold(bool status)
		{
			isSold=status;
		}
		static int getTotalListings(){
			return totalListings; //returns total counting of created properties
		}
		friend class Agency;
};
int Property::totalListings=0;

class ResidentialProperty: public Property{
	private:
		int bedrooms; //for bedroom counts
	public:
		ResidentialProperty():Property()
		{
			bedrooms=0;
		}
		ResidentialProperty(int i,string addr,double a,double pc,int beds):Property(i,addr,a,pc){
			bedrooms=beds;
		}
		double estimatedValue() const override{ //redines pure virtual function to compute estimated tax
			return (area *8000.0)+(bedrooms *50000.0);
		}
		double calculateTax() const override{ //redines pure virtual function to compute tax
			return estimatedValue() *0.012;
		}
		void displayDetails() const override{
			cout<<"\n[RESIDENTIAL PROPERTY]"<<endl;
			Property::displayDetails();
			cout<<"Bedrooms:"<<bedrooms<<endl;
			cout<<"Estimated Market Values: Rs."<<estimatedValue()<<endl;
			cout<<"Calculated Property Tax: Rs."<<calculateTax()<<endl;
			cout<<"------------------------"<<endl;
		}
};
class CommercialProperty:public Property{
	private:
		string businessType; //Specific feature for commercial property(e.g:Office,Shop,Warehouse)
	public:
		CommercialProperty():Property(){
			businessType="";
		}
		CommercialProperty(int i,string addr,double a,double pc,string bType):Property(i,addr,a,pc){
			businessType=bType;
		}
		double estimatedValue() const override{
			return area*15000.0;
		}
		double calculateTax() const override {
			return estimatedValue() *0.025;
		}
		void displayDetails() const override{
			cout<<"\n[COMMERCIAL PROPERTY]"<<endl;
			Property::displayDetails();
			cout<<"Business Type:"<<businessType<<endl;
			cout<<"Estimated Market Values: Rs."<<estimatedValue()<<endl;;
			cout<<"Calculated Property Tax: Rs."<<calculateTax()<<endl;
			cout<<"------------------------"<<endl;
		}
		string getBusinessType() const{
			return businessType;
		}
};
//Global helper function demostrating polymorphic printing via reference
void showListing(const Property &p){
	p.displayDetails(); //dyanimc binding triggers correct subtype function
}
class Agency{
	private:
		Property* propertyList[MAX_PROPERTIES];
		int currentCount;
	public:
		Agency(){
			currentCount=0;
			for(int i=0;i<MAX_PROPERTIES;i++){
				propertyList[i]=nullptr;
			}
		}
		~Agency(){
			for(int i=0;i<MAX_PROPERTIES;i++){
				delete propertyList[i];
				propertyList[i]=nullptr;
			}
		}
		void addProperty(){
			if(currentCount>=MAX_PROPERTIES){
				cout<<"Error:Agency inventory is full!"<<endl;
				return;
			}
			int typechoice;
			cout<<"\n Select Property Type:"<<endl;
			cout<<"1. Residential Property"<<endl;
			cout<<"2. Commercial Property"<<endl;
			cout<<"Enter choice:";
			cin>>typechoice;
			if(typechoice!=1 && typechoice!=2)
			{
				cout<<"Invalid selection!"<<endl;
				return;
			}

			ofstream writefile;
			writefile.open("property_data.txt",ios::app);

			if(!writefile)
			{
				cout<<"Error opening file for writing!"<<endl;
				return;
			}

			int id ;
			string addr;
			double area,price;
			writefile<<((typechoice==1)?"Residential":"Commercial")<<"\n";
			cout<<"Enter Property ID:";
			cin>>id;
			writefile<<id<<"\n";
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Clear input buffer
			cout<<"Enter Address:";
			getline(cin,addr);
			writefile<<addr<<"\n";
			cout<<"Enter Area(sq.ft):";
			cin>>area;
			writefile<<area<<"\n";
			cout<<"Enter Asking Price(Rs.):";
			cin>>price;
			writefile<<price<<"\n";
			if(typechoice==1){
				int beds;
				cout<<"Enter no of bedrooms:";
				cin>>beds;
				writefile<<beds<<"\n";
				propertyList[currentCount]=new ResidentialProperty(id,addr,area,price,beds);
			}
			else{
				string bType;
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Clear input buffer
				cout<<"Enter Business Type:";
				getline(cin,bType);
				writefile<<bType<<"\n";
				propertyList[currentCount]=new CommercialProperty(id,addr,area,price,bType);
			}
			writefile.close();
			currentCount++;
			cout<<"Property Added Successfully!"<<endl;
		}

		void loadproperty(){
    ifstream readfile("property_data.txt");
    if(!readfile.is_open())
    {
        return; // File doesn't exist yet, safe to proceed
    }

    string type;
    while(getline(readfile, type)){
        if(type.empty()) continue;
        
        int id;
        string addr;
        double area, price;

        if(!(readfile >> id)) break;
        readfile.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if(!getline(readfile, addr)) break;
        if(!(readfile >> area >> price)) break;
        readfile.ignore(numeric_limits<streamsize>::max(), '\n');

        if(type == "Residential"){
            int beds;
            if(readfile >> beds){
                readfile.ignore(numeric_limits<streamsize>::max(), '\n');
                if(currentCount < MAX_PROPERTIES){
                    propertyList[currentCount++] = new ResidentialProperty(id, addr, area, price, beds);
                }
            }
        }
        else if(type == "Commercial"){
            string bType;
            if(getline(readfile, bType)){
                if(currentCount < MAX_PROPERTIES){
                    propertyList[currentCount++] = new CommercialProperty(id, addr, area, price, bType);
                }
            }
        }
    }
    readfile.close();
}

		void viewProperty(){
			if(currentCount==0){
				cout<<"No properties currently in the inventory"<<endl;
				return;
			}
			cout<<"\n===CURRENT LISTINGS===\n"<<endl;
			for(int i=0;i<currentCount;i++){
				loadproperty();
				propertyList[i]->displayDetails();
			}
		}
		
		void markAsSold(int searchId){
			fstream checkfile;
			checkfile.open("property_data.txt", ios::in);
			if(!checkfile)
			{
				cout<<"No saved listings file found yet."<<endl;
				return;
			}
			else
			{
				while(!checkfile.eof())
				{
					string type;
					int id;
					getline(checkfile,type);
					checkfile>>id;
					if(id==searchId)
					{
						checkfile.close();
						
						checkfile.open("property_data.txt", ios::app);
						

						/*for(int i=0;i<currentCount;i++){
							if(propertyList[i]->getID()==searchId){
								if(propertyList[i]->getIsSold()){
									cout<<"Property is already marked as sold"<<endl;
								}
								else{
									propertyList[i]->setSold(true);
									cout<<"Property ID "<<searchId<<" successfully marked as Sold"<<endl;
								}
								return;
							}
						}*/
					}
					checkfile.ignore();
					getline(checkfile,type);
					checkfile.ignore();
				}
			}
			for(int i=0;i<currentCount;i++){
				if(propertyList[i]->getID()==searchId){
				if(propertyList[i]->getIsSold()){
					cout<<"Property is already marked as sold"<<endl;
				}
				else{
					propertyList[i]->setSold(true);
					cout<<"Property ID "<<searchId<<" successfully marked as Sold."<<endl;
				}
				return;
			}
		}
	    throw PropertyNotFoundException("Error:ID not found!");
	}		
};
void realEstateMenu()
{
	Agency Agent;
	Agent.loadproperty();
    int choice=0;
	while(true)
	{
		cout<<"====Real Estate and Property Listing===="<<endl;
		cout<<"1.Add Listing"<<endl;
		cout<<"2.View all Listings"<<endl;
		cout<<"3.Mark Property as Sold"<<endl;
		cout<<"4.Save & Exit"<<endl;
		cout<<"Enter your choice:";
		if(!(cin>>choice)){
			cout<<"Invalid input! Please enter a number between 1 and 4."<<endl;
			cin.clear(); //Clear the error flag 
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Ignore the rest of the line
			continue; //Prompt the user again
		}

		switch(choice){
			case 1:
				Agent.addProperty();
				break;
			case 2:
				Agent.viewProperty();
				break;
			case 3:{
				int searchId;
				cout<<"Enter Property Id to mark as sold:"<<endl;
				cin>>searchId;
				try{
				Agent.markAsSold(searchId);
			}
			catch(const PropertyNotFoundException &e)
			{
				cout<<e.getMessage()<<endl;
			}
				break;
			}
			case 4:
				cout<<"Goodbye!!"<<endl;
				exit(0);
				break;
			default:
		       cout<<"Invalid choice! Please select a valid option."<<endl;
			   break;
		}
	}
return;
}
