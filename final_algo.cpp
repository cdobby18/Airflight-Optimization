#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>

using namespace std;

#define INF INT_MAX
#define num_of_cities 5
#define initial_num_of_flights 5

int cost[num_of_cities][num_of_cities] = {{0, 4, 3, 9, 8},
                                          {4, 0, 7, INF, 10},
                                          {3, 7, 0, INF, 9},
                                          {9, INF, INF, 0, 16},
                                          {8, INF, 9, INF, 0}};

string cities[num_of_cities] = {"MNL", "TYO", "BKK", "KWT", "SYD"};

class Flights
{
public:
    string flight_id;
    int flight_cost;
    string flight_path;
    bool high_demand;

    Flights(int source, int destination, bool demand) : high_demand(demand)
    {
        flight_id = flight_id_generator();
        flight_cost = dijkstra(source, destination);
        flight_path = pathing(source, destination);
    }
    static bool compare_sort(const Flights &a, const Flights &b)
    {
        return a.flight_cost < b.flight_cost;
    }

private:
    string flight_id_generator()
    {
        string id;
        random_device rand;
        mt19937 gen(rand());
        uniform_int_distribution<> alpha(65, 90);
        uniform_int_distribution<> numeric(0, 9);
        for (int i = 0; i < 2; i++)
        {
            id += static_cast<char>(alpha(gen));
        }
        for (int i = 0; i < 3; i++)
        {
            id += std::to_string(numeric(gen));
        }
        return id;
    }

    int dijkstra(int source, int destination)
    {
        vector<int> dist(num_of_cities, INF);
        vector<int> parent(num_of_cities, -1);
        vector<bool> visited(num_of_cities, false);

        dist[source] = 0;

        for (int i = 0; i < num_of_cities; i++)
        {
            int minDist = INF;
            int minIndex;

            for (int j = 0; j < num_of_cities; j++)
            {
                if (!visited[j] && dist[j] <= minDist)
                {
                    minDist = dist[j];
                    minIndex = j;
                }
            }

            visited[minIndex] = true;

            for (int j = 0; j < num_of_cities; j++)
            {
                if (!visited[j] && cost[minIndex][j] != INF && dist[minIndex] + cost[minIndex][j] < dist[j])
                {
                    dist[j] = dist[minIndex] + cost[minIndex][j];
                    parent[j] = minIndex;
                }
            }
        }
        return dist[destination];
    }

    string pathing(int source, int destination)
    {
        string path;
        path = cities[source] + " -> " + cities[destination];
        return path;
    }
};

class Aircraft
{
public:
    string aircraft_id;
    int maintenance;
    int planeHours;
    int planehours2;

    Aircraft(string id, int main) : aircraft_id(id), maintenance(main), planeHours(0){};

    static void initialize_aircraft(string filename, vector<Aircraft> &a)
    {
        string id;
        int cap, main;
        ifstream input(filename);
        if (!input)
        {
            cerr << "Error Opening aircrafts.txt!";
        }
        string line;
        getline(input, line);
        while (input >> id >> main)
        {
            a.push_back(Aircraft(id, main));
        }
    }
    void setPlaneHours(int hours)
    { // setter ng planeHours for calculations
        this->planeHours = this->planeHours + hours;
    }
    int getPlaneHours()
    { // getter ng planeHours for calculations
        return this->planeHours;
    }
    void doneMaintenance()
    { // kada after maintenance
        this->planeHours = 0;
    }
};

struct CrewMember
{
    std::string name;
    bool available;

    CrewMember(const std::string &n, bool avail) : name(n), available(avail) {}
};
class Crew
{
public:
    std::vector<CrewMember> pilots;
    std::vector<CrewMember> copilots;
    std::vector<CrewMember> flight_attendants;

