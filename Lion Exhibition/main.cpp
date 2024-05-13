#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <vector>

using namespace std;

// Node class for Doubly Linked List
class DLLNode {
public:
    string name;
    int height;
    int enterTime;
    DLLNode* next;
    DLLNode* prev;

    DLLNode(string _name, int _height, int _enterTime) : name(_name), height(_height), enterTime(_enterTime), next(nullptr), prev(nullptr) {}
};

// Doubly Linked List class
class DLL {
private:
    DLLNode* head;
    DLLNode* tail;

public:
    DLL() {
        head = new DLLNode("", -1, -1); // Dummy head
        tail = new DLLNode("", -1, -1); // Dummy tail
        head->next = tail;
        tail->prev = head;
    }

    // Function to add a new node to DLL
    void addNode(DLLNode* newNode) {
        newNode->next = head->next;
        newNode->prev = head;
        head->next->prev = newNode;
        head->next = newNode;
    }

    // Function to remove a node from DLL
    void removeNode(DLLNode* delNode) {
        delNode->prev->next = delNode->next;
        delNode->next->prev = delNode->prev;
        delete delNode;
    }

    // Destructor to clean up memory
    ~DLL() {
        DLLNode* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Hash function for pair<int, int>
struct pair_hash {
    template <class T1, class T2>
    size_t operator () (const pair<T1, T2> &p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

// LionCompetition class
class LionCompetition {
private:
    unordered_map<string, pair<int, int>> ownLions; // Map of own lions (name, height, enterTime)
    unordered_map<pair<int, int>, DLLNode*, pair_hash > otherLionsMap; // Map to store pointers to DLL nodes of other lions
    DLL otherLionsDLL; // Doubly Linked List for other lions
    set<pair<int, int>> ownLionHeightsSet; // Set to store own lions' heights and entry times
    int otherLionMaxHeight; // Maximum height among other lions
    int otherLionMaxHeightFrequency; // Frequency of maximum height among other lions

public:
    LionCompetition() : otherLionMaxHeight(0), otherLionMaxHeightFrequency(0) {}

    // Function to handle lion entry
    void lionEntered(string name, int height, int enterTime) {
        // Check if the lion is from other trainers
        if (ownLions.find(name) == ownLions.end()) {
            // Lion is from other trainers
            // Add to DLL
            DLLNode* newNode = new DLLNode(name, height, enterTime);
            otherLionsMap[{height, enterTime}] = newNode;
            otherLionsDLL.addNode(newNode);
            // Update maximum height and frequency
            if (height > otherLionMaxHeight) {
                otherLionMaxHeight = height;
                otherLionMaxHeightFrequency = 1;
            } else if (height == otherLionMaxHeight) {
                otherLionMaxHeightFrequency++;
            }
        } else {
            // Lion is mine
            ownLions[name] = {height, enterTime};
            ownLionHeightsSet.insert({height, enterTime});
        }
    }

    // Function to handle lion exit
    void lionLeft(string name, int exitTime) {
        if (ownLions.find(name) != ownLions.end()) {
            // Lion is mine
            ownLionHeightsSet.erase({ownLions[name].first, ownLions[name].second});
            ownLions.erase(name);
        } else {
            // Lion is from other trainers
            // Find lion in map
            auto it = otherLionsMap.find({height, exitTime});
            if (it != otherLionsMap.end()) {
                // Update maximum height and frequency if needed
                if (it->second->height == otherLionMaxHeight) {
                    otherLionMaxHeightFrequency--;
                    if (otherLionMaxHeightFrequency == 0) {
                        // Recalculate max height
                        otherLionMaxHeight = 0;
                        for (auto& entry : otherLionsMap) {
                            if (entry.second->height > otherLionMaxHeight) {
                                otherLionMaxHeight = entry.second->height;
                                otherLionMaxHeightFrequency = 1;
                            } else if (entry.second->height == otherLionMaxHeight) {
                                otherLionMaxHeightFrequency++;
                            }
                        }
                    }
                }
                // Remove lion from DLL
                otherLionsDLL.removeNode(it->second);
                otherLionsMap.erase(it);
            }
        }
    }

    // Function to handle inspection
    vector<string> inspect() {
        // Get the maximum height among other lions
        int maxOtherLionHeight = otherLionMaxHeight;
        vector<string> result;
        // Count own lions with heights greater than or equal to maxOtherLionHeight
        auto it = ownLionHeightsSet.upper_bound({maxOtherLionHeight, -1});
        while (it != ownLionHeightsSet.end()) {
            result.push_back(otherLionsMap[*it]->name);
            it++;
        }
        return result;
    }
};

int main() {
    LionCompetition competition;

    string input;
    while (getline(cin, input)) {
        if (input == "end") break;

        // Parsing input
        string name;
        int height, time;
        if (input == "inspect") {
            // Inspect
            vector<string> result = competition.inspect();
            cout << result.size() << " ";
            for (const auto& lion : result) {
                cout << lion << " ";
            }
            cout << endl;
        } else {
            // Lion enter or exit
            string action;
            cin >> name >> height >> time >> action;
            if (action == "enter") {
                competition.lionEntered(name, height, time);
            } else if (action == "exit") {
                competition.lionLeft(name, time);
            }
        }
    }

    return 0;
}
