#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

const string filename = "Medal.csv";
const string userFile = "users.txt";
const string adminUsername = "admin";
const string adminPassword = "admin123";

struct Record
{
    string country;
    string gold;
    string silver;
    string bronze;
    string medalCount;
    Record* next;
};

// Function to display the banner
void displayBanner()
{
    cout << "===========================================\n";
    cout << "      Olympics Medal Management System      \n";
    cout << "===========================================\n";
}

// Function prototypes
Record* loadRecords(const string& filename);
void saveRecords(const string& filename, Record* head);
void addRecord(Record*& head);
void viewAllRecords(Record* head);
void deleteRecord(Record*& head);
void compareCountries(Record* head);
void searchRecords(Record* head);
void editRecord(Record* head);
void searchTopCountry(Record* head);
void deleteList(Record*& head);
void saveAndExit(Record* head);
bool adminLogin();
bool userLogin();
void registerUser();
void displayBanner();
bool isAdmin = false;

// Function to load all records from CSV into a linked list
Record* loadRecords(const string& filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error: Unable to open the file '" << filename << "'!\n";
        return nullptr;
    }

    Record* head = nullptr;
    Record* tail = nullptr;

    string header;
    getline(file, header);  // Skip the header row

    string country, gold, silver, bronze, medalCount;
    while (getline(file, country, ',') &&
            getline(file, gold, ',') &&
            getline(file, silver, ',') &&
            getline(file, bronze, ',') &&
            getline(file, medalCount, '\n'))
    {

        Record* newRecord = new Record{country, gold, silver, bronze, medalCount, nullptr};

        if (!head)
        {
            head = newRecord;
            tail = newRecord;
        }
        else
        {
            tail->next = newRecord;
            tail = newRecord;
        }
    }

    file.close();
    return head;
}

// Function to save all records from the linked list back to a CSV
void saveRecords(const string& filename, Record* head)
{
    ofstream file(filename, ios::trunc);

    if (!file.is_open())
    {
        cout << "Error: Unable to open the file '" << filename << "'!\n";
        return;
    }

    file << "Country,Gold,Silver,Bronze,Medal Count\n";

    // Save records in alphabetical order of the country name
    Record* current = head;
    while (current)
    {
        file << current->country << ","
             << current->gold << ","
             << current->silver << ","
             << current->bronze << ","
             << current->medalCount << "\n";

        current = current->next;
    }


}


// Function to add a new record
void addRecord(Record*& head)
{
    Record* newRecord = new Record;

    cout << "Enter country name: ";
    cin.ignore();
    getline(cin, newRecord->country);

    cout << "Enter number of gold medals: ";
    cin >> newRecord->gold;

    cout << "Enter number of silver medals: ";
    cin >> newRecord->silver;

    cout << "Enter number of bronze medals: ";
    cin >> newRecord->bronze;

    cout << "Enter total medal count: ";
    cin >> newRecord->medalCount;

    newRecord->next = nullptr;

    if (!head)
    {
        head = newRecord;
    }
    else
    {
        Record* temp = head;
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = newRecord;
    }

    cout << "Record added successfully.\n";
}

// Function to view all records
void viewAllRecords(Record* head)
{
    cout << setw(15) << "Country"
         << setw(10) << "Gold"
         << setw(10) << "Silver"
         << setw(10) << "Bronze"
         << setw(15) << "Medal Count" << "\n";

    Record* current = head;
    while (current)
    {
        cout << setw(15) << current->country
             << setw(10) << current->gold
             << setw(10) << current->silver
             << setw(10) << current->bronze
             << setw(15) << current->medalCount << "\n";

        current = current->next;
    }
}

