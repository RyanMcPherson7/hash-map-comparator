#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stack>
#include <cctype>
#include <vector>
#include <queue>
#include <chrono>
using namespace std;

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// UNORDERED MAP IMPLIMENTATION
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

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


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ORDERED MAP IMPLIMENTATION
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

///////////////////////////////////////
// Avlnode and AvlTree were written by
// another student NOT Ryan McPherson
///////////////////////////////////////

////***** NODE CLASS DECLARATION
class Avlnode{
    ////CONSTRUCTORS AND BASIC VARIABLES
    public:
        string studentName;
        int studentID;
        int maxHeightL;
        int maxHeightR;

        Avlnode *left;
        Avlnode *right;

        Avlnode();
        Avlnode(string studentName, int studentID);
        Avlnode(string studentName, int studentID, Avlnode *left, Avlnode *right);
        Avlnode(string studentName, int studentID, Avlnode *left, Avlnode *right, int maxHeightL, int maxHeightR);

};

////***** TREE CLASS DECLARATION
class Avltree{

    Avlnode* rightRotation(Avlnode *problem);
    Avlnode* leftRotation(Avlnode *problem);
    
    Avlnode* balanceTree(Avlnode *problem);
    void findHeight(Avlnode *current);

    Avlnode* insertHelper(Avlnode* current, string &name, int id, int count, bool &isSuccessful);
    Avlnode* removeHelper(Avlnode *current, int id, bool &isSuccessful);
    Avlnode* removeInOrderHelper(Avlnode *current, int &n);
    Avlnode* deleteNode(Avlnode *current);

    void searchIDHelper(Avlnode *current, int id, string &response);
    void searchNameHelper(Avlnode *current, string &name , queue<int> &nameList);

    void printInOrderHelper(Avlnode *current, bool &comma);
    void printPreOrderHelper(Avlnode *current, bool &comma, string &response);
    void printPostOrderHelper(Avlnode *current, bool &comma);
    void printLevelCountHelper(Avlnode *current);

    public:
        Avlnode *root;
        int idCount = 0;

        bool insert(string &name, int id);

        bool remove(int id);
        void removeInOrder(int &n);

        string searchID(int id);
        void searchName(string name);

        void printInOrder();
        string printPreOrder();
        void printPostOrder();
        void printLevelCount();

};


////***** NODE CLASS DEFINITIONS

    Avlnode::Avlnode(){
        studentName = "";
        studentID = 0;
        left = nullptr;
        right = nullptr;
        maxHeightL = 0;
        maxHeightR = 0;
    }

    Avlnode::Avlnode(string studentName, int studentID){
        this->studentName = studentName;
        this->studentID = studentID;
        left = nullptr;
        right = nullptr;
        maxHeightL = 0;
        maxHeightR = 0;
    }

    Avlnode::Avlnode(string studentName, int studentID, Avlnode *left, Avlnode *right){
        this->studentName = studentName;
        this->studentID = studentID;
        this->left = left;
        this->right = right;
        maxHeightL = 0;
        maxHeightR = 0;
    }

    Avlnode::Avlnode(string studentName, int studentID, Avlnode *left, Avlnode *right, int maxHeightL, int maxHeightR){
        this->studentName = studentName;
        this->studentID = studentID;
        this->left = left;
        this->right = right;
        this->maxHeightL = maxHeightL;
        this->maxHeightR = maxHeightR;
    }

