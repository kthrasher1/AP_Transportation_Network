#include "Navigation.h"
#include "node.h"
#include "arc.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

// Converts latitude/longitude into eastings/northings
extern void LLtoUTM(const double Lat, const double Long, double &UTMNorthing, double &UTMEasting);


/*
------------------------------------------------------------------------------------------------------------------

"Command MaxDist will find the furthest-separated places and calculate the distance between them. It will
output the starting place name, the end place name, and the direct distance between the two.

York Rail,Rotterdam Harbour,416.543
<blank line>"

Code thinking:

• Iteratate and calc the distance between to places.
• Continually iteratate until max has been found with calc'd distance
• Once found cout << placeName1 + "," + placeName2 + "," + maxDist << endl;

------------------------------------------------------------------------------------------------------------------

Max Distance

Functionality is done
Output to a file is done
Returns true
Not yet turned to method
Efficiency not yet optimised

Max Link

Functionality is done
Output to a file is done
Returns true
Not yet turned to method
Efficiency not yet optimised

Find Distance

Functionality is done
Output to a file is done
Returns true
Not yet turned to method
Efficiency not yet optimised

Find Neighbour

Functionality is done
Output to a file is done
Returns true
Not yet turned to method
Efficiency not yet optimised

Check
Functionlity is mostly done, need to add valid routes
Output to a file is complete
Returns true
Not yet attempted to turn to method
Efficiency not yet optimised

FindRoute

Functionality is in progress
Output to a file is not complete or in progress
Returns false
Not yet attempted to turn to method

FindQuickestRoute

Functionality is in progress
Output to a file is not complete or in progress
Returns false
Not yet attempted to turn to method

------------------------------------------------------------------------------------------------------------------

*/

/* Vectors of Nodes and Arcs */

vector<Node*> nodeVector;
vector<Arc*> arcVector;

/* End of Vectors */

Navigation::Navigation() : _outFile("Output.txt")
{

}

Navigation::~Navigation()
{
}

#pragma region ProcessCommand Method
bool Navigation::ProcessCommand(const string& commandString)
{
	istringstream inString(commandString);
	string command;
	inString >> command;

	if (command == "MaxDist")
	{
		double maxDist = 0;
		string placeA;
		string placeB;


		for (Node* const node : nodeVector)
		{

			for (Node* const nodes : nodeVector)
			{


				double c;

				const double x = node->g_northings();
				const double y = node->g_eastings();

				const double x1 = nodes->g_northings();
				const double y1 = nodes->g_eastings();

				Math(x, y, x1, y1, c);

				if (c > maxDist)
				{
					maxDist = c;
					placeA = node->g_placeName();
					placeB = nodes->g_placeName();

				}
			}

		}
		_outFile << "MaxDist" << "\n" << placeA << "," << placeB << "," << maxDist << "\n" << endl;

		return true;
	}
	else if (command == "MaxLink")
	{
		double maxLink = 0;
		int placeA;
		int placeB;



		for (Node* const nodes : nodeVector)
		{
			for (Arc* const arc : arcVector)
			{

				if (arc->g_placeA() == nodes->g_ref())
				{
					for (Node* const node : nodeVector) {



						if (arc->g_placeB() == node->g_ref())
						{


							double c;

							const double x = node->g_northings();
							const double y = node->g_eastings();

							const double x1 = nodes->g_northings();
							const double y1 = nodes->g_eastings();

							Math(x, y, x1, y1, c);

							if (c > maxLink) {

								maxLink = c;
								placeA = arc->g_placeA();
								placeB = arc->g_placeB();

							}

						}
					}
				}

			}
		}


		_outFile << "Max Link" << "\n" << placeA << "," << placeB << "," << maxLink << "\n" << endl;

		return true;

	}
	else if (command == "FindDist")
	{
		int readInRefA;
		inString >> readInRefA;

		int readInRefB;
		inString >> readInRefB;

		_outFile << "FindDist " << readInRefA << " " << readInRefB << endl;


		for (Node* const node : nodeVector)
		{

			for (Node* const nodes : nodeVector)
			{

				if (node->g_ref() == readInRefA && nodes->g_ref() == readInRefB)
				{

					double c;

					const double x = node->g_northings();
					const double y = node->g_eastings();

					const double x1 = nodes->g_northings();
					const double y1 = nodes->g_eastings();

					Math(x, y, x1, y1, c);

					_outFile << node->g_placeName() << "," << nodes->g_placeName() << "," << fixed << setprecision(3) << c << "\n" << endl;

					break;
				}
			}
		}


		return true;

	}
	else if (command == "FindNeighbour")
	{
		int readInRef;
		inString >> readInRef;



		_outFile << "FindNeighbour " << readInRef << endl;
		for (Node* const node : nodeVector) {

			if (node->g_ref() == readInRef)
			{

				vector<Arc*> arcs = node->g_NodeArc();
				for (Arc* const arc : arcs)
				{
					_outFile << arc->g_placeB() << endl;
				}

			}
		}

		return true;
	}
	else if (command == "Check")
	{
		int i;

		string transportMode;
		inString >> transportMode;

		vector<int> RefArray;

		_outFile << "\n" << "Check " << transportMode << " ";

		while (inString >> i) {

			RefArray.push_back(i);
		}

		for (i = 0; i < RefArray.size(); i++)
		{
			_outFile << RefArray.at(i) << " ";
		}

		_outFile << endl;

		vector<string> AllowedTransport;

		AllowedTransport = AllowedTransportFunc(transportMode);

		for (int ref = 0; ref < RefArray.size() - 1; ref++)
		{

			for (Arc* const arc : arcVector)
			{

				if ((arc->g_placeA() == RefArray[ref] || arc->g_placeB() == RefArray[ref + 1]) && (arc->g_placeA() == RefArray[ref + 1] || arc->g_placeB() == RefArray[ref + 1]))
				{

					int c = 0;

					for (string const transport : AllowedTransport)
					{

						if (transport == arc->g_transportType())
						{
							_outFile << RefArray[ref] << "," << RefArray[ref + 1] << "," << "PASS" << endl;
							c++;
						}

					}
					if (c == 0)
					{
						_outFile << RefArray[ref] << "," << RefArray[ref + 1] << "," << "FAIL" << endl;
						return true;

					}

					break;
				}
			}
		}
		_outFile << endl;
		return true;
	}

	else if (command == "FindRoute")
	{


		string transportMode;
		inString >> transportMode;

		int RefA;
		inString >> RefA;

		int RefB;
		inString >> RefB;

		_outFile << "FindRoute " << transportMode << " " << RefA << " " << RefB << endl;

		FindRoute(RefA, RefB, transportMode);


		return true;
	}
	else if (command == "FindShortestRoute")
	{
		string transportMode;
		inString >> transportMode;

		int RefA;
		inString >> RefA;

		int RefB;
		inString >> RefB;

		_outFile << "FindShortestRoute " << transportMode << " " << RefA << " " << RefB << endl;

		FindRoute(RefA, RefB, transportMode);

		return true;
	}

	return false;
}
#pragma endregion

