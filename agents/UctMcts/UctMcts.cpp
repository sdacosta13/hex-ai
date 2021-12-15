#include "UctMcts.h"
#include <limits>
#include <algorithm>
#include <map>
#include <cmath>

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

    while (timeLeft > 0) {
        Node newNode;
        GameState newState;
        std::tie(newNode, newState) = this->selectNode();
        int toPlay = newState.turn();
        int simulationWinner = this->simulate(newState);
        this->updateReward(&newNode, toPlay, simulationWinner);
        time_t end = time(0);
        time_t timeTaken = end-start;
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
    std::vector<std::tuple<int, int>> moves = state.moves();

    while (state.winner() == 0) {
        int random = rand() % moves.size();
        std::tuple<int, int> randomMove = moves[random];
        state.play(randomMove);
        moves.erase(moves.begin() + random);
    }

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
