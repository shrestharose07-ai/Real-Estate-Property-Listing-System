#include<iostream> //used for input and output operation
#include<string> //used to work with text/string variables
#include<fstream> //used for reading and writing file
#include<limits> //used to handle input stream limits
#include<cstdlib> //used for exit() function 
#include<algorithm> //used for transform function(case sensitive search)
#include<iomanip> //used for output formatting
#include<cctype> //used for tolower function
#include<cmath> //used for mathematical operations
#include "realestate.h"
using namespace std;

//ANSI color codes for console text formatting
#define RESET   "\033[0m"
#define BOLD    "\033[1m"       /* Bold */	
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define C "                                                                                                  "// Clear line (used for formatting)

const int MAX_PROPERTIES=50; 

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

// ==========================================
// REUSABLE INPUT VALIDATORS
// ==========================================

// Validates integers with range and positive ID checks
int getValidInt(const string& prompt, int minVal, int maxVal = numeric_limits<int>::max()) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            if (val >= minVal && val <= maxVal) {
                return val;
            }
            cout <<C<< RED << "Invalid range! Enter a value between " << minVal << " and " << maxVal << "." << RESET << endl;
        } else {
            if (cin.eof()) {
                cout <<C<< RED << "\nInput stream closed!" << RESET << endl;
                exit(0);
            }
            cout <<C<< RED << "Invalid input! Please enter a valid integer number." << RESET << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Validates decimals with positivity, finiteness (NaN/Inf) guards
double getValidDouble(const string& prompt, double minVal, double maxVal = numeric_limits<double>::max()) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            if (isnan(val) || isinf(val)) {
                cout <<C<< RED << "Invalid input! Non-finite numbers (NaN/Infinity) are not allowed." << RESET << endl;
                continue;
            }
            if (val >= minVal && val <= maxVal) {
                return val;
            }
            cout <<C<< RED << "Invalid value! Value must be at least " << minVal << "." << RESET << endl;
        } else {
            if (cin.eof()) {
                cout <<C<< RED << "\nInput stream closed!" << RESET << endl;
                exit(0);
            }
            cout <<C<< RED << "Invalid input! Please enter a valid decimal number." << RESET << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Validates strings to ensure they are not blank AND contain at least one letter
string getValidAddress(const string& prompt) {
    string str;
    while (true) {
        cout << prompt;
        getline(cin >> ws, str);
        if (str.empty()) {
            cout <<C<< RED << "Address cannot be empty!" << RESET << endl;
            continue;
        }
        bool hasLetter = false;
        for (char c : str) {
            if (isalpha(c)) {
                hasLetter = true;
                break;
            }
        }
        if (hasLetter) return str;
        cout <<C<< RED << "Invalid address! Address must contain letters (not just numbers/symbols)." << RESET << endl;
    }
}

string getNonEmptyString(const string& prompt) {
    string str;
    while (true) {
        cout << prompt;
        getline(cin >> ws, str);
        if (!str.empty()) return str;
        cout <<C<< RED << "Input cannot be empty!" << RESET << endl;
    }
}

// Abstract Base Class: Represent a generic Property
class Property{ 
	protected:
		int id; 
		string address; 
		double price; 
		double area; 
		bool isSold;
		static int totalListings; 
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
			totalListings++;
		}
		virtual ~Property(){
			if(totalListings > 0){
				totalListings--;
			}
		}
		virtual double calculateTax() const=0;
		virtual double estimatedValue() const=0; 
		virtual string getType() const=0; 
		
		virtual void saveToFile(ofstream &writefile) const{ 
			writefile<<getType()<<"\n";
			writefile<<id<<"\n";
			writefile<<address<<"\n";
			writefile<<area<<"\n";
			writefile<<price<<"\n";
			writefile<<(isSold ? 1 : 0)<<"\n"; 
		}
		virtual void displayDetails()const{ 
			cout<<C<<BOLD<<"ID: "<<RESET<<YELLOW<<id<<RESET<<endl;
			cout<<C<<BOLD<<"Address: "<<RESET<<address<<endl;
			cout<<C<<BOLD<<"Area: "<<RESET<<fixed<<setprecision(2)<<area<<" sq.ft"<<RESET<<endl;
			cout<<C<<BOLD<<"Price: Rs. "<<RESET<<fixed<<setprecision(2)<<price<<endl;
			cout<<C<<BOLD<<"Status: "<<RESET<<(isSold ?(RED+string("SOLD")+RESET):(GREEN+string("AVAILABLE")+RESET))<<endl;
		}
		
		int getID() const{ return id; }
		string getAddress() const{ return address; }
		double getArea() const{ return area; }
		double getPrice() const{ return price; }
		bool getIsSold() const{ return isSold; }
		void setSold(bool status) { isSold=status; }
		static int getTotalListings(){ return totalListings; }
		
		friend class Agency;
};
int Property::totalListings=0;

