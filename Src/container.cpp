#include "container.h"


bool std::greater<Node*>::operator()(const Node *a, const Node *b) const {
    if (a == NULL || b == NULL)
        throw std::invalid_argument("nodePointerComparator : NULL pointer passed.");
    return a->F > b->F;
}


nodeContainer::nodeContainer() {
    node_storage.reserve(DefaultPreallocSize);
}

/*
    adds node to storage (reallocate if needed)
    adds its pointer to opened map
    erases pointer from closed map if present
*/
void nodeContainer::open(nodeId id, double g, double f, Node* parent) {
    if (node_storage.size() == node_storage.capacity()) {
        reallocate_storage();
    }
    node_storage.push_back({id.first, id.second, f, g, 0, parent});
    prqueue.push(&node_storage.back());

    opened[id] = &node_storage.back();
    closed.erase(id);

}


/*
    if node exists in storage : move its pointer from opened to closed map
    else : push node and add to closed map
*/
void nodeContainer::close(nodeId id) {
    auto nodeItOpened = opened.find(id),
         nodeItClosed = closed.find(id);

    if (nodeItOpened != opened.end() && nodeItClosed == closed.end()) {
        opened.erase(nodeItOpened);
        closed[id] = nodeItClosed->second;
    }
    else if (nodeItOpened == opened.end() && nodeItClosed != closed.end()) {
        return;
    }
    /*
    else if (nodeItOpened == opened.end() && nodeItClosed == closed.end()) {
        
    }
    */
    else {
        throw std::runtime_error(
                "nodeContainer::close() | attempt to close non-existing node, or node is present in both opened/closed maps."
                );
    }
}


bool nodeContainer::openedEmpty() const {
    return opened.empty();
}


bool nodeContainer::closedEmpty() const {
    return closed.empty();
}


nodeId nodeContainer::topOpenedMinFVal() {
    if (prqueue.empty())
        throw std::runtime_error("nodeContainer::topOpenedMinFVal() | Top from empty queue");

    Node* nodePtr = prqueue.top();
    // just in case
    if (nodePtr == nullptr)
        throw std::runtime_error("nodeContainer::topOpenedMinFVal() | NULL pointer found");

    // mb non needed
    while (isClosed({nodePtr->i, nodePtr->j})) {
        prqueue.pop();
        if (!prqueue.empty())
            nodePtr = prqueue.top();
        else
            throw std::runtime_error("nodeContainer::topOpenedMinFVal() | Top from empty queue");
    }
    return {nodePtr->i, nodePtr->j};
}


void nodeContainer::popOpenedMinFValAndClose() {
    if (prqueue.empty())
        throw std::runtime_error("nodeContainer::popOpenedMinFVal() | Pop from empty queue");

    Node* nodePtr = prqueue.top();
    while (isClosed({nodePtr->i, nodePtr->j})) {
        prqueue.pop();
        if (!prqueue.empty())
            nodePtr = prqueue.top();
        else
            break;
    }
    if (!prqueue.empty())
        prqueue.pop();
    else
        throw std::runtime_error("nodeContainer::popOpenedMinFVal() | Pop from empty queue");
    opened.erase({nodePtr->i, nodePtr->j});
    closed[{nodePtr->i, nodePtr->j}] = nodePtr; 
}


Node* nodeContainer::at(nodeId id) {
    auto it = opened.find(id);
    if (it != opened.end())
        return it->second;
    else if ((it = closed.find(id)) != closed.end())
        return it->second;
    else
        throw std::runtime_error("nodeContainer::at() | Node does not exist.");
}


void nodeContainer::clear() {
    node_storage.clear();
    node_storage.reserve(DefaultPreallocSize);

    opened.clear();
    closed.clear();

    prqueue = std::priority_queue<Node*, std::vector<Node*>, std::greater<Node*>>();
}


bool nodeContainer::isOpened(nodeId id) const {
    return opened.find(id) != opened.end();
}


bool nodeContainer::isClosed(nodeId id) const {
    return closed.find(id) != closed.end();
}


/*
    reallocates vector storage if not enough
    updates pointers in opened/closed maps
    complexity : O(n*log(n)) - for red-black tree map, O(n) for unordered_map
*/
void nodeContainer::reallocate_storage() {
    prqueue = std::priority_queue<Node*, std::vector<Node*>, std::greater<Node*>>();
    node_storage.reserve(static_cast<size_t>(node_storage.capacity() * DefaultReserveFactor));

    std::_Rb_tree_iterator<std::pair<const nodeId, Node*>> openedIt, closedIt;
    for (auto& node : node_storage) {
        openedIt = opened.find({node.i, node.j});
        closedIt = closed.find({node.i, node.j});

        if (openedIt != opened.end())
            openedIt->second = &node;
        if (closedIt != closed.end())
            closedIt->second = &node;
    }

    for (auto& it : opened) {
        prqueue.push(it.second);
    }
}
