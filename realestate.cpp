#include<iostream>
#include<string>
#include<fstream>
using namespace std;
/*Maximum capacity of the property list
(Global constant to prevent accidental modification)*/
const int MAX_PROPERTIES=50; 
//Custom Exception for Missing Property
class PropertyNotFoundException{
	string message;
	public: 
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
		double area; //Property area
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
		virtual ~Property(){}
		virtual double calculateTax() const=0;
		virtual double estimatedTax() const=0;
		
		virtual void displayDetails()const{
			cout<<"ID:"<<id<<endl;
			cout<<"Address:"<<address<<endl;
			cout<<"Area:"<<area<<"sq.ft"<<endl;
			cout<<"Price: Rs."<<price<<endl;
			cout<<"Status:"<<(isSold ?"SOLD":"AVAILABLE")<<endl;
		}
		
		int getID() const{
			return id;
		}
		double getArea() const{
			return area;
		}
		double getPrice() const{
			return price;
		}
		bool getIsSold() const{
			return isSold;
		}
		void setSold(bool status)
		{
			isSold=status;
		}
		static int getTotalListings(){
			return totalListings;
		}
		friend class Agency;
};
int Property::totalListings=0;

class ResidentialProperty: public Property{
	private:
		int bedrooms;
	public:
		ResidentialProperty():Property()
		{
			bedrooms=0;
		}
		ResidentialProperty(int i,string addr,double pc,int beds):Property(i,addr,pc){
			bedrooms=beds;
		}
		void displayDetails() const override{
			cout<<"[RESIDENTIAL PROPERTY]"<<endl;
			cout<<"ID:"<<id<<endl;
			cout<<"Address:"<<address<<endl;
			cout<<"Price: Rs."<<price<<endl;
			cout<<"Status:"<<(isSold ?"SOLD":"AVAILABLE")<<endl;
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
		CommercialProperty(int i,string addr,double pc,string bType):Property(i,addr,pc){
			businessType=bType;
		}
		void displayDetails() const override{
			cout<<"[COMMERCIAL PROPERTY]"<<endl;
			cout<<"ID:"<<id<<endl;
			cout<<"Address:"<<address<<endl;
			cout<<"Price: Rs."<<price<<endl;
			cout<<"Business Type:"<<businessType<<endl;
			cout<<"Status:"<<(isSold ?"SOLD":"AVAILABLE")<<endl;
			cout<<"------------------------"<<endl;
		}
};
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
		void addProperty(Property* newProperty){
			if(currentCount<MAX_PROPERTIES){
				propertyList[currentCount]=newProperty;
				currentCount++;
				cout<<"Property added successfully!"<<endl;
			}
			else{
				cout<<"Error:Agency inventory is full!"<<endl;
			}
		}
		void viewProperty()const{
			if(currentCount==0){
				cout<<"No properties currently in the inventory"<<endl;
				return;
			}
			cout<<"\n===CURRENT LISTINGS===\n"<<endl;
			for(int i=0;i<currentCount;i++){
				propertyList[i]->displayDetails();
			}
		}
		void markAsSold(int searchId){
			for(int i=0;i<currentCount;i++){
				if(propertyList[i]->getID()==searchId){
				if(propertyList[i]->getIsSold()){
					cout<<"Property is already marked as sold"<<endl;
				}
				else{
					propertyList[i]->setSold(true);
					cout<<"Property ID"<<searchId<<"sucessfully marked as Sold"<<endl;
				}
				return;
			}
		}
		cout<<"Property with ID"<<searchId<<"was not found."<<endl;
	}		
};
int main()
{
	int choice=0;
	while(1)
	{
		cout<<"\n====Real Estate and Propery Listing====\n";
		cout<<"1.Add Listing\n";
		cout<<"2.View all Listings\n";
		cout<<"3.Search Listings\n";
		cout<<"4.Mark Property as Sold\n";
		cout<<"5.Portfolio Summary Report\n";
		cout<<"6.Save & Exit\n";
		cout<<"Enter your choice:";
		cin>>choice;
		
		switch(choice)
		{
			case 1:
				
		}
	}
	return 0;
}