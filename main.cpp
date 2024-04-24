#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm> // For binary_search
#include <sstream> // For parsing CSV lines
#include <limits>
#include <queue> // Include the queue header
#include <map>

using namespace std;

// Offense class to store details of an offense
class Offense {
public:
    // Getter and setter functions for studentUID
    string getStudentUID() const { return studentUID; }
    void setStudentUID(const string& uid) { studentUID = uid; }

    // Getter and setter functions for name
    string getName() const { return name; }
    void setName(const string& newName) { name = newName; }

    // Getter and setter functions for age
    int getAge() const { return age; }
    void setAge(int newAge) { age = newAge; }

    // Getter and setter functions for offenseType
    string getOffenseType() const { return offenseType; }
    void setOffenseType(const string& newType) { offenseType = newType; }

    // Getter and setter functions for offenseAmount
    int getOffenseAmount() const { return offenseAmount; }
    void setOffenseAmount(int newAmount) { offenseAmount = newAmount; }

    // Getter and setter functions for location
    string getLocation() const { return location; }
    void setLocation(const string& newLocation) { location = newLocation; }

    // Getter and setter functions for date
    string getDate() const { return date; }
    void setDate(const string& newDate) { date = newDate; }

    // Overloaded < operator for priority queue sorting
    // Sort in descending order of offenseAmount
    bool operator<(const Offense& other) const {
        return offenseAmount < other.offenseAmount;
    }

private:
    string studentUID; // Unique identifier for a student
    string name; // Name of the student
    int age; // Age of the student
    string offenseType; // Type of offense committed
    int offenseAmount; // Amount or severity of the offense
    string location; // Location where the offense occurred
    string date; // Date when the offense occurred
};    

// Comparator function for binary search (assuming UID is unique)
bool compareUIDs(const Offense& a, const Offense& b) {
    return a.getStudentUID() < b.getStudentUID();
}

// Basic binary search implementation to find an offense record by UID
int binarySearch(const vector<Offense>& records, const string& targetUID) {
    int low = 0;
    int high = records.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (records[mid].getStudentUID() == targetUID) {
            return mid; // Found the target UID
        } else if (records[mid].getStudentUID() < targetUID) {
            low = mid + 1; // Target UID is in the upper half
        } else {
            high = mid - 1; // Target UID is in the lower half
        }
    }

    return -1; // If not found, return -1
}

// Function to add a new offense record
void addData(vector<Offense>& offenseRecords, map<string, vector<Offense> >& offensesByUID, priority_queue<Offense>& amountQueue) {
    // Data Entry
    Offense newOffense;
    string temp;
    cout << "Enter Student UID: ";
    getline(cin, temp);
    newOffense.setStudentUID(temp);
    cout << "Enter Name: ";
    getline(cin, temp);
    newOffense.setName(temp);
    cout << "Enter Age: ";
    getline(cin, temp);
    newOffense.setAge(stoi(temp));
    cout << "Enter Offense Type: ";
    getline(cin, temp);
    newOffense.setOffenseType(temp);
    cout << "Enter Offense Amount: ";
    getline(cin, temp);
    newOffense.setOffenseAmount(stoi(temp));
    cout << "Enter Location: ";
    getline(cin, temp);
    newOffense.setLocation(temp);
    cout << "Enter Date: ";
    getline(cin, temp);
    newOffense.setDate(temp);

    // Insertion
    int insertionIndex = binarySearch(offenseRecords, newOffense.getStudentUID());
    offenseRecords.insert(offenseRecords.begin() + (insertionIndex == -1 ? 0 : insertionIndex), newOffense);

    // Write to CSV file
    ofstream csvFileOut("offense_data.csv");
    for (const Offense& offense : offenseRecords) {
        csvFileOut << offense.getStudentUID() << "," << offense.getName() << "," 
                << offense.getAge() << "," << offense.getOffenseType() << ","
                << offense.getOffenseAmount() << "," << offense.getLocation() << ","
                << offense.getDate() << endl; 
    }
    csvFileOut.close();

    // Add to the map (offensesByUID) for quick lookup by UID
    offensesByUID[newOffense.getStudentUID()].push_back(newOffense); 

    // Add to the priority queue (amountQueue) to keep track of top offenses
    amountQueue.push(newOffense); 

    cout << "New offense record added!" << endl; 
}

