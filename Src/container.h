#ifndef PATHPLANNING_CONTAINER_H
#define PATHPLANNING_CONTAINER_H


#include <queue>
#include "node.h"
#include <set>
#include <map>


bool operator >  (const Node& x, const Node& y);
bool operator == (const Node& x, const Node& y);


class NodeContainer {
    public :
        NodeContainer();

        void        Open(const Node& x);
        void        Close(const Node& x);
        const Node& TopMaxFVal();
        void        PopMaxFVal();

        bool        Opened(const Node& x);
        bool        Closed(const Node& x);
        bool        OpenEmpty();
        bool        CloseEmpty();
        bool        PRQueueEmpty();
        void        Clear();

        const Node& At(int i, int j);
        const Node NullNode;

    private :
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> prqueue;
        std::map<std::pair<int, int>, Node> opened, closed;



};




#endif //PATHPLANNING_CONTAINER_H
