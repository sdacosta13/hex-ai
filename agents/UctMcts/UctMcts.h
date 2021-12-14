#include <tuple>
#include <map>
#include "Node.h"
#include "GameState.h"

class UctMcst {
    public:
        gameState initialState;
        Node rootNode;
        void search(int timeBudget);
}