////***** TREE CLASS DEFINITIONS

    //insert
    bool Avltree::insert(string &name, int id){

        bool isSuccessful = false;

        if (idCount <= 100000){
            int count = 0;
            root = insertHelper(root, name, id, count, isSuccessful);
        }

        return isSuccessful;
    }
    Avlnode* Avltree::insertHelper(Avlnode* current, string &name, int id, int count, bool &isSuccessful){

        if (current == nullptr){
            // cout << "successful" << endl;
            isSuccessful = true;
            ++idCount;
            return new Avlnode(name, id);
        }
        //can't repeat ID, can repeat names
        else if (current->studentID == id){
            // cout << "unsuccessful" << endl;
            isSuccessful = false;
            return current;
        }
        //adding to the left, since passed id is less than current
        else if (current->studentID > id){
            current->left = insertHelper(current->left, name, id, ++count, isSuccessful);

            //updating height
            if (count > current->maxHeightL){
                current->maxHeightL += 1;
            }
            findHeight(current);
            
        }
        //adding to the right, since passed id is more than current
        else if (current->studentID < id){
            current->right = insertHelper(current->right, name, id, ++count, isSuccessful);

            //updating height
            if (count > (current->maxHeightR)){
                current->maxHeightR += 1;
            }
            findHeight(current);
        }

        //checking the balance value
        if (((current->maxHeightL - current->maxHeightR) > 1) || ((current->maxHeightL - current->maxHeightR) < -1)){
                //balance
                current = balanceTree(current);
            }

        return current;
    }

    //deletion
    bool Avltree::remove(int id){
        bool isSuccessful = false;
        root = removeHelper(root, id, isSuccessful);
        return isSuccessful;
    }
    Avlnode* Avltree::removeHelper(Avlnode *current, int id, bool &isSuccessful){
        //Note: I used psuedocode given in the lecture to structure this function

        if (current == nullptr){
            //item isn't in the trr
            // cout << "unsuccessful" << endl;
            isSuccessful = false;
        }
        else if (current->studentID > id){
            //our id is less than current, so go into left
            current->left = removeHelper(current->left, id, isSuccessful);       
            //adjusting height after it gets out of the recursion
            findHeight(current);     
        }
        //adding to the right, since passed id is more than current
        else if (current->studentID < id){
            //our id is more than current, so go into right
            current->right = removeHelper(current->right, id, isSuccessful);
            //adjusting height after it gets out of the recursion
            findHeight(current);
        }
        else{
            //we found the value! We must delete current!
            // cout << "successful" << endl;
            isSuccessful = true;
            --idCount;
            return deleteNode(current);
        }
        return current;
    }

    //remove inorder N
    void Avltree::removeInOrder(int &n){
        root = removeInOrderHelper(root, n);
        if (n > 0){
            //never got to the nth thing in list, was unsuccessful.
            cout << "unsuccessful" << endl; 
        }
        else{
            cout << "successful" << endl;
            --idCount;
        }

    }
    Avlnode* Avltree::removeInOrderHelper(Avlnode *current, int &n){
        if (current != nullptr){
            //left
            current->left = removeInOrderHelper(current->left, n);
            //node
            if (n == 0){
                //we got to the nth node!
                --n;
                //perform removal
                return deleteNode(current);
            }
            else if (n > 0){
                //decrease n, we haven't gotten there yet
                --n;
            }
            else{
                //if n is negative, we just want to go back up and return + change height
                findHeight(current);
                return current;
            }
            //right
            current->right = removeInOrderHelper(current->right, n);

        }

        return current;
        
    }

    //will delete the node ( a helper to both remove functions)
    Avlnode* Avltree::deleteNode(Avlnode *current){
        Avlnode* child = nullptr;

        if ((current->left == nullptr) && (current->right == nullptr)){
            //the current has no children, we can just-
            //delete the node
            delete current;
            return nullptr;
        }
        else if ((current->left == nullptr)){
            //has only a right child
            child = current->right;
            //delete the node
            delete current;

            //adjusting height
            findHeight(child);
            
            return child;
        }
        else if ((current->right == nullptr)){
            //has only a left child
            child = current->left;
            //delete the node
            delete current;

            //adjusting height
            findHeight(child);

            return child;
        }
        else{
            //has both left and right child
            //inorder succession -> right child's left child (right-left grandchild becomes new current)
            child = current->right;
            if (child->left == nullptr){
                child->left = current->left;
                //delete the node
                delete current;

                //adjusting height
                findHeight(child);

                return child;
            }
            else{
                //find the leftmost of child, and the node before
                Avlnode* leftmost = child->left;
                Avlnode* beforeLeft = child;
                //beforeleft could be the child, but it's not guarenteed

                while (leftmost->left != nullptr){
                    leftmost = leftmost->left;
                    beforeLeft = beforeLeft->left;
                }

                leftmost->left = current->left;

                if(leftmost->right != nullptr){
                    
                    beforeLeft->left = leftmost->right;
                    leftmost->right = child;

                    //adjusting height
                    if (beforeLeft->studentID != child->studentID){
                        findHeight(beforeLeft);
                        findHeight(child);
                    }
                    else{
                        findHeight(child);
                    }

                }
                else{
                    
                    child->left = nullptr;
                    leftmost->right = child;

                    //adjusting child height
                    child->maxHeightL = 0;
                    findHeight(child);
                }

                //adjusting leftmost's height
                findHeight(leftmost);

                //delete the node
                delete current;
                return leftmost;
            }
        }
    }

    //search ID
    string Avltree::searchID(int id){
        string response = "";
        searchIDHelper(root, id, response);
        return response;
    }
    void Avltree::searchIDHelper(Avlnode *current, int id, string &response){
        if (current == nullptr){
            //there's nothing in the list, or you've run out of list to go through. 
            // cout << "unsuccessful" << endl;
            response = "";
        }
        else if (current->studentID == id){
            //we found what we were looking for!
            // cout << current->studentName << endl;
            response = current->studentName;
        }
        //adding to the left, since passed id is less than current
        else if (current->studentID > id){
            searchIDHelper(current->left, id, response);
            
        }
        //adding to the right, since passed id is more than current
        else if (current->studentID < id){
            searchIDHelper(current->right, id, response);
        }

    }

    //search Name
    void Avltree::searchName(string name){
        //we will accumulate every name into a queue for us to print out later (or deem unsuccessful)
        queue<int> nameList;
        searchNameHelper(root, name, nameList);

        if (nameList.empty()){
            cout << "unsuccessful" << endl;
        }
        else{
            while (!nameList.empty()){
                cout << nameList.front() << endl;
                nameList.pop();
            }
        }
    }
    void Avltree::searchNameHelper(Avlnode *current, string &name , queue<int> &nameList){
        
        if (current != nullptr){
            //we must traverse the entire list to find each instance of name! Pre order traversal necessary
            //node
            if (current->studentName == name){
                //we found what we were looking for!
                nameList.push(current->studentID);
            }
            //left
            searchNameHelper(current->left, name, nameList);
            //right
            searchNameHelper(current->right, name, nameList);

        }

    }

    //print inorder
    void Avltree::printInOrder(){
        bool comma = false;
        printInOrderHelper(root, comma);
        cout << endl;
    }
    void Avltree::printInOrderHelper(Avlnode *current , bool &comma){

        if (current != nullptr){
            //left
            printInOrderHelper(current->left, comma);
            //node
            if (!comma){
                //if comma is true, we start adding commas after each node. 
                cout << current->studentName;
                comma = true;
            }
            else{
                cout << ", " << current->studentName;
            }
            //right
            printInOrderHelper(current->right, comma);

        }

    }

    //print preorder
    string Avltree::printPreOrder(){
        string response = "";
        bool comma = false;
        printPreOrderHelper(root, comma, response);
        // cout << endl;
        return response;
    }
    void Avltree::printPreOrderHelper(Avlnode *current, bool &comma, string &response){
        if (current != nullptr){
            //node
            if (!comma){
                //if comma is true, we start adding commas after each node. 
                response += current->studentName;
                comma = true;
            }
            else{
                response += ", " + current->studentName;
            }
            //left
            printPreOrderHelper(current->left, comma, response);
            //right
            printPreOrderHelper(current->right, comma, response);
        }
    }

    //print postorder
    void Avltree::printPostOrder(){
        bool comma = false;
        printPostOrderHelper(root, comma);
        cout << endl;
    }
    void Avltree::printPostOrderHelper(Avlnode *current, bool &comma){
        if (current != nullptr){
            //left
        printPostOrderHelper(current->left, comma);
        //right
        printPostOrderHelper(current->right, comma);
        //node
        if (!comma){
            //if comma is true, we start adding commas after each node. 
            cout << current->studentName;
            comma = true;
        }
        else{
            cout << ", " << current->studentName;
        }
        }
    }

    //print levelCount
    void Avltree::printLevelCount(){
        printLevelCountHelper(root);
    }
    void Avltree::printLevelCountHelper(Avlnode *current){

        if (current == nullptr){
            cout << "0" << endl;
        }
        else if ((current->maxHeightL) >= (current->maxHeightR)){
            cout << current->maxHeightL + 1 << endl;
        }
        else if ((current->maxHeightL) < (current->maxHeightR)){
            cout << current->maxHeightR + 1 << endl;
        }
    }

