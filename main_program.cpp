#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <queue>
#include <algorithm>
using namespace std;

//red-black
enum Color {RED, BLACK};

struct Node {
    int price;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    string suburb;
    string address;
    int rooms;
    char type;
    int postcode;
    int bathrooms;
    int bedrooms;
    int land_size;
    int building_area;
    int year_built;
    string council_area;
    string region;
    int property_count;

    Node(int _data) {
        price = _data;
        color = RED;
        left = right = parent = nullptr;

        type = char();
        suburb = address = council_area = region = "";
        rooms = postcode = bathrooms = bedrooms = land_size = building_area = year_built = property_count = 0;
    }

    Node(
        string _suburb, string _address, int _rooms, char _type,
        int _price, int _postcode, int _bathrooms, int _bedrooms,
        int _land_size, int _building_area, int _year_built,
        string _council_area, string _region, int _property_count
    )
    {
        price = _price;
        color = RED;
        left = right = parent = nullptr;

        suburb = _suburb;
        address = _address;
        rooms = _rooms;
        type = _type;
        postcode = _postcode;
        bathrooms = _bathrooms;
        bedrooms = _bedrooms;
        land_size = _land_size;
        building_area = _building_area;
        year_built = _year_built;
        council_area = _council_area;
        region = _region;
        property_count = _property_count;
    }
};

// Query structure for user input
struct PropertyQuery {
    int rooms;
    char type;
    int price;
    int postcode;
    int bathrooms;
    int bedrooms;
    int land_size;
    int building_area;
    int year_built;
    string suburb;
    string council_area;
    string region;

    PropertyQuery() {
        rooms = bathrooms = bedrooms = 0;
        price = postcode = land_size = building_area = year_built = 0;
        type = '\0';
        suburb = council_area = region = "";
    }
};

class RedBlackTree {
private:
    Node* root;
    Node* NIL;

    void rotateLeft(Node* node) {
        Node* newParent = node->right;
        node->right = newParent->left;

        if (newParent->left != NIL) {
            newParent->left->parent = node;
        }

        newParent->parent = node->parent;

        if (node->parent == NIL) {
            root = newParent;
        }
        else if (node == node->parent->left) {
            node->parent->left = newParent;
        }
        else {
            node->parent->right = newParent;
        }

        newParent->left = node;
        node->parent = newParent;
    }

    void rotateRight(Node* node) {
        Node* newParent = node->left;
        node->left = newParent->right;

        if (newParent->right != NIL) {
            newParent->right->parent = node;
        }

        newParent->parent = node->parent;

        if (node->parent == NIL) {
            root = newParent;
        }
        else if (node == node->parent->right) {
            node->parent->right = newParent;
        }
        else {
            node->parent->left = newParent;
        }

        newParent->right = node;
        node->parent = newParent;
    }

