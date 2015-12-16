// Allen White
// CSE 4252
// Professor Madrid


#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <map>
#include <list>
#include <queue>
#include <ctime>
#include <iterator>
#include <set>
#include <algorithm>

using namespace std;





/*
----------------------------- CONSTANTS -----------------------------
*/
const int POINTS_PER_ANSWER = 23;
const int TIME_PER_ROUND 	= 120;
const int NUMBER_OF_ROUNDS 	= 3;
const string COUNTRY_FILENAME = "countriestocities.txt";
const string USERS_FILENAME = "users.txt";
const string SCORES_FILENAME = "scores.txt";
const string TXT_FILE_OBJ_HEADER = "@@@";





/*
----------------------------- NECCESARY FUNCTION DEFINITIONS -----------------------------
*/
void showMainMenu();
map< string, list<string> > mapCountries();
void waitForEnter();
string alphaNumOnly(string strToConvert);





/*
----------------------------- CLASS DECLARATIONS -----------------------------
*/

///////////////// USER
class User
{
	private:
		
	public:
		string username;
		int age;
		char gender;
		char sendByEmail;
		string email;
		User();
		User(string u, string e, int a, char g, char sbe);
		bool wantsScoreByEmail();
};


User::User(){
	username = "~";
	email = "~";
	age = 0;
	gender = '~';
	sendByEmail = '~';
}


User::User(string u, string e, int a, char g, char sbe){
	username = u;
	email = e;
	age = a;
	gender = g;
	sendByEmail = sbe;
}


bool User::wantsScoreByEmail(){
	if (sendByEmail == 'y')
		return true;
	return false;
}





//////////////// USER DB

class UserDB
{
	private:
		vector<User> userObjects;
	public:
		UserDB();
		void addUserToDB(User u);
		void removeUserFromDB(User u);
		vector<User> getAllUsers();
		void operator+(User u);
		void operator-(User u);
};



UserDB::UserDB(){
	ifstream c(USERS_FILENAME.c_str());
	if (!c)
	{
		cout << "\nError opening input file\n" << endl;
		exit(-1);
	}
	// read the file's content into a string
	

	User container;

	while(!c.eof()){
		string t;

		getline(c,t);
		istringstream iss(t);
		string word;
		while(iss >> word) {
			word = word.c_str();
			/* check for TXT_FILE_OBJ_HEADER */
			if ( word.find(TXT_FILE_OBJ_HEADER) != string::npos){
				string age, gender, sendByEmail;
				container.username = alphaNumOnly(word.c_str());
				getline(c, container.email);
				getline(c, age);
				getline(c, gender);
				getline(c, sendByEmail);
				
				container.age = stoi(age);
				container.gender = gender.at(0);
				container.sendByEmail = sendByEmail.at(0);
				userObjects.push_back(container);
			}
		}
	}    
}



void UserDB::addUserToDB(User u){
	userObjects.push_back(u);
	// save to file
	ofstream out;

	// std::ios::app is the open mode "append" meaning
	// new data will be written to the end of the file.
	out.open(USERS_FILENAME.c_str(), ios::app);

	out << u.username << TXT_FILE_OBJ_HEADER << endl 
		<<u.email << endl 
		<<u.age << endl 
		<<u.gender << endl 
		<<u.sendByEmail << endl;
}



void UserDB::removeUserFromDB(User u){
	int pos = 0;
	vector<User>::iterator it;
	for(it = userObjects.begin(); it != userObjects.end(); ++it) {
    	
    	if ((*it).username.compare(u.username) == 0)
		{
			break;
		}
	}

	userObjects.erase(it);

	remove(USERS_FILENAME.c_str());

	ofstream out;

	out.open(USERS_FILENAME.c_str(), ios::app);

	for (int i = 0; i < userObjects.size(); ++i)
	{
		User w = userObjects[i];
		out << w.username << TXT_FILE_OBJ_HEADER << endl 
			<< w.email << endl 
			<< w.age << endl 
			<< w.gender << endl 
			<< w.sendByEmail << endl;
	}
}



vector<User> UserDB::getAllUsers(){
	return userObjects;
}


void UserDB::operator+(User u){
	addUserToDB(u);
}

void UserDB::operator-(User u){
	removeUserFromDB(u);
}


/////////////// SCORES

class Scores
{
	private:
		map< string, vector<int> > scoresByUser;
		vector<int> scores;
		vector<string> players;
	public:
		Scores();
		void addNewScore(int score, string uname);
		vector<int> topScores();
		vector<string> topScorers();
		vector<int> recentScores(string username);
};


