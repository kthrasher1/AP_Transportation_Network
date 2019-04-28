#pragma once
#include "Node.h"
#include <fstream>
#include <string>
#include <fstream>

class Navigation
{
	std::ofstream _outFile;

	// Add your code here

public:
	Navigation();
	~Navigation();


	const bool BuildNetwork(const std::string& fileNamePlaces, const std::string& fileNameLinks)const;
	bool ProcessCommand(const std::string& commandString);
	inline bool Math(const double &x, const double &y, const double &x1, const double &y1, double &c)const
	{
		const double a = x1 - x;
		const double b = y1 - y;

		c = sqrt(a*a + b * b) / 1000;

		return true;
	}
	inline std::vector<string> AllowedTransportFunc(const string &transportMode)const
	{
		vector<string> AllowedTransport;

		if (transportMode == "Rail")
		{
			AllowedTransport.push_back("Rail");
		}
		else if (transportMode == "Ship")
		{
			AllowedTransport.push_back("Ship");
		}
		else if (transportMode == "Bus")
		{
			AllowedTransport.push_back("Bus");
			AllowedTransport.push_back("Ship");
		}
		else if (transportMode == "Car")
		{
			AllowedTransport.push_back("Car");
			AllowedTransport.push_back("Bus");
			AllowedTransport.push_back("Ship");
		}
		else if (transportMode == "Bike")
		{
			AllowedTransport.push_back("Bike");
			AllowedTransport.push_back("Rail");
			AllowedTransport.push_back("Ship");
			AllowedTransport.push_back("Bus");
			AllowedTransport.push_back("Car");
		}
		else if (transportMode == "Foot")
		{
			AllowedTransport.push_back("Foot");
			AllowedTransport.push_back("Bike");
			AllowedTransport.push_back("Rail");
			AllowedTransport.push_back("Ship");
			AllowedTransport.push_back("Bus");
			AllowedTransport.push_back("Car");
		}

		return AllowedTransport;
	}
	bool FindRoute(const int &RefA, const int &RefB, const string &transportMode);

	// Add your code here
};