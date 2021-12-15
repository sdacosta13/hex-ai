#include "UctMcts.h"
#include <limits>
#include <algorithm>
#include <map>
#include <cmath>
#include <iostream>

int Node::getUctValue(float explore = 0.5) {
    if (this->numOfVisits == 0) {
        if (explore == 0) {
            return 0;
        } else {
            return std::numeric_limits<int>::max(); // return infinity
        };
    } else {
        return (this->reward/this->numOfVisits) + (explore * sqrt((2*log(this->parent->numOfVisits))/this->numOfVisits));
    };
};

void UctMcts::search(float timeBudget) {
    time_t start = time(0);
    float timeLeft = timeBudget;
    std::cout << "1\n";
    while (timeLeft > 0) {
        Node newNode;
        GameState newState;
        std::cout << "2\n";
        std::tie(newNode, newState) = selectNode();
        std::cout << "3\n";
        int toPlay = newState.turn();
        std::cout << "4\n";
        int simulationWinner = simulate(newState);
        std::cout << "5\n";
        updateReward(&newNode, toPlay, simulationWinner);
        time_t end = time(0);
        time_t timeTaken = end-start;
        timeLeft = timeLeft - timeTaken;
    };
};

std::tuple<Node, GameState> UctMcts::selectNode() {
    std::cout << "sn1\n";
    Node node = this->rootNode;
    GameState state = this->initialState;

    //stop if we reach leaf node
    std::cout << "sn2\n";
    while (node.children.size() != 0){
        //descend to the node with the highest UCT value
        int bestUctValue = 0;
        for (int i = 0; i < node.children.size(); i++){
            Node child = *(std::next(node.children.begin(), i));
            int newUctValue = child.getUctValue();
            if (newUctValue > bestUctValue) {
                bestUctValue = newUctValue;
                node = child;
            };
        }

        //if best child of node has not been explored, explore before expanding
        state.play(node.coord);
        if (node.numOfVisits == 0) {
            return std::make_tuple(node, state);
        };
    }
    std::cout << "sn3\n";

    //if we reach a leaf node, generate and return one of its children
    if (state.winner() == 0) {
        node.children = expand(node, state);
        std::cout << "sn4\n";
        Node ranChild = *(std::next(node.children.begin(), rand() % node.children.size()));
        std::cout << "sn5\n";
        state.play(ranChild.coord);
    }

    std::cout << "sn6\n";
    //if we reach a terminal node, return it
    return std::make_tuple(node, state);
};

std::list<Node> UctMcts::expand(Node parent, GameState state){
    std::cout << "e1\n";
    std::list<Node> children;

    std::cout << "e2\n";
    std::vector<std::tuple<int, int>> moves = state.moves();
    for (int i=0; i < moves.size(); i++) {
        Node newChild;
        newChild.coord = moves[i];
        newChild.parent = &parent;
        children.push_back(newChild);
    }
    std::cout << "e3\n";

    return children;
};

int UctMcts::simulate(GameState state) {
    std::cout << "s1\n";
    std::vector<std::tuple<int, int>> moves = state.moves();
    std::cout << "s2\n";
    //while (state.winner() == 0) {
    while (moves.size() != 0) {
        //std::cout << "s3\n";
        int random = rand() % moves.size();
        std::tuple<int, int> randomMove = moves[random];
        state.play(randomMove);
        //std::cout << std::get<0>(randomMove) << " " << std::get<1>(randomMove) << "\n";
        std::swap(moves[random], moves.back());
        moves.pop_back();
        //std::cout << state.winner() << "\n";
    };
    std::cout << "s4\n";
    return state.winner();
};

void UctMcts::updateReward(Node* node, int toPlay, int simulationWinner) {
    int reward = 1;
    if (simulationWinner == toPlay) {
        reward = 0;
    }
    while (node->parent != 0) {
        node->numOfVisits += 1;
        node->reward += reward;
        node = node->parent;
        if (reward == 1) {
            reward = 0;
        } else {
            reward = 1;
        }
    }
};

std::tuple<int, int> UctMcts::getBestMove() {
    std::list<Node> children = this->rootNode.children;
    Node node;
    int bestNumOfVisits = 0;
    for (int i = 0; i < children.size(); i++){
        Node child = *(std::next(children.begin(), i));
        int newNumOfVisits = child.numOfVisits;
        if (newNumOfVisits > bestNumOfVisits) {
            bestNumOfVisits = newNumOfVisits;
            node = child;
        };
    }
    return node.coord;
    // return (*std::max_element(children.begin(), children.end(),
    //     []( const Node &a, const Node &b )
    //     {
    //         return a.numOfVisits < b.numOfVisits;
    //     } )).coord;
};
