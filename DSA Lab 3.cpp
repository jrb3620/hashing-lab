#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <array>
#include <cmath>
using namespace std;

struct Record {
    string key;
    bool isOccupied = false;
    int initialHashAddress = -1;
    int probeCount = 0;
};

const int TABLE_SIZE = 128;
Record hashTable[TABLE_SIZE];

int myAbs(int x) {
    return x < 0 ? -x : x;
}


int burrisHash(const string& str) {
    long long B = str[1];
    long long A = str[0];
    long long K = str[10];
    long long DE = (str[3] * 10 + str[4]) - ('0' * 11);
    long long GH = (str[6] * 10 + str[7]) - ('0' * 11);

    long long temp = B + ((DE + GH) / 381.0 + A) / 587.0 - K;
    return myAbs(static_cast<int>(temp)) % TABLE_SIZE;
}


// new hash function

void newHash(const string& str, int& HA) {
    long hash = 5381;
    for (char c : str) {
        if (!isspace(c)) {
            hash = (((hash << 5) + hash) + c) % TABLE_SIZE;
        }
    }
    HA = hash;
}


array<int, 25> first25Probes;
array<int, 25> last25Probes;
int numKeysInserted = 0;

int burrisHash(const string& str);
void newHash(const string& str, int& HA);

void insert(const string& key, bool useBurris = true) {
    int index;
    int R = 1;

    // Decide which hash function to use
    if (useBurris) {
        index = burrisHash(key);
    }
    else {
        newHash(key, index);
    }

    int start_index = index;
    int probe_count = 0;

    while (true) {
        if (!hashTable[index].isOccupied) {
            hashTable[index].key = key;
            hashTable[index].isOccupied = true;
            hashTable[index].initialHashAddress = start_index;
            hashTable[index].probeCount = probe_count + 1;

            if (numKeysInserted < 25) {
                first25Probes[numKeysInserted] = probe_count + 1;
            }
            if (numKeysInserted >= 25) {
                last25Probes[numKeysInserted % 25] = probe_count + 1; // Circular buffer
            }
            numKeysInserted++;

            return;
        }

        index = (index + 1) % TABLE_SIZE;
        probe_count++;

        

        if (index == start_index) {
            cerr << "Hash table is full" << endl;
            return;
        }
    }
}


int search(const string& key, bool useBurris = true) {
    int R = 1;
    int index;
    if (useBurris) {
        index = burrisHash(key);
    }
    else {
        newHash(key, index);
    }
    int start_index = index;
    int probe_count = 0;
    int visited_count = 0; 

    while (visited_count < TABLE_SIZE) {  // Ensure we don't loop indefinitely
        if (hashTable[index].isOccupied && hashTable[index].key == key) {
            return probe_count + 1;  // Found the key
        }

        
        index = (index + 1) % TABLE_SIZE;
        probe_count++;
        
        
        // If we've visited all possible slots or the current slot is not occupied
        if (!hashTable[index].isOccupied) {
            cout << "Could not find key: " << key << endl;
            return -1; 
        }
    }
}




void calculateActualAverageProbes(ostream& os) {
    int totalProbes = 0;
    int count = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i].isOccupied) {
            int probes = search(hashTable[i].key, true); //true for burris, false for new hash
            if (probes != -1) {  // Only count if the key was found
                totalProbes += probes;
                count++;
            }
        }
    }

    if (count > 0) {
        os << "Actual Average Number of Probes to Find All Keys: " << (static_cast<double>(totalProbes) / count) << endl;
    }
    else {
        os << "No keys were found." << endl;
    }
}


void computeStats(ostream& os, const array<int, 25>& probes) {
    int minProbes = INT_MAX;
    int maxProbes = INT_MIN;
    double sum = 0;

    for (int probe : probes) {
        if (probe < minProbes) minProbes = probe;
        if (probe > maxProbes) maxProbes = probe;
        sum += probe;
    }

    os << "Minimum Probes: " << minProbes << endl;
    os << "Maximum Probes: " << maxProbes << endl;
    os << "Average Probes: " << (sum / probes.size()) << endl;
    
}


void printExpectedProbes(ostream& os, int n, int m) {
    double alpha = static_cast<double>(n) / m;
    double expectedProbesSuccessful = 0.5 * (1 + 1 / (1 - alpha));

    os << "Theoretical Expected Number of Probes (Linear): " << expectedProbesSuccessful << endl;
}


void writeResultsToConsole() {
    cout << "Hash Table Contents:" << endl;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i].isOccupied) {
            cout << "Index " << i << ": Key " << hashTable[i].key
                << ", Initial Hash: " << hashTable[i].initialHashAddress
                << ", Probes: " << hashTable[i].probeCount << endl;
        }
        else {
            cout << "Index " << i << ": Empty" << endl;
        }
    }

    cout << "Statistics for First 25 Keys:" << endl;
    computeStats(cout, first25Probes);
    cout << "Statistics for Last 25 Keys:" << endl;
    computeStats(cout, last25Probes);

    cout << "Theoretical Expected Probes:" << endl;
    printExpectedProbes(cout, numKeysInserted, TABLE_SIZE);
    calculateActualAverageProbes(cout);
}


int main() {
    vector<string> keys = {
       "ABCDEFGHIJKLMNOP","1234567890123456","Aguirrie        ","Alcantara       ","Bhandari        ","Carmona         ","Casper          ","Cook            ","Daniels         ",
"Nienberg        ","Paschal         ","Red             ","Salkowski       ","Zulfiqar        ","Qamruddin       ","Acevedo         ","Ajose           ","Arauza          ","Buck            ","Clark           ",
"Crouch          ","Davies          ","Dugger          ","Egbe            ","Ellington       ","Farral          ","Garza           ","Gurung          ","Joseph          ","Kelly           ",
"Corey           ","Adam            ","Clayton         ","Dustin          ","Robert          ","Kyle            ","Scott           ","Octavio         ","Judy            ","Derek           ",
"Jeffrey         ","Jordon          ","Vinnela         ","Lisa            ","Todd            ","Veronica        ","Matthew         ","Michael         ","Akhila          ","John            ",
"Charles         ","James           ","Chris           ","Wade            ","Christopher     ","Fernando        ","Batbold         ","Joel            ","Fabulous        ","Misogamist      ",
"Maiden          ","Eye             ","Constriction    ","Necromancer     ","Syncopate       ","Yolk            ","Afterwards      ","Person          ","Northwest       ","Irreversible    ",
"Fabricate       ","Honor           ","Staple          ","Under           ","Jutty           ","Finagle         ","Cook            ","Rush            ","Wine            ","Screen          ","Perfect         ",
"mole            ","parasympathetic ","poison          ","brutalize       ","cap             ","ratiocination   ","cauldron        ","prepossess      ","wince           ",
"orthodontist    ","live            ","magnetic        ", "inlet           ", "constrain       ", "marsupial       ", "rationalize     ", "scat            ", "toluene         ", "wet             "

    };

    for (const string& key : keys) {
        insert(key, true);  // Set true to use Burris hash, false for new Hash
    }


    // Directly work with hash table in memory for result calculations
    writeResultsToConsole();  // Adapt this function to output results directly to console
    return 0;
}