Scores::Scores(){
	ifstream c(SCORES_FILENAME.c_str());
	if (!c)
	{
		cout << "\nError opening input file\n" << endl;
		exit(-1);
	}
	// read the file's content into a string
	
	while(!c.eof()){
		string t;
		getline(c,t);
		istringstream iss(t);
		string word;
		while(iss >> word) {
			word = word.c_str();
			/* check for TXT_FILE_OBJ_HEADER */
			if ( word.find(TXT_FILE_OBJ_HEADER) != string::npos){ //is score
				string uname;
				int score = stoi(alphaNumOnly(word.c_str()));
				getline(c, uname); 
				
				//save to map
				map<string, vector<int> >::iterator it = scoresByUser.find(uname);
				if(it != scoresByUser.end())
				{
				   //element found;
				   (it->second).push_back(score);
				}else{
					vector<int> playerScores;
					playerScores.push_back(score);
					scoresByUser[uname] = playerScores;
				}

				int i = 0;
				for (i = 0; i < scores.size(); ++i)
				{
					if (scores[i] < score)
					{
						break;
					}
				}
				
				scores.insert(scores.begin()+i, score);
				players.insert(players.begin()+i, uname);
			}
		}
	}
}


void Scores::addNewScore(int score, string uname){
	//save to map
	map<string, vector<int> >::iterator it = scoresByUser.find(uname);
	if(it != scoresByUser.end())
	{
	   //element found;
	   (it->second).push_back(score);
	}else{
		vector<int> playerScores;
		playerScores.push_back(score);
		scoresByUser[uname] = playerScores;
	}

	int i = 0;
	for (i = 0; i < scores.size(); ++i)
	{
		if (scores[i] < score)
		{
			break;
		}
	}
	
	scores.insert(scores.begin()+i, score);
	players.insert(players.begin()+i, uname);

	// save to file
	ofstream out;

	// std::ios::app is the open mode "append" meaning
	// new data will be written to the end of the file.
	out.open(SCORES_FILENAME.c_str(), ios::app);

	out << score << TXT_FILE_OBJ_HEADER << endl 
		<< uname << endl;
}


vector<int> Scores::recentScores(string username){
	return scoresByUser[username];
}


vector<int> Scores::topScores(){
	return scores;
}



vector<string> Scores::topScorers(){
	return players;
}


UserDB *dbOfUsers;
Scores gameScores;
User currentUser;


//////////////// GAME

class Game
{
	private:
		// int startEpoch;
		time_t  startEpoch;
		int currentScore;
		map< string, list<string> >countriesAndCities;
	public:
		Game();
		void start();
};



Game::Game(){ currentScore = 0; 
}



void Game::start(){
	for (int i = 0; i < NUMBER_OF_ROUNDS; ++i)
	{
		// get the data for a country
		countriesAndCities = mapCountries();
		srand (time(NULL));
		map< string, list<string> >::iterator it = countriesAndCities.begin();
		int numOfAdvances = rand()%(countriesAndCities.size());
		advance( it, numOfAdvances );
		string country = it->first;
		list<string> cities = countriesAndCities[country];
		vector<string> currentCities;
		vector<string> guessedCities;

		while(currentCities.size() < 5){
			list<string>::iterator cit = cities.begin();
			advance(cit, rand()%cities.size());
			string city = *cit;
			if (find(currentCities.begin(), currentCities.end(), city.c_str()) == currentCities.end())
			{
				currentCities.push_back(city.c_str());
			}
		}

		int correctSoFar = 0;

		cout << "New Round!" << endl;
		cout << "ready?" << endl;
		waitForEnter();
		startEpoch = time(nullptr);
		while(startEpoch + TIME_PER_ROUND > time(nullptr)){
			//show question
			cout << "\n\n------------------------------------------------------------------------\n"
				"geoBrain\n\n"
				"Select a city from " << country << "\t\t\t" << "Time Remaining:" << startEpoch + TIME_PER_ROUND - time(nullptr) <<
				"\n------------------------------------------------------------------------\n";

			// print out the cities
			random_shuffle(currentCities.begin(), currentCities.end());

			for (int w = 0; w < currentCities.size(); w++)
			{
			    cout << w + 1 << ". " << currentCities[w].c_str() << endl;
			    
			}

			cout << "Enter city number: \t";
			int cityNumber;
			cin >> cityNumber;
			// read in answer

			// if right, increment currentScore, increment currentCities, increment correctSoFar
			if ((startEpoch + TIME_PER_ROUND > time(nullptr)))
			{
				if (find(guessedCities.begin(), guessedCities.end(), currentCities[cityNumber - 1].c_str()) == guessedCities.end() )
				{
					printf("\ngood answer! \n");
					guessedCities.push_back(currentCities[cityNumber - 1].c_str());
					correctSoFar ++;

					// make sure this adds one if at all possible
					list<string>::iterator cit = cities.begin();
					advance(cit, rand()%cities.size());
					string city = *cit;

					if (find(currentCities.begin(), currentCities.end(), city.c_str()) == currentCities.end())
					{
						currentCities.push_back(city.c_str());
					}

				}else{
					printf("\nincorrect! \n");
				}

			}else{
				printf("\noutta time!\n");
			}

		}
		currentScore += correctSoFar * POINTS_PER_ANSWER;
		cout << "Current score: " << currentScore << endl;
	}
	cout << endl << endl << "GAME OVER" << endl << endl;
	
	gameScores.addNewScore(currentScore, currentUser.username);
	showMainMenu();
}










