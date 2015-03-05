/*
Kelvin Nguyen
603892561

Poker Manager: a program that tracks a player's poker sessions.

*/

#include <iostream>
#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
#include <numeric>
#include <conio.h>
#include <stdexcept>
#include <cassert>

using namespace std;

// Constant global variables:
const char delim = ':';
const char slash = '/';
const int hoursPerDay = 24;
const int minutesPerHour = 60;
const int monthsPerYear = 12;
const int daysPerMonth = 31;
const string FILENAME = "SessionDatabase.txt"; // SessionDatabase.txt
int NumberOfSessionsCounter = 0; // counts total number of sessions added
int session_line_entries = 10; // count white space/empty lines too; for how many lines that each session takes
int total_net_profit_value_line = 4; // index for 'Total net profit' line
int total_time_played_line = 3; // index for 'Total time played' line
int location_line = 5; // index for 'Location' line
int game_type_line = 6; // index for 'Game-Type' line
int blinds_line = 7; // index for 'Blinds' line

// Function Declarations:
void getTimeStarted(int &, int &); // prints out time user starts session
void getTimeEnded(int &, int &); // prints out time user ends session
bool isValidTime(int, int); // checks if time the user entered is valid
int word_counter(string target_word); // counts how many times the target word appears in txt.file

// More Function Declarations (for editing/adding session database):
void input_session();
void show_all_sessions();
void delete_entire_database();
void delete_session();
void stats_overview();
void exit_program();

// Sorting for maps, such that value is sorted, other than keys:
template <typename T1, typename T2>
struct less_second
{
	typedef pair<T1, T2> type;
	bool operator() (type const&a, type const& b) const
	{
		return a.second < b.second;
	}
};

int main()
{
	// Title of program
	system("title Poker Manager Program");
	system("cls");
	system("color 0F");

	// Welcome message:
	cout << "POKER MANAGER" << endl << endl;

	// Text file:
	cout << "Textfile: " << FILENAME << endl;

	// Program Menu
	cout << "Program Menu:" << endl;
	int choice;
	cout << "1. Input a new session" << endl;
	cout << "2. Show entire database for poker sessions" << endl;
	cout << "3. Clear all sessions" << endl;
	cout << "4. Delete a session" << endl;
	cout << "5. Show overall statistics" << endl;
	cout << "6. Exit the Poker Manager program" << endl;

	cin >> choice;
	switch (choice)
	{
	case 1:
		input_session();
		break;
	case 2:
		show_all_sessions();
		break;
	case 3:
		delete_entire_database();
		break;
	case 4:
		delete_session();
		break;
	case 5:
		stats_overview();
		break;
	case 6:
		exit_program();
		break;

		cin.get();
	}

	return 0;
}

// Function Declarations:
void getTimeStarted(int &h, int &m) // function that prints out the time the user entered that starts the session
{
	string in;
	char c;
	stringstream ss;

	do
	{
		cout << "Enter time started (HH:MM) : ";
		getline(cin, in);
		ss.clear(); ss.str(in);
	} while (!(ss >> h >> c >> m) || ss.good() ||
		(c != delim) || (isValidTime(h, m) == false));
}

void getTimeEnded(int &h, int &m) // function that prints out the time the user entered to end the session
{
	string in;
	char c;
	stringstream ss;

	do
	{
		cout << "Enter time ended (HH:MM) : ";
		getline(cin, in);
		ss.clear(); ss.str(in);
	} while (!(ss >> h >> c >> m) || ss.good() ||
		(c != delim) || (isValidTime(h, m) == false));
}

bool isValidTime(int h, int m) // checks if time is valid
{
	return (h >= 0) && (h < hoursPerDay) && (m >= 0) && (m < minutesPerHour);
}

// Count how many times a word appears in a text file;
int word_counter(string word)
{
	int counter = 0;
	string target_word = word;

	ifstream fileStream(FILENAME);
	if (fileStream.is_open())
	while (fileStream.good())
	{
		string in;
		fileStream >> in;

		if (in == target_word)
			counter++;
	}

	return counter;

	fileStream.close();
}

