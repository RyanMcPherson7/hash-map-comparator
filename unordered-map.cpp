#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stack>
using namespace std;

////////////////////////////////////////////
// LINKED LIST NODE CLASS
////////////////////////////////////////////
struct ListNode {
    pair<string, string> data;
    ListNode* next;
    ListNode(const string& id, const string& name) : data(id, name), next(nullptr) {}
};

////////////////////////////////////////////
// HASH FUNCTION 
////////////////////////////////////////////
unsigned int hashFunction(char const *key, int table_size) {

    unsigned int hashCode = 0;
    string s(key);

    for (int n = 0; n < s.length(); n++) {

        unsigned int b;

        if (n % 2 == 0)
            b = (hashCode << 7) ^ s[n] ^ (hashCode >> 3);
        else 
            b = ~((hashCode << 11) ^ s[n] ^ (hashCode >> 5));

        hashCode ^= b;
    }

    // switching 32nd bit to zero
    hashCode &= ~(1 << 31);

    return hashCode % table_size;
}

////////////////////////////////////////////
// UNORDERED MAP CLASS
////////////////////////////////////////////
class UnorderedMap 
{
    private: 
        unsigned int numKeys;
        // unsigned int bucketCount;
        double LF;
        // ListNode** table;

        bool idValid(const string& id);
        bool nameValid(const string& name);
    public:
        unsigned int bucketCount;
        ListNode** table;

        class Iterator;
        UnorderedMap(unsigned int bucketCount, double loadFactor);
        ~UnorderedMap();
        Iterator begin() const;
        Iterator end() const;
        string& operator[] (string const& key);
        void rehash();
        void remove(string const& key);
        unsigned int size();
        double loadFactor();

        class Iterator 
        {
            public:
                //this constructor does not need to be a default constructor;
                //the parameters for this constructor are up to your discretion.
                //hint: you may need to pass in an UnorderedMap object.
                // Iterator() { }
                // Iterator& operator=(Iterator const& rhs) { }
                // Iterator& operator++() { }
                // bool operator!=(Iterator const& rhs) { }
                // bool operator==(Iterator const& rhs) { }
                // pair<string, string> operator*() const { }
                // friend class UnorderedMap;
        };
};

////////////////////////////////////////////
// CHECKING VALIDITY OF INPUT
////////////////////////////////////////////
bool UnorderedMap::idValid(const string& id) {

    // if id is not 8 digits
    if (id.length() != 8) {
        // cout << "unsuccessful" << endl;
        return false;
    }

    // if id contains non-numbers
    for (int i = 0; i < id.length(); i++) {
        if (id[i] < '0' || id[i] > '9') {
            // cout << "unsuccessful" << endl;
            return false;
        }
    }

    return true;
}

