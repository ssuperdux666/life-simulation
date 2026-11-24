#include "../include/Network.h"
#include "../include/game.h"
#include "../include/creature.h"
#include <iostream>
#include <algorithm>
#include <cassert>

NeuralNetwork::NeuralNetwork(int inputs, int outputs) {
    globalID = 0;
    hiddenlayers = 0;

    for (int i = 0; i < inputs; i++) {
        Create_Node(0, 0, i);
    }

    for (int i = 0; i < outputs; i++) {
        Create_Node(0, 2, 0);
    }
}

void NeuralNetwork::Create_Node(int layer, int type, int inputType) {//gwn aanmaken weetjee?
    Node thisNode = {globalID, layer, type, inputType};

    if (type == 0) {//dit moet eig een switch statement zijn maar ik gun mezelf voor nu python heeft ze nieteens dus hou op
        nodesInput.push_back(thisNode);
    }else if (type == 1) {
        nodesHidden.push_back(thisNode);
    }else {
        nodesOutput.push_back(thisNode);
    }

    nodeValues.push_back(0.0f);
    incomingConnections.emplace_back();//dit moet omdat iedere node ID een incommign connection is verwaccht die is hier nu niet zonder die

    globalID++;
}
float NeuralNetwork::Get_Input_Value(int x, Creature& creature, Game& game) {// lijkt op een bottleneck maar zonder geeft echt maar + 5 fps wat wel goeie is maar hij is nodig

    float value;

    Food* nearest;

    if (x == 3 || x == 4) {
        nearest = game.Get_Nearest_Food(creature.x, creature.y);

        if (nearest == nullptr) {
            return 0.0f;
        }

        creature.nearestFood = nearest;
        creature.nearestDistance = hypot(nearest->x - creature.x, nearest->y - creature.y);
    }
    

    switch (x) {
        case 0:  value = 1; break;//constand
        case 1:  value = std::pow((-creature.hunger/creature.maxHunger)+1,1.2); break;//hunger
        case 2:  value = creature.health/(float)creature.maxHealth; break;//health
        case 3:  value = creature.nearestDistance / 1200.0f; break;//nearest ditance to food
        case 4:  value = cos(atan2(nearest->y - creature.y, nearest->x - creature.x) - creature.angle*(PI/180)); break;//cosine angle to nearest food
        default: value = 0; break;
    }

    return value;
}

void NeuralNetwork::Fill_Inputs(Game& game, Creature& creature) {//helper functie ja!
    for (Node& currentNode : nodesInput) {
        nodeValues[currentNode.id] = Get_Input_Value(currentNode.inputType, creature, game);
    }
}

//hier is het graf van get_node_value en clear_nodes functions, rip.. maar nu ze weg zijn is het +0.000265fps per entity... worth it

float sigmoidMODDED(float x) {// wie houd er niet van mods?
    return (((1/(1+exp(-x))) * 2) - 1);
}

float Get_Random_Weight_Value() {//ik vraag me af of een langere functie naam meer fps eet
    return GetRandomValue(-1000,1000) / 1000.0f;
}

void NeuralNetwork::Change_Random_Weight() { //aan de top van de normaal distributie! want hij komt het meeste voor... ha. ha.
    if (Connections.empty())
        return;

    int index = GetRandomValue(0, Connections.size() - 1);

    if (GetRandomValue(0,5) == 3) {//3 erg of mnu?
        Connections[index].weight = Get_Random_Weight_Value();
    }else {
        float finalValue = std::clamp(GetRandomValue(-5, 5) / 10.0f + Connections[index].weight, -5.0f, 5.0f);
        Connections[index].weight = finalValue;
    }
}

void NeuralNetwork::Enabdisab_Random_Connection() {//dit is een woord trust
    if (Connections.empty())
        return;

    int index = GetRandomValue(0, Connections.size() - 1);

    Connections[index].enabled = !Connections[index].enabled;//soo easy eindelijk
}