void input_session() {
	// loading locations vector file:
	const char *locations[] = { "Commerce", "Bicycle", "Hawaiian Gardens", "Hollywood Park", "Pechanga", "Morongo", "Barona" };

	vector<string> playing_locations(locations, std::end(locations));
	vector<string> test1;

	// opening locations save text file;
	ofstream loc_open("locations.txt", ios::app);
	for (auto x : playing_locations)
		loc_open << x << endl;

	// Opening text file:
	ofstream myfile(FILENAME, ios::app);
	system("cls");

	// Variables
	int month, day, year;
	int hours_started;
	int minutes_started;
	int hours_ended;
	int minutes_ended;
	int total_time_hours;
	int total_time_mins;

	double total_buy_in;
	double total_cashed_out;

	string location;
	string game_type;
	string blinds;

	// Directions for date format:
	cout << "Date format: MM/DD/YYYY" << endl;
	cout << " where: MM = months, 1 ... 12" << endl;
	cout << " DD = days, 1 ... 31" << endl;
	cout << " YYYY = years" << endl << endl;

	// Date:
	do {
		cout << "Please enter the date. " << endl
			<< "Month (MM): ";
		cin >> month;
		cout << "Day (DD): ";
		cin >> day;
		cout << "Year (YYYY): ";
		cin >> year;
		cout << endl;
	} while (month > 12 || month < 1 || day > 31 || day < 1);

	// Time started, time ended:
	do
	{
		// Directions for time format:
		cout << "Time format: <HH>:<MM>" << endl;
		cout << " where: HH = hours, 0 ... 23" << endl;
		cout << " MM = minutes, 0 ... 59" << endl << endl;

		cin.sync();

		getTimeStarted(hours_started, minutes_started);
		cout << setw(2) << setfill('0') << hours_started << ':'
			<< setw(2) << setfill('0') << minutes_started << endl << endl;

		getTimeEnded(hours_ended, minutes_ended);
		cout << setw(2) << setfill('0') << hours_ended << ':'
			<< setw(2) << setfill('0') << minutes_ended << endl << endl;

		if (hours_started > hours_ended || (hours_started == hours_ended && minutes_started > minutes_ended))
			cout << "Invalid time. Please try again. " << endl;
	} while (hours_started > hours_ended || (hours_started == hours_ended && minutes_started > minutes_ended));

	// Total time played in that session:
	if (minutes_ended < minutes_started)
	{
		total_time_mins = minutesPerHour - minutes_started + minutes_ended;
		total_time_hours = hours_ended - hours_started - 1;
		//cout << "total time hours: " << total_time_hours << endl;
		//cout << "total time mins: " << total_time_mins << endl;
	}

	else
	{
		total_time_hours = hours_ended - hours_started;
		total_time_mins = minutes_ended - minutes_started;
	}

	// Earnings for that session:
	cout << "Enter total buy-in amount: $";
	cin >> total_buy_in;
	cout << "Enter total cash-out amount: $";
	cin >> total_cashed_out;
	double net_session_profit = abs(total_cashed_out) - abs(total_buy_in);

	cin.clear();
	cin.sync();

	// new method for entering location:
	bool location_exists = false;

	do
	{
		// Print the locations:
		cout << endl << "Locations: " << endl;
		for (int i = 0; i < playing_locations.size(); i++)
			cout << "(" << i + 1 << ") " << playing_locations[i] << endl;

		// Location entry:
		cout << endl << "Enter correspoding number for location, or press 0 to type in new location: ";
		int location_entry;
		cin >> location_entry;

		if (location_entry == 0)
		{
			string new_location_entry;
			cout << "Please enter a new location: ";
			cin.sync();
			cin.clear();
			getline(cin, new_location_entry);

			//check if new location does not already exist
			for (int i = 0; i < playing_locations.size(); i++)
			{
				if (playing_locations[i] == new_location_entry)
				{
					location_exists = true;
					cout << "Error: Location already exists. Please choose again. " << endl;
					break;
				}
				else
				{
					location_exists = false;
					location = new_location_entry;
				}
			}
		}
		if (location_entry > 0 && location_entry < playing_locations.size())
		{
			location_exists = false;
			location = playing_locations[location_entry - 1];
		}
	} while (location_exists == true);

	cout << "Location: " << location << endl;

	/*
	// Entering location of the session:
	cout << endl << "Enter the location of where you played at" << endl <<  "(ex. Commerce, Bicycle, Hawaiian Gardens, etc): ";
	getline(cin, location);
	*/
	cin.clear();
	cin.sync();

	// Entering the game-type of the session:
	cout << endl << "Enter the type of poker game" << endl << "(NL Texas Holdem, Limit Texas Holdem, Omaha, etc.): ";
	getline(cin, game_type);

	// Entering the blinds for the session:
	cout << endl << "Enter the blinds of poker game" << endl << "(1/2. 2/3, etc.): ";
	getline(cin, blinds);

	cout << endl << endl;
	// Printing to console the Session (everything - date, time, winings, etc.)
	cout << "Session " << month << "/" << day << "/" << year << endl
		<< "Time started: " << setw(2) << setfill('0') << hours_started << ':'
		<< setw(2) << setfill('0') << minutes_started << endl
		<< "Time ended: " << setw(2) << setfill('0') << hours_ended << ':'
		<< setw(2) << setfill('0') << minutes_ended << endl
		<< "Total time played: " << setw(2) << setfill('0') << total_time_hours << ':'
		<< setw(2) << setfill('0') << total_time_mins << endl
		<< fixed << setprecision(2)
		<< "Total net profit: $" << net_session_profit << endl
		<< "Location: " << location << endl
		<< "Game-type: " << game_type << endl
		<< "Blinds: " << blinds << endl
		<< endl << endl;

	// Printing to text file:
	myfile << "Session " << month << "/" << day << "/" << year << endl
		<< "Time started: " << setw(2) << setfill('0') << hours_started << ':'
		<< setw(2) << setfill('0') << minutes_started << endl
		<< "Time ended: " << setw(2) << setfill('0') << hours_ended << ':'
		<< setw(2) << setfill('0') << minutes_ended << endl
		<< "Total time played: " << setw(2) << setfill('0') << total_time_hours << ':'
		<< setw(2) << setfill('0') << total_time_mins << endl
		<< fixed << setprecision(2)
		<< "Total net profit: $" << net_session_profit << endl
		<< "Location: " << location << endl
		<< "Game-type: " << game_type << endl
		<< "Blinds: " << blinds << endl
		<< endl << endl;

	// Closing text file:
	myfile.close();
	cin.get();
	main();
}