#pragma region BuildNetwork Method

const bool Navigation::BuildNetwork(const string &fileNamePlaces, const string &fileNameLinks)const
{
	fstream finPlaces(fileNamePlaces);
	fstream finLinks(fileNameLinks);

	if (finPlaces.fail() || finLinks.fail())
	{
		return false;
	}
	else
	{
		while (finLinks.good()) {
			/*Variables for Links*/
			//Maybe add them have as pointers 
			char line[255];
			int placeA;
			int placeB;
			string transportType;



			/*Read in Link file*/

			finLinks.getline(line, 255, ',');
			istringstream charIn(line);
			charIn >> placeA;

			finLinks.getline(line, 255, ',');
			istringstream charIn2(line);
			charIn2 >> placeB;

			finLinks.getline(line, 255);
			transportType = string(line);



			/*Putting read in data to node, then to vector*/

			Arc * const arcNode = new Arc(placeA, placeB, transportType);

			arcVector.push_back(arcNode);



		}
		while (finPlaces.good())
		{
			/* Variables for Places */

			vector<Arc*> tempArcs;
			string placeName;
			char line[255];
			int ref;
			double lat;
			double longitude;

			/* Assigns the value from the CSV, reads in the line then stops at ' , ' */

			finPlaces.getline(line, 255, ',');
			placeName = string(line);

			finPlaces.getline(line, 255, ',');
			istringstream charInA(line);
			charInA >> ref;

			finPlaces.getline(line, 255, ',');
			istringstream charInB(line);
			charInB >> lat;

			finPlaces.getline(line, 255);
			istringstream charInC(line);
			charInC >> longitude;

			/* Converting lat and long to x and y values */

			double northings;
			double eastings;


			LLtoUTM(lat, longitude, northings, eastings);

			/* Adds Arcs to Nodes */

			for (Arc* arc : arcVector) // Iterates through Arc vector to find arcs that == node ref
			{
				if (arc->g_placeA() == ref)
				{
					tempArcs.push_back(new Arc(*arc));
				}
				else if (arc->g_placeB() == ref)
				{
					string transport = arc->g_transportType();

					tempArcs.push_back(new Arc(arc->g_placeB(), arc->g_placeA(), transport));

				}
			}

			/* Assigning data to the node, then pushing it on to the vector */

			Node *const placeNode = new Node(placeName, ref, eastings, northings, tempArcs);
			nodeVector.push_back(placeNode);

		}



		return true;
	}

	return false;
}
#pragma endregion

