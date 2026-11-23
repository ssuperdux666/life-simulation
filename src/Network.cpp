#include "../include/Network.h"
#include "../include/game.h"
#include "../include/creature.h"
#include <iostream>
#include <algorithm>

NeuralNetwork::NeuralNetwork(int inputs, int outputs) {
    globalID = 0;
    hiddenlayers = 0;

    for (int i = 0; i < inputs; i++) {
        Create_Node(0, 0, i);
    }

    for (int i = 0; i < outputs; i++) {
        Create_Node(0, 0, 0);
    }
}

void NeuralNetwork::Create_Node(int layer, int type, int inputType) {
    Node thisNode = {globalID, layer, type, inputType};

    if (type == 0) {
        nodesInput.push_back(thisNode);
    }else if (type == 1) {
        nodesHidden.push_back(thisNode);
    }else {
        nodesOutput.push_back(thisNode);
    }

    nodeValues.push_back(0.0f);

    globalID++;
}
float NeuralNetwork::Get_Input_Value(int x, Creature& creature, Game& game) {

    float value;

    Food nearest = game.Get_Nearest_Food(creature.x, creature.y);

    switch (x) {
        case 0:  value = 1; break;
        case 1:  value = std::pow((-creature.hunger/creature.maxHunger)+1,1.2); break;
        case 2:  value = creature.health/(float)creature.maxHealth; break;
        case 3:  value = hypot(nearest.x - creature.x, nearest.y - creature.y) / 1200.0f; break;
        case 4:  value = cos(atan2(nearest.y - creature.y, nearest.x - creature.x) - creature.angle*(PI/180)); break;
        default: value = 0; break;
    }

    return value;
}

void NeuralNetwork::Fill_Inputs(Game& game, Creature& creature) {
    for (Node& currentNode : nodesInput) {
        nodeValues[currentNode.id] = Get_Input_Value(currentNode.inputType, creature, game);
    }
}

void NeuralNetwork::Clear_Nodes() {
    for (Node& currentNode : nodesInput)
        nodeValues[currentNode.id] = 0;

    for (Node& currentNode : nodesHidden)
        nodeValues[currentNode.id] = 0;

    for (Node& currentNode : nodesOutput)
        nodeValues[currentNode.id] = 0;
}

float NeuralNetwork::Get_Node_Value(int id) {

    if (id < 0 || id >= nodeValues.size()) {
        return 0;
    }

    return nodeValues[id];
}

float sigmoidMODDED(float x) {
    return (((1/(1+exp(-x))) * 2) - 1);
}

void NeuralNetwork::Create_Random_Connecttion() {
    std::vector<Node> possibleInputs;
    std::vector<Node> possibleOutputs;

    for (Node currentNode : nodesInput)
        possibleInputs.push_back(currentNode);

    for (Node currentNode : nodesHidden)
        possibleInputs.push_back(currentNode);

    for (Node currentNode : nodesHidden)
        possibleOutputs.push_back(currentNode);

    for (Node currentNode : nodesOutput)
        possibleOutputs.push_back(currentNode);

    if (possibleInputs.empty() || possibleOutputs.empty())
        return;

    for (int attempt = 0; attempt < 100; attempt++) {
        Node* inNode = &possibleInputs[GetRandomValue(0,possibleInputs.size()-1)];
        Node* outNode = &possibleOutputs[GetRandomValue(0,possibleOutputs.size()-1)];

        if (inNode->layer >= outNode->layer && !(inNode->type == 0 && outNode->type == 2))
            continue;

        bool exists = false;

        for (Connection currentConnection : Connections) {
            if (currentConnection.in == inNode->id && currentConnection.out == outNode->id) {
                exists = true;
                break;
            }
        }

        if (exists)
            continue;

        Connection newConnection;

        newConnection.enabled = true;
        newConnection.in = inNode->id;
        newConnection.out = outNode->id;
        newConnection.weight = GetRandomValue(-1000,1000) / 1000.0f;

        int index = Connections.size();

        Connections.push_back(newConnection);

        incomingConnections[newConnection.out].push_back(index);

        break;
    }
}


void NeuralNetwork::Rebuild_Execution_Order() {

    executionOrder.clear();

    executionOrder.reserve(nodesHidden.size());

    for (int i = 0; i < nodesHidden.size(); i++) {
        executionOrder.push_back(i);
    }

    std::sort(executionOrder.begin(), executionOrder.end(), [this](int a, int b) {return nodesHidden[a].layer < nodesHidden[b].layer;});
}

void NeuralNetwork::Forward_Pass(Game& game, Creature& creature) {

    Clear_Nodes();
    Fill_Inputs(game, creature);

    for (int nodeIndex : executionOrder) {

        Node& node = nodesHidden[nodeIndex];

        float sum = 0.0f;

        for (int connectionIndex : incomingConnections[node.id]) {
            const Connection& connection = Connections[connectionIndex];

            if (!connection.enabled)
                continue;

            sum += Get_Node_Value(Connections[connectionIndex].in) * Connections[connectionIndex].weight;
        }

        nodeValues[node.id] = sigmoidMODDED(sum);
    }

    for (Node& node : nodesOutput) {

        float sum = 0;

        for (int connectionIndex : incomingConnections[node.id]) {

            const Connection& connection = Connections[connectionIndex];

            if (!connection.enabled)
                continue;

            if (connection.out == node.id) {

                float input = Get_Node_Value(connection.in);

                sum += input * connection.weight;
            }
        }

        nodeValues[node.id] = sigmoidMODDED(sum);
    }

    realOutputs.clear();

    for (Node node : nodesOutput) {
        realOutputs.push_back(nodeValues[node.id]);
    }
}