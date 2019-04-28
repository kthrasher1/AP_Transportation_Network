#include "Arc.h"

using namespace std;

Arc::Arc(const int placeA, const int placeB, string& transportTypes): m_placeA(placeA), m_placeB(placeB), m_transportType(transportTypes)
{
}

int Arc::g_placeA(void)const 
{
	return m_placeA;
}

int Arc::g_placeB(void)const 
{
	return m_placeB;
}
const string& Arc::g_transportType(void)const 
{
	return m_transportType;
}

Arc::~Arc()
{
}

