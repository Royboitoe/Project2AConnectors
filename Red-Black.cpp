#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <chrono>
using namespace std;

enum Color {RED, BLACK};

struct Node {
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    int id;
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

    Node(int _data) {
        data = _data;
        color = RED;
        left = right = parent = nullptr;

        type = char();
        suburb = address = council_area = region = "";
        id = rooms = price = postcode = bathrooms = bedrooms = land_size = building_area = year_built = property_count = 0;
    }

    Node(
        int _id, string _suburb, string _address, int _rooms, char _type,
        int _price, int _postcode, int _bathrooms, int _bedrooms,
        int _land_size, int _building_area, int _year_built,
        string _council_area, string _region, int _property_count
    ) {
        data = _id;
        color = RED;
        left = right = parent = nullptr;

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

struct NodeDistance {
    Node* node;
    double distance;

    NodeDistance(Node* n, double d) : node(n), distance(d) {}

    // For sorting by distance
    bool operator<(const NodeDistance& other) const {
        return distance < other.distance;
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

    struct Stats {
        double price_max, price_min;
        double rooms_max, rooms_min;
        double postcode_max, postcode_min;
        double bathrooms_max, bathrooms_min;
        double bedrooms_max, bedrooms_min;
        double land_size_max, land_size_min;
        double building_area_max, building_area_min;
        double year_built_max, year_built_min;

        Stats() {
            price_max = rooms_max = postcode_max = bathrooms_max = 0;
            bedrooms_max = land_size_max = building_area_max = year_built_max = 0;
            price_min = rooms_min = postcode_min = bathrooms_min = numeric_limits<double>::max();
            bedrooms_min = land_size_min = building_area_min = year_built_min = numeric_limits<double>::max();
        }
    };

    void rotateLeft(Node* node) {
        Node* newParent = node->right;
        node->right = newParent->left;
        // Node* grandchild = node->right->left;

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
        // Node* grandchild = node->right->left;

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
        if (node == NIL) {
            return;
        }
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    void collectNodes(vector<Node*>& nodes) {
        if (root == NIL) {
            return;
        }

        vector<Node*> queue;
        queue.push_back(root);
        int front = 0;

        while (front < queue.size()) {
            Node* current = queue[front++];
            nodes.push_back(current);

            if (current->left != NIL) {
                queue.push_back(current->left);
            }
            if (current->right != NIL) {
                queue.push_back(current->right);
            }
        }
    }

    Stats stats;

    // Helper: Calculate statistics for normalization
    void calculateStats() {
        if (root == NIL) return;

        // Reset stats
        stats = Stats();

        vector<Node*> queue;
        queue.push_back(root);
        int front = 0;

        while (front < queue.size()) {
            Node* node = queue[front++];

            // Update max/min for each numeric field
            stats.price_max = max(stats.price_max, (double)node->price);
            stats.price_min = min(stats.price_min, (double)node->price);

            stats.rooms_max = max(stats.rooms_max, (double)node->rooms);
            stats.rooms_min = min(stats.rooms_min, (double)node->rooms);

            stats.postcode_max = max(stats.postcode_max, (double)node->postcode);
            stats.postcode_min = min(stats.postcode_min, (double)node->postcode);

            stats.bathrooms_max = max(stats.bathrooms_max, (double)node->bathrooms);
            stats.bathrooms_min = min(stats.bathrooms_min, (double)node->bathrooms);

            stats.bedrooms_max = max(stats.bedrooms_max, (double)node->bedrooms);
            stats.bedrooms_min = min(stats.bedrooms_min, (double)node->bedrooms);

            stats.land_size_max = max(stats.land_size_max, (double)node->land_size);
            stats.land_size_min = min(stats.land_size_min, (double)node->land_size);

            stats.building_area_max = max(stats.building_area_max, (double)node->building_area);
            stats.building_area_min = min(stats.building_area_min, (double)node->building_area);

            stats.year_built_max = max(stats.year_built_max, (double)node->year_built);
            stats.year_built_min = min(stats.year_built_min, (double)node->year_built);

            if (node->left != NIL) {
                queue.push_back(node->left);
            }
            if (node->right != NIL) {
                queue.push_back(node->right);
            }
        }
    }

    // Normalize a value to [0, 1] range
    double normalize(double value, double min_val, double max_val) {
        if (max_val == min_val) return 0.0;
        return (value - min_val) / (max_val - min_val);
    }

    // String similarity (simple: 0 if different, 1 if same, case-insensitive)
    double stringSimilarity(const string& s1, const string& s2) {
        if (s1.empty() && s2.empty()) return 0.0;  // Both empty, no penalty
        if (s1.empty() || s2.empty()) return 1.0;  // One empty, max distance

        string lower1 = s1, lower2 = s2;
        transform(lower1.begin(), lower1.end(), lower1.begin(), ::tolower);
        transform(lower2.begin(), lower2.end(), lower2.begin(), ::tolower);

        return (lower1 == lower2) ? 0.0 : 1.0;
    }

    // Calculate weighted Euclidean distance between query and node
    double calculateDistance(const PropertyQuery& query, Node* node) {
        double distance = 0.0;

        // Numeric features (normalized and weighted)
        if (query.price > 0) {
            double norm_query = normalize(query.price, stats.price_min, stats.price_max);
            double norm_node = normalize(node->price, stats.price_min, stats.price_max);
            distance += 5.0 * pow(norm_query - norm_node, 2);  // Price heavily weighted
        }

        if (query.rooms > 0) {
            double norm_query = normalize(query.rooms, stats.rooms_min, stats.rooms_max);
            double norm_node = normalize(node->rooms, stats.rooms_min, stats.rooms_max);
            distance += 3.0 * pow(norm_query - norm_node, 2);
        }

        if (query.bedrooms > 0) {
            double norm_query = normalize(query.bedrooms, stats.bedrooms_min, stats.bedrooms_max);
            double norm_node = normalize(node->bedrooms, stats.bedrooms_min, stats.bedrooms_max);
            distance += 4.0 * pow(norm_query - norm_node, 2);  // Bedrooms important
        }

        if (query.bathrooms > 0) {
            double norm_query = normalize(query.bathrooms, stats.bathrooms_min, stats.bathrooms_max);
            double norm_node = normalize(node->bathrooms, stats.bathrooms_min, stats.bathrooms_max);
            distance += 2.0 * pow(norm_query - norm_node, 2);
        }

        if (query.postcode > 0) {
            double norm_query = normalize(query.postcode, stats.postcode_min, stats.postcode_max);
            double norm_node = normalize(node->postcode, stats.postcode_min, stats.postcode_max);
            distance += 3.0 * pow(norm_query - norm_node, 2);
        }

        if (query.land_size > 0) {
            double norm_query = normalize(query.land_size, stats.land_size_min, stats.land_size_max);
            double norm_node = normalize(node->land_size, stats.land_size_min, stats.land_size_max);
            distance += 2.0 * pow(norm_query - norm_node, 2);
        }

        if (query.building_area > 0) {
            double norm_query = normalize(query.building_area, stats.building_area_min, stats.building_area_max);
            double norm_node = normalize(node->building_area, stats.building_area_min, stats.building_area_max);
            distance += 2.0 * pow(norm_query - norm_node, 2);
        }

        if (query.year_built > 0) {
            double norm_query = normalize(query.year_built, stats.year_built_min, stats.year_built_max);
            double norm_node = normalize(node->year_built, stats.year_built_min, stats.year_built_max);
            distance += 1.5 * pow(norm_query - norm_node, 2);
        }

        // Categorical features
        if (query.type != '\0') {
            distance += (query.type == node->type) ? 0.0 : 3.0;  // Type mismatch penalty
        }

        // String features
        if (!query.suburb.empty()) {
            distance += 4.0 * stringSimilarity(query.suburb, node->suburb);
        }

        if (!query.council_area.empty()) {
            distance += 2.0 * stringSimilarity(query.council_area, node->council_area);
        }

        if (!query.region.empty()) {
            distance += 2.0 * stringSimilarity(query.region, node->region);
        }

        return sqrt(distance);
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
        deleteTree(root);
        delete NIL;
    }

    void insert(Node* newNode) {
        newNode->color = RED;
        newNode->left = NIL;
        newNode->right = NIL;

        Node* parent = NIL;
        Node* current = root;

        while (current != NIL) {
            parent = current;
            if (newNode->data < current->data) {
                current = current->left;
            }
            else if (newNode->data > current->data) {
                current = current->right;
            }
            else {
                return;  // Duplicate
            }
        }

        newNode->parent = parent;

        if (parent == NIL) {
            root = newNode;
        }
        else if (newNode->data < parent->data) {
            parent->left = newNode;
        }
        else {
            parent->right = newNode;
        }

        fixInsert(newNode);
    }

    vector<Node*> findKNearest(const PropertyQuery& query, int k = 10) {
        // Step 1: Calculate statistics for normalization
        calculateStats();

        // Step 2: Collect all nodes using BFS
        vector<Node*> allNodes;
        collectNodes(allNodes);

        // Step 3: Calculate distances for all nodes
        vector<NodeDistance> distances;
        for (Node* node : allNodes) {
            double dist = calculateDistance(query, node);
            distances.push_back(NodeDistance(node, dist));
        }

        // Step 4: Sort by distance
        sort(distances.begin(), distances.end());

        // Step 5: Return top k
        vector<Node*> result;
        int count = min(k, (int)distances.size());
        for (int i = 0; i < count; i++) {
            result.push_back(distances[i].node);
        }

        return result;
    }

    void displayResults(const vector<Node*>& results) {
        cout << "\n=== Top " << results.size() << " Matching Properties ===\n\n";

        for (size_t i = 0; i < results.size(); i++) {
            Node* n = results[i];
            cout << "Result #" << (i + 1) << ":\n";
            cout << "  ID: " << n->id << "\n";
            cout << "  Suburb: " << n->suburb << "\n";
            cout << "  Address: " << n->address << "\n";
            cout << "  Type: " << n->type << "\n";
            cout << "  Rooms: " << n->rooms << ", Bedrooms: " << n->bedrooms
                 << ", Bathrooms: " << n->bathrooms << "\n";
            cout << "  Price: $" << n->price << "\n";
            cout << "  Postcode: " << n->postcode << "\n";
            cout << "  Land Size: " << n->land_size << " sqm\n";
            cout << "  Building Area: " << n->building_area << " sqm\n";
            cout << "  Year Built: " << n->year_built << "\n";
            cout << "  Council: " << n->council_area << "\n";
            cout << "  Region: " << n->region << "\n";
            cout << "  ---\n\n";
        }
    }
};
int main() {
RedBlackTree tree;
  
ifstream file("cleaned.csv");
string line;
getline(file, line);
while(getline (file,line)){
    stringstream ss(line);
    string id, suburb, address, rooms, type, price, postcode, bathrooms, bedrooms, land, building, year, council, region, prop_count;
    getline(ss, id, ',');
    getline(ss, suburb, ',');
    getline(ss, address, ',');
    getline(ss, rooms, ',');
    getline(ss, type, ',');
    getline(ss, price, ',');
    getline(ss, postcode, ',');
    getline(ss, bathrooms, ',');
    getline(ss, bedrooms, ',');
    getline(ss, land, ',');
    getline(ss, building, ',');
    getline(ss, year, ',');
    getline(ss, council, ',');
    getline(ss, region, ',');
    getline(ss, prop_count, ',');

    Node* n = new Node(
        stoi(id), suburb, address stoi(rooms), type.empty() ? 'h' : type[0], 
        stoi(price), stoi(postcode), stoi(bathrooms), stoi(bedrooms), 
        stoi(land), stoi(building), 
        stoi(year), council, region, stoi(prop_count)); 
    tree.insert(n);
}
file.close();

    




    // Insert some sample properties
    tree.insert(new Node(1, "Richmond", "123 Church St", 3, 'h', 850000, 3121,
                        2, 3, 400, 150, 1920, "Yarra", "Melbourne", 100));
    tree.insert(new Node(2, "Carlton", "45 Lygon St", 2, 'u', 650000, 3053,
                        1, 2, 0, 80, 2010, "Melbourne", "Melbourne", 150));
    tree.insert(new Node(3, "Hawthorn", "78 Glenferrie Rd", 4, 'h', 1200000, 3122,
                        2, 4, 600, 200, 1930, "Boroondara", "Melbourne", 120));
    tree.insert(new Node(4, "Richmond", "90 Bridge Rd", 2, 't', 720000, 3121,
                        1, 2, 150, 100, 2015, "Yarra", "Melbourne", 100));
    tree.insert(new Node(5, "Carlton", "12 Elgin St", 3, 'h', 950000, 3053,
                        2, 3, 350, 140, 1925, "Melbourne", "Melbourne", 150));

    // Create a query
    PropertyQuery query;
    query.suburb = "Richmond";
    query.type = 'h';
    query.bedrooms = 3;
    query.price = 800000;
    query.postcode = 3121;

    cout << "Searching for properties similar to:\n";
    cout << "  Suburb: " << query.suburb << "\n";
    cout << "  Type: " << query.type << "\n";
    cout << "  Bedrooms: " << query.bedrooms << "\n";
    cout << "  Price: $" << query.price << "\n";
    cout << "  Postcode: " << query.postcode << "\n";

    // Find 10 nearest properties
 
    auto start = chrono::high_resolution_clock::now();
    vector<Node*> results = tree.findKNearest(query, 10);
    auto end = chrono::high_resolution_clock::now();
    
    double elapsed = chrono::duration<double, milli> (end - start).count();
    cout << "Search time: " << elapsed << " ms/n";


    // Display results
    tree.displayResults(results);

    return 0;

}


