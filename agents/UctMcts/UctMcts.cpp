#include <tuple>
#include <limits>
#include <cmath>
#include "Node.h"
#include "UctMcts.h"

int Node::getUctValue(float explore = 0.5) {
    if (this->numOfVisits == 0) {
        if (explore == 0) {
            return 0;
        } else {
            return std::numeric_limits<int>::max(); // return infinity
        }
    } else {
        return (this->reward/this->numOfVisits) + (explore * sqrt((2*log(this->parent.numOfVisits))/this->numOfVisits))
    }
};

void Node::addChildren(std::list<Node> children) {
    for (auto it = children.begin(); it != children.end(); ++it) {
        node.children[it->coord] = it;
    }
}

void UctMcts::search(int timeBudget) {
    time_t start = time(0);
    int timeLeft = timeBudget;

    while (timeLeft > 0) {
        auto [Node newNode, GameState newState] = this->selectBestNode();
        int toPlay = newState.turn();
        simulationWinner = this->simulate(newState);
        this->updateReward(newNode, toPlay, simulationWinner)
        time_t end = time(0);
        time_t timeTaken = end-start;
        timeLeft = timeLeft - timeTaken;
    }
};

tuple<Node, GameState> UctMcts::selectBestNode() {
    Node node = this->rootNode;
    GameState state = this->initialState;
    pair<int, int> entryWithMaxValue = make_pair(0, 0); //1:node coords, 2:node uct value
    for (int i=0; i<node.children.size()){
        map<std::tuple<int, int>, Node>::iterator currentEntry;
        for (currentEntry = node.children.begin(); currentEntry != node.children.end(); ++currentEntry) {
            if (currentEntry->second.getUctValue() > entryWithMaxValue.second) {
                entryWithMaxValue = make_pair(currentEntry->first, currentEntry->second.getUctValue());
                node = currentEntry->second
            }
        }
        state.play(entryWithMaxValue.first)
        if (node.numOfVisits == 0) {
            return node, state
        }
    }

    if (this->expand(node, state)){
        auto it = node.children.begin();
        std::advance(it, rand() % node.children.size());
        std::tuple<int, int> randomMove = it->first;
        state.play(randomMove)
    }
    return node, state
};

bool UctMcts::expand(Node parent, GameState state){
    std::list<Node> children;

    if (state.winner() != 0) {
        return false
    }

    std::vector<std::tuple<int, int>> moves = state.moves();
    for (std::tuple<int, int>::iterator it = moves.begin(); it != moves.end(); ++it) {
        Node newChild;
        newChild.coord = it
        newChild.parent = parent
        children.push_back(newChild)
    }

    parent.add_children(children);
    return true
}

int UctMcts::simulate(GameState state) {
    std::vector<std::tuple<int, int>> moves = state.moves();

    while (state.winner() == 0) {
        int random = rand() % moves.size()
        std::tuple<int, int> randomMove = moves[random];
        state.play(randomMove);
        moves.erase(random);
    }

    return state.winner()
}

void UctMcts::updateReward(Node node, int toPlay, int simulationWinner) {
    int reward = 1;
    if (simulationWinner == toPlay) {
        reward = 0;
    }

    while (node) {
        node.numOfVisits += 1;
        node.reward += reward;
        node = node.parent;
        if (reward == 1) {
            reward = 0;
        } else {
            reward = 1;
        }
    }
}