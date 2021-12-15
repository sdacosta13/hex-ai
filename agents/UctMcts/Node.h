#include <tuple>
#include <list>

class Node {
    public:
        std::tuple<int, int> coord; //coordinates of the node
        Node* parent; //the nodes parent
        std::list<Node> children; // children of this node
        int numOfVisits; //number of visits to this node
        int reward; //average reward from this position based on wins/losses
        int outcome; //Always None unless node is a leaf then represents winner (1 red wins, -1 blue wins)
        int getUctValue(float explore);
};