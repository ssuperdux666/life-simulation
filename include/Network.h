#pragma once
#include <vector>
#include <cmath>

class Game;
class Creature;

struct Node {
    int id;
    int layer;
    int type;
    int inputType;
};

struct Connection {
    int in;
    int out;
    float weight;
    bool enabled;
};

class Creature;

class NeuralNetwork {
    public:
        int globalID;

        int hiddenlayers;

        NeuralNetwork(int inputs, int outputs);

        void Create_Node(int layer, int type, int inputType);

        std::vector<float> realOutputs; 

        float Get_Input_Value(int x, Creature& creature, Game& game);

        void Fill_Inputs(Game& game, Creature& creature);

        void Clear_Nodes();

        float Get_Node_Value(int id);

        void Create_Random_Connecttion();

        void Rebuild_Execution_Order();

        void Forward_Pass(Game& game, Creature& creature);

    private:
        std::vector<Node> nodesInput;
        std::vector<Node> nodesHidden;
        std::vector<Node> nodesOutput;

        std::vector<float> nodeValues;

        std::vector<int> executionOrder;

        std::vector<std::vector<int>> incomingConnections;

        std::vector<Connection> Connections;
};