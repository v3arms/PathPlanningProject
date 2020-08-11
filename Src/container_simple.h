#include "container.h"


const Node __default_end_node_opened = {-1437, -1437, 0, 0, 0, nullptr};
const Node __default_end_node_closed = {-1438, -1438, 0, 0, 0, nullptr};


class simpleOpened : public openedContainer {
public :
    simpleOpened();
    void insert(const Node& x);
    bool erase(const nodeId& x);
    const Node& top_min_fval() const;
    void pop_min_fval();
    bool empty() const;
    void clear();
    const Node& find(const nodeId& x) const;
    const Node& end() const;

private :
    std::map<nodeId, Node> data;
    const Node _end;
    
};


class simpleClosed : public closedContainer {
public :
    simpleClosed();
    void insert(const Node& x);
    bool erase(const nodeId& x);
    bool empty() const;
    void clear();
    const Node& find(const nodeId& x) const;
    const Node& end() const;

private :
    std::map<nodeId, Node> data;
    const Node _end;

};



