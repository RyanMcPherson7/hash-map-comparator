#pragma once
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

    for (int i = 0; i < bucketCount; i++)
        if (table[i] != nullptr) {
            beginIter.bucketIndex = i;
            beginIter.pointer = table[i];   
            break;
        }


    return beginIter;
}

// the end iterator will be signified by an iterator 
// with a pointer pointing to nullptr
UnorderedMap::Iterator UnorderedMap::end() const {
    return UnorderedMap::Iterator(this);
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

        ListNode* temp = table[hashCode];
        table[hashCode] = temp->next;
        delete temp;
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