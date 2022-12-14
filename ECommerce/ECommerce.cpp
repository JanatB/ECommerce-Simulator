#include <iostream>
#include <string>
#include "ECommerce.h"
#include "Customer.h"
#include "Product.h"
#include "Book.h" 
#include <algorithm>

int sizeProdArray = 7;

bool compareName(Product* a, Product* b) {
	return a->name < b->name;
}

bool comparePrice(Product* a, Product* b) {
	return a->price < b->price;
}

bool compareCust(Customer a, Customer b) {
	return a.custName < b.custName;
}

// Operator Overrides

std::ostream& operator<<(std::ostream& os, const Customer& cust) {
	os << "Name: " << cust.custName << " | ";
	os << "ID: " << cust.custID << " | ";
	os << "Address: " << cust.custAddress << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, Product& prod) {
	// deleted COSNT from prod
	return prod.Print(os);  
}

std::ostream& operator<<(std::ostream& os, const ProductOrder& order) {
	os << "Order ID: " << order.orderID << " | ";
	os << "Prod Name: " << order.prod->name << " | ";
	os << "Cust Name: " << order.cust.custName << " | ";
	os << "Prod Options: " << order.options << std::endl;
	return os;
}

// Actions

void ECommerce::HELP() {
	std::cout << "All the actions you can preform: " << std::endl;
	std::cout << "-> Customer Actions: " << "CUSTS, NEWCUST, CUSTORDERS, SORTBYCUST" << std::endl;
	std::cout << "-> Product Actions: " << "PRODS, ORDER, BOOKS, ORDERS, SHIP, SHIPPED, CANCEL, SORTBYNAME, SORTBYPRICE" << std::endl;
}

void ECommerce::CUSTS() {
	for (Customer i : custVector) {
		std::cout << i << std::flush;
	}
}

void ECommerce::NEWCUST(int inputCustID) {
	std::string custName;
	int custID = inputCustID;
	std::string custAddress;

	std::cout << "Enter Name: "; 
	
	std::cin.ignore();
	std::getline(std::cin, custName);

	std::cout << "Enter Shipping Address: ";
	std::getline(std::cin, custAddress);

	if (custName == "") {
		throw std::runtime_error("Must provide a name.");
	} 

	if (custAddress == "") {
		throw std::runtime_error("Must provide an address.");
	}

	Customer newCust(custName, custID, custAddress);

	this->AddCustVector(newCust);
	
} 

void ECommerce::PRODS() {
	for (int i = 0; i < sizeProdArray; i++) {
		prodArray[i]->Print(std::cout);
	} 
} 

void ECommerce::ORDER() {
	std::string inputProdID;
	std::string inputCustID;
	std::string options = "None";

	std::cout << "Product ID: ";
	std::cin.ignore();
	std::getline(std::cin, inputProdID);

	std::cout << "Customer ID: ";
	std::getline(std::cin, inputCustID); 

	// Error Handling
	if (!verifyProdID(std::stoi(inputProdID))) throw std::runtime_error("ERROR: Incorrect Product ID.");

	if (!verifyCustID(std::stoi(inputCustID))) throw std::runtime_error("ERROR: Incorrect Customer ID.");
	
	Product* orderedProd = getProd(stoi(inputProdID));
	Customer orderedCust = getCust(stoi(inputCustID));
	
	if (orderedProd->type == Product::Types::BOOKS) {
		std::cout << "Choose Book Options: 'H' for [Hardcover], 'P' for [Paperback]: " << std::flush;
		std::cin >> options;
	}
	
	ProductOrder newOrder = ProductOrder(GenerateOrderID(), orderedProd, orderedCust, options);
	AddOrderVector(newOrder);
	std::cout << "Product Ordered." << std::endl;
	std::cout << "Product ID : " << newOrder.orderID << std::endl;
}

void ECommerce::BOOKS() {
	for (int i = 0; i < sizeProdArray; i++) {
		if (prodArray[i]->type == Product::Types::BOOKS) {
			prodArray[i]->Print(std::cout);
		}
	}
}

void ECommerce::ORDERS() {
	for (ProductOrder i : orderVector) {
		std::cout << i << std::flush;
	}
}
 
