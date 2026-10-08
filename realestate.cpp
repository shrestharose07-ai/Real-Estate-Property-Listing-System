#include<iostream> //used for input and output operation
#include<string> //used to work with text/string variables
#include<fstream> //used for reading and writing file
#include<limits> //used to handle input stream limits
#include<cstdlib> //used for exit() function 
#include "realestate.h"
using namespace std;
//ANSII color codes for console text formatting
#define RESET   "\033[0m"
#define BOLD    "\033[1m"       /* Bold */	
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
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
		virtual string getType() const=0; //returns property type
		
		virtual void saveToFile(ofstream &writefile) const{ //Save property details to file
			writefile<<getType()<<"\n";
			writefile<<id<<"\n";
			writefile<<address<<"\n";
			writefile<<area<<"\n";
			writefile<<price<<"\n";
			writefile<<(isSold ? 1 : 0)<<"\n"; //Save sold status as 1 or 0
		}
		virtual void displayDetails()const{ //Display property details to console
			cout<<BOLD<<"ID:"<<YELLOW<<RESET<<id<<endl;
			cout<<BOLD<<"Address:"<<RESET<<address<<endl;
			cout<<BOLD<<"Area:" <<RESET<<area<<"sq.ft"<<RESET<<endl;
			cout<<BOLD<<"Price: Rs."<<RESET<<price<<endl;
			cout<<BOLD<<"Status:"<<RESET<<(isSold ?(RED+string("SOLD")+RESET):(GREEN+string("AVAILABLE")+RESET))<<endl;
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
		string getType() const override{
			return "Residential";
		}
		void saveToFile(ofstream &writefile) const override{
			Property::saveToFile(writefile);
			writefile<<bedrooms<<"\n"; //Save bedroom count for residential property
		}
		void displayDetails() const override{
			cout<<"\n"<<BOLD<<MAGENTA<<"[RESIDENTIAL PROPERTY]"<<RESET<<endl;
			Property::displayDetails();
			cout<<BOLD<<"Bedrooms:"<<RESET<<YELLOW<<bedrooms<<RESET<<endl;
			cout<<BOLD<<"Estimated Market Values: Rs."<<RESET<<YELLOW<<estimatedValue()<<RESET<<endl;
			cout<<BOLD<<"Calculated Property Tax: Rs."<<RESET<<YELLOW<<calculateTax()<<RESET<<endl;
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
		string getType() const override{
			return "Commercial";
		}
		void saveToFile(ofstream &writefile) const override{
			Property::saveToFile(writefile);
			writefile<<businessType<<"\n"; //Save business type for commercial property
		}
		void displayDetails() const override{
			cout<<"\n"<<BOLD<<BLUE<<"[COMMERCIAL PROPERTY]"<<RESET<<endl;
			Property::displayDetails();
			cout<<BOLD<<"Business Type:"<<RESET<<YELLOW<<businessType<<RESET<<endl;
			cout<<BOLD<<"Estimated Market Values: Rs."<<RESET<<YELLOW<<estimatedValue()<<RESET<<endl;
			cout<<BOLD<<"Calculated Property Tax: Rs."<<RESET<<YELLOW<<calculateTax()<<RESET<<endl;
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
		Agency(const Agency&)=delete; //Disable copy constructor to prevent shallow copy issues
		Agency& operator=(const Agency&)=delete; //Disable assignment operator to prevent shallow
		~Agency(){
			for(int i=0;i<currentCount;i++){
				delete propertyList[i];
				propertyList[i]=nullptr;
			}
		}
		void addProperty(){
			if(currentCount>=MAX_PROPERTIES){
				cout<<RED<<"Error:Agency inventory is full!"<<RESET<<endl;
				return;
			}
			int typechoice;
			cout<<"\n Select Property Type:"<<endl;
			cout<<"1. Residential Property"<<endl;
			cout<<"2. Commercial Property"<<endl;
			cout<<"Enter choice:";
			cin>>typechoice;
			Property* newProperty=nullptr;
			if(typechoice!=1 && typechoice!=2)
			{
				cout<<RED<<"Invalid selection!"<<RESET<<endl;
				return;
			}
			int id ;
			string addr;
			double area,price;
			cout<<"Enter Property ID:";
			cin>>id;
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Clear input buffer

			cout<<"Enter Address:";
			getline(cin,addr);

			cout<<"Enter Area(sq.ft):";
			cin>>area;

			cout<<"Enter Asking Price(Rs.):";
			cin>>price;

			if(typechoice==1){
				int beds;
				cout<<"Enter no of bedrooms:";
				cin>>beds;
				newProperty=new ResidentialProperty(id,addr,area,price,beds);
			}
			else{
				string bType;
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Clear input buffer
				cout<<"Enter Business Type:";
				getline(cin,bType);
				newProperty=new CommercialProperty(id,addr,area,price,bType);
			}
            propertyList[currentCount++]=newProperty ;//Increment count after adding new property
			saveAllPropertiesToFile(); //Save to file after adding new property
			cout<<GREEN<<"Property added successfully!"<<RESET<<endl;
		}

	void loadproperty(){
		ifstream readfile("property_data.txt");
		if(!readfile.is_open()){
			return; // File doesn't exist yet, safe to proceed
		}

		string type;
		while(getline(readfile, type)){
			if(type.empty()) continue;

			int id, soldInt;
			string addr;
			double area, price;

			if(!(readfile >> id)) break;
			readfile.ignore(numeric_limits<streamsize>::max(), '\n');
			if(!getline(readfile, addr)) break;
			if(!(readfile >> area >> price >> soldInt)) break;
			readfile.ignore(numeric_limits<streamsize>::max(), '\n');

			Property* loadProperty = nullptr;
			if(type == "Residential"){
				int beds;
				if(!(readfile >> beds)) break;
				readfile.ignore(numeric_limits<streamsize>::max(), '\n');
				loadProperty = new ResidentialProperty(id, addr, area, price, beds);
			}
			else if(type == "Commercial"){
				string businessType;
				if(!getline(readfile, businessType)) break;
				loadProperty = new CommercialProperty(id, addr, area, price, businessType);
			}
			else{
				break;
			}

			loadProperty->isSold = (soldInt == 1);
			if(currentCount < MAX_PROPERTIES){
				propertyList[currentCount++] = loadProperty;
			}
			else{
				delete loadProperty;
				break;
			}
		}
		readfile.close();
	}
		void viewProperty(){
			if(currentCount==0){
				cout<<YELLOW<<"No properties currently in the inventory"<<RESET<<endl;
				return;
			}
			cout<<"\n"<<BOLD<<CYAN<<"===CURRENT LISTINGS==="<<RESET<<"\n"<<endl;
			for(int i=0;i<currentCount;i++){
				propertyList[i]->displayDetails();
			}
		}
		void searchProperty(){
			if(currentCount==0){
				cout<<YELLOW<<"No properties currently in the inventory"<<RESET<<endl;
				return;
			}
			int choice;
			cout<<BOLD<<CYAN<<"Search by: 1.ID 2.Address 3.Price Range"<<RESET<<endl;
			cin>>choice;
			bool found=false;
			if(choice==1){
				int searchId;
				cout<<"Enter Property ID to search:"<<endl;
				cin>>searchId;
				for(int i=0;i<currentCount;i++){
					if(propertyList[i]->getID()==searchId){
						propertyList[i]->displayDetails();
						found=true;
						break;
					}
				}
			}
			else if(choice==2){
				string searchAddr;
				cout<<"Enter Address to search:"<<endl;
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Clear input buffer
				getline(cin,searchAddr);
				for(int i=0;i<currentCount;i++){
					if(propertyList[i]->address.find(searchAddr)!=string::npos){
						propertyList[i]->displayDetails();
						found=true;
					}
				}
			}
			else if(choice==3){
				double minPrice,maxPrice;
				cout<<"Enter Minimum Price:"<<endl;
				cin>>minPrice;
				cout<<"Enter Maximum Price:"<<endl;
				cin>>maxPrice;
				for(int i=0;i<currentCount;i++){
					if(propertyList[i]->getPrice()>=minPrice && propertyList[i]->getPrice()<=maxPrice){
						propertyList[i]->displayDetails();
						found=true;
					}
				}
			}
			else{
				cout<<RED<<"Invalid choice!"<<RESET<<endl;
				return;
			}
			if(!found){
				cout<<RED<<"Property not found!"<<RESET<<endl;
			}
		}
		void displayPortfolio(){
			if(currentCount==0){
				cout<<YELLOW<<"No properties currently in the inventory"<<RESET<<endl;
				return;
			} 
			int soldCount=0,res=0,com=0;
			double totalPrice=0.0,totalArea=0.0,totalTax=0.0;
			for(int i=0;i<currentCount;i++){
				if(propertyList[i]->getIsSold()){
					soldCount++;
				}
				if(propertyList[i]->getType()=="Residential"){
					res++;
				}
				else if(propertyList[i]->getType()=="Commercial"){
					com++;
				}
				totalPrice=totalPrice+propertyList[i]->getPrice();
				totalArea=totalArea+propertyList[i]->getArea();
				totalTax=totalTax+propertyList[i]->calculateTax();

			}
			cout<<"\n"<<BOLD<<CYAN<<"===PORTFOLIO SUMMARY==="<<RESET<<"\n"<<endl;
			cout<<BOLD<<"Total Properties: "<<RESET<<currentCount<<endl;
			cout<<BOLD<<"Sold Properties: "<<RESET<<soldCount<<endl;
			cout<<BOLD<<"Residential Properties: "<<RESET<<res<<endl;
			cout<<BOLD<<"Commercial Properties: "<<RESET<<com<<endl;
			cout<<BOLD<<"Total Price: Rs"<<RESET<<totalPrice<<endl;
			cout<<BOLD<<"Total Area: "<<RESET<<totalArea<<" sq.ft"<<endl;
			cout<<BOLD<<"Total Tax: Rs"<<RESET<<totalTax<<endl;
			for(int i=0;i<currentCount;i++){
				propertyList[i]->displayDetails();
			}
		}
		void saveAllPropertiesToFile(){
			ofstream writefile("property_data.txt");
			if(!writefile)
			{
				cout<<RED<<"Error opening file for writing!"<<RESET<<endl;
				return;
			}
			for(int i=0;i<currentCount;i++){
				propertyList[i]->saveToFile(writefile);
			}
			writefile.close();
		}
		void markAsSold(int searchId){
			for(int i=0;i<currentCount;i++){
				if(propertyList[i]->getID()==searchId){
				if(propertyList[i]->getIsSold()){
					cout<<YELLOW<<"Property is already marked as sold"<<RESET<<endl;
				}
				else{
					propertyList[i]->setSold(true);
					saveAllPropertiesToFile(); //Update file after marking as sold
					cout<<GREEN<<"Property ID "<<searchId<<" successfully marked as Sold."<<RESET<<endl;
				}
				return;
			}
		}
	    throw PropertyNotFoundException(RED+string("Error:ID not found!")+RESET);
	}		
};
void realEstateMenu()
{
	Agency Agent;
	Agent.loadproperty();
    int choice=0;
	while(true)
	{
		cout<<BOLD<<YELLOW<<"====Real Estate and Property Listing===="<<RESET<<endl;
		cout<<"1.Add Listing"<<endl;
		cout<<"2.View all Listings"<<endl;
		cout<<"3.Search Property"<<endl;
		cout<<"4.Mark Property as Sold"<<endl;
		cout<<"5.Display Portfolio Summary"<<endl;
		cout<<"6.Save & Exit"<<endl;
		cout<<"Enter your choice:";
		if(!(cin>>choice)){
			cout<<RED<<"Invalid input! Please enter a number between 1 and 6."<<RESET<<endl;
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
			case 3:
				Agent.searchProperty();
				break;	
			case 4:{
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
			case 5:
				Agent.displayPortfolio();
				break;
			case 6:
				Agent.saveAllPropertiesToFile();	
				cout<<GREEN<<"Goodbye!!"<<RESET<<endl;
				return;
			default:
		       cout<<RED<<"Invalid choice! Please select a valid option."<<RESET<<endl;
			   break;
		}
	}
return;
}
int main()
{
	realEstateMenu();
	return 0;
}