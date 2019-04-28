#pragma once
#include <string>
#include <vector>

using namespace std;

class Arc
{
public:
	Arc();
	~Arc();

	Arc(int refA, int refB, string &transportType);

	int g_placeA(void)const;
	int g_placeB(void)const;
	const string& g_transportType(void)const;

private: 

	int m_placeA;
	int m_placeB;
	string m_transportType;
};