    // Constructor to populate the crew members and availability
    Crew()
    {
        // Initialize random number generator
        srand(time(nullptr));

        // Populate pilots and their availability
        populateWithRandomAvailability(pilots, {"Cheryl Gentry", "Louisa Holmes", "Layton Rubio", "Kristina Swanson", "Rafferty Frazier",
                                                "Lillie Stein", "Audrey Hart", "Isobel Bridges", "Gerald Frank", "Haseeb House",
                                                "Eshal Callahan", "Ivan Le", "Callum Myers", "Katrina Nichols", "Bryony Jarvis",
                                                "Brett Hendrix", "Oliver Alexander", "Shania Saunders", "Lilia Avila", "Muhammed Dixon"});

        // Populate copilots and their availability
        populateWithRandomAvailability(copilots, {"Rosie Chan", "Alyssia Aguirre", "Tom Bender", "Priya Irwin", "Nikodem Berry",
                                                  "Frankie Donovan", "Penny Lamb", "Jason Macias", "Ronnie Scott", "Noel Cochran",
                                                  "Siena Schultz", "Jessie Gardner", "Anais Everett", "Lulu Shepherd", "Monty Boyd",
                                                  "Soraya O'Sullivan", "Beatrix Evans", "Angela Shepard", "Gabrielle Glover", "Aleena Huffman"});

        // Populate flight attendants and their availability
        populateWithRandomAvailability(flight_attendants, {"Ezekiel Carney", "Abdulrahman Chan", "Victoria Newman", "Saffron Lowe", "Jacques Pineda",
                                                           "Sachin Meyers", "Antonia Jimenez", "Cecil Wilkinson", "Andrew Oconnor", "Honey Randall",
                                                           "Francesca Bowen", "Mitchell Larsen", "Carmen Walker", "Neve Lozano", "Yusra Ryan",
                                                           "Ali Osborn", "Kobi Santiago", "Francesco Prince", "Ebony Everett", "Hope Morrison",
                                                           "Chelsea Doyle", "Amirah Gaines", "Suzanne Bright", "Antony Hatfield", "Lois Frost",
                                                           "Habiba Boyd", "Rhiannon Payne", "Felicity Blanchard", "Harmony Hutchinson", "Gethin Todd",
                                                           "Ashleigh Pittman", "Sahil Rios", "Tegan Burgess", "Ilyas Horn", "Ronald Diaz", "Corey Lynn",
                                                           "Bartosz Edwards", "Axel Potts", "Will Carroll", "Kamil Villegas"});

        // Sort availability arrays with true being the first
        mergeSortAvailability(pilots);
        mergeSortAvailability(copilots);
        mergeSortAvailability(flight_attendants);
    }
    void displayAvailability(const std::vector<CrewMember> &crew)
    {
        for (const auto &member : crew)
        {
            cout << left << setfill(' ') << setw(20) << member.name << ": " << (member.available ? "Yes" : "No") << endl;
        }
    }

private:
    // Helper function to populate crew members and availability
    void populateWithRandomAvailability(std::vector<CrewMember> &crew,
                                        const std::vector<std::string> &names)
    {
        // Populate crew members
        for (const auto &name : names)
        {
            // Determine availability randomly
            bool avail = (rand() % 2 == 0); // 50% chance of being available
            crew.emplace_back(name, avail);
        }
    }

    // Helper function to merge sort availability with true being the first
    void mergeSortAvailability(std::vector<CrewMember> &crew)
    {
        // Implement merge sort
        if (crew.size() <= 1)
            return;

        std::vector<CrewMember> left_crew, right_crew;

        size_t mid = crew.size() / 2;

        for (size_t j = 0; j < mid; ++j)
        {
            left_crew.push_back(crew[j]);
        }

        for (size_t j = mid; j < crew.size(); ++j)
        {
            right_crew.push_back(crew[j]);
        }

        mergeSortAvailability(left_crew);
        mergeSortAvailability(right_crew);

        crew.clear();

        size_t left_index = 0, right_index = 0;

        while (left_index < left_crew.size() && right_index < right_crew.size())
        {
            if (left_crew[left_index].available)
            {
                crew.push_back(left_crew[left_index]);
                left_index++;
            }
            else
            {
                crew.push_back(right_crew[right_index]);
                right_index++;
            }
        }

        while (left_index < left_crew.size())
        {
            crew.push_back(left_crew[left_index]);
            left_index++;
        }

        while (right_index < right_crew.size())
        {
            crew.push_back(right_crew[right_index]);
            right_index++;
        }
    }
};

