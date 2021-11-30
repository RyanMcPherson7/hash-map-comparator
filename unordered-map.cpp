#include "functionality/unordered-map.hpp"

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