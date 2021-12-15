#include <tuple>
#include "Node.h"
#include "GameState.h"

class UctMcts {
    public:
        GameState initialState;
        Node rootNode;
        void search(float timeBudget);
        std::tuple<Node, GameState> selectNode();
        bool expand(Node* parent, GameState state);
        int simulate(GameState state);
        void updateReward(Node node, int toPlay, int simulationWinner);
        std::tuple<int, int> getBestMove();
        UctMcts(GameState initialStateIn) {
            initialState = initialStateIn;
        };
};