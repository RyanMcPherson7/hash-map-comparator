#include "functionality/ordered-map.hpp"

int main()
{
    OrderedMap myMap;
    int lines = 0;
    std::string command = "", ufid = "", name = "";
    std::cin >> lines;
    while(lines--)
    {
        std::cin >> command;
        if(command == "insert") 
        {
            std::cin >> ufid >> name;
            std::cout << myMap.insert(ufid, name) << "\n";
        }
        else if(command == "search")
        {
            std::cin >> ufid;
            std::cout << myMap.search(ufid) << "\n";
        }
        else if(command == "traverse")
        {
            std::cout << myMap.traverse() << "\n";
        }
        else if(command == "remove")
        {
            std::cin >> ufid;
            std::cout << myMap.remove(ufid) << "\n";
        }  
    }

    return 0;
}