bool Navigation::FindRoute(const int &RefA, const int &RefB, const string &transportMode)
{
	vector<string> AllowedTransport;
	AllowedTransport = AllowedTransportFunc(transportMode);


	vector<int> CurrentRefs;
	for (Node* const node : nodeVector)
	{
		if (node->g_ref() == RefA)
		{
			vector<Arc*> arcs = node->g_NodeArc();

			for (Arc* const arc : arcs)
			{
				if (RefB == arc->g_placeB() && (count(AllowedTransport.begin(), AllowedTransport.end(), arc->g_transportType())))
				{
					_outFile << RefA << "\n" << RefB << "\n" << endl;
					return true;
				}
				else if (count(AllowedTransport.begin(), AllowedTransport.end(), arc->g_transportType())) {

					CurrentRefs.push_back(arc->g_placeB());

				}

			}

		}
	}
	vector<int> CloneRefs = CurrentRefs;

	vector<int>visitedNodes;
	vector<int> PastRefs;
	for (int i = 0; i < CurrentRefs.size(); i++)
	{
		for (int j = 0; j < nodeVector.size(); j++)
		{
			if (nodeVector[j]->g_ref() == CurrentRefs[i])
			{

				visitedNodes.push_back(CurrentRefs[i]);
				vector<Arc*> TempArcs = nodeVector[j]->g_NodeArc();

				for (int k = 0; k < TempArcs.size(); k++)
				{
					if (count(AllowedTransport.begin(), AllowedTransport.end(), TempArcs[k]->g_transportType()))
					{

						if (TempArcs[k]->g_placeB() == RefB)
						{

							vector<int>finalArc;
							finalArc.push_back(RefB);
							PastRefs.push_back(RefB);
							visitedNodes.push_back(RefB);
							reverse(visitedNodes.begin(), visitedNodes.end());

							for (int x = 0; x < visitedNodes.size(); x++)
							{
								for (int y = 0; y < arcVector.size(); y++)
								{
									for (const int c : CloneRefs)
									{
										if (c == arcVector[y]->g_placeA() && finalArc.back() == arcVector[y]->g_placeB())
										{
											finalArc.push_back(c);
											finalArc.push_back(RefA);
											reverse(finalArc.begin(), finalArc.end());
											for (int a = 0; a < finalArc.size(); a++)
											{
												_outFile << finalArc.at(a) << "\n";
											}
											_outFile << "\n";
											return true;
										}
										else if (c == arcVector[y]->g_placeB() && finalArc.back() == arcVector[y]->g_placeA())
										{
											finalArc.push_back(c);
											finalArc.push_back(RefA);
											reverse(finalArc.begin(), finalArc.end());
											for (int a = 0; a < finalArc.size(); a++)
											{
												_outFile << finalArc.at(a) << "\n";
											}
											_outFile << "\n";
											return true;
										}
									}
									if (finalArc.back() == arcVector[y]->g_placeA() && visitedNodes[x + 1] == arcVector[y]->g_placeB() && (!count(PastRefs.begin(), PastRefs.end(), visitedNodes[x + 1])))
									{
										finalArc.push_back(visitedNodes[x + 1]);
										PastRefs.push_back(visitedNodes[x + 1]);
									}
									else if (finalArc.back() == arcVector[y]->g_placeB() && visitedNodes[x + 1] == arcVector[y]->g_placeA() && (!count(PastRefs.begin(), PastRefs.end(), visitedNodes[x + 1])))
									{
										finalArc.push_back(visitedNodes[x + 1]);
										PastRefs.push_back(visitedNodes[x + 1]);
									}
								}
							}
						}

						else if ((!count(visitedNodes.begin(), visitedNodes.end(), TempArcs[k]->g_placeB())))
						{
							CurrentRefs.push_back(TempArcs[k]->g_placeB());
						}
					}
					else if ((!count(visitedNodes.begin(), visitedNodes.end(), TempArcs[k]->g_placeB())))
					{
						visitedNodes.push_back(TempArcs[k]->g_placeB());

					}

				}
				CurrentRefs.erase(CurrentRefs.begin());
				i = 0;
				j = -1;
			}

		}
		return true;
	}
	_outFile << "FAIL" << "\n" << endl;
	return true;
}





