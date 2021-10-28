#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
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

    return hashCode % 100;
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
        ListNode* table[];

        bool idValid(const string& id);
        bool nameValid(const string& name);
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

// returns true if input id is valid
bool UnorderedMap::idValid(const string& id) {

    // if id is not 8 digits
    if (id.length() != 8) {
        cout << "unsuccessful" << endl;
        return false;
    }

    // if id contains non-numbers
    for (int i = 0; i < id.length(); i++) {
        if (id[i] < '0' || id[i] > '9') {
            cout << "unsuccessful" << endl;
            return false;
        }
    }

    return true;
}

// returns true if input name is valid
bool UnorderedMap::nameValid(const string& name) {

    // if name contains non-letters
    for (int i = 0; i < name.length(); i++) {
        if (!(name[i] >= 'a' && name[i] <= 'z') && !(name[i] >= 'A' && name[i] <= 'Z') && name[i] != ' ') {
            cout << "unsuccessful" << endl;
            return false;
        }
    }

    return true;
}

// UnorderedMap::UnorderedMap(unsigned int bucketCount, double loadFactor) 
// {

// }

// UnorderedMap::~UnorderedMap() 
// {

// }

// UnorderedMap::Iterator UnorderedMap::begin() const 
// {

// }

// UnorderedMap::Iterator UnorderedMap::end() const 
// {

// }

// string& UnorderedMap::operator[] (string const& key) 
// {

// }

// void UnorderedMap::rehash() 
// {

// }

// void UnorderedMap::remove(string const& key) 
// {

// }

// unsigned int UnorderedMap::size()
// {

// }

// double UnorderedMap::loadFactor()
// {

// }

//implement other operators in Iterator class










// ================================================================
// ================================================================
// ================================================================
// ================================================================
// ================================================================
// ================================================================
// ================================================================

int main() {
    
    cout << hashFunction("Gator", 100);


    return 0;
}

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