bool UnorderedMap::nameValid(const string& name) {

    // if name contains non-letters
    for (int i = 0; i < name.length(); i++) {
        if (!(name[i] >= 'a' && name[i] <= 'z') && !(name[i] >= 'A' && name[i] <= 'Z') && name[i] != ' ') {
            // cout << "unsuccessful" << endl;
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////
// UNORDERED MAP CLASS FUNCTIONS
////////////////////////////////////////////
UnorderedMap::UnorderedMap(unsigned int bucketCount, double loadFactor) {
    this->bucketCount = bucketCount;
    LF = loadFactor;
    numKeys = 0;
    table = new ListNode*[bucketCount];

    for (int i = 0; i < bucketCount; i++)
        table[i] = nullptr;
}

UnorderedMap::~UnorderedMap() {
    delete [] table;
}

// UnorderedMap::Iterator UnorderedMap::begin() const 
// {

// }

// UnorderedMap::Iterator UnorderedMap::end() const 
// {

// }


string& UnorderedMap::operator[] (string const& key) {

    unsigned int hashCode = hashFunction(key.c_str(), bucketCount);

    // inserting if bucket is empty
    if (!table[hashCode]) {
        numKeys++;
        table[hashCode] = new ListNode(key, "");
        return table[hashCode]->data.second;
    }
    
    ListNode* slot = table[hashCode];

    // if key already in map
    while (slot) {
        if (slot->data.first == key) {
            return slot->data.second;
        }
        
        // at tail node
        if (!slot->next) 
            break;
        
        slot = slot->next; 
    }

    // inserting key to back
    numKeys++;
    slot->next = new ListNode(key, "");

    // rehashing if load factor surpassed
    if ((float)numKeys / (float)bucketCount >= LF) {
        rehash();

        // updating latest added value
        unsigned int newHashCode = hashFunction(key.c_str(), bucketCount);
        ListNode* updNode = table[newHashCode];
        while (updNode) {
            if (updNode->data.first == key)
                return updNode->data.second;

            updNode = updNode->next;
        }
    }

    return slot->next->data.second;
}


void UnorderedMap::rehash() {

    // copying old data
    stack<pair<string, string>> mem;
    for (int i = 0; i < bucketCount; i++) {
        ListNode* slot = table[i];
        while (slot) {
            mem.push(slot->data);
            slot = slot->next;
        }
    }

    // updating new values
    delete [] table;
    bucketCount *= 2;
    table = new ListNode*[bucketCount];
    for (int i = 0; i < bucketCount; i++)
        table[i] = nullptr;

    // remapping old data
    while (!mem.empty()) {

        string key = mem.top().first;
        string value = mem.top().second;
        unsigned int hashCode = hashFunction(key.c_str(), bucketCount);
        
        if (!table[hashCode]) {
            table[hashCode] = new ListNode(key, value);
        }
        else {
            ListNode* slot = table[hashCode];
            while (slot->next) 
                slot = slot->next;

            slot->next = new ListNode(key, value);
        }

        mem.pop();
    }
}

void UnorderedMap::remove(string const& key) {

    unsigned int hashCode = hashFunction(key.c_str(), bucketCount);

    if (!table[hashCode])
        return;

    // if key is head of list 
    if (table[hashCode]->data.first == key) {
        delete table[hashCode];
        table[hashCode] = nullptr;
        numKeys--;
    }
    // removing from body of list
    else {
        ListNode* slot = table[hashCode];
        while (slot->next && slot->next->data.first != key) 
            slot = slot->next;

        if (slot->next) {
            ListNode* temp = slot->next;
            slot->next = temp->next;
            delete temp;
            numKeys--;
        }
    }
}

unsigned int UnorderedMap::size() {
    return numKeys;
}

double UnorderedMap::loadFactor() {
    return LF;
}

//implement other operators in Iterator class










// ================================================================
// ================================================================
// ================================================================
// ================================================================
// ================================================================
// ================================================================
// ================================================================

int main() {
    
    UnorderedMap map(1, 0.75f);

    // map.table[0] = new ListNode("12341234", "da baby");
    // map.table[0]->next = new ListNode("44444444", "da mihir");
    // map.table[0]->next->next = new ListNode("78787878", "da bobby");

    // map["77777777"] = "54";
    // map["88888888"] = "other val";
    // map["99999999"] = "additional name";
    // map["99999999"] = "3425345";
    // map["12341234"] = "fasdf";

    // cout << map["77777777"] << endl;
    // cout << map["88888888"] << endl;
    // cout << map["99999999"] << endl;
    // cout << map["12341234"] << endl;


    for (int i = 0; i < 2000; i++) {
        map[to_string(i)] = "nice";
    }
    cout << map.bucketCount << endl;
    map.remove("234");
    map["234"] = "pls work";

    cout << map.size() << endl;
    cout << map["234"] << endl;



    // while (map.table[0]) {
    //     cout << map.table[0]->data.second << endl;
    //     map.table[0] = map.table[0]->next;
    // }

    

    



    return 0;
}




////////////////////////////////////////////
// MAIN TO PARSE INPUT COMMANDS
////////////////////////////////////////////
// //Do not change main() 
// int main()
// {
//     int lines = 0, buckets = 0;
//     double maxLoadFactor = 0.0;
//     string command = "", ufid = "", name = "", key = "";
//     cin >> lines >> buckets >> maxLoadFactor;
//     UnorderedMap myMap = UnorderedMap(buckets, maxLoadFactor);
//     while(lines--)
//     {
//         cin >> command;
//         if(command == "hash")
//         {
//             cin >> key;
//             const char* convertedKey = key.c_str();
//             cout << hashFunction(convertedKey, buckets) << "\n";
//         }
//         else if(command == "insert") 
//         {
//             cin >> ufid >> name;
//             myMap[ufid] = name;
//         }
//         else if(command == "size") 
//         {
//             cout << myMap.size() <<"\n";
//         }
//         else if(command == "load") 
//         {
//             cout << fixed << setprecision(2) << myMap.loadFactor() <<"\n";
//         }
//         else if(command == "search")
//         {
//             cin >> ufid;
//             cout << myMap[ufid] << "\n";
//         }
//         else if(command == "traverse")
//         {
//             for (UnorderedMap::Iterator iter = myMap.begin(); iter != myMap.end(); ++iter) 
//             {
//                 cout << (*iter).first << " " << (*iter).second << "\n";
//             }

//             /* This should also work
//                 for (auto tableEntry: myMap) 
//                 {
//                     cout << tableEntry.first << " " << tableEntry.second << "\n";
//                 }
//             */
//         }
//         else if(command == "remove")
//         {
//             cin >> ufid;
//             myMap.remove(ufid);
//         }  
//     }

//     return 0;
// }