class ResidentialProperty: public Property{
	private:
		int bedrooms; 
	public:
		ResidentialProperty():Property() { bedrooms=0; }
		ResidentialProperty(int i,string addr,double a,double pc,int beds):Property(i,addr,a,pc){
			bedrooms=beds;
		}
		double estimatedValue() const override{ 
			return (area *8000.0)+(bedrooms *50000.0);
		}
		double calculateTax() const override{ 
			return estimatedValue() *0.012;
		}
		string getType() const override{ return "Residential"; }
		void saveToFile(ofstream &writefile) const override{
			Property::saveToFile(writefile);
			writefile<<bedrooms<<"\n"; 
		}
		void displayDetails() const override{
			cout<<"\n"<<C<<BOLD<<MAGENTA<<"[RESIDENTIAL PROPERTY]"<<RESET<<endl;
			Property::displayDetails();
			cout<<C<<BOLD<<"Bedrooms: "<<RESET<<YELLOW<<bedrooms<<RESET<<endl;
			cout<<C<<BOLD<<"Estimated Market Value: Rs. "<<RESET<<YELLOW<<fixed<<setprecision(2)<<estimatedValue()<<RESET<<endl;
			cout<<C<<BOLD<<"Calculated Property Tax: Rs. "<<RESET<<YELLOW<<fixed<<setprecision(2)<<calculateTax()<<RESET<<endl;
			cout<<C<<"------------------------"<<RESET<<endl;
		}
};

class CommercialProperty:public Property{
	private:
		string businessType; 
	public:
		CommercialProperty():Property(){ businessType=""; }
		CommercialProperty(int i,string addr,double a,double pc,string bType):Property(i,addr,a,pc){
			businessType=bType;
		}
		double estimatedValue() const override{ return area*15000.0; }
		double calculateTax() const override { return estimatedValue() *0.025; }
		string getType() const override{ return "Commercial"; }
		void saveToFile(ofstream &writefile) const override{
			Property::saveToFile(writefile);
			writefile<<businessType<<"\n"; 
		}
		void displayDetails() const override{
			cout<<"\n"<<C<<BOLD<<BLUE<<"[COMMERCIAL PROPERTY]"<<RESET<<endl;
			Property::displayDetails();
			cout<<C<<BOLD<<"Business Type: "<<RESET<<YELLOW<<businessType<<RESET<<endl;
			cout<<C<<BOLD<<"Estimated Market Value: Rs. "<<RESET<<YELLOW<<fixed<<setprecision(2)<<estimatedValue()<<RESET<<endl;
			cout<<C<<BOLD<<"Calculated Property Tax: Rs. "<<RESET<<YELLOW<<fixed<<setprecision(2)<<calculateTax()<<RESET<<endl;
			cout<<C<<"------------------------"<<RESET<<endl;
		}
		string getBusinessType() const{ return businessType; }
};

// Global polymorphic helper function
void showListing(const Property &p){
	p.displayDetails(); 
}

class Agency{
	private:
		Property* propertyList[MAX_PROPERTIES];
		int currentCount;

		bool isDuplicateID(int id) const {
			for (int i = 0; i < currentCount; i++) {
				if (propertyList[i] != nullptr && propertyList[i]->getID() == id) {
					return true;
				}
			}
			return false;
		}

	public:
		Agency(){
			currentCount=0;
			for(int i=0;i<MAX_PROPERTIES;i++){
				propertyList[i]=nullptr;
			}
		}
		Agency(const Agency&)=delete; 
		Agency& operator=(const Agency&)=delete; 
		~Agency(){
			for(int i=0;i<currentCount;i++){
				delete propertyList[i];
				propertyList[i]=nullptr;
			}
		}

