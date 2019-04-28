#pragma once
#include <vector>
#include <string>
#include "Arc.h"

using namespace std;

class Node
{
public:

	Node(string &placeName, int ref, double eastings, double northings, vector<Arc*>&NodeArc);
	~Node();

	const string &g_placeName(void)const;
	int g_ref(void)const;
	double g_eastings(void)const;
	double g_northings(void)const;
	const vector<Arc*>& g_NodeArc(void)const;

private:

	string m_placeName;
	int m_ref;
	double m_eastings;
	double m_northings;
	vector<Arc*> m_NodeArc;

};