void show_all_sessions()
{
	// Check to see if there are any sessions available
	int session_counter = word_counter("Session");
	if (session_counter == 0)
	{
		cout << endl << "No Sessions available. Please add a Session. " << endl;
		system("PAUSE");
	}

	// Variables
	vector<string> sessions;
	string line;
	int index;

	// Open file
	ifstream in(FILENAME);

	// Parse each line of text in txt file into sessions vector
	while (getline(in, line))
	{
		sessions.push_back(line);
	}

	system("cls"); // clear console screen

	// Print out list of Sessions
	// in order of:
	// month << ' ' << day << ' ' << year << ' ' << hours_started << ' ' << minutes_started << ' '
	// << hours_ended << ' ' << minutes_ended << ' ' << total_time_hours << ' ' << total_time_mins << ' '
	//	<< net_session_profit;
	for (int i = 0; i < sessions.size(); i++)
		cout << sessions[i] << endl;

	in.close();

	system("PAUSE");

	main();
}

void delete_entire_database()
{
	system("cls");
	string decision;
	cout << "Are you sure you want to delete the entire Session Datebase? [y] or [n]: ";
	cin >> decision;

	if (decision == "y")
	{
		ofstream openFile(FILENAME);
		system("cls");
		cout << "Sucessfully deleted " << FILENAME << "." << endl;
		system("pause");
		openFile.close();
		main();
	}

	else
		main();
}