		void addProperty(){
			if(currentCount >= MAX_PROPERTIES){
				cout <<C<< RED << "Error: Agency inventory is full!" << RESET << endl;
				return;
			}
			cout << "\n"<<C<<"Select Property Type:" << endl;
			cout << C<<"1. Residential Property" << endl;
			cout << C<<"2. Commercial Property" << endl;
			int typechoice = getValidInt(string(C)+"Enter choice (1 or 2): ", 1, 2);

			int id;
			while (true) {
				id = getValidInt(string(C)+"Enter Property ID (Positive Integer): ", 1);
				if (!isDuplicateID(id)) break;
				cout <<C<< RED << "Error: Property ID " << id << " already exists! Enter a unique ID." << RESET << endl;
			}

			string addr = getValidAddress(string(C)+"Enter Address: ");
			double area = getValidDouble(string(C)+"Enter Area (sq.ft): ", 1.0, 1000000.0);
			double price = getValidDouble(string(C)+"Enter Asking Price (Rs.): ", 1.0, 10000000000.0);

			Property* newProperty = nullptr;

			if (typechoice == 1) {
				int beds = getValidInt(string(C)+"Enter no of bedrooms (0-100): ", 0, 100);
				newProperty = new ResidentialProperty(id, addr, area, price, beds);
			} else {
				string bType = getNonEmptyString(string(C)+"Enter Business Type (e.g., Office/Shop): ");
				newProperty = new CommercialProperty(id, addr, area, price, bType);
			}

			propertyList[currentCount++] = newProperty; 
			saveAllPropertiesToFile(); 
			cout <<C<< GREEN << "Property added successfully!" << RESET << endl;
		}

		void loadproperty(){
			ifstream readfile("property_data.txt");
			if(!readfile.is_open()){
				cout <<C<< RED << "Error: Unable to open property data file!" << RESET << endl;
				return; 
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

				// FILE VALIDATION: Ignore bad/corrupt/duplicate records
				if (id <= 0 || area <= 0 || price <= 0 || isDuplicateID(id)) {
					if(type == "Residential") { int b; readfile >> b; readfile.ignore(numeric_limits<streamsize>::max(), '\n'); }
					else if(type == "Commercial") { string s; getline(readfile, s); }
					continue;
				}

				Property* loadProperty = nullptr;
				if(type == "Residential"){
					int beds;
					if(!(readfile >> beds)) break;
					readfile.ignore(numeric_limits<streamsize>::max(), '\n');
					if(beds < 0) continue;
					loadProperty = new ResidentialProperty(id, addr, area, price, beds);
				}
				else if(type == "Commercial"){
					string businessType;
					if(!getline(readfile, businessType)) break;
					if(businessType.empty()) continue;
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
				cout<<C<<YELLOW<<"No properties currently in the inventory"<<RESET<<endl;
				return;
			}
			cout<<"\n"<<C<<BOLD<<CYAN<<"===CURRENT LISTINGS==="<<RESET<<"\n"<<endl;
			for(int i=0;i<currentCount;i++){
				if(propertyList[i] != nullptr){
					showListing(*propertyList[i]); // Uses assignment-required showListing helper
				}
			}
		}

		void searchProperty(){
			if(currentCount == 0){
				cout << C << YELLOW << "No properties currently in the inventory" << RESET << endl;
				return;
			}
			cout << C << BOLD << CYAN << "\nSearch Options:" << RESET << endl;
			cout << C<<"1. By Property ID" << endl;
			cout << C<<"2. By Address" << endl;
			cout << C<<"3. By Price Range" << endl;
			int choice = getValidInt(string(C)+"Enter choice (1-3): ", 1, 3);

			bool found = false;
			if(choice == 1){
				int searchId = getValidInt(string(C)+"Enter Property ID to search (Positive Integer): ", 1);
				for(int i = 0; i < currentCount; i++){
					if(propertyList[i]->getID() == searchId){
						showListing(*propertyList[i]);
						found = true;
						break;
					}
				}
			}
			else if(choice == 2){
				string searchAddr = getNonEmptyString(string(C)+"Enter Address keyword to search: ");
				auto toLowerStr = [](string s) {
					transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return tolower(c); });
					return s;
				};
				string lowerSearchAddr = toLowerStr(searchAddr);
				for(int i = 0; i < currentCount; i++){
					string lowerPropertyAddr = toLowerStr(propertyList[i]->getAddress());
					if(lowerPropertyAddr.find(lowerSearchAddr) != string::npos){
						showListing(*propertyList[i]);
						found = true;
					}
				}
			}
			else if(choice == 3){
				double minPrice = getValidDouble(string(C)+"Enter Minimum Price (Rs.): ", 0.0);
				double maxPrice = getValidDouble(string(C)+"Enter Maximum Price (Rs.): ", minPrice);

				for(int i = 0; i < currentCount; i++){
					if(propertyList[i]->getPrice() >= minPrice && propertyList[i]->getPrice() <= maxPrice){
						showListing(*propertyList[i]);
						found = true;
					}
				}
			}

