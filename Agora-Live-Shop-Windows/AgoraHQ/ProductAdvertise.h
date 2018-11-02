#pragma once

class CProductAdvertise
{
public:
	CProductAdvertise(int id);

public:
	std::string type ;
	int         id;         
	std::string productId;  // id of product
	std::string title;      // product title
	std::vector<std::string> m_vecColors;
	unsigned int sold;      // total sold number
	double price;           // original price, 2 decimal place
	std::string currrency;  // currency symbol, applies to price and group deal
	double groupDeal;       // group deal price
	unsigned int streamId;  // streamId of product camera
	std::string prod;
	/*
{
	"type": "prod",
		"product" : {
		"id": "XCZ24123", // id of product
		"title" : "QuietComfort 35 wireless headphones II", // product title
		"variants" : [{"name": "Color"	"values" : ["Black", "White", "Grey"]}], // product variants
		"sold" : 1298, // total sold number
		"price" : 349.95, // original price, 2 decimal place
		"currency" : "$", // currency symbol, applies to price and group deal
		"groupDeal" : 150.50, // group deal price
		"streamId" : 10000 // streamId of product camera
	}
}
*/
};

class CProductAdModule
{
public:
	CProductAdModule();

	int getProductCount(){ return count;  }
	int getCurrentIndex() { return cur_index; }

	std::string getCurrentProductJson();
	void ResetProductAdvertise(){ cur_index = -1; m_vecProducts.clear(); }
	CProductAdvertise next();
	CProductAdvertise current();
	bool IsLastProduct();
private:
	int _getFirstProductAdvertise();
	int _getNextProductAdvertise();
	int cur_index = -1;
	int count = 0;
	std::vector<CProductAdvertise> m_vecProducts;
	unsigned int streamId;
};
CProductAdModule* getProductAdModule();