// Function to delete a record by country name
void deleteRecord(Record*& head)
{
    int choice;
    string value;
    cout << "Delete by:\n"
         << "1. Country\n"
         << "2. Medal Count\n"
         << "Enter your choice: ";
    cin >> choice;
    cin.ignore();  // Clear the buffer

    if (choice == 1)
    {
        cout << "Enter the country name to delete: ";
    }
    else if (choice == 2)
    {
        cout << "Enter the medal count to delete: ";
    }
    else
    {
        cout << "Invalid choice! Returning to menu.\n";
        return;
    }
    getline(cin, value);

    Record* current = head;
    Record* prev = nullptr;
    bool recordDeleted = false;

    // Traverse the linked list to find and delete the matching record
    while (current)
    {
        bool match = (choice == 1 && current->country == value) ||
                     (choice == 2 && current->medalCount == value);

        if (match)
        {
            if (prev)
            {
                prev->next = current->next;
            }
            else
            {
                head = current->next;
            }
            delete current;
            recordDeleted = true;
            cout << "Record for " << (choice == 1 ? value : "medal count " + value) << " deleted successfully.\n";
            current = (choice == 1) ? prev : nullptr;  // Re-assign current after deletion
        }
        else
        {
            prev = current;
        }
        current = current->next;
    }

    if (!recordDeleted)
    {
        cout << "No matching record found to delete.\n";
    }

    // Save the updated linked list back to the file
    saveRecords(filename, head);
}