void delete_session()
{
	// Check to see if there are any sessions available
	int session_counter = word_counter("Session");
	if (session_counter == 0)
	{
		system("cls");
		cout << "No Sessions available. Please add a Session. " << endl;
		cin.get();
	}

	// First show all the sessions to let the user choose which session to delete (by numerical value);
	else

	{
		// Variables
		vector<string> sessions;
		string line;
		int index;

		// Open file
		ifstream in(FILENAME);

		// Parse each line of text in txt file into sessions vector
		while (getline(in, line))
		{
			sessions.push_back(line);
		}

		system("cls"); // clear console screen

		// Print out list of Sessions with numerical value before each session so the User can choose which session to delete
		// Ex: (0) Session 4/25/2014... etc.
		// in order of:
		// month << ' ' << day << ' ' << year << ' ' << hours_started << ' ' << minutes_started << ' '
		// << hours_ended << ' ' << minutes_ended << ' ' << total_time_hours << ' ' << total_time_mins << ' '
		//	<< net_session_profit;

		int k = 0;
		for (int i = 0; i < sessions.size(); i++)
		{
			if (!(i% session_line_entries)) // for how many lines that each Session takes
			{
				cout << "(" << k << ") ";
				k++;
			}
			cout << sessions[i] << endl; // change to: cout << sessions[i] << endl; later
		}

		// Choose Session to delete:
		cout << "Please enter which Session you would like to delete, or press 'n' to exit: ";
		int delete_session_index;
		cin >> delete_session_index;
		if (!cin)
			return;
		if (delete_session_index > (session_counter - 1))
		{
			cout << "Invalid Session entry. " << endl;
			return;
		}

		else
		{
			int true_delete_session_index = (delete_session_index)*(session_line_entries);
			// Delete Sessions vector elements:
			sessions.erase(sessions.begin() + true_delete_session_index, sessions.begin() + true_delete_session_index + (session_line_entries));

			//cout << "session counter-1 : " << session_counter - 1 << endl;

			cout << "Are you sure you want to delete Session " << delete_session_index << "? (y/n) ";
			string ans;
			cin >> ans;

			if (ans == "y")
			{
				// Delete datbase:
				ofstream openFile(FILENAME);

				// Rewrite new database:
				for (int i = 0; i < sessions.size(); i++)
					openFile << sessions[i] << endl;

				openFile.close();

				cout << "Sucessfully deleted!" << endl;
				cin.get();
			}
		}
	}

	cin.get();
	main();
}

