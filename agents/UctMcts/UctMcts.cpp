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
        std::tie(newNode, newState) = this->selectNode();
        std::cout << "3\n";
        int toPlay = newState.turn();
        std::cout << "4\n";
        int simulationWinner = this->simulate(newState);
        std::cout << "5\n";
        this->updateReward(&newNode, toPlay, simulationWinner);
        std::cout << "6\n";
        time_t end = time(0);
        std::cout << "7\n";
        time_t timeTaken = end-start;
        std::cout << "8\n";
        timeLeft = timeLeft - timeTaken;
    };
};

std::tuple<Node, GameState> UctMcts::selectNode() {
    Node node = this->rootNode;
    GameState state = this->initialState;

    for (int i=0; i<node.children.size(); i++){
        int bestUctValue = 0;
        for (auto it = node.children.begin(); it != node.children.end(); it++) {
            int newUctValue = it->getUctValue();
            if (newUctValue > bestUctValue) {
                bestUctValue = newUctValue;
                node = *it;
            };
        };

        state.play(node.coord);
        if (node.numOfVisits == 0) {
            return std::make_tuple(node, state);
        };
    }

    if (this->expand(&node, state)){
        auto it = node.children.begin();
        std::advance(it, rand() % node.children.size());
        state.play(it->coord);
    };
    return std::make_tuple(node, state);
};

bool UctMcts::expand(Node* parent, GameState state){
    std::list<Node> children;

    if (state.winner() != 0) {
        return false;
    };

    std::vector<std::tuple<int, int>> moves = state.moves();
    for (int i=0; i < moves.size(); i++) {
        Node newChild;
        newChild.coord = moves[i];
        newChild.parent = parent;
        children.push_back(newChild);
    }

    parent->children = children;
    return true;
};

int UctMcts::simulate(GameState state) {
    std::cout << "s1\n";
    std::vector<std::tuple<int, int>> moves = state.moves();
    std::cout << "s2\n";
    while (state.winner() == 0) {
        int random = rand() % moves.size();
        std::tuple<int, int> randomMove = moves[random];
        state.play(randomMove);
        std::swap(moves[random], moves.back());
        moves.pop_back();
        return 0;
    };
    std::cout << "s5\n";
    return state.winner();
};

void UctMcts::updateReward(Node* node, int toPlay, int simulationWinner) {
    int reward = 1;
    if (simulationWinner == toPlay) {
        reward = 0;
    }
    do {
        node->numOfVisits += 1;
        node->reward += reward;
        node = node->parent;
        if (reward == 1) {
            reward = 0;
        } else {
            reward = 1;
        }
    } while (node->parent);
};

std::tuple<int, int> UctMcts::getBestMove() {
    std::list<Node> children = this->rootNode.children;
    return (*std::max_element(children.begin(), children.end(),
        []( const Node &a, const Node &b )
        {
            return a.numOfVisits < b.numOfVisits;
        } )).coord;
};