class Schedule
{
public:
    vector<Flights> flight;
    vector<Aircraft> aircraft;

    Schedule(vector<Flights> flight, vector<Aircraft> aircraft)
        : flight(flight), aircraft(aircraft)
    {
    }
};

void simulation(vector<Schedule> &s1, vector<Schedule> &s2, vector<Aircraft> &ac, vector<Flights> &depart, vector<Flights> &arrive, int source)
{
    int depsize = depart.size();

    for (int hour = 0; hour < 24; hour++)
    {
        system("cls");
        cout << setfill('0') << setw(2) << hour << ":"
             << "00\n\n";

        int departure_hour[depsize] = {0};
        int departure_mins[depsize] = {0};
        int arrival_hour[depsize] = {0};
        int arrival_mins[depsize] = {0};
        cout << "\nDEPARTURES\n";
        cout << "FLIGHT ORDER \t| FLIGHT ID \t| FLIGHT PATH \t| DEPARTURE TIME \t| ARRIVAL TIME \t| ASSIGNED AIRCRAFT \t| STATUS \n";
        cout << setfill('-') << setw(17) << "|" << setfill('-') << setw(72) << "|" << setfill('-') << setw(23) << " "
             << "|" << setfill('-') << setw(15) << " " << endl;

        for (int i = 0; i < depart.size(); i++)
        {

            int planehour = 2 * s1[i].flight[i].flight_cost;
            ac[i].setPlaneHours(planehour);
            if (i > 0)
            {
                if (planehour > 14)
                    departure_hour[i] = (departure_hour[i - 1] + 4) % 24;
                else
                    departure_hour[i] = (departure_hour[i - 1] + 1) % 24;
            }
            arrival_hour[i] = (departure_hour[i] + s2[i].flight[i].flight_cost) % 24;
            cout << i + 1 << "\t\t"
                 << "| " << s1[i].flight[i].flight_id << "\t\t|" << s1[i].flight[i].flight_path << "\t|" << setfill('0') << setw(2) << departure_hour[i] << ":" << setfill('0') << setw(2) << departure_mins[i] << "\t\t\t| " << setfill('0') << setw(2) << arrival_hour[i] << ":" << setfill('0') << setw(2) << arrival_mins[i] << "\t\t|" << s1[i].aircraft[i].aircraft_id;
            if (hour == departure_hour[i])
                cout << "\t\t\t    Departed";
            else if (hour == arrival_hour[i])
                cout << "\t\t\t    Arrived";
            else
                cout << "\t\t\t      -  ";

            cout << endl;
        }

        for (int i = 0; i < depsize; i++)
        {
            departure_hour[i] = arrival_hour[i];
            departure_mins[i] = arrival_mins[i];
        }

        arrival_hour[depsize] = 0;
        arrival_mins[depsize] = 0;
        cout << endl
             << endl;

        cout << "\nARRIVALS\n";
        cout << "FLIGHT ORDER \t| FLIGHT ID \t| FLIGHT PATH \t| DEPARTURE TIME \t| ARRIVAL TIME \t| ASSIGNED AIRCRAFT \t| STATUS \n";
        cout << setfill('-') << setw(17) << "|" << setfill('-') << setw(72) << "|" << setfill('-') << setw(23) << " "
             << "|" << setfill('-') << setw(15) << " " << endl;
        for (int i = 0; i < arrive.size(); i++)
        {
            int planehour = 2 * s2[i].flight[i].flight_cost;

            if (i > 0)
            {

                if (planehour > 14)
                    departure_hour[i] = (departure_hour[i - 1] + 4) % 24;
                else
                    departure_hour[i] = (departure_hour[i - 1] + 1) % 24;
            }
            arrival_hour[i] = (departure_hour[i] + s2[i].flight[i].flight_cost + 1) % 24;
            if (hour == 1 + departure_hour[i])
            {
                cout << i + 1 << "\t\t"
                     << "| " << s2[i].flight[i].flight_id << "\t\t|" << s2[i].flight[i].flight_path << "\t|" << setfill('0') << setw(2) << departure_hour[i] + 1 << ":" << setfill('0') << setw(2) << departure_mins[i] << "\t\t\t| " << setfill('0') << setw(2) << arrival_hour[i] << ":" << setfill('0') << setw(2) << arrival_mins[i] << "\t\t|" << s2[i].aircraft[i].aircraft_id << "\t\t\t    Departed";
            }

            else if (hour == arrival_hour[i] && departure_hour[i] < arrival_hour[i])
            {
                cout << i + 1 << "\t\t"
                     << "| " << s2[i].flight[i].flight_id << "\t\t|" << s2[i].flight[i].flight_path << "\t|" << setfill('0') << setw(2) << departure_hour[i] + 1 << ":" << setfill('0') << setw(2) << departure_mins[i] << "\t\t\t| " << setfill('0') << setw(2) << arrival_hour[i] << ":" << setfill('0') << setw(2) << arrival_mins[i] << "\t\t|" << s2[i].aircraft[i].aircraft_id << "\t\t\t    Arrived";
            }

            else
            {
                cout << i + 1 << "\t\t"
                     << "| " << s2[i].flight[i].flight_id << "\t\t|" << s2[i].flight[i].flight_path << "\t|" << setfill('0') << setw(2) << departure_hour[i] + 1 << ":" << setfill('0') << setw(2) << departure_mins[i] << "\t\t\t| " << setfill('0') << setw(2) << arrival_hour[i] << ":" << setfill('0') << setw(2) << arrival_mins[i] << "\t\t|" << s2[i].aircraft[i].aircraft_id << "\t\t\t      -  ";
            }

            cout << endl;
        }
        cout << endl;

        for (int i = 0; i < ac.size(); i++)
            ac[i].doneMaintenance();

        for (int i = 0; i < ac.size(); i++)
        {
            int planehour = 2 * s2[i].flight[i].flight_cost;
            if (planehour < 14 && hour == departure_hour[i])
                cout << "Aircraft " << ac[i].aircraft_id << "'s Crew has rested for 2 hours" << endl;

            else if (planehour >= 14 && hour == 1 + departure_hour[i])
                cout << "Aircraft " << ac[i].aircraft_id << "'s Crew has rested for 8 hours" << endl;

            if (planehour >= 16 && hour == arrival_hour[i] && departure_hour[i] < arrival_hour[i])
            {
                cout << "Aircraft " << ac[i].aircraft_id << " has undergone maintenance at time " << arrival_hour[i] << ":00" << endl;
                ac[i].doneMaintenance();
            }
            if (planehour >= 14 && hour == arrival_hour[i] && departure_hour[i] < arrival_hour[i])
            {
                cout << "Aircraft " << ac[i].aircraft_id << "'s Crew has changed at " << arrival_hour[i] << ":00" << endl;
                ac[i].doneMaintenance();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void processAirport(int source, vector<Flights> &depart, vector<Flights> &arrive, vector<Aircraft> &a)
{
    system("cls");
    int booking, yn;
    vector<Schedule> sched;
    vector<Schedule> sched2;
    cout << "Enter Demand for each flight: \n\n";
    cout << "[1] High Demand | [0] No Demand\n";
    for (int i = 0; i < initial_num_of_flights; i++)
    {
        if (source != i)
        {
            cout << endl;
            cout << cities[source] + " -> " + cities[i] << endl;
            cout << ">";
            cin >> yn;
            while (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\nInvalid Input!\n";
                cout << "> ";
                cin >> yn;
            }

            if (yn == 1)
            {
                depart.push_back(Flights(source, i, true));
                arrive.push_back(Flights(i, source, true));
            }
            else
            {
                depart.push_back(Flights(source, i, false));
                arrive.push_back(Flights(i, source, false));
            }
        }
    }

    sort(depart.begin(), depart.end(), Flights::compare_sort);
    sort(arrive.begin(), arrive.end(), Flights::compare_sort);

    vector<Flights> high_demand_flights;
    vector<Flights> high_demand_flights2;

    // Move high-demand flights to the front of the depart vector, prioritizing by flight cost
    for (int i = depart.size() - 1; i >= 0; i--)
    {
        if (depart[i].high_demand)
        {
            high_demand_flights.push_back(depart[i]); // Store high-demand flight
            depart.erase(depart.begin() + i);         // Remove high-demand flight from its original position
        }
    }
    // Sort high-demand flights by flight cost
    sort(high_demand_flights.begin(), high_demand_flights.end(), Flights::compare_sort);
    // Place sorted high-demand flights at the beginning of the depart vector
    for (int i = high_demand_flights.size() - 1; i >= 0; i--)
    {
        depart.insert(depart.begin(), high_demand_flights[i]);
    }
    ////////////////////////
    for (int i = arrive.size() - 1; i >= 0; i--)
    {
        if (arrive[i].high_demand)
        {
            high_demand_flights2.push_back(arrive[i]); // Store high-demand flight
            arrive.erase(arrive.begin() + i);          // Remove high-demand flight from its original position
        }
    }
    // Sort high-demand flights by flight cost
    sort(high_demand_flights2.begin(), high_demand_flights2.end(), Flights::compare_sort);
    // Place sorted high-demand flights at the beginning of the depart vector
    for (int i = high_demand_flights2.size() - 1; i >= 0; i--)
    {
        arrive.insert(arrive.begin(), high_demand_flights2[i]);
    }

    /////////////////////////////
    for (int i = 0; i < depart.size(); i++)
        sched.push_back(Schedule(depart, a));

    for (int i = 0; i < arrive.size(); i++)
        sched2.push_back(Schedule(arrive, a));
    int depsize = depart.size();
    int departure_hour[depsize] = {0};
    int departure_mins[depsize] = {0};
    int arrival_hour[depsize] = {0};
    int arrival_mins[depsize] = {0};
    cout << "\nDEPARTURES\n";
    cout << "FLIGHT ORDER \t| FLIGHT ID \t| FLIGHT PATH \t| DEPARTURE TIME \t| ARRIVAL TIME \t| ASSIGNED AIRCRAFT \n";
    cout << setfill('-') << setw(17) << "|" << setfill('-') << setw(72) << "|" << setfill('-') << setw(20) << " " << endl;

    for (int i = 0; i < depart.size(); i++)
    {
        int planehour = 2 * sched[i].flight[i].flight_cost;
        a[i].setPlaneHours(planehour);
        if (i > 0)
        {
            if (planehour > 14)
                departure_hour[i] = (departure_hour[i - 1] + 4) % 24;
            else
                departure_hour[i] = (departure_hour[i - 1] + 1) % 24;
        }
        arrival_hour[i] = (departure_hour[i] + sched[i].flight[i].flight_cost) % 24;

        cout << i + 1 << "\t\t"
             << "| " << sched[i].flight[i].flight_id << "\t\t|" << sched[i].flight[i].flight_path << "\t|" << setfill('0') << setw(2) << departure_hour[i] << ":" << setfill('0') << setw(2) << departure_mins[i] << "\t\t\t| " << setfill('0') << setw(2) << arrival_hour[i] << ":" << setfill('0') << setw(2) << arrival_mins[i] << "\t\t|" << sched[i].aircraft[i].aircraft_id << endl;
    }

    for (int i = 0; i < depsize; i++)
    {
        departure_hour[i] = arrival_hour[i];
        departure_mins[i] = arrival_mins[i];
    }

    arrival_hour[depsize] = 0;
    arrival_mins[depsize] = 0;
    cout << endl
         << endl;

    cout << "\nARRIVALS\n";
    cout << "FLIGHT ORDER \t| FLIGHT ID \t| FLIGHT PATH \t| DEPARTURE TIME \t| ARRIVAL TIME \t| ASSIGNED AIRCRAFT \n";
    cout << setfill('-') << setw(17) << "|" << setfill('-') << setw(72) << "|" << setfill('-') << setw(20) << " " << endl;
    for (int i = 0; i < arrive.size(); i++)
    {
        int planehour = 2 * sched[i].flight[i].flight_cost;
        if (i > 0)
        {

            if (planehour > 14)
                departure_hour[i] = (departure_hour[i - 1] + 4) % 24;
            else
                departure_hour[i] = (departure_hour[i - 1] + 1) % 24;
        }
        arrival_hour[i] = (departure_hour[i] + sched2[i].flight[i].flight_cost + 1) % 24;

        cout << i + 1 << "\t\t"
             << "| " << sched2[i].flight[i].flight_id << "\t\t|" << sched2[i].flight[i].flight_path << "\t|" << setfill('0') << setw(2) << departure_hour[i] + 1 << ":" << setfill('0') << setw(2) << departure_mins[i] << "\t\t\t| " << setfill('0') << setw(2) << arrival_hour[i] << ":" << setfill('0') << setw(2) << arrival_mins[i] << "\t\t|" << sched2[i].aircraft[i].aircraft_id << endl;
    }
    cout << endl;
    for (int i = 0; i < a.size(); i++)
    {

        a[i].planeHours = a[i].getPlaneHours();

        if (a[i].planeHours >= a[i].maintenance)
        {
            cout << "Aircraft " << a[i].aircraft_id << " has undergone maintenance at time " << arrival_hour[i] << ":00" << endl;
            a[i].doneMaintenance();
        }
    }

    int c;
    while (true)
    {

        cout << "\n< Back - 1                              Simulation - 2\n";
        cout << "> ";
        cin >> c;
        if (c == 1)
        {
            sched.clear();
            sched2.clear();
            depart.clear();
            arrive.clear();

            for (int i = 0; i < a.size(); i++)
            {
                arrival_hour[i] = {};
                departure_hour[i] = {};
                a[i].doneMaintenance();
            }
            return;
        }
        else if (c == 2)
        {
            simulation(sched, sched2, a, depart, arrive, source);
        }
        else
        {
            cout << "\nInvalid Input!\n";
        }
    }
}

void crew_menu(vector<Aircraft> &a, vector<vector<string>> &fleet747, vector<vector<string>> &fleet330)
{
    Crew crew;
    // Populate pilots for fleet747 747 aircraft
    int fa_count = 2;
    for (int i = 0; i < a.size(); i++)
    {

        int len = a[i].aircraft_id.length();
        string type = a[i].aircraft_id.substr(len - 3, 3);
        if (type == "747")
        {
            // Assign crew members to the fleet747 vector
            fleet747[i][0] = crew.pilots[i].name;
            fleet747[i][1] = crew.copilots[i].name;
            for (int j = 2; j < 8; j++)
            {
                fleet747[i][j] = crew.flight_attendants[fa_count].name;
                fa_count++;
            }
        }
        else if (type == "330")
        {
            fleet330[i][0] = crew.pilots[i].name;
            fleet330[i][1] = crew.copilots[i].name;
            for (int j = 2; j < 7; j++)
            {
                fleet330[i][j] = crew.flight_attendants[fa_count].name;
                fa_count++;
            }
        }
    }
}

void display_crew(vector<Aircraft> &a, vector<vector<string>> &fleet747, vector<vector<string>> &fleet330)
{
    for (int i = 0; i < a.size(); i++)
    {
        int len = a[i].aircraft_id.length();
        string type = a[i].aircraft_id.substr(len - 3, 3);
        cout << a[i].aircraft_id << " \n";
        if (type == "747")
        {

            cout << "Pilot: " << fleet747[i][0] << endl;
            cout << "Co-pilot: " << fleet747[i][1] << endl;
            cout << "Flight Attendants: ";
            for (int j = 2; j < 8; j++)
            {
                cout << fleet747[i][j] << ", ";
            }
        }
        else if (type == "330")
        {
            cout << "Pilot: " << fleet330[i][0] << endl;
            cout << "Co-pilot: " << fleet330[i][1] << endl;
            cout << "Flight Attendants: ";
            for (int j = 2; j < 7; j++)
            {
                cout << fleet330[i][j] << ", ";
            }
        }
        cout << endl
             << endl;
    }
}

int main()
{
    int o;
    int source;
    vector<vector<string>> crew;
    string choice;
    vector<Flights> depart;
    vector<Flights> arrive;
    vector<Aircraft> a;
    vector<vector<string>> fleet747(4, vector<string>(8));
    vector<vector<string>> fleet330(4, vector<string>(7));

    Crew c;

    Aircraft::initialize_aircraft("aircrafts.txt", a);
    crew_menu(a, fleet747, fleet330);

    while (true)
    {
        system("cls");
        cout << "   __ _____   ___     ______  _____  _______    ______   ___________ ________  ___________ _________  __  ____   _______ " << endl;
        cout << "  / // / _ | / _ \\__ / /  _/ /  _/ |/ / ___/   / __/ /  /  _/ ___/ // /_  __/ / __/ ___/ // / __/ _ \\/ / / / /  / __/ _ \\" << endl;
        cout << " / _  / __ |/ // / // // /  _/ //    / /___   / _// /___/ // (_ / _  / / /   _\\ \\/ /__/ _  / _// // / /_/ / /__/ _// , _/  " << endl;
        cout << "/_//_/_/ |_/____/\\___/___/ /___/_/|_/\\___(_) /_/ /____/___/\\___/_//_/ /_/   /___/\\___/_//_/___/____/\\____/____/___/_/|_| " << endl
             << endl;

        cout << "[1]. Airport\t\t\t"
             << "[2]. Crew\t\t\t"
             << "[3]. Crew Availability\t\t\t"
             << "[4]. Exit\t\t\t\n";
        cout << "\n> ";
        cin >> o;
        while (cin.fail() || o <= 0 || o > 4)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid Input!\n";
            cout << "> ";
            cin >> o;
        }
        if (o == 1)
        {
            cout << "Choose an Airport: \n";
            cout << "    __  ____   ____                ________  ______              ____  __ __ __ __             __ ___       ________            _______  ______ " << endl;
            cout << "   /  |/  / | / / /               /_  __/\\ \\/ / __ \\            / __ )/ //_// //_/            / //_/ |     / /_  __/           / ___/\\ \\/ / __ \\" << endl;
            cout << "  / /|_/ /  |/ / /      ______     / /    \\  / / / /  ______   / __  / ,<  / ,<     ______   / ,<  | | /| / / / /    ______    \\__ \\  \\  / / / /" << endl;
            cout << " / /  / / /|  / /___   /_____/    / /     / / /_/ /  /_____/  / /_/ / /| |/ /| |   /_____/  / /| | | |/ |/ / / /    /_____/   ___/ /  / / /_/ / " << endl;
            cout << "/_/  /_/_/ |_/_____/             /_/     /_/\\____/           /_____/_/ |_/_/ |_|           /_/ |_| |__/|__/ /_/              /____/  /_/_____/  " << endl
                 << endl;

            do
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "> ";
                cin >> choice;
                if (choice == "MNL")
                {
                    source = 0;
                    break;
                }

                else if (choice == "TYO")
                {
                    source = 1;
                    break;
                }

                else if (choice == "BKK")
                {
                    source = 2;
                    break;
                }

                else if (choice == "KWT")
                {
                    source = 3;
                    break;
                }

                else if (choice == "SYD")
                {
                    source = 4;
                    break;
                }

                else
                {
                    cout << "\nInvalid Input!\n";
                    continue;
                }

            } while (choice != "SYD" || choice != "KWT" || choice != "BKK" || choice != "TYO" || choice != "MNL");
            cin.ignore();
            cin.clear();
            processAirport(source, depart, arrive, a);
        }
        else if (o == 2)
        {
            system("cls");
            display_crew(a, fleet747, fleet330);
            system("pause");
        }
        else if (o == 3)
        {
            system("cls");
            cout << "\t\tAvailability\n";
            cout << "Pilots:\n";
            c.displayAvailability(c.pilots);
            cout << endl;
            cout << "Copilots:\n";
            c.displayAvailability(c.copilots);
            cout << endl;
            cout << "Flight Attendantss:\n";
            c.displayAvailability(c.flight_attendants);
            cout << endl;
            system("pause");
        }
        else if (o == 4)
        {
            cout << "\nThank You";
            return 0;
        }
        else
        {
            cout << "\nInvalid Input!\n";
        }
    }
}