// Function to search for offenses by student UID
void searchByUID(const map<string, vector<Offense> >& offensesByUID) { 
    string targetUID;
    cout << "Enter Student UID to search: ";
    cin >> targetUID;

    // Find the vector of offenses for the given UID
    auto it = offensesByUID.find(targetUID);
    if (it != offensesByUID.end()) {
        const vector<Offense>& offenses = it->second;
        for (const Offense& offense : offenses) {
            cout << "Student UID: " << offense.getStudentUID() << endl;
            cout << "Name: " << offense.getName() << endl;
            cout << "Age: " << offense.getAge() << endl;
            cout << "Offense Type: " << offense.getOffenseType() << endl;
            cout << "Offense Amount: " << offense.getOffenseAmount() << endl;
            cout << "Location: " << offense.getLocation() << endl;
            cout << "Date: " << offense.getDate() << endl;
            cout << endl; // Spacing between offenses
        }

    } else {
        cout << "Student UID not found.\n";
    }
}

// Function to filter offenses based on age range
void filterOffenses(const vector<Offense>& offenseRecords, int minAge, int maxAge) {
    for (const Offense& offense : offenseRecords) {
        if (offense.getAge() >= minAge && offense.getAge() <= maxAge) {
            // Display offense details within the specified age range
            cout << "Student UID: " << offense.getStudentUID() << endl;
            cout << "Name: " << offense.getName() << endl;
            cout << "Age: " << offense.getAge() << endl;
            cout << "Offense Type: " << offense.getOffenseType() << endl;
            cout << "Offense Amount: " << offense.getOffenseAmount() << endl;
            cout << "Location: " << offense.getLocation() << endl;
            cout << "Date: " << offense.getDate() << endl;
            cout << endl; // Spacing between offenses
        }
    }
    // If no offenses found in the specified age range
    cout << "None found in the range." << endl;
}




int main() {
    vector<Offense> offenseRecords; // Store all offense records
    priority_queue<Offense> amountQueue; // Priority queue to keep track of top offenses
    map<string, vector<Offense> > offensesByUID; // Map to store offenses by UID

    // Load any existing offenses from the CSV file
    ifstream csvFileIn("offense_data.csv");
    if (csvFileIn.is_open()) {
        string line;
        while (getline(csvFileIn, line)) {
            Offense offense;
            stringstream ss(line); // To parse comma-separated values
            string temp;
        
            getline(ss, temp, ',');
            offense.setStudentUID(temp);
            getline(ss, temp, ',');
            offense.setName(temp);
            getline(ss, temp, ',');
            offense.setAge(stoi(temp));
            getline(ss, temp, ',');
            offense.setOffenseType(temp);
            getline(ss, temp, ',');
            offense.setOffenseAmount(stoi(temp));
            getline(ss, temp, ',');
            offense.setLocation(temp);
            getline(ss, temp, ',');
            offense.setDate(temp);

            offenseRecords.push_back(offense); // Add offense to the vector
            amountQueue.push(offense); // Add offense to the priority queue
            offensesByUID[offense.getStudentUID()].push_back(offense); // Add offense to the map
        }
        csvFileIn.close();
    } else {
        cout << "Error: Could not open offense_data.csv" << endl;
    }

    int choice;

    do {
        // Display the menu
        cout << "\nAutomated Offense Tracking System\n";
        cout << "1. Add Data\n";
        cout << "2. Search Data\n";
        cout << "3. Display top 3 offenses\n";
        cout << "4. Filter offenses based on age range\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n'); // Consume the newline character

        switch (choice) {
            case 1:
                addData(offenseRecords, offensesByUID, amountQueue); // Add a new offense record
                break;
            case 2:
                searchByUID(offensesByUID); // Search for offenses by UID
                break;
            case 3:
                if (amountQueue.size() >= 3) {
                    cout << "\nTop 3 Most Severe Offenses:\n";
                    for (int i = 0; i < 3; i++) {
                        Offense current = amountQueue.top(); // Get the offense with the highest amount
                        cout << "UID: " << current.getStudentUID() << ", Name: " << current.getName() << ", Amount: " << current.getOffenseAmount() << endl;
                        amountQueue.pop(); // Remove the offense from the queue
                    }
                } else {
                    cout << "Not enough offenses to display top 3.\n";
                }
                break;
            case 4: // Filter option
                cout << "Enter minimum age: ";
                int minAge;
                cin >> minAge;

                cout << "Enter maximum age: ";
                int maxAge;
                cin >> maxAge;

                filterOffenses(offenseRecords, minAge, maxAge); // Filter offenses based on age range
                break;

            case 9:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 9); // Loop until the user chooses to exit

    return 0;
}

