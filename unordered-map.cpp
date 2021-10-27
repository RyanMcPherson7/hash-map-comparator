#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

unsigned int hashFunction(char const* key, int table_size) {}

class UnorderedMap 
{
    private:
        //define your data structure here
        //define other attributes e.g. bucket count, maximum load factor, size of table, etc. 
        unsigned int count;
        unsigned int bucketCount;
        double loadFactor;
        vector<pair<string, string>> table[];

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
                Iterator() { }
                Iterator& operator=(Iterator const& rhs) { }
                Iterator& operator++() { }
                bool operator!=(Iterator const& rhs) { }
                bool operator==(Iterator const& rhs) { }
                pair<string, string> operator*() const { }
                friend class UnorderedMap;
        };
};

UnorderedMap::UnorderedMap(unsigned int bucketCount, double loadFactor) 
{

}

UnorderedMap::~UnorderedMap() 
{

}

UnorderedMap::Iterator UnorderedMap::begin() const 
{

}

UnorderedMap::Iterator UnorderedMap::end() const 
{

}

string& UnorderedMap::operator[] (string const& key) 
{

}

void UnorderedMap::rehash() 
{

}

void UnorderedMap::remove(string const& key) 
{

}

unsigned int UnorderedMap::size()
{

}

double UnorderedMap::loadFactor()
{

}

//implement other operators in Iterator class










// ================================================================
// ================================================================
// ================================================================
// ================================================================
// ================================================================
// ================================================================
// ================================================================
//Do not change main() 
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