    void fixInsert(Node* node) {
        while (node->parent->color == RED) {
            Node* uncle;
            if (node->parent == node->parent->parent->left) { // Which side of the grandparent are we on?
                uncle = node->parent->parent->right;

                if (uncle->color == RED) {                    // Uncle is Red, Recolor
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else {
                    // Uncle is Black, Rotations
                    if (node == node->parent->right) {        // Left-Right case
                        node = node->parent;
                        rotateLeft(node);
                    }

                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateRight(node->parent->parent);
                }
            }
            else {
                uncle = node->parent->parent->left;

                if (uncle->color == RED) {                    // Uncle is Red, Recolor
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else {
                    // Uncle is Black, Rotations
                    if (node == node->parent->left) {        // Right-Left Case
                        node = node->parent;
                        rotateRight(node);
                    }

                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateLeft(node->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void deleteTree(Node* node) {
        if (node == NIL || node == nullptr) {
            return;
        }
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    struct NodeScore {
        Node* node;
        int distance;

        NodeScore(Node* n, int d) {
            node = n;
            distance = d;
        }

        bool operator<(const NodeScore& other) const {
            return this->distance < other.distance;
        }

        bool operator>(const NodeScore& other) const {
            return this->distance > other.distance;
        }
    };

    void dfsKNearest(Node* node, int target, priority_queue<NodeScore>& maxHeap, int k) {
        if (node == NIL || node == nullptr) return;

        int distance = abs(node->price - target);

        if (maxHeap.size() < k) {
            maxHeap.emplace(node, distance);
        } else if (distance < maxHeap.top().distance) {
            maxHeap.pop();
            maxHeap.emplace(node, distance);
        }

        Node* first = nullptr;
        Node* second = nullptr;

        if (target < node->price) {
            first = node->left;
            second = node->right;
        } else {
            first = node->right;
            second = node->left;
        }

        dfsKNearest(first, target, maxHeap, k);

        if (maxHeap.size() < k || abs(node->price - target) < maxHeap.top().distance) {
            dfsKNearest(second, target, maxHeap, k);
        }
    }

public:
    RedBlackTree() {
        NIL = new Node(0);
        NIL->color = BLACK;
        NIL->left = NIL;
        NIL->right = NIL;
        NIL->parent = NIL;
        root = NIL;
    }
    ~RedBlackTree() {
        if (root != NIL)
            deleteTree(root);
        delete NIL;
    }

    // Main method for finding houses
    vector<Node*> findHouses(int price, int min_beds, int min_baths, int min_sqr_feet, int min_year) {
        priority_queue<NodeScore> myHeap;
        dfsKNearest(root, price, myHeap, 20);

        vector<Node*> vect1;
        while (!myHeap.empty()) {
            vect1.push_back(myHeap.top().node);
            myHeap.pop();
        }

        reverse(vect1.begin(), vect1.end());

        // Reusing NodeDistance but for minHeap to store house points
        priority_queue<NodeScore, vector<NodeScore>, greater<NodeScore>> minHeap;
        int bed_point, bath_point, sqr_feet_point, year_point;

        for (Node* i : vect1) {
            if (i->bedrooms >= min_beds) bed_point = 1; else bed_point = 0;
            if (i->bathrooms >= min_baths) bath_point = 1; else bath_point = 0;
            if (i->land_size >= min_sqr_feet) sqr_feet_point = 1; else sqr_feet_point = 0;
            if (i->year_built >= min_year) year_point = 1; else year_point = 0;

            int total = bed_point + bath_point + sqr_feet_point + year_point;

            // top 10
            if (minHeap.size() < 10) {
                minHeap.emplace(i, total);
            }
            else if (total > minHeap.top().distance) {
                minHeap.pop();
                minHeap.emplace(i, total);
            }
        }

        vect1.clear();
        while (!minHeap.empty()) {
            vect1.push_back(minHeap.top().node);
            minHeap.pop();
        }

        // vector has least-point nodes first, lets reverse this...
        reverse(vect1.begin(), vect1.end());

        return vect1;
    }

    void insert(Node* newNode) {
        newNode->color = RED;
        newNode->left = NIL;
        newNode->right = NIL;

        Node* parent = NIL;
        Node* current = root;

        while (current != NIL) {
            parent = current;
            if (newNode->price < current->price) {
                current = current->left;
            }
            else if (newNode->price > current->price) {
                current = current->right;
            }
        }

        newNode->parent = parent;

        if (parent == NIL) {
            root = newNode;
        }
        else if (newNode->price < parent->price) {
            parent->left = newNode;
        }
        else {
            parent->right = newNode;
        }

        fixInsert(newNode);
    }

    void displayInOrder(Node* node) {
        if (node == NIL) return;
        displayInOrder(node->left);
        cout << "$" << node->price << " (" << node->suburb << ") ";
        displayInOrder(node->right);
    }

    void displayAllPrices() {
        cout << "\nAll properties in order by price:\n";
        displayInOrder(root);
        cout << "\n";
    }

    void displayResults(const vector<Node*>& results, int targetPrice) {
        cout << "\n=== Top " << results.size() << " Properties Closest to $" << targetPrice << " ===\n\n";

        for (size_t i = 0; i < results.size(); i++) {
            Node* n = results[i];
            int priceDiff = n->price - targetPrice;
            cout << "Result #" << (i + 1) << ":\n";
            cout << "  Price: $" << n->price;
            if (priceDiff >= 0) {
                cout << " (+" << priceDiff << ")\n";
            } else {
                cout << " (" << priceDiff << ")\n";
            }
            cout << "  Suburb: " << n->suburb << "\n";
            cout << "  Address: " << n->address << "\n";
            cout << "  Type: " << n->type << "\n";
            cout << "  Rooms: " << n->rooms << ", Bedrooms: " << n->bedrooms
                 << ", Bathrooms: " << n->bathrooms << "\n";
            cout << "  Postcode: " << n->postcode << "\n";
            cout << "  Land Size: " << n->land_size << " sqm\n";
            cout << "  Building Area: " << n->building_area << " sqm\n";
            cout << "  Year Built: " << n->year_built << "\n";
            cout << "  Council: " << n->council_area << "\n";
            cout << "  Region: " << n->region << "\n";
            cout << "  ---\n\n";
        }
    }

    void VisualiseResults(vector<Node*>& results, int targetPrice) {

    cout << "\n=== Top " << results.size()<<"Homes Based on User Selections===\n\n";

    // Print table header
    cout << left << setw(6) << "Rank"
         << setw(8)  << "Rooms"
         << setw(12) << "Type"
         << setw(10) << "Price"
         << setw(11) << "Bedrooms"
         << setw(12) << "Bathrooms"
         << setw(11) << "LandSize"
         << setw(12) << "YearBuilt"
         << "Address" << endl;

    cout << string(100, '-') << endl;

    // Print each property in table rows
    for (size_t i = 0; i < results.size(); i++) {
        Node* n = results[i];
        int priceDiff = n->price - targetPrice;

        cout << left << setw(6)  << (i + 1)
             << setw(8)  << n->rooms
             << setw(12) << n->type
             << setw(10) << n->price
             << setw(11) << n->bedrooms
             << setw(12) << n->bathrooms
             << setw(11) << n->land_size
             << setw(12) << n->year_built
             << n->address
             << endl;
    }

}
};

//B-tree
struct Property {
    string suburb;
    string address;
    int rooms;
    char type;
    int price;
    int postcode;
    int bathrooms;
    int bedrooms;
    int land_size;
    int building_area;
    int year_built;
    string council_area;
    string region;
    int property_count;

    Property(string _suburb, string _address, int _rooms, char _type,
             int _price, int _postcode, int _bathrooms, int _bedrooms,
             int _land_size, int _building_area, int _year_built,
             string _council_area, string _region, int _property_count) {
        suburb = _suburb;
        address = _address;
        rooms = _rooms;
        type = _type;
        price = _price;
        postcode = _postcode;
        bathrooms = _bathrooms;
        bedrooms = _bedrooms;
        land_size = _land_size;
        building_area = _building_area;
        year_built = _year_built;
        council_area = _council_area;
        region = _region;
        property_count = _property_count;
    }
};

struct PropertyScore {
    Property* prop;
    int value;  // Price diff or feature score

    PropertyScore(Property* p, int v) {
        prop = p;
        value = v;
    }

    bool operator<(const PropertyScore& other) const {
        return this->value < other.value;
    }

    bool operator>(const PropertyScore& other) const {
        return this->value > other.value;
    }
};

class BTree {
private:

    class BTreeNode {
    public:
        Property** properties;
        int t;
        BTreeNode** children;
        int n;
        bool isLeaf;

        BTreeNode(int t, bool isLeaf) {
            this->t = t;
            this->isLeaf = isLeaf;
            properties = new Property*[2 * t - 1];
            children = new BTreeNode*[2 * t];
            n = 0;
        }

        ~BTreeNode() {
            delete[] properties;
            delete[] children;
        }

        void insertNonFull(Property* prop) {
            int i = n - 1;

            if (isLeaf) {
                while (i >= 0 && properties[i]->price > prop->price) {
                    properties[i + 1] = properties[i];
                    i--;
                }
                properties[i + 1] = prop;
                n++;
            } else {
                while (i >= 0 && properties[i]->price > prop->price)
                    i--;
                i++;

                if (children[i]->n == 2 * t - 1) {
                    splitChild(i, children[i]);
                    if (properties[i]->price < prop->price)
                        i++;
                }
                children[i]->insertNonFull(prop);
            }
        }

        void splitChild(int i, BTreeNode* child) {
            BTreeNode* newChild = new BTreeNode(child->t, child->isLeaf);
            newChild->n = t - 1;

            for (int j = 0; j < t - 1; j++)
                newChild->properties[j] = child->properties[j + t];

            if (!child->isLeaf) {
                for (int j = 0; j < t; j++)
                    newChild->children[j] = child->children[j + t];
            }

            child->n = t - 1;

            for (int j = n; j >= i + 1; j--)
                children[j + 1] = children[j];

            children[i + 1] = newChild;

            for (int j = n - 1; j >= i; j--)
                properties[j + 1] = properties[j];

            properties[i] = child->properties[t - 1];
            n++;
        }

        Property* search(int price) {
            int i = 0;
            while (i < n && price > properties[i]->price)
                i++;

            if (i < n && properties[i]->price == price)
                return properties[i];

            if (isLeaf)
                return nullptr;

            return children[i]->search(price);
        }
    };

    BTreeNode* root;
    int t;

    // DFS with pruning for B-Tree
    void dfsKNearest(BTreeNode* node, int targetPrice, priority_queue<PropertyScore>& maxHeap, int k) {
        if (!node) return;

        for (int i = 0; i < node->n; i++) {
            Property* prop = node->properties[i];
            int distance = abs(prop->price - targetPrice);

            if (maxHeap.size() < k) {
                maxHeap.emplace(prop, distance);
            } else if (distance < maxHeap.top().value) {
                maxHeap.pop();
                maxHeap.emplace(prop, distance);
            }
        }

        if (node->isLeaf) return;

        // Find which child range the target falls into
        int i = 0;
        while (i < node->n && targetPrice > node->properties[i]->price)
            i++;

        if (i >= 0 && i <= node->n && node->children[i])
            dfsKNearest(node->children[i], targetPrice, maxHeap, k);

        // Check left-side children
        for (int j = i - 1; j >= 0; j--) {
            int boundaryDist = abs(node->properties[j]->price - targetPrice);
            if (maxHeap.size() < k || boundaryDist < maxHeap.top().value) {
                if (node->children[j])
                    dfsKNearest(node->children[j], targetPrice, maxHeap, k);
            } else break;
        }

        // Check right-side children
        for (int j = i + 1; j <= node->n; j++) {
            int boundaryDist = abs(node->properties[j - 1]->price - targetPrice);
            if (maxHeap.size() < k || boundaryDist < maxHeap.top().value) {
                if (node->children[j])
                    dfsKNearest(node->children[j], targetPrice, maxHeap, k);
            } else break;
        }
    }

    void deleteTree(BTreeNode* node) {
        if (!node) return;

        if (!node->isLeaf) {
            for (int i = 0; i <= node->n; i++) {
                deleteTree(node->children[i]);
            }
        }

        for (int i = 0; i < node->n; i++) {
            delete node->properties[i];
            node->properties[i] = nullptr;
        }

        delete node;
    }

public:
    BTree(int t = 3) {
        root = nullptr;
        this->t = t;
    }

    ~BTree() {
        if (root) {
            deleteTree(root);
        }
    }

    void insert(Property* prop) {
        if (!root) {
            root = new BTreeNode(t, true);
            root->properties[0] = prop;
            root->n = 1;
            return;
        }

        if (root->n == 2 * t - 1) {
            BTreeNode* newRoot = new BTreeNode(t, false);
            newRoot->children[0] = root;
            newRoot->splitChild(0, root);

            int i = 0;
            if (newRoot->properties[0]->price < prop->price)
                i++;
            newRoot->children[i]->insertNonFull(prop);

            root = newRoot;
        } else {
            root->insertNonFull(prop);
        }
    }

    Property* search(int price) {
        return (root == nullptr) ? nullptr : root->search(price);
    }

    // Red-Black Equivalent
    vector<Property*> findHouses(int price, int min_beds, int min_baths, int min_sqr_feet, int min_year) {

        priority_queue<PropertyScore> priceHeap;
        dfsKNearest(root, price, priceHeap, 20);

        priority_queue<PropertyScore, vector<PropertyScore>, greater<PropertyScore>> pointHeap;
        int bed_point, bath_point, sqr_feet_point, year_point;

        while (!priceHeap.empty()) {
            Property* house = priceHeap.top().prop;
            priceHeap.pop();

            if (house->bedrooms >= min_beds) bed_point = 1; else bed_point = 0;
            if (house->bathrooms >= min_baths) bath_point = 1; else bath_point = 0;
            if (house->land_size >= min_sqr_feet) sqr_feet_point = 1; else sqr_feet_point = 0;
            if (house->year_built >= min_year) year_point = 1; else year_point = 0;

            int total = bed_point + bath_point + sqr_feet_point + year_point;

            // Keep top 10 by feature score
            if (pointHeap.size() < 10) {
                pointHeap.emplace(house, total);
            }
            else if (total > pointHeap.top().value) {
                pointHeap.pop();
                pointHeap.emplace(house, total);
            }
        }

        // Extract results (reverse to get highest scores first)
        vector<Property*> result;
        while (!pointHeap.empty()) {
            result.push_back(pointHeap.top().prop);
            pointHeap.pop();
        }

        reverse(result.begin(), result.end());
        return result;
    }

    // Display results
    void displayResults(const vector<Property*>& results, int targetPrice,
                       int min_beds, int min_baths, int min_sqr_feet, int min_year) {
        cout << "\n=== Top " << results.size() << " Properties ===\n";
        cout << "Target Price: $" << targetPrice << "\n";
        cout << "Min Bedrooms: " << min_beds << ", Min Bathrooms: " << min_baths << "\n";
        cout << "Min Land Size: " << min_sqr_feet << " sqm, Min Year: " << min_year << "\n\n";

        for (size_t i = 0; i < results.size(); i++) {
            Property* p = results[i];

            // Calculate scores
            int priceDiff = p->price - targetPrice;

            cout << "Result #" << (i + 1) << "\n";
            cout << "  Price: $" << p->price;
            if (priceDiff >= 0)
                cout << " (+" << priceDiff << ")\n";
            else
                cout << " (" << priceDiff << ")\n";

            cout << "  Suburb: " << p->suburb << "\n";
            cout << "  Address: " << p->address << "\n";
            cout << "  Type: " << p->type << "\n";
            cout << "  Rooms: " << p->rooms << ", Bedrooms: " << p->bedrooms
                 << ", Bathrooms: " << p->bathrooms << "\n";
            cout << "  Postcode: " << p->postcode << "\n";
            cout << "  Land Size: " << p->land_size << " sqm\n";
            cout << "  Building Area: " << p->building_area << " sqm\n";
            cout << "  Year Built: " << p->year_built << "\n";
            cout << "  Council: " << p->council_area << "\n";
            cout << "  Region: " << p->region << "\n";
            cout << "  ---\n\n";
        }
    }

    // Display tree structure
    void displayTree(BTreeNode* node, int level = 0) {
        if (!node) return;

        cout << "Level " << level << ": [";
        for (int i = 0; i < node->n; i++) {
            cout << "$" << node->properties[i]->price;
            if (i < node->n - 1) cout << ", ";
        }
        cout << "]\n";

        if (!node->isLeaf) {
            for (int i = 0; i <= node->n; i++) {
                displayTree(node->children[i], level + 1);
            }
        }
    }

    void displayTreeStructure() {
        cout << "\n=== B-Tree Structure ===\n";
        if (!root) {
            cout << "Empty tree\n";
        } else {
            displayTree(root);
        }
    }

    void VisualiseResults(const vector<Property*>& results) {


        cout << "\n=== Top " << results.size()<<"Homes Based on User Selections===\n\n";

        // Print table header
        cout << left << setw(6) << "Rank"
             << setw(8)  << "Rooms"
             << setw(12) << "Type"
             << setw(10) << "Price"
             << setw(11) << "Bedrooms"
             << setw(12) << "Bathrooms"
             << setw(11) << "LandSize"
             << setw(12) << "YearBuilt"
             << "Address" << endl;

        cout << string(100, '-') << endl;

        // Print each property in table rows
        for (size_t i = 0; i < results.size(); i++) {
            Property* n = results[i];

            cout << left << setw(6)  << (i + 1)
                 << setw(8)  << n->rooms
                 << setw(12) << n->type
                 << setw(10) << n->price
                 << setw(11) << n->bedrooms
                 << setw(12) << n->bathrooms
                 << setw(11) << n->land_size
                 << setw(12) << n->year_built
                 << n->address
                 << endl;
        }
    }
};

void generateDataFromCSV(RedBlackTree &tree, BTree &bt, const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string suburb, address, type, council_area, region;
        string s_rooms, s_price, s_postcode, s_bedrooms, s_bathrooms, s_land, s_building, s_year, s_count;

        getline(ss, suburb, ',');
        getline(ss, address, ',');
        getline(ss, s_rooms, ',');
        getline(ss, type, ',');
        getline(ss, s_price, ',');
        getline(ss, s_postcode, ',');
        getline(ss, s_bedrooms, ',');
        getline(ss, s_bathrooms, ',');
        getline(ss, s_land, ',');
        getline(ss, s_building, ',');
        getline(ss, s_year, ',');
        getline(ss, council_area, ',');
        getline(ss, region, ',');
        getline(ss, s_count, ',');

        // Convert string → numeric
        char typi = type[0];
        int rooms = stoi(s_rooms);
        int price = stoi(s_price);
        int postcode = stoi(s_postcode);
        int bedrooms = stoi(s_bedrooms);
        int bathrooms = stoi(s_bathrooms);
        int land_size = stoi(s_land);
        int building_area = stoi(s_building);
        int year_built = stoi(s_year);
        int property_count = stoi(s_count);

        Node *node = new Node(suburb, address, rooms, typi, price, postcode, bathrooms, bedrooms,
                              land_size, building_area, year_built, council_area, region, property_count);
        Property *property = new Property(suburb, address, rooms, typi, price, postcode, bathrooms, bedrooms,
                              land_size, building_area, year_built, council_area, region, property_count);
        tree.insert(node);
        bt.insert(property);
    }

    file.close();
}

void printMenu() {
    cout << "\n******** Welcome, let's Find you a Home? *********\n";
    cout <<"1. Begin Search\n";
    cout <<"2. View Insertion time for 10 houses\n";
    cout <<"0. Exit\n";
    cout <<"Select an option :";
}

// ------------------------------------------------------
// Main Program
// ------------------------------------------------------
int main() {

    RedBlackTree RDB;
    BTree BT;
    generateDataFromCSV(RDB,BT, "C:/Users/wmark/Documents/Fall 2025/COP3530 - DSA/Project 2/CPP Programs/cleaned_pt2.csv");

    vector<Node*> RDBhousevec;
    vector<Property*> BTreehousevec;
    int setprice;
    int choice;
    bool statec = true;
    bool statesearch = false;

    cout << "----------------------------------------------\n";
    cout << " Housing Searching Project by the Connectors\n";
    cout << "Disclaimer: Trust this program at your own risk :)\n";

    while (statec) {

        printMenu();
        cin >> choice;

        // Validate input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        else if (choice == 1) {

        int price, minbed, minbath, minsize, minyear;
        cout <<"Enter price: $";
        cin>>price;
        setprice = price;
        cout <<"Enter min Number of bedrooms :";
        cin>>minbed;
        cout <<"Enter min Number of bathrooms :";
        cin>>minbath;
        cout <<"Enter min Sqr Feet :";
        cin>>minsize;
        cout <<"Enter Year-Built:";

        cin>>minyear;
        statesearch = true;

            auto start_rbt = chrono::high_resolution_clock::now();
            RDBhousevec = RDB.findHouses(price, minbed, minbath, minsize, minyear);
            auto end_rbt = chrono::high_resolution_clock::now();
            double rbt_search_time = chrono::duration<double, milli>(end_rbt - start_rbt).count();

            auto start_bt = chrono::high_resolution_clock::now();
            BTreehousevec = BT.findHouses(price, minbed, minbath, minsize, minyear);
            auto end_bt = chrono::high_resolution_clock::now();
            double bt_search_time = chrono::duration<double, milli>(end_bt - start_bt).count();

            RDB.VisualiseResults(RDBhousevec, price);
            cout << "\n Search Time for Red-Black Tree : " << rbt_search_time << " ms\n\n";

            cout <<"-----------------------------------------------\n\n";

            BT.VisualiseResults(BTreehousevec);
            cout << "\n Search Time for B Tree : " << bt_search_time << " ms\n";

            }

        else if (choice == 2) {
            if (!statesearch) {
                cout<<"To continue please, Perform the search Operation from option 1";
            }else {
                cout<<"------- Insertion Timing  Test--------\n";

                RedBlackTree testRBT;
                BTree testb;
                auto start_rbt = chrono::high_resolution_clock::now();
                for (auto& h : RDBhousevec) {
                    testRBT.insert(h);
                }

                auto end_rbt = chrono::high_resolution_clock::now();
                double rbt_insert_time = chrono::duration<double, milli>(end_rbt - start_rbt).count();

                auto start_bt = chrono::high_resolution_clock::now();
                for (auto& h : BTreehousevec) {
                    testb.insert(h);
                }

                auto end_bt = chrono::high_resolution_clock::now();
                double bt_insert_time = chrono::duration<double, milli>(end_bt - start_bt).count();

                cout << "Insertion Time for 10 records\n";
                cout << " - B-Tree: " << bt_insert_time<<" ms\n";
                cout << " - Red-Black Tree : " << rbt_insert_time << " ms\n";
            }
        }

        else if (choice == 0) {
            cout << "Exiting program. Goodbye!\n";
            statec = false;

        }
        else{
            cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}