#include "Node.h"
#include "Arc.h"
#include <vector>

using namespace std;

Node::Node(string &placeName, const int ref, const double eastings, const double northings, vector<Arc*>&NodeArc) : m_placeName(placeName), m_ref(ref), m_eastings(eastings), m_northings(northings), m_NodeArc(NodeArc)
{

}

const string& Node::g_placeName(void)const
{
	return m_placeName;
}

int Node::g_ref(void)const
{

	return m_ref;

}

double Node::g_eastings(void)const
{
	return m_eastings;
}

double Node::g_northings(void)const
{
	return m_northings;
}

const vector<Arc*>& Node::g_NodeArc(void)const
{

	return m_NodeArc;

}



Node::~Node()
{
}
