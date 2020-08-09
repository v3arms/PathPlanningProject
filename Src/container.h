#ifndef PATHPLANNING_CONTAINER_H
#define PATHPLANNING_CONTAINER_H


#include "node.h"
#include <vector>
#include <map>
#include <queue>
#include <functional>


const int   DefaultPreallocSize  = 1000000;
const float DefaultReserveFactor = 2.0;
typedef std::pair<int, int> nodeId;


// #define DEBUG


#ifdef DEBUG
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x) (void())
#endif


// pointer comparator specification for prqueue
template<> class std::greater<Node*> {
public:
    bool operator()(const Node *a, const Node *b) const;
};


class nodeContainer {
public :
    nodeContainer();

    void open(nodeId id, double g, double f, Node* parent);
    void close(nodeId id);
    bool openedEmpty() const;
    bool closedEmpty() const;
    bool isOpened(nodeId id) const;
    bool isClosed(nodeId id) const;
    nodeId topOpenedMinFVal();
    void popOpenedMinFValAndClose();
    Node* at(nodeId);
    void clear();

private:
    std::vector<Node> node_storage;
    std::map<nodeId, Node*> opened, closed;
    std::priority_queue<Node*, std::vector<Node*>, std::greater<Node*>> prqueue;


    void reallocate_storage();
};


#endif //PATHPLANNING_CONTAINER_H
