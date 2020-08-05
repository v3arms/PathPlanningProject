#include "container.h"


bool operator > (const Node& x, const Node& y) {
    return x.F > y.F;
}


bool operator == (const Node& x, const Node& y) {
    return x.i == y.i &&
           x.j == y.j &&
           x.g == y.g &&
           x.H == y.H &&
           x.F == y.F;
}


NodeContainer::NodeContainer()
: NullNode({-1, -1, 0, 0, 0, NULL})
{
    
}


void NodeContainer::Open(const Node &x) {
    prqueue.push(x);
    opened[{x.i, x.j}] = x;
    closed.erase({x.i, x.j});
}


void NodeContainer::Close(const Node &x) {
    closed[{x.i, x.j}] = x;
    opened.erase({x.i, x.j});
}


bool NodeContainer::Opened(const Node &x) {
    return opened.find({x.i, x.j}) != opened.end();
}


bool NodeContainer::Closed(const Node &x) {
    return opened.find({x.i, x.j}) != opened.end();
}


bool NodeContainer::OpenEmpty() {
    return opened.empty();
}


bool NodeContainer::CloseEmpty() {
    return closed.empty();
}


const Node& NodeContainer::TopMaxFVal() {
    Node tmp = prqueue.top();
    while(Closed(tmp) && !prqueue.empty()) {
        prqueue.pop();
    }
    return prqueue.top();
}


void NodeContainer::PopMaxFVal() {
    Node tmp = prqueue.top();
    while(Closed(tmp) && !prqueue.empty()) {
        prqueue.pop();
    }
    opened.erase({prqueue.top().i, prqueue.top().j});
    prqueue.pop();
}


bool NodeContainer::PRQueueEmpty() {
    return prqueue.empty();
}


void NodeContainer::Clear() {
    prqueue = std::priority_queue<Node, std::vector<Node>, std::greater<Node>>();
    opened  = std::map<std::pair<int, int>, Node>();
    closed  = std::map<std::pair<int, int>, Node>();
}


const Node& NodeContainer::At(int i, int j) {
    auto tmp_o = opened.find({i, j}),
         tmp_c = closed.find({i, j});

    if (tmp_c != opened.end()) return tmp_c->second;
    if (tmp_o != opened.end()) return tmp_o->second;
    return NullNode;
}