void stats_overview()
{
	system("cls");

	// Variables
	int total_sessions = word_counter("Session");
	vector<string> sessions_stats;
	string line;
	int index;
	vector<double>total_net_profit_values_vector; //stores net profit values
	vector<int> time_played_hours_vector; // stores total time played hours
	vector<int> time_played_mins_vector; // stores total time played minutes
	vector<string> locations_vector; // stores all the locations inputted
	int total_time_played_hours;
	int total_time_played_mins;

	//Open File:
	ifstream in(FILENAME);

	// Parse each line of text in txt file into sessions_stats vector:
	while (getline(in, line))
	{
		sessions_stats.push_back(line);
	}

	//TOTAL NET PROFITS CALCULATION:
	// parse through session text database and take only Total Net Profits:
	for (int i = total_net_profit_value_line; i < sessions_stats.size(); i += session_line_entries)
	{
		//cout << "line: " << sessions_stats[i] << endl;
		unsigned pos = sessions_stats[i].find(":");
		string str1 = sessions_stats[i].substr(pos + 3); // gets rid of : and 2 spaces and $
		//cout <<"str1: " << str1 << endl;
		double value = stod(str1);
		//cout << "value: " << value << endl;
		total_net_profit_values_vector.push_back(value);
	}

	// Overall profit/loss calculation:
	double overall_profit = accumulate(total_net_profit_values_vector.begin(), total_net_profit_values_vector.end(), 0.00);

	//TOTAL TIME PLAYED CALCULATION:
	// parse through session text database, take only Total Time played:
	for (int i = total_time_played_line; i < sessions_stats.size(); i += session_line_entries)
	{
		unsigned pos = sessions_stats[i].find(":");
		string str1 = sessions_stats[i].substr(pos + 2);
		//cout << "str1: " << str1 << endl;
		int time_hours = stoi(str1);
		//cout << "time_hours: " << time_hours << endl;
		time_played_hours_vector.push_back(time_hours);

		unsigned pos2 = str1.find(":");
		string str2 = str1.substr(pos2 + 1);
		//cout << "str2:" << str2 << endl;
		int time_mins = stoi(str2);
		//cout << "time_mins: " << time_mins << endl;
		time_played_mins_vector.push_back(time_mins);
	}

	// calculate total time played in hours, and minutes:
	// calculating hours...
	total_time_played_hours = accumulate(time_played_hours_vector.begin(), time_played_hours_vector.end(), 0);
	//cout << "total time played in hours: " << total_time_played_hours << endl;

	// calculating minutes...
	total_time_played_mins = accumulate(time_played_mins_vector.begin(), time_played_mins_vector.end(), 0);
	//cout << "total time played in mins: " << total_time_played_mins << endl;
	if (total_time_played_mins > minutesPerHour)
	{
		int remainder = total_time_played_mins % minutesPerHour;
		//cout << "Remainder: " << remainder << endl;
		int add_to_hours = total_time_played_mins / minutesPerHour;
		//cout << "Add to hours: " << add_to_hours << endl;

		total_time_played_mins = remainder;
		total_time_played_hours += remainder;
	}

	//HOURLY PROFIT/LOSS CALCULATION:
	//	cout << "Total time: " << total_time_played_hours << ":" << total_time_played_mins << endl;
	//	cout << "Overall Profit: " << overall_profit << endl;
	//
	// converting Total Time into minutes:
	double total_time_played_converted_into_mins = (total_time_played_hours * minutesPerHour) + total_time_played_mins;
	//	cout << "Total time in mins: " << total_time_played_converted_into_mins << endl;
	double pl_per_min = overall_profit / total_time_played_converted_into_mins;
	//cout << "Profit/loss per min: " << pl_per_min << endl;
	double pl_per_hour = pl_per_min * minutesPerHour;
	//cout << "Profit/loss per hour: " << pl_per_hour << endl;

	// AVERAGE PROFIT/LOSS PER SESSION CALCULATION:
	//cout << "Total sessions: " << total_sessions << endl;
	//cout << "Overall Profit: " << overall_profit << endl;
	double avg_profit_per_session = overall_profit / total_sessions;
	//cout << "Avg profit/loss per session: " << avg_profit_per_session << endl;

	// MOST PLAYED LOCATION CALCULATION:
	//parse through session text database, take only Locations
	for (int i = location_line; i < sessions_stats.size(); i += session_line_entries)
	{
		unsigned pos = sessions_stats[i].find(":");
		string str1 = sessions_stats[i].substr(pos + 2);
		//cout << "str1:" << str1 << endl;
		locations_vector.push_back(str1);
	}

	// Print locations_vector:
	//cout << "locations_vector: " << endl;
	//for (auto x : locations_vector)
	//	cout << x << endl;

	// Parse through elements in locations_vector and transfer into locations_map
	map <string, int> locations_map;
	//cout << endl << endl;

	for (int i = 0; i < locations_vector.size(); i++)
		locations_map[locations_vector[i]] = 0; // initialize each location's value to 0

	for (int i = 0; i < locations_vector.size(); i++)
		for (auto it = locations_map.begin(); it != locations_map.end(); ++it)
		{
			if (it->first == locations_vector[i])
				it->second++; // then iterate through the map and vector again and update the locations, based on how many times it occurs in the vector
		}

	//cout << "map: " << endl;
	//for (auto it = locations_map.begin(); it != locations_map.end(); ++it)
	//	cout << it->first << " " << it->second << endl;

	vector<pair<string, int>> mapcopy(locations_map.begin(), locations_map.end());
	sort(mapcopy.begin(), mapcopy.end(), less_second<string, int>());
	//cout << endl;
	//cout << "sorted map: " << endl;
	////for (auto x = mapcopy.begin(); x != mapcopy.end(); x++)
	//{
	//	cout << x->first << " " << x->second << endl;
	//}
	string most_played_location = mapcopy[mapcopy.size() - 1].first;
	int most_played_location_number = mapcopy[mapcopy.size() - 1].second;
	//cout << most_played_location << endl;
	//cout << most_played_location_number << endl;

	// MOST SUCCESSFUL LOCATION GAME-TYPE:

	//print total_net_profit_values vector
	//cout << endl;
	//for (auto x : total_net_profit_values_vector)
	//	cout << x << endl;

	//cout << endl << endl;

	//for (auto x : locations_vector)
	//	cout << x << endl;
	//cout << endl << endl;

	map<string, double> successful_locations_map;

	for (int i = 0; i < locations_vector.size(); i++)
		successful_locations_map[locations_vector[i]] += total_net_profit_values_vector[i]; // initialize each location's value to 0

	//for (auto it = successful_locations_map.begin(); it != successful_locations_map.end(); it++)
	//	cout << it->first << " " << it->second << endl;

	//cout << "map2: " << endl;
	//for (auto it = locations_map.begin(); it != locations_map.end(); ++it)
	//	cout << it->first << " " << it->second << endl;

	vector<pair<string, double>> mapcopy2(successful_locations_map.begin(), successful_locations_map.end());
	sort(mapcopy2.begin(), mapcopy2.end(), less_second<string, int>());
	//cout << endl;
	//cout << "sorted map2: " << endl;
	//	for (auto x = mapcopy2.begin(); x != mapcopy2.end(); x++)
	//	{
	//cout << x->first << " " << x->second << endl;
	//}
	string success_location = mapcopy2[mapcopy2.size() - 1].first;
	int success_location_number = mapcopy2[mapcopy2.size() - 1].second;
	//cout << "best location: " << success_location << endl;
	//cout << success_location_number << endl;

	// MOST PLAYED GAME TYPE CALCULATION:
	// parse through session text databsae, take only Game-Types:
	vector<string> game_types_vector;

	for (int i = game_type_line; i < sessions_stats.size(); i += session_line_entries)
	{
		unsigned pos = sessions_stats[i].find(":");
		string str1 = sessions_stats[i].substr(pos + 2);
		//cout << "str1:" << str1 << endl;
		game_types_vector.push_back(str1);
	}

	//print game_types vector:
	//cout << "game types vector: " << endl;
	//for (auto x : game_types_vector)
	//cout << x << endl;

	// Parse through elements in game_types vector and transfer into game_types map
	map <string, int> game_types_map;
	for (int i = 0; i < game_types_vector.size(); i++)
		game_types_map[game_types_vector[i]] = 0; // initialize each game type value to 0

	for (int i = 0; i < game_types_vector.size(); i++)
	{
		for (auto it = game_types_map.begin(); it != game_types_map.end(); it++)
		{
			if (it->first == game_types_vector[i])
				it->second++;
		}
	}

	//cout << "game types map: " << endl;
	//for (auto it = game_types_map.begin(); it != game_types_map.end(); it++)
	//cout << it->first << " " << it->second << endl;
	vector<pair<string, int>> game_types_map_copy(game_types_map.begin(), game_types_map.end());
	sort(game_types_map_copy.begin(), game_types_map_copy.end(), less_second<string, int>());
	//cout << "sorted map: " << endl;
	//for (auto x = game_types_map_copy.begin(); x != game_types_map_copy.end(); x++)
	//cout << x->first << " " << x->second << endl;
	string most_played_game_type = game_types_map_copy[game_types_map_copy.size() - 1].first;
	int most_played_game_type_number = game_types_map_copy[game_types_map_copy.size() - 1].second;

	// Biggest session win CALCULATION:
	//cout << "sessions " << endl;
	//sort(sessions_stats.begin(), sessions_stats.end());
	//for (auto x : sessions_stats)
	//	cout << x << endl;

	/*
		// parse through session text database and take only Total Net Profits:
		for (int i = total_net_profit_value_line; i < sessions_stats.size(); i += session_line_entries)
		{
		//cout << "line: " << sessions_stats[i] << endl;
		unsigned pos = sessions_stats[i].find(":");
		string str1 = sessions_stats[i].substr(pos+3); // gets rid of : and 2 spaces and $
		//cout <<"str1: " << str1 << endl;
		double value = stod(str1);
		//cout << "value: " << value << endl;
		total_net_profit_values_vector.push_back(value);
		}
		*/

	int max_profit = 0;
	int max_profit_index = 0;
	// parse through session text database and take only Total Net Profits:

	for (int i = total_net_profit_value_line; i < sessions_stats.size(); i += session_line_entries)
	{
		//cout << "line: " << sessions_stats[i] << endl;
		unsigned pos = sessions_stats[i].find(":");
		string str1 = sessions_stats[i].substr(pos + 3); // gets rid of : and 2 spaces and $
		//cout <<"str1: " << str1 << endl;
		double value = stod(str1);
		//cout << "value: " << value << endl;
		//total_net_profit_values_vector.push_back(value);
		if (max_profit < value)
		{
			max_profit += value;
			max_profit_index = i;
		}
	}

	//cout << max_profit << endl;
	//cout << max_profit_index << endl;

	//cout << sessions_stats[max_profit_index-4] << endl;
	//cout << sessions_stats[max_profit_index + 3] << endl;

	//Most played blinds CALCULATION:
	vector<string> blinds_vectors;
	for (int i = blinds_line; i < sessions_stats.size(); i += session_line_entries)
	{
		unsigned pos = sessions_stats[i].find(":");
		string str1 = sessions_stats[i].substr(pos + 2);
		blinds_vectors.push_back(str1);
	}

	//for (auto& x : blinds_vectors)
	//cout << x << " ";

	map<string, int> blinds_map;
	for (int i = 0; i < blinds_vectors.size(); i++)
	{
		blinds_map[blinds_vectors[i]] = 0;
	}

	for (int i = 0; i < blinds_vectors.size(); i++)
	{
		for (auto it = blinds_map.begin(); it != blinds_map.end(); it++)
		{
			if (it->first == blinds_vectors[i])
				it->second++;
		}
	}

	vector<pair<string, int>> blinds_map_copy(blinds_map.begin(), blinds_map.end());
	sort(blinds_map_copy.begin(), blinds_map_copy.end(), less_second<string, int>());

	//cout << endl << "sorted blinds_map_copy: " << endl;
	//for (auto x = blinds_map_copy.begin(); x != blinds_map_copy.end(); x++)
	//cout << x->first << " " << x->second << endl;

	string most_played_blind_type = blinds_map_copy[blinds_map_copy.size() - 1].first;
	//cout << most_played_blind_type << endl;
	int most_played_blind_type_number = blinds_map_copy[blinds_map_copy.size() - 1].second;
	//cout << most_played_blind_type_number << endl;

	// PRINTING STATISTICS TO CONSOLE:
	cout << "STATISTICS OVERVIEW: " << endl << endl;
	cout << "Overall profit/loss: $" << fixed << setprecision(2) << overall_profit << endl;
	cout << "Total poker time logged: " << setw(2) << setfill('0') << total_time_played_hours << ':'
		<< setw(2) << setfill('0') << total_time_played_mins << " (hr:min)" << endl;
	cout << "Hourly profit/loss in total: $" << fixed << setprecision(2) << pl_per_hour << endl; // calculates average $ the player makes per hour in total time that he/she has logged into the program
	cout << "Average profit/loss per session: $" << fixed << setprecision(2) << avg_profit_per_session << endl; // calculates average $ the player makes per session
	cout << "Most played location: " << most_played_location << " (Played " << most_played_location_number << " times)" << endl;
	cout << "Most successful location: " << success_location << " ($" << success_location_number << ")" << endl;
	cout << "Most played game-type: " << most_played_game_type << " (Played " << most_played_game_type_number << " times)" << endl;
	cout << "Most played blinds: " << most_played_blind_type << " (Played " << most_played_blind_type_number << " times)" << endl;
	cout << "Total Sessions: " << total_sessions << endl;
	cout << endl;
	cout << "Biggest session win: " << endl;
	for (int i = -4; i <= 3; i++)
	{
		cout << sessions_stats[max_profit_index + i] << endl;
	}

	cin.ignore();
	cin.get();
	main();
}