////***** OTHER HELPER FUNCTIONS

    //right rotation
    //code for right rotation is derived but slightly altered from the lecture notes. 
    Avlnode* Avltree::rightRotation(Avlnode *problem){

        Avlnode* child = problem->left;

        if (problem->left->right != nullptr){
            Avlnode* grandchild = problem->left->right;
            problem->left = grandchild;
        }
        else{
            problem->left = nullptr;
        }
        
        child->right = problem; 

        return child;    
    }

    //left rotation
    //code for left rotation is derived but slightly altered from the lecture notes. 
    Avlnode* Avltree::leftRotation(Avlnode *problem){

        Avlnode* child = problem->right;

        if (problem->right->left != nullptr){
            Avlnode* grandchild = problem->right->left;
            problem->right = grandchild;
        }
        else{
            problem->right = nullptr;
        }
        
        child->left = problem; 

        return child;  

    }

    void Avltree::findHeight(Avlnode *current){

        int right = 0;
        int left = 0;

        //we are changing the right max height of current
        if (current->right == nullptr){
            //there's no node on the right
            current->maxHeightR =  0;
        }
        else{
            //to know what to return, we compare the right child's left and right height

            right = current->right->maxHeightR;
            left = current->right->maxHeightL;

            if (right >= left){
                current->maxHeightR =  right + 1;
            }
            else if (left < right){
                current->maxHeightR =  left + 1;
            }
        }

        //we are changing the left max height of current
        if (current->left == nullptr){
            //there's no node on the right
            current->maxHeightL =  0;
        }
        else{
            // to know what to return, we compare the right child's left and right height

            right = current->left->maxHeightR;
            left = current->left->maxHeightL;

            if (right >= left){
                current->maxHeightL =  right + 1;
            }
            else if (left < right){
                current->maxHeightL =  left + 1;
            }
        }

    }

    //check and balance tree
    Avlnode* Avltree::balanceTree(Avlnode *problem){
        Avlnode* node = nullptr;

        if ((problem->maxHeightL - problem->maxHeightR) > 0){
            //for numbers like 2, we'll work with the left child
            if (problem->left != nullptr){
                if ((problem->left->maxHeightL - problem->left->maxHeightR) > 0){
                    // for a positive child height, you have left-left case.
                    // do right rotation

                    node = rightRotation(problem);

                    //readjusting the height
                    findHeight(node->right);
                    findHeight(node);

                    return node;
            }
                else{
                    //for a negative child height, you have left-right case.
                    //do left right rotation

                    problem->left = leftRotation(problem->left);
                    node = rightRotation(problem);

                    //readjusting the height
                    findHeight(node->left);
                    findHeight(node->right);
                    findHeight(node);

                    return node;
                
                }
            }
            
        }
        else if ((problem->maxHeightL - problem->maxHeightR) < 0){
            // for numbers like -2, we'll work with the right child
            if (problem->right != nullptr){

                if ((problem->right->maxHeightL - problem->right->maxHeightR) < 0){
                    //for a negative child height, you have right-right case.
                    //do left rotation

                    node = leftRotation(problem);

                    //readjusting the height
                    findHeight(node->left);
                    findHeight(node);

                    return node;
                
                }
                else{
                    // for a positive child height, you have right-left case.
                    // do right-left rotation

                    problem->right = rightRotation(problem->right);
                    node = leftRotation(problem);

                    //readjusting the height
                    findHeight(node->right);
                    findHeight(node->left);
                    findHeight(node);

                    return node;
                }
            }
        }

        return problem;

    }