/*
----------------------------- UTILITY METHODS -----------------------------
*/
void waitForEnter(){
	cin.ignore(10, '\n');
	cin.get();
}


string alphaNumOnly(string strToConvert)
{
    string strConverted; //strToConvert with only alpha-numeric characters.

    for (unsigned int i=0; i<strToConvert.length();i++)
    {
        if (isalnum(strToConvert[i])) //If current character is alpha-numeric
        strConverted += strToConvert[i]; //Add the character to strConverted
    }

    return strConverted;
}


char validateCharBool(char one, char two, string inputPrompt){
	char choice = '$';
	string line;
	while (choice != one && choice != two){
		cin >> line;
		if(line.length() > 1 || (line[0] != one && line[0] != two) ){
			cout << "Invalid input." << endl;
			cout << inputPrompt;
			cin.clear();
		}else{
			choice = line[0];
		}
	}
	return choice;
}


map< string, list<string> > mapCountries(){
	map< string,list<string> > countriesMapped;

	ifstream c(COUNTRY_FILENAME.c_str());
	if (!c)
	{
		cout << "\nError opening input file\n" << endl;
		exit(-1);
	}
	// read the file's content into a string
	

	string country;
	list<string> cities;

	while(!c.eof()){
		string t;

		getline(c,t);
		istringstream iss(t);
		string word;
		while(iss >> word) {
			word = word.c_str();
			/* check for TXT_FILE_OBJ_HEADER */
			if ( word.find(TXT_FILE_OBJ_HEADER) != string::npos){
				// add to map, flush list and reset string
				if (country.length() > 0)
				{
					countriesMapped[country] = cities;
				}
				country = alphaNumOnly(word);
				cities.clear();

			}else{
				cities.push_back(alphaNumOnly(word));
			}
		}
	}    

	return countriesMapped;
}


string visualScore(int score){
	int numBlox = score / 50;
	string visual = "";
	for (int i = 0; i < numBlox; ++i)
	{
		visual = visual + "X";
	}
	return visual;
}

/*
----------------------------- PROGRAM FLOW -----------------------------
*/
void showChooseUser(){
	cout << "\n\n------------------------------------------------------------------------\n"
			"geoBrain\n\n"
			"Select a user below.\n\n";

	//get all da users
	//spit them all out


	vector<User> users = (*dbOfUsers).getAllUsers();
	int i = 0;
	for (i = 0; i < users.size(); ++i)
	{
		cout << i + 1 << ".\t" << users[i].username << endl;
	}


	cout << i + 1 <<".\tCancel\n\n" << "Enter user number: \t";
	int userChoice;
	cin >> userChoice;


	if (userChoice > users.size()) // cancel
	{
		cout << "\nCanceled.\n";
		showMainMenu();
	}else{
		cout << "\nWelcome back, " << users[userChoice - 1].username << ". Press enter to start the game." << endl;
		waitForEnter();
		Game g;
		currentUser = users[userChoice - 1];
		g.start();
	}
	
}


void showCreateNewUser(){
	cout << "\n\n------------------------------------------------------------------------\n"
			"geoBrain – New user\n\n"
			"Enter user name:          	\t";
	string uname;
	cin >> uname;
	//validate string somehow
	uname = alphaNumOnly(uname);

	cout << "Enter age:                	\t";
	int age;
	string line;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (getline(cin, line))
    {
        stringstream ss(line);
        if (ss >> age)
        {
            if (ss.eof())
            {   // Success
                break;
            }
        }
        cout << "Not an integer. Try again" << endl;
        cout << "Enter age:                	\t";
    }
	//validate age
	
	string inputPrompt = "Enter gender m/f:         	\t";
	cout << inputPrompt;
	char gender = validateCharBool('m', 'f', inputPrompt);
	

	string email;
	while((email.find(".") == string::npos) || (email.find("@") == string::npos)){
		cout << "Enter email:              	\t";
		cin >> email;
		if ((email.find(".") == string::npos) || (email.find("@") == string::npos))
			cout << "Invalid input." << endl;
	}
	
	//validate email
	inputPrompt = "Send results by mail? y/n:	\t";
	cout << inputPrompt;
	char sendByEmail = validateCharBool('y', 'n', inputPrompt);
	//validate the char

	//create new user

	User *newUser = new User(uname, email, age, gender, sendByEmail);
	*dbOfUsers + *newUser;


	Game g;
	currentUser = *newUser;
	cout << endl << endl << "Weclome, " << uname << "! Press enter to start your game." << endl;
	waitForEnter();
	g.start();
}


