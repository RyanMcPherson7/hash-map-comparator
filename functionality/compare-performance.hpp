#include "ordered-map.hpp"
#include "unordered-map.hpp"
#include <chrono>
#include <unordered_map>
#include <map>

// Compares the performance of STL and user implemented 
// ordered and unordered maps
void compareOrderAndUnorder (const int numEntries) {

    UnorderedMap* unMap = new UnorderedMap(1, 0.75);
    OrderedMap* orMap = new OrderedMap;
    map<string, string>* stlOrMap = new map<string, string>;
    unordered_map<string, string>* stlUnMap = new unordered_map<string, string>;

    // ==================
    // INSERTING
    // ==================
    // UNORDERED MAP
    // USER
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        (*unMap)[to_string(rand() % numEntries * 2)] = "test";
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<float> duration = end - start;
    cout << "User: Ins " << numEntries << " UM took: " << duration.count() << " s" << endl;
    // STL
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        (*stlUnMap)[to_string(rand() % numEntries * 2)] = "test";
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "stl:  Ins " << numEntries << " UM took: " << duration.count() << " s" << endl;

    // OM
    // USER
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        string test = "test";
        orMap->insert(to_string(rand() % numEntries * 2), test);
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "User: Ins " << numEntries << " OM took: " << duration.count() << " s" << endl;
    // STL
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        string test = "test";
        (*stlOrMap)[to_string(rand() % numEntries * 2)] = test;
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "stl:  Ins " << numEntries << " OM took: " << duration.count() << " s" << endl;

    // ==================
    // SEARCHING
    // ==================
    // UM
    // USER
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        (*unMap)[to_string(rand() % numEntries * 2)];
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "User: Sea " << numEntries << " UM took: " << duration.count() << " s" << endl;
    // STL
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        stlUnMap->find(to_string(rand() % numEntries * 2));
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "stl:  Sea " << numEntries << " UM took: " << duration.count() << " s" << endl;

    // OM
    // USER
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        orMap->search(to_string(rand() % numEntries * 2));
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "User: Sea " << numEntries << " OM took: " << duration.count() << " s" << endl;
    // STL
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        stlOrMap->find(to_string(rand() % numEntries * 2));
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "stl:  Sea " << numEntries << " OM took: " << duration.count() << " s" << endl;

    // ==================
    // REMOVING
    // ==================
    // UM
    // USER
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        unMap->remove(to_string(rand() % numEntries * 2));
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start; 
    cout << "User: Rem " << numEntries << " UM took: " << duration.count() << " s" << endl;
    // STL
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        stlUnMap->erase(to_string(rand() % numEntries * 2));
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start; 
    cout << "stl:  Rem " << numEntries << " UM took: " << duration.count() << " s" << endl;

    // OM
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        orMap->remove(to_string(rand() % numEntries * 2));
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "User: Rem " << numEntries << " OM took: " << duration.count() << " s" << endl;
    // STL
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        stlOrMap->erase(to_string(rand() % numEntries * 2));
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "stl:  Rem " << numEntries << " OM took: " << duration.count() << " s" << endl;

    // ==================
    // CLEANING
    // ==================
    delete unMap;
    delete orMap;
    delete stlUnMap;
    delete stlOrMap;
    cout << endl;
}