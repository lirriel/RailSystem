#pragma warning (disable:4786)
#pragma warning (disable:4503)

#include "RailSystem.h"

void RailSystem::reset(void)
{

    // TODO: reset the data objects of the 
    // City objects' contained in cities

    for (map<string, City*>::iterator it = cities.begin(); it != cities.end(); it++)
    {
        (*it).second->visited = false;
        (*it).second->total_distance = 0;
        (*it).second->from_city = "";
        (*it).second->total_fee = INT_MAX;
    }
}

RailSystem::RailSystem(string const &filename)
{
    
    load_services(filename);
}

void RailSystem::load_services(string const &filename)
{

	ifstream inf(filename.c_str());
	string from, to;
	int fee, distance;

	while ( inf.good() )
    {

		// Read in the from city, to city, the fee, and distance.
		inf >> from >> to >> fee >> distance;

		if ( inf.good() )
        {
		
			// TODO: Add entries in the cities container and
			// and services in the rail system for the new 
            // cities we read in.

            // check the existence of such city in services' data base
            if (outgoing_services.find(from) == outgoing_services.end())
            {
                std::list<Service*> list1;
                outgoing_services.insert(std::make_pair(from, list1));

            }

            // add new railway between cities to database
            (&outgoing_services.at(from))->push_back(new Service(to, fee, distance));

            // if city is still missing, add to the list
            if (cities.find(from) == cities.end())
                cities.insert(std::make_pair(from, new City(from)));
        }
	}

	inf.close();
}

RailSystem::~RailSystem(void)
{

    // TODO: release all the City* and Service*
    // from cities and outgoing_services

    // clear cities
    for (map<string, City*>::iterator it = cities.begin(); it != cities.end(); it++)
        delete (*it).second;
    cities.clear();

    // clear services
    for (map<string, std::list<Service*> >::iterator it = outgoing_services.begin();
            it != outgoing_services.end(); it++)
    {
        for (list<Service*>::iterator it1 = (*it).second.begin(); it1 != (*it).second.end(); it1++)
            delete (*it1);

        (*it).second.clear();
    }
    outgoing_services.clear();
}

void RailSystem::output_cheapest_route(const string& from,
                const string& to, ostream& out)
{

	reset();
	pair<int, int> totals = calc_route(from, to);

	if (totals.first == INT_MAX)
		out << "There is no route from " << from << " to " << to << "\n";
    else
    {
		out << "The cheapest route from " << from << " to " << to << "\n";
		out << "costs " << totals.first << " euros and spans " << totals.second
			<< " kilometers\n";
        cout << recover_route(to) << "\n\n";
	}
}

bool RailSystem::is_valid_city(const string& name)
{

	return cities.count(name) == 1;
}

pair<int, int> RailSystem::calc_route(string from, string to)
{
    // You can use another container
	priority_queue<City*, vector<City*>, Cheapest> candidates;

    // TODO: Implement Dijkstra's shortest path algorithm to
    // find the cheapest route between the cities

    // fix the start point of journey
    candidates.push(cities[from]);
    cities[from]->total_fee = 0;
    cities[from]->total_distance = 0;
    cities[from]->visited = true;

    // finding shortest paths to cities from 'from'
    while (!candidates.empty())
    {
        // take city with cheapest total fee
        City* city = candidates.top();
        // set city as visited
        city->visited = true;
        candidates.pop();

        // fo through the list of adjacency cities
        std::list<Service*> list1 = outgoing_services[city->name];
        for(list<Service*>::iterator it = list1.begin(); it != list1.end(); it++)
        {
            // take on of reachable from 'city' cities
            City* currentCity = cities[(*it)->destination];

            // and compare costs
            if (currentCity->total_fee > city->total_fee + (*it)->fee)
            {
                // reset total_fee
                currentCity->total_fee = city->total_fee + (*it)->fee;
                // reset total_distance
                currentCity->total_distance = city->total_distance + (*it)->distance;
                // point the city from which can get
                currentCity->from_city = city->name;

                // new city for consideration
                candidates.push(currentCity);
            }

        }
    }


    // Return the total fee and total distance.
    // Return (INT_MAX, INT_MAX) if not path is found.
    if (cities[to]->visited)
        return pair<int,int>(cities[to]->total_fee, cities[to]->total_distance);
    else
        return pair<int,int>(INT_MAX, INT_MAX);
}

string RailSystem::recover_route(const string& city)
{
    // start from the final city
    City* currentCity = cities[city];
    string from = "";
    string path = city;

    // move backward until the start city
    while (currentCity->from_city != "")
    {
        from = currentCity->from_city;
        currentCity = cities[from];

        // form path description
        path = from + " to " + path;
    }

    return path;
}


Route RailSystem::getCheapestRoute(const string& from, const string& to)
{
    assert(is_valid_city(from));
    assert(is_valid_city(to));
    reset();
    pair<int, int> p =  calc_route(from,to);
    return Route(from,to,p.first,p.second);
}