			if(!found){
				cout << C << RED << "Property not found!" << RESET << endl;
			}
		}

		void displayPortfolio(){
			if(currentCount==0){
				cout<<C<<YELLOW<<"No properties currently in the inventory"<<RESET<<endl;
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
				totalPrice += propertyList[i]->getPrice();
				totalArea += propertyList[i]->getArea();
				totalTax += propertyList[i]->calculateTax();
			}
			cout<<"\n"<<C<<BOLD<<CYAN<<"===PORTFOLIO SUMMARY==="<<RESET<<"\n"<<endl;
			cout<<C<<BOLD<<"Total Active Listings: "<<RESET<<Property::getTotalListings()<<endl;
			cout<<C<<BOLD<<"Sold Properties: "<<RESET<<soldCount<<endl;
			cout<<C<<BOLD<<"Residential Properties: "<<RESET<<res<<endl;
			cout<<C<<BOLD<<"Commercial Properties: "<<RESET<<com<<endl;
			cout<<C<<BOLD<<"Total Price Value: Rs. "<<RESET<<fixed<<setprecision(2)<<totalPrice<<endl;
			cout<<C<<BOLD<<"Total Area: "<<RESET<<fixed<<setprecision(2)<<totalArea<<" sq.ft"<<endl;
			cout<<C<<BOLD<<"Total Estimated Tax: Rs. "<<RESET<<fixed<<setprecision(2)<<totalTax<<endl;
			cout<<"\n"<<C<<BOLD<<"Detailed Property Items:"<<RESET<<endl;
			for(int i=0;i<currentCount;i++){
				showListing(*propertyList[i]);
			}
		}

		void saveAllPropertiesToFile(){
			ofstream writefile("property_data.txt");
			if(!writefile)
			{
				cout<<C<<RED<<"Error opening file for writing!"<<RESET<<endl;
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
						cout<<C<<YELLOW<<"Property ID "<<searchId<<" is already marked as sold."<<RESET<<endl;
					}
					else{
						propertyList[i]->setSold(true);
						saveAllPropertiesToFile(); 
						cout<<C<<GREEN<<"Property ID "<<searchId<<" successfully marked as Sold."<<RESET<<endl;
					}
					return;
				}
			}
			throw PropertyNotFoundException(string(C)+RED+string("Error: Property ID ") + to_string(searchId) + string(" not found!")+RESET);
		}		
};

void realEstateMenu()
{
	Agency Agent;
	Agent.loadproperty();
	while(true)
	{
		cout<<"\n";
		cout<<C<<BOLD<<YELLOW<<"==== Real Estate and Property Listing System ===="<<RESET<<endl;
		cout<<C<<"1. Add Listing"<<endl;
		cout<<C<<"2. View all Listings"<<endl;
		cout<<C<<"3. Search Property"<<endl;
		cout<<C<<"4. Mark Property as Sold"<<endl;
		cout<<C<<"5. Display Portfolio Summary"<<endl;
		cout<<C<<"6. Save & Exit"<<endl;
		
		int choice = getValidInt(string(C)+"Enter your choice (1-6): ", 1, 6);
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
				int searchId = getValidInt(string(C)+"Enter Property ID to mark as sold (Positive Integer): ", 1);
				try{
					Agent.markAsSold(searchId);
				}
				catch(const PropertyNotFoundException &e) {
					cout <<C<< e.getMessage() << endl;
				}
				break;
			}
			case 5:
				Agent.displayPortfolio();
				break;
			case 6:
				Agent.saveAllPropertiesToFile();	
				cout<<C<<GREEN<<"Goodbye!!"<<RESET<<endl;
				return;
		}
	}
}