////***** MAIN FUNCTIONS

    //check Name
    bool checkName (string &name){
        
        //before real check, check if name is complete
        string tempName = "";
        while (name[name.size()-1] != '"'){
            cin >> tempName;
            name.append(" " + tempName);
        }

        //for an accurate check, remove the quotations at the start and end
        name = name.substr(1, name.size()-1);
        name = name.substr(0, name.size()-1);

        //now, check if there are any numbers. If there are, I'm afraid it doesn't pass the vibe check
        for (int i = 0 ; i < name.size() ; i++){
            if (!isalpha(name[i])){
                if (name[i] != ' '){
                    return false;
                }
            } 
        }
        return true;
        
        
    }

    //check ID
    int checkID (string &id){

        //if the size of digits aint 8, it doesn't pass the vibe check
        if (id.size() != 8){
            return -1;
        }

        //checking each digit to see if it is a digit
        for (int i = 0 ; i < id.size() ; i++){
            if (!isdigit(id[i])){
                return -1;
            } 
        }
        //everything looks good, so convert the digit into int and pass it back
        return stoi(id);

    }


//Add your peer's code for the AVL Tree

class OrderedMap 
{
    private:
        //create an object of the AVL Tree which your peer implemented 
        // Avltree data;
        Avltree* data;

    public:
        OrderedMap();
        ~OrderedMap();
        bool insert(const std::string &ID, std::string &NAME);
        std::string search(const std::string &ID);
        std::string traverse();
        bool remove(const std::string ID);
};