void ECommerce::SHIP() {
	ProductOrder prodOrder;
	std::string orderID;

	std::cin.ignore();
	std::cout << "Enter the order number: ";
	std::cin >> orderID; 

	// Error Handling
	if (!verifyOrderID(std::stoi(orderID))) throw std::runtime_error("ERROR: Incorrect Order ID.");
	
	ProductOrder ordered = getOrder(stoi(orderID));

	AddShipVector(ordered); 

	std::cout << "Success last." << std::endl;
}  

void ECommerce::SHIPPED() {
	for (ProductOrder i : shipVector) {
		std::cout << i << std::flush;
	}
}

void ECommerce::CUSTORDERS() {
	std::string inputCustID;
	
	std::cin.ignore();
	std::cout << "Customer ID: " << std::flush;
	std::cin >> inputCustID;

	if (!verifyCustID(std::stoi(inputCustID))) throw std::runtime_error("ERROR: Incorrect Customer ID."); 

	for (ProductOrder i : orderVector) {
		if (i.cust.custID == stoi(inputCustID)) {
			std::cout << i << std::endl;
		}
	}

}

void ECommerce::CANCEL() {
	std::string inputOrderID;

	std::cin.ignore();
	std::cout << "Order ID: ";
	std::cin >> inputOrderID;

	if (!verifyOrderID(stoi(inputOrderID))) throw std::runtime_error("ERROR: Incorrect Product ID.");

	for (int i = 0; i < orderVector.size(); i++) {
		if (orderVector[i].orderID == stoi(inputOrderID)) {
			orderVector.erase(orderVector.begin() + i);
		}
	} 

	std::cout << "Order Cancelled." << std::endl;
}

void ECommerce::SORTBYNAME() {
	std::sort(prodArray, prodArray + sizeProdArray, compareName);

	for (Product* i : prodArray) {
		i->Print(std::cout);
	}
}

void ECommerce::SORTBYPRICE() {
	std::sort(prodArray, prodArray + sizeProdArray, comparePrice);

	for (Product* i : prodArray) {
		i->Print(std::cout);
	}
}

void ECommerce::SORTBYCUST() {
	std::sort(custVector.begin(), custVector.end(), compareCust);

	for (Customer i : custVector) {
		std::cout << i << std::endl;
	}
}

// Other Methods

int ECommerce::GenerateCustID() {	
	// Use Static Locals Variables 
	static int ID = 49;
	ID += 1;
	return ID;
}

int ECommerce::GenerateProdID() {
	// Use Static Local Variables 
	static int ID = 699;
	ID += 1;
	return ID;
}

int ECommerce::GenerateOrderID() {
	// Use Static Local Variables
	static int ID = 999;
	ID += 1;
	return ID;
}

void ECommerce::AddCustVector(Customer newCust) {
	custVector.push_back(newCust);
}  

void ECommerce::AddOrderVector(ProductOrder newOrder) {
	orderVector.push_back(newOrder);
}

void ECommerce::AddShipVector(ProductOrder prodOrder) {
	// Remove from order vector 

	for (int i = 0; i < orderVector.size(); i++) {
		if (orderVector[i].orderID == prodOrder.orderID) {
			orderVector.erase(orderVector.begin() + i);
		}
	}

	// Add to shipped 
	shipVector.push_back(prodOrder);
}

// Helper Functions

bool ECommerce::verifyProdID(int prodID) {
	bool verified = false;
	for (int i = 0; i < sizeProdArray; i++) {
		if (prodArray[i]->prodID == prodID) {
			verified = true;
		}
	} 
	return verified;
} 

bool ECommerce::verifyCustID(int custID) {
	bool verified = false;
	for (Customer i : custVector) {
		if (i.custID == custID) {
			verified = true;
		}
	}
	return verified;

} 

bool ECommerce::verifyOrderID(int orderID) {
	bool verified = false;
	for (ProductOrder i : orderVector) {
		if (i.orderID == orderID) { 
			verified = true;
		}
	} 

	return verified;
}

Product* ECommerce::getProd(int prodID) {
	for (int i = 0; i < sizeProdArray; i++) {
		if (prodArray[i]->prodID == prodID) {
			return prodArray[i];
		}
	}
} 

Customer ECommerce::getCust(int custID) {
	for (Customer i : custVector) {
		if (i.custID == custID) {
			return i;
		}
	}
}

ProductOrder ECommerce::getOrder(int orderID) {
	for (ProductOrder i : orderVector) {
		if (i.orderID == orderID) {
			return i;
		}
	}
}