// Function to compare two countries
void compareCountries(Record* head)
{
    cout << "Enter the first country: ";
    string country1;
    cin.ignore();
    getline(cin, country1);

    cout << "Enter the second country: ";
    string country2;
    getline(cin, country2);

    Record *first = nullptr, *second = nullptr;

    Record* current = head;
    while (current)
    {
        if (current->country == country1)
        {
            first = current;
        }
        if (current->country == country2)
        {
            second = current;
        }
        current = current->next;
    }

    if (first && second)
    {
        cout << "\nComparison Between Countries\n";
        cout << string(50, '-') << "\n";
        cout << setw(15) << "Category"
             << setw(15) << country1
             << setw(15) << country2 << "\n";
        cout << string(50, '-') << "\n";
        cout << setw(15) << "Gold"
             << setw(15) << first->gold
             << setw(15) << second->gold << "\n";
        cout << setw(15) << "Silver"
             << setw(15) << first->silver
             << setw(15) << second->silver << "\n";
        cout << setw(15) << "Bronze"
             << setw(15) << first->bronze
             << setw(15) << second->bronze << "\n";
        cout << setw(15) << "Total"
             << setw(15) << first->medalCount
             << setw(15) << second->medalCount << "\n";
    }
    else
    {
        cout << "One or both countries were not found.\n";
    }
}
string trim(const string& str)
{
    const char* whitespace = " \t\n\r";
    size_t start = str.find_first_not_of(whitespace);
    if (start == string::npos) return "";

    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

void searchRecords(Record* head)
{
    cout << "Enter the field to search (Country, Gold, Silver, Bronze): ";
    string field;
    cin >> field;

    string searchValue;

    if (field == "Country")
    {
        cout << "Enter the country name to search: ";
        cin.ignore();
        getline(cin, searchValue);
        searchValue = trim(searchValue);  // Remove leading spaces
    }
    else if (field == "Gold" || field == "Silver" || field == "Bronze")
    {
        cout << "Searching for countries with at least one " << field << " medal.\n";
    }
    else
    {
        cout << "Invalid field! Returning to menu.\n";
        return;
    }

    cout << setw(15) << "Country"
         << setw(10) << "Gold"
         << setw(10) << "Silver"
         << setw(10) << "Bronze"
         << setw(15) << "Medal Count\n";

    bool found = false;
    Record* current = head;

    // Traverse through the linked list
    while (current)
    {
        try
        {
            int goldCount = stoi(current->gold);
            int silverCount = stoi(current->silver);
            int bronzeCount = stoi(current->bronze);

            if ((field == "Country" && current->country == searchValue) ||
                    (field == "Gold" && goldCount > 0) ||
                    (field == "Silver" && silverCount > 0) ||
                    (field == "Bronze" && bronzeCount > 0))
            {

                cout << setw(15) << current->country
                     << setw(10) << current->gold
                     << setw(10) << current->silver
                     << setw(10) << current->bronze
                     << setw(15) << current->medalCount << "\n";

                found = true;
            }
        }
        catch (const invalid_argument&)
        {
        }

        current = current->next;
    }

    if (!found)
    {
        cout << "No matching records were found in the linked list.\n";
    }
}



void editRecord(Record* head)
{
    cout << "Enter the country to edit: ";
    string country;
    cin.ignore();
    getline(cin, country);

    Record* current = head;

    while (current)
    {
        if (current->country == country)
        {
            int choice;
            cout << "\nWhich field would you like to edit?\n"
                 << "1. Gold Medals\n"
                 << "2. Silver Medals\n"
                 << "3. Bronze Medals\n"
                 << "4. Total Medal Count\n"
                 << "Enter your choice: ";

            cin >> choice;

            switch (choice)
            {
            case 1:
                cout << "Enter new number of gold medals: ";
                cin >> current->gold;
                break;

            case 2:
                cout << "Enter new number of silver medals: ";
                cin >> current->silver;
                break;

            case 3:
                cout << "Enter new number of bronze medals: ";
                cin >> current->bronze;
                break;

            case 4:
                cout << "Enter new total medal count: ";
                cin >> current->medalCount;
                break;

            default:
                cout << "Invalid field choice!\n";
                return;
            }

            // Automatically recalculate total medals
            int goldCount = stoi(current->gold);
            int silverCount = stoi(current->silver);
            int bronzeCount = stoi(current->bronze);
            int totalMedals = goldCount + silverCount + bronzeCount;
            current->medalCount = to_string(totalMedals);

            saveRecords(filename, head);
            cout << "Record updated successfully!\n";
            return;
        }
        current = current->next;
    }

    cout << "Country not found.\n";
}

// Function to search and find the country with the highest number of medals
void searchTopCountry(Record* head)
{
    Record* current = head;
    Record* topCountry = nullptr;
    int highestMedalCount = -1;

    while (current)
    {
        int medalCount = stoi(current->medalCount);

        if (medalCount > highestMedalCount)
        {
            highestMedalCount = medalCount;
            topCountry = current;
        }

        current = current->next;
    }

    if (topCountry)
    {
        cout << "\nThe country with the highest number of medals is: " << topCountry->country << "\n";
        cout << "Total Medals: " << topCountry->medalCount << "\n";
        cout << "Gold: " << topCountry->gold << ", Silver: " << topCountry->silver << ", Bronze: " << topCountry->bronze << "\n";
    }
    else
    {
        cout << "No records found.\n";
    }
}


// Clean up dynamically allocated memory
void deleteList(Record*& head)
{
    while (head)
    {
        Record* temp = head;
        head = head->next;
        delete temp;
    }
}

// Save and clean everything up before exiting
void saveAndExit(Record* head)
{
    saveRecords(filename, head);
    deleteList(head);
    cout << "Exiting the system. Goodbye!\n";
}

// Admin login system
bool adminLogin()
{
    string username, password;
    char ch;

    cout << "\n--- Admin Login ---\n";
    cout << "Enter username: ";
    cin >> username;

    cout << "Enter password: ";
    password = "";

    while ((ch = getch()) != '\r') // '\r' is the Enter key
    {
        if (ch == '\b') // Handle backspace
        {
            if (!password.empty())
            {
                cout << "\b \b"; // Move back, overwrite with space, and move back again
                password.pop_back();
            }
        }
        else
        {
            cout << '*';
            password.push_back(ch);
        }
    }
    cout << endl;

    if (username == adminUsername && password == adminPassword)
    {
        isAdmin = true;
        return true;
    }

    cout << "Invalid admin credentials.\n";
    return false;
}

// User login system
bool userLogin()
{
    string username, password;
    char ch;

    cout << "\n--- User Login ---\n";
    cout << "Enter username: ";
    cin >> username;

    cout << "Enter password: ";
    password = "";

    while ((ch = getch()) != '\r') // '\r' is the Enter key
    {
        if (ch == '\b') // Handle backspace
        {
            if (!password.empty())
            {
                cout << "\b \b"; // Move back, overwrite with space, and move back again
                password.pop_back();
            }
        }
        else
        {
            cout << '*';
            password.push_back(ch);
        }
    }
    cout << endl;

    ifstream file(userFile);
    if (!file.is_open())
    {
        cout << "Error: Unable to open users file!\n";
        return false;
    }

    string fileUsername, filePassword;
    while (file >> fileUsername >> filePassword)
    {
        if (username == fileUsername && password == filePassword)
        {
            isAdmin = false;
            return true;
        }
    }

    cout << "Invalid username or password.\n";
    return false;
}

// Register a new user
void registerUser()
{
    string username, password;
    cout << "\n--- Register ---\n";
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter new password: ";
    cin >> password;

    ofstream file(userFile, ios::app);
    if (!file.is_open())
    {
        cout << "Error: Unable to open users file!\n";
        return;
    }

    file << username << " " << password << "\n";
    file.close();
    cout << "Registration successful.\n";
}

// Structures
class Athlete
{
public:
    string name;
    string shortName;
    string gender;
    string disciplines;  // Changed to string to store one discipline
    string events;
    string birthDate;
    string birthPlace;
    string nickname;
    string hobbies;
    string education;
    string family;
    string languages;
    string reason;
    string hero;
    string influence;
    string philosophy;
    Athlete* next;
};

struct Country
{
    string name;
    string countryCode;
    string countryLong;
    string nationality;
    string nationalityLong;
    vector<Athlete*> athletes;   // List of athletes from the country
    Country* next;
};
int calculateAge(const string& birthDate)
{
    // Current date
    time_t t = time(0);
    tm* now = localtime(&t);

    // Parse the birthDate in YYYY-MM-DD format
    int year, month, day;
    char dash1, dash2; // To handle the '-' in the date format

    stringstream ss(birthDate);
    ss >> year >> dash1 >> month >> dash2 >> day;

    // Validate the parsed date
    if (ss.fail() || dash1 != '-' || dash2 != '-' || year < 1900 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        //cerr << "Error: Invalid birthDate format - " << birthDate << endl;
        cerr << "N/A" << birthDate << endl;
        return -1; // Return -1 for invalid age
    }

    // Current year, month, and day
    int currentYear = now->tm_year + 1900;
    int currentMonth = now->tm_mon + 1;
    int currentDay = now->tm_mday;

    // Calculate age
    int age = currentYear - year;

    // Adjust if the birth date hasn't occurred yet this year
    if (currentMonth < month || (currentMonth == month && currentDay < day))
    {
        age--;
    }

    return age;
}


// Binary search method


class CountryManager
{
private:
    vector<Country*> countries;

    Country* binarySearchCountry(const string& searchName)
    {
        int left = 0;
        int right = countries.size() - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            int compareResult = searchName.compare(countries[mid]->name);

            if (compareResult == 0)
            {
                return countries[mid];
            }

            if (compareResult < 0)
            {
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }

        return nullptr;
    }

public:
    void addCountry(Country* country)
    {
        // Check if country already exists to avoid duplicates
        for (auto& existingCountry : countries)
        {
            if (existingCountry->name == country->name)
            {
                // If country exists, add athletes to existing country
                existingCountry->athletes.insert(
                    existingCountry->athletes.end(),
                    country->athletes.begin(),
                    country->athletes.end()
                );
                delete country;
                return;
            }
        }
        countries.push_back(country);
    }

    // Add a debug method to print country information


    // Add a debug method to print country information
    void printCountryInfo()
    {
        cout << "Total Countries: " << countries.size() << endl;
        for (auto& country : countries)
        {
            cout << "Country: " << country->name
                 << ", Athletes: " << country->athletes.size() << endl;
        }
    }

public:


    // Sort countries alphabetically for binary search
    void sortCountries()
    {
        sort(countries.begin(), countries.end(),
             [](const Country* a, const Country* b)
        {
            return a->name < b->name;
        });
    }

    void searchCountry()
    {
        string searchCountryName;
        cout << "Enter the country name to search: ";
        getline(cin, searchCountryName);

        Country* current = binarySearchCountry(searchCountryName);

        if (current)
        {
            cout << "\nAthletes from " << current->name << ":\n";
            cout << "-----------------------------------------------------------------------------------------------------\n";
            cout << left << setw(35) << "Name"
                 << setw(6) << "Age"
                 << setw(15) << "Discipline"
                 << setw(30) << "Events"
                 << setw(10) << "Gender\n";
                 //<< "Birth Place\n";
            cout << "-----------------------------------------------------------------------------------------------------\n";

            for (Athlete* athlete : current->athletes)
            {
                int age = calculateAge(athlete->birthDate);

                cout << left << setw(35) << athlete->name
                     << setw(6) << (age >= 0 ? to_string(age) : "N/A")
                     << setw(15) << athlete->disciplines
                     << setw(30) << athlete->events
                     << setw(10) << athlete->gender << "\n";
                     //<< athlete->birthPlace << "\n";
            }
            cout << "-----------------------------------------------------------------------------------------------------\n";
        }
        else
        {
            cout << "Country not found in the dataset.\n";
        }
    }
};



// Function to clean a string field
string cleanField(const string& field)
{
    string result = field;

    // Remove the surrounding square brackets and single quotes
    result.erase(remove(result.begin(), result.end(), '['), result.end());
    result.erase(remove(result.begin(), result.end(), ']'), result.end());
    result.erase(remove(result.begin(), result.end(), '\''), result.end());

    // Trim leading/trailing spaces
    result.erase(result.find_last_not_of(" \t\n\r") + 1); // Trim trailing spaces
    result.erase(0, result.find_first_not_of(" \t\n\r"));  // Trim leading spaces

    return result;
}



// Function to create a new athlete
Athlete* createAthlete(vector<string> fields)
{
    Athlete* newAthlete = new Athlete;
    newAthlete->name = fields[2];
    newAthlete->shortName = fields[3];
    newAthlete->gender = fields[5];

    // Store cleaned discipline as a string
    newAthlete->disciplines = cleanField(fields[15]); // Assuming one discipline per athlete

    // Continue with other fields
    newAthlete->events = cleanField(fields[16]);
    newAthlete->birthDate = fields[17];
    newAthlete->birthPlace = fields[18];
    newAthlete->nickname = fields[21];
    newAthlete->hobbies = fields[22];
    newAthlete->education = fields[24];
    newAthlete->family = fields[25];
    newAthlete->languages = fields[26];
    newAthlete->reason = fields[27];
    newAthlete->hero = fields[28];
    newAthlete->influence = fields[29];
    newAthlete->philosophy = fields[30];
    newAthlete->next = nullptr;
    return newAthlete;
}

// Function to find or create a country node
Country* findOrCreateCountry(Country*& head, const string& countryName, const string& countryCode)
{
    Country* current = head;
    while (current)
    {
        if (current->name == countryName)
        {
            return current;
        }
        current = current->next;
    }
    // Create a new country if not found
    Country* newCountry = new Country;
    newCountry->name = countryName;
    newCountry->countryCode = countryCode;
    newCountry->next = head;
    head = newCountry;
    return newCountry;
}
void searchCountry(Country* head)
{
    string searchCountryName;
    cout << "Enter the country name to search: ";
    getline(cin, searchCountryName);

    Country* current = head;
    while (current)
    {
        if (current->name == searchCountryName)
        {
            cout << "\nAthletes from " << current->name << ":\n";
            cout << "-----------------------------------------------------------------------------------------------------\n";
            cout << left << setw(25) << "Name"
                 << setw(6) << "Age"
                 << setw(20) << "Discipline"
                 << setw(15) << "Events"
                 << setw(10) << "Gender"
                 << "Birth Place\n";
            cout << "-----------------------------------------------------------------------------------------------------\n";

            for (Athlete* athlete : current->athletes)
            {
                int age = calculateAge(athlete->birthDate);

                cout << left << setw(25) << athlete->name
                     << setw(6) << (age >= 0 ? to_string(age) : "N/A")
                     << setw(20) << athlete->disciplines
                     << setw(15) << athlete->events
                     << setw(10) << athlete->gender
                     << athlete->birthPlace << "\n";
            }
            cout << "-----------------------------------------------------------------------------------------------------\n";
            return;
        }
        current = current->next;
    }
    cout << "Country not found in the dataset.\n";
}
void readCSV(const string& filename, CountryManager& countryManager)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file." << endl;
        return;
    }

    string line;
    // Skip the header row
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        vector<string> fields;
        string field;

        // Split the line into fields
        while (getline(ss, field, ','))
        {
            fields.push_back(field);
        }

        if (fields.size() < 31)   // Ensure there are enough fields
        {
            cerr << "Error: Incomplete data in line." << endl;
            continue;
        }

        // Extract country and athlete data
        string countryName = fields[8];
        string countryCode = fields[7];

        // Create country (without using countryHead)
        Country* country = new Country;
        country->name = countryName;
        country->countryCode = countryCode;

        Athlete* athlete = createAthlete(fields);

        // Add athlete to the country's athlete list
        country->athletes.push_back(athlete);

        // Add to CountryManager
        countryManager.addCountry(country);
    }

    file.close();
}
// Main Menu
int main()
{
    CountryManager countryManager;
    readCSV("olympics.csv", countryManager);
    displayBanner();
    Record* head = loadRecords(filename);

    int choice;
    do
    {
        cout << "\n1. Admin Login\n2. User Login\n3. Register\n0. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (adminLogin())
            {
                int adminChoice;
                do
                {
                    cout << "\nAdmin Menu:\n";
                    cout << "1. View All Records\n";
                    cout << "2. Add Record\n";
                    cout << "3. Edit Record\n";
                    cout << "4. Delete Record\n";
                    cout << "5. Search Records\n";
                    cout << "6. Search Top Country\n";
                    cout << "7. Compare Countries\n";
                    cout << "8. Search Country for Athletes\n";
                    cout << "0. Logout\nEnter your choice: ";
                    cin >> adminChoice;

                    switch (adminChoice)
                    {
                    case 1:
                        viewAllRecords(head);
                        break;
                    case 2:
                        addRecord(head);
                        break;
                    case 3:
                        editRecord(head);
                        break;
                    case 4:
                        deleteRecord(head);
                        break;
                    case 5:
                        searchRecords(head);
                        break;
                    case 6:
                        searchTopCountry(head);
                        break;
                    case 7:
                        compareCountries(head);
                        break;
                    case 8:

                        // Debug: Print country information
                        countryManager.printCountryInfo();

                        // Sort countries for binary search
                        countryManager.sortCountries();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        // Search for a country
                        countryManager.searchCountry();
                        break;

                    case 0:
                        cout << "Logged out successfully.\n";
                        break;
                    default:
                        cout << "Invalid choice!\n";
                    }
                }
                while (adminChoice != 0);
            }
            break;
        case 2:
            if (userLogin())
            {
                int userChoice;
                do
                {
                    cout << "\nUser Menu:\n";
                    cout << "1. View All Records\n";
                    cout << "2. Search Records\n";
                    cout << "3. Search Top Country\n";
                    cout << "4. Compare Countries\n";
                    cout << "5. Search Country for Athletes\n";
                    cout << "0. Logout\nEnter your choice: ";
                    cin >> userChoice;

                    switch (userChoice)
                    {
                    case 1:
                        viewAllRecords(head);
                        break;
                    case 2:
                        searchRecords(head);
                        break;
                    case 3:
                        searchTopCountry(head);
                        break;
                    case 4:
                        compareCountries(head);
                        break;
                    case 5:

                        // Debug: Print country information
                        countryManager.printCountryInfo();

                        // Sort countries for binary search
                        countryManager.sortCountries();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        // Search for a country
                        countryManager.searchCountry();
                        break;
                    case 0:
                        cout << "Logged out successfully.\n";
                        break;
                    default:
                        cout << "Invalid choice!\n";
                    }
                }
                while (userChoice != 0);
            }
            break;
        case 3:
            registerUser();
            break;
        case 0:
            saveAndExit(head);
            return 0;
        default:
            cout << "Invalid choice!\n";
        }
    }
    while (choice != 0);

    return 0;
}