void exit_program()
{
	system("CLS");
	cout << "Thank you for using the Poker Manager program. " << endl;

	cout << endl;
	cout << "Created by: Kelvin Nguyen" << endl << "PIC 10C Project" << endl << endl;

	//system("PAUSE");
	cin.get();
}

/*Extra Code:

To the grader:

I am going off track on my weekly goals that I initially wrote on my proposal.
For this week, I will not be working on the Session class, but instead I chose to
start working on my main and writing some functions that will be useful for entering
in the time and date for each session.

New Update:
I added many more functions, made a switch for different cases, created a menu for my program,
started to create my own database.

cout << "Please enter the time the session started: " << endl;
cout << "First enter the hour: (format: hh; use military time - e.g. 23 = 11pm) ";
cin >> time_started_hour;
cout << "Now enter the mins: (format: mm) ";
cin >> time_started_mins;
cout << endl;

cout << "Please enter the time the session ended: " << endl; cout << "First enter the hour: (format: hh; use military time - e.g. 23 = 11pm) ";
cin >> time_ended_hour;
cout << "Now enter the mins: (format: mm) ";
cin >> time_ended_mins;
cout << endl;

// Variables:
int month;
int day;
int year;
int time_started_hour;
int time_started_mins;
int time_ended_hour;
int time_ended_mins;

double buy_in_amount;
double cash_out_amount;
double profit;

string print_textFile_answer;
string textFileName;

// Date:
do {
cout << "Please enter the date. " << endl
<< "First enter the month: (format: mm; eg. 01 = January) ";
cin >> month;
cout << "Now enter the day: (format: dd; eg. 01 = first day of the month) ";
cin >> day;
cout << "Now enter the year: (format: yyyy; eg. 2014) ";
cin >> year;
cout << endl;
} while (month > 12 || month < 1 || day > 31 || day < 1);

// Time started:
do {
cout << "Please enter the time the session started: " << endl;
cout << "First enter the hour: (format: hh; use military time - e.g. 23 = 11pm) ";
cin >> time_started_hour;
cout << "Now enter the mins: (format: mm) ";
cin >> time_started_mins;
cout << endl;
} while (time_started_hour > 23 || time_started_hour < 0 || time_started_mins > 59 || time_started_mins < 0);

// Time ended:
cout << "Please enter the time the session ended: " << endl; cout << "First enter the hour: (format: hh; use military time - e.g. 23 = 11pm) ";
cin >> time_ended_hour;
cout << "Now enter the mins: (format: mm) ";
cin >> time_ended_mins;
cout << endl;

// Outputting Data into Console:
cout << "Session: " << month << "/" << day << "/" << year << endl;
cout << "Time started: " << setw(2) << setfill('0') << time_started_hour << ':'
<< setw(2) << setfill('0') << time_started_mins << endl;
cout << "Time ended: " << time_ended_hour << ":" << time_ended_mins << endl;

// Initial Buy-in:
cout << "Initial buy-in: (eg. 400) ";
cin >> buy_in_amount;

// Cashed out:
cout << "Cashed out: (eg. 800) ";
cin >> cash_out_amount;

// Outputting Total Net Profit into Console:
profit = cash_out_amount - buy_in_amount;
cout << "Net Profit: ";
cout << profit << endl;

// Printing to Text FIle:
cout << "Print to text file? (y/n) ";
cin >> print_textFile_answer;
if (print_textFile_answer == "y")
{
cout << "Enter text file name: (eg. example.txt) ";
cin >> textFileName;
ofstream myfile;
myfile.open(textFileName);

myfile << "Session: " << month << "/" << day << "/" << year << endl;
myfile << "Time started: " << time_started_hour << ":" << time_started_mins << endl;
myfile << "Time ended: " << time_ended_hour << ":" << time_ended_mins << endl;

myfile << "Profit: ";
myfile << profit << endl;

myfile.close();
}

system("PAUSE");
return 0;
}

void getTimeStarted(int &h, int &m)
{
string in;
char c;
stringstream ss;

do
{
cout << "Enter time started (HH:MM) : ";
getline(cin, in);
ss.clear();
ss.str(in);
} while (!(ss >> h >> c >> m) || ss.good() || (c != delim) || (isValidTime(h, m) == false));
}

bool isValidTime(int h, int m)
{
return (h >= 0) && (h < hoursPerDay) && (m >= 0) && (m < minutesPerHour);
}

/*
// DElete later
cout << endl << endl << "Printing out new session database... " << endl;
int k1 = 0;
for (int i = 0; i < sessions.size(); i++)
{
if (!(i% session_line_entries)) // for how many lines that each Session takes
{
cout << "(" << k1<< ")";
k1++;
}
cout << sessions[i] << endl; // change to: cout << sessions[i] << endl; later
}

// Delete later
*/