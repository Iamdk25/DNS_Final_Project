#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm> // For binary_search
#include <sstream> // For parsing CSV lines
#include <limits>
#include <queue> // Include the queue header

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

    int getOffenseSeverity() const { return offenseSeverity; }
    void setOffenseSeverity(int newSeverity) { offenseSeverity = newSeverity; }

    string getLocation() const { return location; }
    void setLocation(const string& newLocation) { location = newLocation; }

    string getDate() const { return date; }
    void setDate(const string& newDate) { date = newDate; }

    bool operator<(const Offense& other) const {
        return offenseSeverity > other.offenseSeverity; // Descending order priority
    }
    
private:
    string studentUID;
    string name;
    int age;
    string offenseType;
    int offenseSeverity; 
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


void addData(vector<Offense>& offenseRecords, priority_queue<Offense>& severityQueue) {
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
    cout << "Enter Offense Severity: ";
    getline(cin, temp);
    newOffense.setOffenseSeverity(stoi(temp));
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
                << offense.getOffenseSeverity() << "," << offense.getLocation() << ","
                << offense.getDate() << endl; 
    }
    csvFileOut.close();

    // Add to priority queue
    severityQueue.push(newOffense); 


    cout << "New offense record added!" << endl; 
}

void searchByUID(const vector<Offense>& offenseRecords) {
    string targetUID;
    cout << "Enter Student UID to search: ";
    cin >> targetUID;

    int index = binarySearch(offenseRecords, targetUID); 

    if (index >= 0 && index < offenseRecords.size()) {
        const Offense& foundOffense = offenseRecords[index]; 
        // Display all the details of foundOffense 
        cout << "Name: " << foundOffense.getName() << endl;
        cout << "Age: " << foundOffense.getAge() << endl;
        cout << "Offense Type: " << foundOffense.getOffenseType() << endl;
        cout << "Offense Severity: " << foundOffense.getOffenseSeverity() << endl;
        cout << "Location: " << foundOffense.getLocation() << endl;
        cout << "Date: " << foundOffense.getDate() << endl;
    } else {
        cout << "Student UID not found.\n";
    }
}


int main() {
    vector<Offense> offenseRecords;
    priority_queue<Offense> severityQueue; 

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
            offense.setOffenseSeverity(stoi(temp));
            getline(ss, temp, ',');
            offense.setLocation(temp);
            getline(ss, temp, ',');
            offense.setDate(temp);

            offenseRecords.push_back(offense);
            severityQueue.push(offense); // Add to priority queue as well
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
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n'); // Consume the newline character

        switch (choice) {
            case 1: 
                addData(offenseRecords, severityQueue);
                break;
            case 2:
                searchByUID(offenseRecords);
                break;
            case 3:
                if (severityQueue.size() >= 3) {
                    cout << "\nTop 3 Most Severe Offenses:\n";
                    for (int i = 0; i < 3; i++) {
                        Offense current = severityQueue.top();
                        cout << "UID: " << current.getStudentUID() << ", Severity: " << current.getOffenseSeverity() << endl; 
                        severityQueue.pop(); 
                    }
                } else {
                    cout << "Not enough offenses to display top 3.\n";
                }
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 3); 


    return 0;
}

