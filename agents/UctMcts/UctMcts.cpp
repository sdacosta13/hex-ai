#include <tuple>
#include <limits>
#include <cmath>

class Node {
    public:
        tuple<int, int> coord; //coordinates of the node
        Node parent; //the nodes parent
        map<tuple<int, int>, Node> children; // children of this node
        int numOfVisits; //number of visits to this node
        int reward; //average reward from this position based on wins/losses
        int outcome; //Always None unless node is a leaf then represents winner (1 red wins, -1 blue wins)

        int getUctValue(float explore = 0.5) {
            if (numOfVisits == 0) {
                if (explore == 0) {
                    return 0;
                } else {
                    return std::numeric_limits<int>::max(); // return infinity
                }
            } else {
                return (reward/numOfVisits) + (explore * sqrt((2*log(parent.numOfVisits))/numOfVisits))
            }
        }

}
    

class UctMcst {
    public:
        gameState initialState;
        Node rootNode;
        
        void search(int timeBudget){
            time_t start = time(0);
            int timeLeft = timeBudget;

            while (timeLeft > 0) {
                auto [Node newNode, gameState newState] = selectBestNode();
                time_t end = time(0);
                time_t timeTaken = end-start;
                timeLeft = timeLeft - timeTaken;
            }
        }

        tuple<Node, gameState> selectBestNode(){

        }

}