#include "container.h"


const Node __default_end_node_opened = {-1437, -1437, 0, 0, 0, nullptr};
const Node __default_end_node_closed = {-1438, -1438, 0, 0, 0, nullptr};
const size_t __alloc_size = 100000;


struct hash_pair final {
    template<class TFirst, class TSecond>
    std::size_t operator()(const std::pair<TFirst, TSecond>& p) const noexcept;
};


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
    size_t size() const;

private :
    std::unordered_map<nodeId, Node, hash_pair> data;
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
    size_t size() const;


private :
    std::unordered_map<nodeId, Node, hash_pair> data;
    const Node _end;

};


