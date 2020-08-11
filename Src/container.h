#ifndef PATHPLANNING_CONTAINER_H
#define PATHPLANNING_CONTAINER_H


#include "node.h"
#include <vector>
#include <map>
#include <queue>
#include <functional>
#include <iostream>


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
/*
template<> class std::greater<Node*> {
public:
    bool operator()(const Node *a, const Node *b) const;
};
*/


class openedContainer {
public :
    virtual void insert(const Node& x) = 0;
    virtual bool erase(const nodeId& x) = 0;
    virtual const Node& top_min_fval() const = 0;
    virtual void pop_min_fval() = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;
    virtual const Node& find(const nodeId& x) const = 0;
    virtual ~openedContainer() {};
    virtual const Node& end() const = 0;

};


class closedContainer {
public :
    virtual void insert(const Node& x) = 0;
    virtual bool erase(const nodeId& x) = 0;
    virtual const Node& find(const nodeId& x) const = 0;
    virtual void clear() = 0;
    virtual bool empty() const = 0;
    virtual ~closedContainer() {};
    virtual const Node& end() const = 0;

};


bool operator == (const Node& x, const Node& y);
bool operator != (const Node& x, const Node& y);


#endif