void NeuralNetwork::Create_Random_Connecttion() {//deze wilt niet geoptimized worden als ik probeer bijt hij en de naam is fout maar het boeit me niet 
    std::vector<Node> possibleInputs;
    std::vector<Node> possibleOutputs;

    for (Node& currentNode : nodesInput)
        possibleInputs.push_back(currentNode);

    for (Node& currentNode : nodesHidden)
        possibleInputs.push_back(currentNode);

    for (Node& currentNode : nodesHidden)
        possibleOutputs.push_back(currentNode);

    for (Node& currentNode : nodesOutput)
        possibleOutputs.push_back(currentNode);

    if (possibleInputs.empty() || possibleOutputs.empty())
        return;

    for (int attempt = 0; attempt < 100; attempt++) {
        Node* inNode = &possibleInputs[GetRandomValue(0,possibleInputs.size()-1)];
        Node* outNode = &possibleOutputs[GetRandomValue(0,possibleOutputs.size()-1)];

        if (inNode->layer >= outNode->layer && !(inNode->type == 0 && outNode->type == 2))
            continue;

        bool exists = false;

        for (Connection& currentConnection : Connections) {
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
        newConnection.weight = Get_Random_Weight_Value();

        int index = Connections.size();

        Connections.push_back(newConnection);

        incomingConnections[newConnection.out].push_back(index);

        break;
    }
}

void NeuralNetwork::Add_Hidden_Node() {//hij is verstopt... nee hij is daarzo ->                                                                                                                                                                                              O
    int maxLayer = GetRandomValue(0,hiddenlayers);

    Create_Node(maxLayer, 1, 0);//hidden layer is 1 input is 0 output is 2 btw
}//dit was eig zo makkelijk ik dacht hell


void NeuralNetwork::Rebuild_Execution_Order() {

    executionOrder.clear();

    executionOrder.reserve(nodesHidden.size());

    for (int i = 0; i < nodesHidden.size(); i++) {
        executionOrder.push_back(i);
    }

    std::sort(executionOrder.begin(), executionOrder.end(), [this](int a, int b) {return nodesHidden[a].layer < nodesHidden[b].layer;});
}

void NeuralNetwork::Mutate() {//D value staat voor distribution value
    int weightDValue = 80;
    int connectionAddDValue = 8;
    int enabdisabDValue = 7;
    int addNodeDValue = 5;

    int value = GetRandomValue(1, 100);

    if (value <= weightDValue) {
        Change_Random_Weight();
    }
    else if (value <= weightDValue + connectionAddDValue) {
        Create_Random_Connecttion();
    }
    else if (value <= weightDValue + connectionAddDValue + enabdisabDValue) {
        Enabdisab_Random_Connection();
    }
    else if (value <= weightDValue + connectionAddDValue + enabdisabDValue + addNodeDValue) {
        Add_Hidden_Node();
    }

    Rebuild_Execution_Order();//kijk hoe mooi
}

void NeuralNetwork::Forward_Pass(Game& game, Creature& creature) {
    Fill_Inputs(game, creature);

    for (int nodeIndex : executionOrder) {

        Node& node = nodesHidden[nodeIndex];

        float sum = 0.0f;

        for (int connectionIndex : incomingConnections[node.id]) {
            const Connection& connection = Connections[connectionIndex];

            if (!connection.enabled)
                continue;

            assert(node.id < nodeValues.size());
            sum += nodeValues[connection.in] * connection.weight;
        }
        assert(node.id < nodeValues.size());
        nodeValues[node.id] = sigmoidMODDED(sum);
    }

    for (Node& node : nodesOutput) {

        float sum = 0;

        for (int connectionIndex : incomingConnections[node.id]) {

            const Connection& connection = Connections[connectionIndex];

            if (!connection.enabled)
                continue;
            assert(node.id < nodeValues.size());
            float input = nodeValues[connection.in];

            sum += input * connection.weight;
        }
        assert(node.id < nodeValues.size());
        nodeValues[node.id] = sigmoidMODDED(sum);
    }

    realOutputs.clear();

    for (Node& node : nodesOutput) {
        realOutputs.push_back(nodeValues[node.id]);
    }
}