OrderedMap::OrderedMap()
{
    data = new Avltree();
}

OrderedMap::~OrderedMap()
{
    delete data;
}

bool OrderedMap::insert(const std::string &ID, std::string &NAME)
{
    return data->insert(NAME, stoi(ID)); 
}

std::string OrderedMap::search(const std::string &ID)
{
    return data->searchID(stoi(ID));
}

std::string OrderedMap::traverse()
{
    return data->printPreOrder();
}

bool OrderedMap::remove(const std::string ID)
{
    return data->remove(stoi(ID));
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// FUNCTION TO COMPARE PERFORMANCE
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void compareOrderAndUnorder (const int numEntries) {

    UnorderedMap* unMap = new UnorderedMap(1, 0.75);
    OrderedMap* orMap = new OrderedMap;

    // ==================
    // INSERTING
    // ==================
    // UNORDERED MAP
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        (*unMap)[to_string(i)] = "test";
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<float> duration = end - start;
    cout << "Inserting " << numEntries << " items into Unordered Map took: " << duration.count() << " seconds" << endl;
    // ORDERED MAP
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        string test = "test";
        orMap->insert(to_string(i), test);
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "Inserting " << numEntries << " items into Ordered Map took: " << duration.count() << " seconds" << endl;

    // ==================
    // SEARCHING
    // ==================
    // UNORDERED MAP
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        (*unMap)[to_string(rand() % numEntries)];
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "Searching " << numEntries << " items in Unordered Map took: " << duration.count() << " seconds" << endl;

    // ORDERED MAP
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        orMap->search(to_string(rand() % numEntries));
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "Searching " << numEntries << " items in Ordered Map took: " << duration.count() << " seconds" << endl;

    // ==================
    // REMOVING
    // ==================
    // UNORDERED MAP
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        unMap->remove(to_string(rand() % numEntries));
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start; 
    cout << "Removing ~" << numEntries << " items in Unordered Map took: " << duration.count() << " seconds" << endl;

    // ORDERED MAP
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < numEntries; i++) {
        orMap->remove(to_string(rand() % numEntries));
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "Removing ~" << numEntries << " items in Ordered Map took: " << duration.count() << " seconds" << endl;

    // ==================
    // CLEANING
    // ==================
    delete unMap;
    delete orMap;
    cout << endl;
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// MAIN TO COMPARE PERFORMANCE
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
int main() {

    // compareOrderAndUnorder(1000);
    // compareOrderAndUnorder(10000);
    compareOrderAndUnorder(100000);
    compareOrderAndUnorder(200000);
    compareOrderAndUnorder(300000);
    compareOrderAndUnorder(400000);
    compareOrderAndUnorder(500000);

    return 0;
}

