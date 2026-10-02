#include<iostream> //used for input and output operation
#include<string> //used to work with text/string variables
#include<fstream> //used for reading and writing file
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
			cout<<"Area:"<<area<<"sq.ft"<<endl;
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
			int id ;
			string addr;
			double area,price;
			cout<<"Enter Property ID:";
			cin>>id;
			cin.ignore();
			cout<<"Enter Address:";
			getline(cin,addr);
			cout<<"Enter Area(sq.ft)";
			cin>>area;
			cout<<"Enter Asking Price(Rs.):";
			cin>>price;
			if(typechoice==1){
				int beds;
				cout<<"Enter no of bedrooms:";
				cin>>beds;
				propertyList[currentCount]=new ResidentialProperty(id,addr,price,area,beds);
			}
			else{
				string bType;
				cin.ignore();
				cout<<"Enter Business Type:";
				getline(cin,bType);
				propertyList[currentCount]=new CommercialProperty(id,addr,price,area,bType);
			}
			currentCount++;
			cout<<"Property Added Successfully!"<<endl;
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
	    throw PropertyNotFoundException("Error:ID not found!");
	}		
};
int main()
{
	Agency Agent;
    int choice=0;
    real:
	while(1)
	{
		cout<<"\n====Real Estate and Propery Listing====\n";
		cout<<"1.Add Listing\n";
		cout<<"2.View all Listings\n";
		cout<<"3.Mark Property as Sold\n";
		cout<<"4.Save & Exit\n";
		cout<<"Enter your choice:";
		cin>>choice;
		if(choice<1 || choice>4)
		{
			cout<<"Out of range:";
			goto real;
		}
		else{
		switch(choice){
			case 1:
				Agent.addProperty();
				break;
			case 2:
				Agent.viewProperty();
				break;
			case 3:{
				int searchId;
				cout<<"Enter Property Id to mark as sold:";
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
		}
}
}
return 0;
}