//
// Created by wmark on 10/30/2025.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <limits>
using namespace std;

// Property data structure
struct Property {
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

    Property() {
        type = char();
        suburb = address = council_area = region = "";
        id = rooms = price = postcode = bathrooms = bedrooms = 0;
        land_size = building_area = year_built = property_count = 0;
    }

    Property(int _id, string _suburb, string _address, int _rooms, char _type,
             int _price, int _postcode, int _bathrooms, int _bedrooms,
             int _land_size, int _building_area, int _year_built,
             string _council_area, string _region, int _property_count) {
        id = _id;
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

// Structure to hold a property with its distance
struct PropertyDistance {
    Property* prop;
    double distance;

    PropertyDistance(Property* p, double d) : prop(p), distance(d) {}

    bool operator<(const PropertyDistance& other) const {
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

// B-Tree Node
class BTreeNode {
public:
    Property** properties;    // Array of properties
    int t;                    // Minimum degree
    BTreeNode** children;     // Array of child pointers
    int n;                    // Current number of properties
    bool isLeaf;              // True if leaf node

    BTreeNode(int t, bool isLeaf);
    ~BTreeNode();

    void insertNonFull(Property* prop);
    void splitChild(int i, BTreeNode* child);
    Property* search(int id);
};

BTreeNode::BTreeNode(int t, bool isLeaf) {
    this->t = t;
    this->isLeaf = isLeaf;

    properties = new Property*[2 * t - 1];
    children = new BTreeNode*[2 * t];

    n = 0;
}

BTreeNode::~BTreeNode() {
    delete[] properties;
    delete[] children;
}

void BTreeNode::insertNonFull(Property* prop) {
    int i = n - 1;

    if (isLeaf) {
        while (i >= 0 && properties[i]->id > prop->id) {
            properties[i + 1] = properties[i];
            i--;
        }

        properties[i + 1] = prop;
        n++;
    } else {
        while (i >= 0 && properties[i]->id > prop->id)
            i--;
        i++;

        if (children[i]->n == 2 * t - 1) {
            splitChild(i, children[i]);

            if (properties[i]->id < prop->id)
                i++;
        }
        children[i]->insertNonFull(prop);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* child) {
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

Property* BTreeNode::search(int id) {
    int i = 0;
    while (i < n && id > properties[i]->id)
        i++;

    if (i < n && properties[i]->id == id)
        return properties[i];

    if (isLeaf)
        return nullptr;

    return children[i]->search(id);
}

// B-Tree Class
class BTree {
private:
    BTreeNode* root;
    int t;

    // Statistics for normalization
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

    Stats stats;

    // Helper: Collect all properties via BFS
    void collectProperties(vector<Property*>& props) {
        if (!root) return;

        vector<BTreeNode*> queue;
        queue.push_back(root);
        int front = 0;

        while (front < queue.size()) {
            BTreeNode* node = queue[front++];

            // Collect all properties from this node
            for (int i = 0; i < node->n; i++) {
                props.push_back(node->properties[i]);
            }

            // Add children to queue if not leaf
            if (!node->isLeaf) {
                for (int i = 0; i <= node->n; i++) {
                    queue.push_back(node->children[i]);
                }
            }
        }
    }

    // Helper: Calculate statistics using BFS
    void calculateStats() {
        if (!root) return;

        stats = Stats();

        vector<BTreeNode*> queue;
        queue.push_back(root);
        int front = 0;

        while (front < queue.size()) {
            BTreeNode* node = queue[front++];

            // Update stats for all properties in this node
            for (int i = 0; i < node->n; i++) {
                Property* prop = node->properties[i];

                stats.price_max = max(stats.price_max, (double)prop->price);
                stats.price_min = min(stats.price_min, (double)prop->price);

                stats.rooms_max = max(stats.rooms_max, (double)prop->rooms);
                stats.rooms_min = min(stats.rooms_min, (double)prop->rooms);

                stats.postcode_max = max(stats.postcode_max, (double)prop->postcode);
                stats.postcode_min = min(stats.postcode_min, (double)prop->postcode);

                stats.bathrooms_max = max(stats.bathrooms_max, (double)prop->bathrooms);
                stats.bathrooms_min = min(stats.bathrooms_min, (double)prop->bathrooms);

                stats.bedrooms_max = max(stats.bedrooms_max, (double)prop->bedrooms);
                stats.bedrooms_min = min(stats.bedrooms_min, (double)prop->bedrooms);

                stats.land_size_max = max(stats.land_size_max, (double)prop->land_size);
                stats.land_size_min = min(stats.land_size_min, (double)prop->land_size);

                stats.building_area_max = max(stats.building_area_max, (double)prop->building_area);
                stats.building_area_min = min(stats.building_area_min, (double)prop->building_area);

                stats.year_built_max = max(stats.year_built_max, (double)prop->year_built);
                stats.year_built_min = min(stats.year_built_min, (double)prop->year_built);
            }

            // Add children to queue
            if (!node->isLeaf) {
                for (int i = 0; i <= node->n; i++) {
                    queue.push_back(node->children[i]);
                }
            }
        }
    }

    // Normalize a value to [0, 1] range
    double normalize(double value, double min_val, double max_val) {
        if (max_val == min_val) return 0.0;
        return (value - min_val) / (max_val - min_val);
    }

    // String similarity
    double stringSimilarity(const string& s1, const string& s2) {
        if (s1.empty() && s2.empty()) return 0.0;
        if (s1.empty() || s2.empty()) return 1.0;

        string lower1 = s1, lower2 = s2;
        transform(lower1.begin(), lower1.end(), lower1.begin(), ::tolower);
        transform(lower2.begin(), lower2.end(), lower2.begin(), ::tolower);

        return (lower1 == lower2) ? 0.0 : 1.0;
    }

    // Calculate weighted distance
    double calculateDistance(const PropertyQuery& query, Property* prop) {
        double distance = 0.0;

        // Numeric features (normalized and weighted)
        if (query.price > 0) {
            double norm_query = normalize(query.price, stats.price_min, stats.price_max);
            double norm_prop = normalize(prop->price, stats.price_min, stats.price_max);
            distance += 5.0 * pow(norm_query - norm_prop, 2);
        }

        if (query.rooms > 0) {
            double norm_query = normalize(query.rooms, stats.rooms_min, stats.rooms_max);
            double norm_prop = normalize(prop->rooms, stats.rooms_min, stats.rooms_max);
            distance += 3.0 * pow(norm_query - norm_prop, 2);
        }

        if (query.bedrooms > 0) {
            double norm_query = normalize(query.bedrooms, stats.bedrooms_min, stats.bedrooms_max);
            double norm_prop = normalize(prop->bedrooms, stats.bedrooms_min, stats.bedrooms_max);
            distance += 4.0 * pow(norm_query - norm_prop, 2);
        }

        if (query.bathrooms > 0) {
            double norm_query = normalize(query.bathrooms, stats.bathrooms_min, stats.bathrooms_max);
            double norm_prop = normalize(prop->bathrooms, stats.bathrooms_min, stats.bathrooms_max);
            distance += 2.0 * pow(norm_query - norm_prop, 2);
        }

        if (query.postcode > 0) {
            double norm_query = normalize(query.postcode, stats.postcode_min, stats.postcode_max);
            double norm_prop = normalize(prop->postcode, stats.postcode_min, stats.postcode_max);
            distance += 3.0 * pow(norm_query - norm_prop, 2);
        }

        if (query.land_size > 0) {
            double norm_query = normalize(query.land_size, stats.land_size_min, stats.land_size_max);
            double norm_prop = normalize(prop->land_size, stats.land_size_min, stats.land_size_max);
            distance += 2.0 * pow(norm_query - norm_prop, 2);
        }

        if (query.building_area > 0) {
            double norm_query = normalize(query.building_area, stats.building_area_min, stats.building_area_max);
            double norm_prop = normalize(prop->building_area, stats.building_area_min, stats.building_area_max);
            distance += 2.0 * pow(norm_query - norm_prop, 2);
        }

        if (query.year_built > 0) {
            double norm_query = normalize(query.year_built, stats.year_built_min, stats.year_built_max);
            double norm_prop = normalize(prop->year_built, stats.year_built_min, stats.year_built_max);
            distance += 1.5 * pow(norm_query - norm_prop, 2);
        }

        // Categorical features
        if (query.type != '\0') {
            distance += (query.type == prop->type) ? 0.0 : 3.0;
        }

        // String features
        if (!query.suburb.empty()) {
            distance += 4.0 * stringSimilarity(query.suburb, prop->suburb);
        }

        if (!query.council_area.empty()) {
            distance += 2.0 * stringSimilarity(query.council_area, prop->council_area);
        }

        if (!query.region.empty()) {
            distance += 2.0 * stringSimilarity(query.region, prop->region);
        }

        return sqrt(distance);
    }

    void deleteTree(BTreeNode* node) {
        if (!node) return;

        if (!node->isLeaf) {
            for (int i = 0; i <= node->n; i++) {
                deleteTree(node->children[i]);
            }
        }

        // Delete properties in this node
        for (int i = 0; i < node->n; i++) {
            delete node->properties[i];
        }

        delete node;
    }

public:
    BTree(int t) {
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
            if (newRoot->properties[0]->id < prop->id)
                i++;
            newRoot->children[i]->insertNonFull(prop);

            root = newRoot;
        } else {
            root->insertNonFull(prop);
        }
    }

    Property* search(int id) {
        return (root == nullptr) ? nullptr : root->search(id);
    }

    // K-Nearest Neighbors search using BFS
    vector<Property*> findKNearest(const PropertyQuery& query, int k = 10) {
        // Step 1: Calculate statistics for normalization
        calculateStats();

        // Step 2: Collect all properties using BFS
        vector<Property*> allProps;
        collectProperties(allProps);

        // Step 3: Calculate distances for all properties
        vector<PropertyDistance> distances;
        for (Property* prop : allProps) {
            double dist = calculateDistance(query, prop);
            distances.push_back(PropertyDistance(prop, dist));
        }

        // Step 4: Sort by distance
        sort(distances.begin(), distances.end());

        // Step 5: Return top k (We return top 10 houses)
        vector<Property*> result;
        int count = min(k, (int)distances.size());
        for (int i = 0; i < count; i++) {
            result.push_back(distances[i].prop);
        }

        return result;
    }

    // Display search results
    void displayResults(const vector<Property*>& results) {
        cout << "\n=== Top " << results.size() << " Matching Properties ===\n\n";

        for (size_t i = 0; i < results.size(); i++) {
            Property* p = results[i];
            cout << "Result #" << (i + 1) << ":\n";
            cout << "  ID: " << p->id << "\n";
            cout << "  Suburb: " << p->suburb << "\n";
            cout << "  Address: " << p->address << "\n";
            cout << "  Type: " << p->type << "\n";
            cout << "  Rooms: " << p->rooms << ", Bedrooms: " << p->bedrooms
                 << ", Bathrooms: " << p->bathrooms << "\n";
            cout << "  Price: $" << p->price << "\n";
            cout << "  Postcode: " << p->postcode << "\n";
            cout << "  Land Size: " << p->land_size << " sqm\n";
            cout << "  Building Area: " << p->building_area << " sqm\n";
            cout << "  Year Built: " << p->year_built << "\n";
            cout << "  Council: " << p->council_area << "\n";
            cout << "  Region: " << p->region << "\n";
            cout << "  ---\n\n";
        }
    }

    // Display tree structure (level-order using BFS)
    void displayTree() {
        if (!root) {
            cout << "Tree is empty\n";
            return;
        }

        cout << "\n=== B-Tree Structure (BFS Level Order) ===\n";

        vector<BTreeNode*> queue;
        queue.push_back(root);
        int front = 0;
        int level = 0;

        while (front < queue.size()) {
            int levelSize = queue.size() - front;
            cout << "\nLevel " << level << ":\n";

            for (int i = 0; i < levelSize; i++) {
                BTreeNode* node = queue[front++];

                cout << "  [";
                for (int j = 0; j < node->n; j++) {
                    cout << node->properties[j]->id;
                    if (j < node->n - 1) cout << ", ";
                }
                cout << "]";

                if (!node->isLeaf) {
                    for (int j = 0; j <= node->n; j++) {
                        queue.push_back(node->children[j]);
                    }
                }

                if (i < levelSize - 1) cout << "  ";
            }
            cout << "\n";
            level++;
        }
    }
};

// Example usage
int main() {
    BTree tree(3);  // Minimum degree t = 3 (each node has 2-5 keys)

    // Insert sample properties
    tree.insert(new Property(1, "Richmond", "123 Church St", 3, 'h', 850000, 3121,
                            2, 3, 400, 150, 1920, "Yarra", "Melbourne", 100));
    tree.insert(new Property(2, "Carlton", "45 Lygon St", 2, 'u', 650000, 3053,
                            1, 2, 0, 80, 2010, "Melbourne", "Melbourne", 150));
    tree.insert(new Property(3, "Hawthorn", "78 Glenferrie Rd", 4, 'h', 1200000, 3122,
                            2, 4, 600, 200, 1930, "Boroondara", "Melbourne", 120));
    tree.insert(new Property(4, "Richmond", "90 Bridge Rd", 2, 't', 720000, 3121,
                            1, 2, 150, 100, 2015, "Yarra", "Melbourne", 100));
    tree.insert(new Property(5, "Carlton", "12 Elgin St", 3, 'h', 950000, 3053,
                            2, 3, 350, 140, 1925, "Melbourne", "Melbourne", 150));
    tree.insert(new Property(6, "Fitzroy", "34 Brunswick St", 2, 'u', 700000, 3065,
                            1, 2, 0, 90, 2012, "Yarra", "Melbourne", 100));
    tree.insert(new Property(7, "Kew", "56 High St", 5, 'h', 1500000, 3101,
                            3, 5, 800, 250, 1925, "Boroondara", "Melbourne", 120));
    tree.insert(new Property(656383600,"Abbotsford","68 Studley St",2,'h',1143384,3067,2,1,126,431,1946,"Yarra City Council","Northern Metropolitan",4019));
    tree.insert(new Property(223093719,"Abbotsford","85 Turner St",2,'h',1480000,3067,2,1,202,431,1946,"Yarra City Council","Northern Metropolitan",4019));
    tree.insert(new Property(998104759,"Abbotsford","25 Bloomburg St",2,'h',1035000,3067,2,1,156,79,1900,"Yarra City Council","Northern Metropolitan",4019));
    tree.insert(new Property(409803011,"Abbotsford","18/659 Victoria St",3,'u',1143384,3067,3,2,482,431,1946,"Yarra City Council","Northern Metropolitan",4019));

    // Display tree structure
    tree.displayTree();

    // Create a query
    PropertyQuery query;
    query.suburb = "Richmond";
    query.type = 'h';
    query.bedrooms = 3;
    query.bathrooms = 2;
    query.price = 800000;
    query.postcode = 3121;

    cout << "\n\nSearching for properties similar to:\n";
    cout << "  Suburb: " << query.suburb << "\n";
    cout << "  Type: " << query.type << "\n";
    cout << "  Bedrooms: " << query.bedrooms << "\n";
    cout << "  Bathrooms: " << query.bathrooms << "\n";
    cout << "  Price: $" << query.price << "\n";
    cout << "  Postcode: " << query.postcode << "\n";

    // Find 10 nearest properties
    vector<Property*> results = tree.findKNearest(query, 10);

    // Display results
    tree.displayResults(results);

    return 0;
}