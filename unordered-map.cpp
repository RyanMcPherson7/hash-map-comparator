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
        unsigned int bucketCount;
        double LF;
        ListNode** table;
    public:
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
            private:
                const UnorderedMap* map;
                ListNode* pointer;
                unsigned int bucketIndex;
            public:
                Iterator(const UnorderedMap* map) : map(map), pointer(nullptr), bucketIndex(0) {}
                Iterator& operator=(Iterator const& rhs) { 
                    pointer = rhs.pointer;
                    return *this;  
                }

                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                // IDK THIS IS PROBABLY WRONG PLS FIX ME !!!!!!
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

                Iterator& operator++() { 

                    // move to right node
                    if (pointer->next) {
                        pointer = pointer->next;
                        return *this;
                    }
                    else {
                        // visit next filled bucket
                        for (int i = bucketIndex + 1; i < map->bucketCount; i++)
                            if (map->table[i]) {
                                pointer = map->table[i];
                                bucketIndex = i;
                                return *this;
                            }
                    }

                    // reached end iterator
                    pointer = nullptr;
                    bucketIndex = map->bucketCount - 1;

                    return *this;

                    // if (!pointer->next && bucketIndex == map->bucketCount - 1) {
                    //     for (int i = map->bucketCount - 1; i >= 0; i--)
                    //         if (map->table[i]) {
                    //             ListNode* slot = map->table[i];

                    //             while (slot)
                    //                 slot = slot->next;

                    //             pointer = slot;
                    //         }
                    // }

                    // return *this;
                }
                bool operator!=(Iterator const& rhs) { return this->pointer != rhs.pointer; }
                bool operator==(Iterator const& rhs) { return this->pointer == rhs.pointer; }
                pair<string, string> operator*() const { return pointer->data; }
                friend class UnorderedMap;
        };
};


////////////////////////////////////////////
// UOM CONSTRUCTOR / DESTRUCTOR
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


////////////////////////////////////////////
// UOM ITERATORS
////////////////////////////////////////////
UnorderedMap::Iterator UnorderedMap::begin() const {
    UnorderedMap::Iterator beginIter(this);

    // for some bizzar reason, table[0] is not null despite not containing any items
    for (int i = 0; i < bucketCount; i++)
        if (table[i] != nullptr) {
            beginIter.bucketIndex = i;
            beginIter.pointer = table[i];   
            break;
        }


    return beginIter;
}

UnorderedMap::Iterator UnorderedMap::end() const {
    UnorderedMap::Iterator endIter(this);

    // for (int i = bucketCount - 1; i >= 0; i--)
    //     if (table[i]) {
    //         ListNode* slot = table[i];

    //         while (slot)
    //             slot = slot->next;

    //         endIter.bucketIndex = i;
    //         endIter.pointer = slot;
    //     }

    endIter.pointer = nullptr;

    return endIter;
}

////////////////////////////////////////////
// UOM [] OPERATOR (INSERT / GET)
////////////////////////////////////////////
string& UnorderedMap::operator[] (string const& key) {

    unsigned int hashCode = hashFunction(key.c_str(), bucketCount);

    // inserting if bucket is empty
    if (!table[hashCode]) {
        numKeys++;
        table[hashCode] = new ListNode(key, "");

        // rehashing if load factor surpassed
        if ((double)numKeys / (double)bucketCount >= LF) {
            rehash();
            unsigned int newHashCode = hashFunction(key.c_str(), bucketCount);
            return table[newHashCode]->data.second;
        }

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
    if ((double)numKeys / (double)bucketCount >= LF) {
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

////////////////////////////////////////////
// UOM REHASH MAP
////////////////////////////////////////////
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

    // updating new map variables
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

////////////////////////////////////////////
// UOM REMOVE KEY
////////////////////////////////////////////
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

////////////////////////////////////////////
// UOM GETTERS
////////////////////////////////////////////
unsigned int UnorderedMap::size() {
    return numKeys;
}

double UnorderedMap::loadFactor() {
    return (double)numKeys / (double)bucketCount;
}




// // ====================================================
// // ====================================================
// // ====================================================
// // ====================================================
// // ====================================================
// // ====================================================

// int main() {

//     UnorderedMap map(4, 0.75);
//     map["key1"] = "val1";
//     map["key2"] = "val2";
//     map["key3"] = "val3";
//     map["key4"] = "val4";
//     map["key5"] = "val5";
//     map["key6"] = "val6";
    
//     cout << map.loadFactor() << endl;


//     // UnorderedMap::Iterator iter2 = map.begin();
//     // ++iter;

    

//     return 0;
// }



////////////////////////////////////////////
// MAIN TO PARSE INPUT COMMANDS
////////////////////////////////////////////
int main()
{
    int lines = 0, buckets = 0;
    double maxLoadFactor = 0.0;
    string command = "", ufid = "", name = "", key = "";
    cin >> lines >> buckets >> maxLoadFactor;
    UnorderedMap myMap = UnorderedMap(buckets, maxLoadFactor);
    while(lines--)
    {
        cin >> command;
        if(command == "hash")
        {
            cin >> key;
            const char* convertedKey = key.c_str();
            cout << hashFunction(convertedKey, buckets) << "\n";
        }
        else if(command == "insert") 
        {
            cin >> ufid >> name;
            myMap[ufid] = name;
        }
        else if(command == "size") 
        {
            cout << myMap.size() <<"\n";
        }
        else if(command == "load") 
        {
            cout << fixed << setprecision(2) << myMap.loadFactor() <<"\n";
        }
        else if(command == "search")
        {
            cin >> ufid;
            cout << myMap[ufid] << "\n";
        }
        else if(command == "traverse")
        {
            for (UnorderedMap::Iterator iter = myMap.begin(); iter != myMap.end(); ++iter) 
            {
                cout << (*iter).first << " " << (*iter).second << "\n";
            }

            /* This should also work
                for (auto tableEntry: myMap) 
                {
                    cout << tableEntry.first << " " << tableEntry.second << "\n";
                }
            */
        }
        else if(command == "remove")
        {
            cin >> ufid;
            myMap.remove(ufid);
        }  
    }

    return 0;
}