void showRemoveUser(){
	cout << "\n\n------------------------------------------------------------------------\n"
			"geoBrain\n\n"
			"Select a user to remove below.\n\n";

	//get all da users
	//spit them all out
	vector<User> users = (*dbOfUsers).getAllUsers();
	int i = 0;
	for (i = 0; i < users.size(); ++i)
	{
		cout << i + 1 << ".\t" << users[i].username << endl;
	}


	cout << i + 1 <<".\tCancel\n\n" << "Enter user number: \t";
	int userChoice;
	cin >> userChoice;
	if (userChoice > users.size()) // cancel
	{
		cout << "\nCanceled.\n";
		
	}else{
		cout << "\nAre you sure you want to erase " << users[userChoice - 1].username << "? Press enter to confirm." << endl;
		waitForEnter();
		*dbOfUsers - (users[userChoice - 1]);
	}
	showMainMenu();
}


void showScores(){
	cout << "\n\n------------------------------------------------------------------------\n"
			"geoBrain\n\n";

	//get TOP TEN OF da user's scores
	vector<int> usersRecentScores = gameScores.recentScores(currentUser.username);
	if (usersRecentScores.size() < 10)
	{
		cout << "Last " << usersRecentScores.size() << " scores for " << currentUser.username << "\n\n";
	}else{
		cout << "Last " << 10 << " scores for " << currentUser.username << "\n\n";
	}

	for (int i = 0; i < usersRecentScores.size() && i < 10; ++i)
	{
		cout << usersRecentScores[i] << " " << visualScore(usersRecentScores[i]) << endl;
	}

	//spit them all out, with graphical display and number

	cout << "\nUser ranking -- Highest Scores" << endl << endl;
	//get 5 highest scores and spit em out
	vector<string> users = gameScores.topScorers();
	vector<int> scores = gameScores.topScores();
	
	for (int i = 0; i < scores.size() && i < 5; ++i)
	{
		cout << i + 1 << ".\t" << scores[i] << "\t\t" << users[i] << endl;
	}
	cout << endl << endl;
	showMainMenu();
}


void showHelpDescription(){
	cout << "\n\n------------------------------------------------------------------------\n"
			"geoBrain\n\n"
			"When the game starts, it will show the user an initial list of three (3) cities from a country randomly selected. The user then should pick one of them.\n"
			"In the next pass, the game will show four cities, the same three from the previous phase plus a new one, all of them in a random order. The user should pick a second city different from the one picked previously, to move to the next pass.\n"
			"In the third pass, the game will show the user five cities, the same four from the previous phase plus a new one, in a random order, again the user should pick a third city different from the ones picked previously to move to the next pass.\n"
			"The program keeps going through new passes adding one city at a time, as far as the user does not pick one city already selected.\n"
			"When the user selects a city already picked in a previous pass, the first round ends, a score is calculated, shown in the screen and saved in the users statistics. There will be three rounds for a given country, so the same process starts all over again.\n"
			"There is also a time limit of 120 seconds (2 minutes) for each round, so if the player keeps picking new cities correctly when it reaches two minutes, the round must end, show the score and proceed with the next round until complete three rounds.\n\n";
	waitForEnter();
	showMainMenu();
}



void showMainMenu(){
	cout << "\n\n------------------------------------------------------------------------\n"
			"geoBrain – Main Menu\n\n"
			"Select an option below\n\n"
			"1. Choose Username\t2. New user       \t3. Remove user    \t4. Scores         \t5. Exit           \t6. Help\n\n" << "Enter number of choice: \t";
	int choice;
	cin >> choice;
	// Ignore to the end of line
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	switch(choice){
		case 1:
			showChooseUser();
			break;
		case 2:
			showCreateNewUser();
			break;
		case 3:
			showRemoveUser();
			break;
		case 4:
			showScores();
			break;
		case 5:
			exit(0);
			break;
		case 6:
			showHelpDescription();
			break;
		default:
			showMainMenu();
			break;
	}

}




/*
----------------------------- MAIN METHOD -----------------------------
*/
int main(int argc, char* pArgv[])
{
	dbOfUsers = new UserDB();
	gameScores = *(new Scores());
	showMainMenu();

	exit(0);
	return 0;
}
