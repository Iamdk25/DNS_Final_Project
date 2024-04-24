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


class Offense {
public:
    string getStudentUID() const { return studentUID; }
    void setStudentUID(const string& uid) { studentUID = uid; }

    string getName() const { return name; }
    void setName(const string& newName) { name = newName; }

    int getAge() const { return age; }
    void setAge(int newAge) { age = newAge; }

    string getOffenseType() const { return offenseType; }
    void setOffenseType(const string& newType) { offenseType = newType; }

    int getOffense_amount() const { return offense_amount; }
    void setOffense_amount(int new_amount) { offense_amount = new_amount; }

    string getLocation() const { return location; }
    void setLocation(const string& newLocation) { location = newLocation; }

    string getDate() const { return date; }
    void setDate(const string& newDate) { date = newDate; }

    bool operator<(const Offense& other) const {
        return offense_amount < other.offense_amount; // Descending order priority
    }
    
private:
    string studentUID;
    string name;
    int age;
    string offenseType;
    int offense_amount; 
    string location;
    string date; 
};    


// Comparator for binary search (assuming UID is unique)
bool compareUIDs(const Offense& a, const Offense& b) {
    return a.getStudentUID() < b.getStudentUID();
}

// Basic binary search implementation 
int binarySearch(const vector<Offense>& records, const string& targetUID) {
    int low = 0;
    int high = records.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (records[mid].getStudentUID() == targetUID) {
            return mid; 
        } else if (records[mid].getStudentUID() < targetUID) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1; // If not found, returns the index where it should be inserted
}


void addData(vector<Offense>& offenseRecords, map<string, vector<Offense>>& offensesByUID, priority_queue<Offense>& _amountQueue) {
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
    newOffense.setOffense_amount(stoi(temp));
    cout << "Enter Location: ";
    getline(cin, temp);
    newOffense.setLocation(temp);
    cout << "Enter Date: ";
    getline(cin, temp);
    newOffense.setDate(temp);

    // Insertion
    int insertionIndex = binarySearch(offenseRecords, newOffense.getStudentUID());
    offenseRecords.insert(offenseRecords.begin() + (insertionIndex == -1 ? 0 : insertionIndex), newOffense);

    // Write to CSV
    ofstream csvFileOut("offense_data.csv");
    for (const Offense& offense : offenseRecords) {
        csvFileOut << offense.getStudentUID() << "," << offense.getName() << "," 
                << offense.getAge() << "," << offense.getOffenseType() << ","
                << offense.getOffense_amount() << "," << offense.getLocation() << ","
                << offense.getDate() << endl; 
    }
    csvFileOut.close();

    // Add to map
    offensesByUID[newOffense.getStudentUID()].push_back(newOffense); 

    // Add to priority queue
    _amountQueue.push(newOffense); 


    cout << "New offense record added!" << endl; 
}



void searchByUID(const map<string, vector<Offense>>& offensesByUID) { 
    string targetUID;
    cout << "Enter Student UID to search: ";
    cin >> targetUID;

    auto it = offensesByUID.find(targetUID);
    if (it != offensesByUID.end()) {
        const vector<Offense>& offenses = it->second;
        for (const Offense& offense : offenses) {
            cout << "Student UID: " << offense.getStudentUID() << endl;
            cout << "Name: " << offense.getName() << endl;
            cout << "Age: " << offense.getAge() << endl;
            cout << "Offense Type: " << offense.getOffenseType() << endl;
            cout << "Offense Amount: " << offense.getOffense_amount() << endl;
            cout << "Location: " << offense.getLocation() << endl;
            cout << "Date: " << offense.getDate() << endl; // Assuming a suitable format for Date
            cout << endl; // Spacing between offenses
        }

    } else {
        cout << "Student UID not found.\n";
    }
}


void filterOffenses(const vector<Offense>& offenseRecords, int minAge, int maxAge) {
    for (const Offense& offense : offenseRecords) {
        if (offense.getAge() >= minAge && offense.getAge() <= maxAge) {
            // Display offense details
            cout << "Student UID: " << offense.getStudentUID() << endl;
            cout << "Name: " << offense.getName() << endl;
            cout << "Age: " << offense.getAge() << endl;
            cout << "Offense Type: " << offense.getOffenseType() << endl;
            cout << "Offense _amount: " << offense.getOffense_amount() << endl;
            cout << "Location: " << offense.getLocation() << endl;
            cout << "Date: " << offense.getDate() << endl; // Assuming a suitable format for Date
            cout << endl; // Spacing between offenses
            // ... Output rest of the offense information 
        } else{
            cout << "None found in the range." << endl;
        }
    }
}




int main() {
    vector<Offense> offenseRecords;
    priority_queue<Offense> _amountQueue;
    map<string, vector<Offense>> offensesByUID;

    // Load any existing offenses from your CSV
    // CSV Loading 
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
            offense.setOffense_amount(stoi(temp));
            getline(ss, temp, ',');
            offense.setLocation(temp);
            getline(ss, temp, ',');
            offense.setDate(temp);

            offenseRecords.push_back(offense);
            _amountQueue.push(offense); // Add to priority queue as well
            // Add to map
            offensesByUID[offense.getStudentUID()].push_back(offense); 
        }
        csvFileIn.close();
    } else {
        cout << "Error: Could not open offense_data.csv" << endl;
    }


    int choice;

    do {
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
                addData(offenseRecords, offensesByUID, _amountQueue);
                break;
            case 2:
                searchByUID(offensesByUID);
                break;
            case 3:
                if (_amountQueue.size() >= 3) {
                    cout << "\nTop 3 Most Severe Offenses:\n";
                    for (int i = 0; i < 3; i++) {
                        Offense current = _amountQueue.top();
                        cout << "UID: " << current.getStudentUID() << ", Name: " << current.getName() << ", _amount: " << current.getOffense_amount() << endl; 
                        _amountQueue.pop(); 
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

                filterOffenses(offenseRecords, minAge, maxAge);
                break;

            case 9:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 9); 


    return 0;
}
