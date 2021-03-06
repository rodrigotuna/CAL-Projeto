#include "MenuInterface.h"

#include <iostream>


using namespace std;

MenuInterface::MenuInterface(Manager *manager) {
    this->manager = manager;
}



void MenuInterface::drawTitle(){
    cout<<"===============================================================================" << endl
        <<" _   _                _             ______            _                       " << endl
        <<"| | | |              (_)            | ___ \\          | |                     " << endl
        <<"| | | | __ _  ___ ___ _ _ __   ___  | |_/ /___  _   _| |_ ___ _ __            " << endl
      <<R"(| | | |/ _` |/ __/ __| | '_ \ / _ \ |    // _ \| | | | __/ _ \ '__|           )" << endl
      <<R"(\ \_/ / (_| | (_| (__| | | | |  __/ | |\ \ (_) | |_| | ||  __/ |              )" << endl
      <<R"( \___/ \__,_|\___\___|_|_| |_|\___| \_| \_\___/ \__,_|\__\___|_|              )" << endl
      << "                                                                               " << endl
       << "==============================================================================" << "\n"  << endl;

}

void MenuInterface::drawMainMenu() {
    cout << "       MAIN MENU            " << endl
         <<"=============================" << endl
         << "Show Map                 [1]" << endl
         << "Show Points of Interest  [2]" << endl
         << "Show SCC's               [3]" << endl
         << "Calculate Routes         [4]" << endl
         << "Calculate Shortest Path  [5]" << endl
         << "Algorithm Visualizer     [6]" << endl
         << "Exit                     [0]" << endl
         <<"=============================" << endl;
}

void MenuInterface::drawPathsAlgorithmsMenu() {
    cout << "       ALGORITHM            " << endl
         <<"=============================" << endl
         << "Dijkstra                 [1]" << endl
         << "Bidirectional Dijkstra   [2]" << endl
         << "A star (A*)              [3]" << endl
         << "Back                     [0]" << endl
         <<"=============================" << endl;
}

void MenuInterface::drawSCCAlgorithmsMenu() {
    cout << "       ALGORITHM            " << endl
         <<"=============================" << endl
         << "Kosaraju                 [1]" << endl
         << "Tarjan                   [2]" << endl
         << "Back                     [0]" << endl
         <<"=============================" << endl;
}

int MenuInterface::readOption(int min, unsigned int max) {
    int option;
    while (true) {
        cout << "Your option: ";
        if (cin >> option && option >= min && option <= max) {
            cin.ignore(1000, '\n');
            return option;
        } else {
            cin.clear();
            cin.ignore(1000, '\n');
            cerr << endl
                 << "Invalid input. Please try again:" << endl
                 << endl;
        }
    }
}

void MenuInterface::mainMenuStart() {
    drawTitle();
    int option;
    do {
        drawMainMenu();

        option = readOption(0,6);
        switch(option){
            case 1: {
                GraphDisplayer gd(manager->getGraph());
                gd.display();
                gd.join();
            }
                break;
            case 2:
                poiDisplay();
                break;
            case 3:
                sccDisplay();
                break;
            case 4:
                routesDisplay();
                break;
            case 5:
                pathDisplay();
                break;
            case 6:
                animationDisplay();
                break;
            default:
                option = 0;
                break;
        }
    } while(option != 0);
}

void MenuInterface::animationDisplay() {
    int option;
    do{
        option = pathsMenu();
        if(option != 0){
            GraphDisplayer gd(manager->getGraph());
            unsigned int time;
            Node * source = nodeChoice(true);
            Node * dest = (option != 1 ? nodeChoice(false) : nullptr);
            vector<Edge*> pathForward;
            vector<Edge*> pathBackward;
            vector<Edge*> path;
            manager->getTrace(option, pathForward, pathBackward, source, dest);
            manager->getPath(option, time, path, source, dest);
            gd.highlightNode(source, GraphViewer::RED);
            gd.highlightNode(dest, GraphViewer::GREEN);
            gd.display();
            gd.traceAnimation(pathForward, pathBackward);
            gd.pathAnimation(path);
            gd.join();
        }
    }while (option != 0);
}
void MenuInterface::pathDisplay() {
    int option;
    do{
        option = pathsMenu();
        if(option != 0){
            GraphDisplayer gd(manager->getGraph());
            vector<Edge*> path;
            Node * source = nodeChoice(true);
            Node * dest = nodeChoice(false);
            unsigned int time;
            double distance = manager->getPath(option, time, path, source, dest);
            cout << "Algorithm ran in: " << time << "ms." << endl;
            cout << "Distance between points is: "<<  distance << endl;
            gd.highlightNode(source, GraphViewer::RED);
            gd.highlightNode(dest, GraphViewer::GREEN);
            gd.highlightPath(path);
            gd.display();
            gd.join();
        }
    } while(option != 0);
}

void MenuInterface::poiDisplay() {
    GraphDisplayer gd(manager->getGraph());
    vector<Node*> depots;
    unordered_map<Node*, double> clients;
    manager->getPOI(depots, clients);
    gd.highlightPOI(clients, depots);
    gd.display();
    gd.join();
}

void MenuInterface::routesDisplay() {
    GraphDisplayer gd(manager->getGraph());
    vector<vector<vector<Edge*>>> routes;
    unsigned int time;
    manager->getRoutes(routes,time);
    cout << "Algorithm ran in: " << time << " ms."<< endl;
    vector<Node*> depots;
    unordered_map<Node*, double> clients;
    manager->getPOI(depots,clients);
    gd.highlightPOI(clients, depots);
    gd.highlightRoutes(routes);
    gd.display();
    gd.join();
}

int MenuInterface::pathsMenu() {
    drawPathsAlgorithmsMenu();
    return readOption(0,3);
}

Node *MenuInterface::nodeChoice(bool isSource) {
    Node * node = nullptr;
    unsigned int userChoice;
    do{
        cout << "Choose a " << (isSource ? "source" : "destiny") << " node: ";
        if(cin >> userChoice){
            cin.ignore(1000, '\n');
            node = manager->getGraph()->getNode(userChoice);
            if(node == nullptr) {
                cerr << endl
                     << "Nonexistent node. Please try again:" << endl
                     << endl;
            }
        } else {
            cin.clear();
            cin.ignore(1000, '\n');
            cerr << endl
                 << "Invalid input. Please try again:" << endl
                 << endl;
        }
    } while(node == nullptr);
    return node;
}

void MenuInterface::sccDisplay() {
    int option;
    do{
        drawSCCAlgorithmsMenu();
        option = readOption(0,2);
        if(option != 0){
            GraphDisplayer gd(manager->getGraph());
            Node * source = nodeChoice(true);
            unordered_map<Node *, Node *> scc;
            unsigned int time;
            std::vector<Node*> sccNodes;
            sccNodes = manager->getSCC(option, scc,time,source);
            cout << "Algorithm ran in: " << time << "ms." << endl;
            cout << "Number of nodes of the SCC: " << sccNodes.size() << endl;

            gd.highlightSCCNodes(sccNodes);
            gd.highlightNode(source, GraphViewer::RED);
            gd.display();
            gd.join();
        }
    } while(option != 0);
}
