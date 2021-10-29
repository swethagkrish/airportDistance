#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono> 
#include <vector> 
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#define INFINITY 9999
using namespace std;

typedef struct {
	map<string, int> indexMap; // index of all the airports
	vector<vector<pair<int, int>>> graph; // storing the various routes and their distances
	vector<string> airport; // list of airports corresponding to their index
	typedef pair<int, int> iPair; 
	/**
	 * Add a new routes with the distance between them. 
	   Note: We are adding both the to and from route 
	 */
	void addEdge(string from, string to, int distance) {
		// "the from airport"  is not added to the map
		if (indexMap.count(from) == 0) {
			indexMap[from] = graph.size(); // new map
			vector<pair<int, int>> adj; // Empty Array
			graph.push_back(adj);
			airport.push_back(from);
		}
		// "the to airport"  is not added to the map
		if (indexMap.count(to) == 0) {
			indexMap[to] = graph.size(); //new map
			vector<pair<int, int>> adj;
			graph.push_back(adj);
			airport.push_back(to);
		}
		int fromIndex = indexMap[from];
		int toIndex = indexMap[to];
		graph[fromIndex].push_back(make_pair(toIndex, distance));
		graph[toIndex].push_back(make_pair(fromIndex, distance));
	}

	// allows us to print the route that we have found through dijkstra's algorithm 
	void print_route(vector<int> const& prev, int i) {
		if (i < 0) {
			return;
		}
		print_route(prev, prev[i]);
		cout << airport[i] << " <- ";
	}

	// dijkstra's algorithm to find the shortest distance between two airports 
	int dijkstra(string from, string to) {
		if (indexMap.count(from) == 0 || indexMap.count(to) == 0) {
			return -1; // to determine if a path even exists in the graph 
		}
		int toIndex = indexMap[to]; 
		int fromIndex = indexMap[from];
		priority_queue< iPair, vector <iPair>, greater<iPair> > pq;
		vector<int> prev(graph.size(), -1); // find the choosen path 
		vector<int> dist(graph.size(), INFINITY); 
		pq.push(make_pair(0, toIndex));
		dist[toIndex] = 0;
		while (!pq.empty())
		{

			int u = pq.top().second;
			pq.pop();
			vector< pair<int, int> >::iterator i;
			for (i = graph[u].begin(); i != graph[u].end(); ++i)
			{
				int v = (*i).first;
				int weight = (*i).second;

				if (dist[v] > dist[u] + weight)
				{
					dist[v] = dist[u] + weight;
					pq.push(make_pair(dist[v], v));
					prev[v] = u;
				}
			}
		}
		print_route(prev, fromIndex); // prints the shortest route between two airport 
		return dist[fromIndex]; // returns the shortest distance between two points
	}
} Graph;


int main() {
	ifstream input;
	input.open("airports.csv");
	string line;
	Graph graph;
	string airport1, airport2;
	map<string, vector<int>> city_dict;
	int count = 0;
	while (input >> line) {
		int firstSplit = line.find_first_of(','); // Index of the first comma
		int secondSplit = line.find_last_of(','); // Index of the second comma 
		string from = line.substr(0, firstSplit); // From airport (departing airport) 
		string to = line.substr(firstSplit + 1, secondSplit - firstSplit - 1); // To airport (arrival airport)
		int distance = stoi(line.substr(secondSplit + 1)); // Distance 
		graph.addEdge(from, to, distance);
	}
	input.close();
	string answer = "Y";
	while (answer == "Y") {
	cout << "Select an airport to depart from: ";
	cin >> airport1;
	transform(airport1.begin(), airport1.end(), airport1.begin(), ::toupper);
	cout << "Select an airport to arrive at: ";
	cin >> airport2;
	transform(airport2.begin(), airport2.end(), airport2.begin(), ::toupper);
	int miles = graph.dijkstra(airport1, airport2);
	if (miles < 0) {
		cout << "There is no path between " << airport1 << " and " << airport2 << endl;
	}
	else {
		cout << miles << " miles" << endl;
	}
	cout << endl;
	cout << "Do you want to continue checking for routes?" << endl;
	cout << "Enter Y and press enter for more route checking. Enter any other key to quit" << endl;
	cin >> answer;
	}
	return 0;
}