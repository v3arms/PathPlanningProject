#include "container_simple.h"


template<typename TFirst, typename TSecond>
size_t hash_pair::operator()(const std::pair<TFirst, TSecond>& p) const noexcept {
    uintmax_t hash = std::hash<TFirst>()(p.first);
    hash <<= sizeof(uintmax_t) * 4;
    hash ^= std::hash<TSecond>()(p.second);
    return std::hash<uintmax_t>()(hash);
}


/*
    opened impl
*/
simpleOpened::simpleOpened()
: data(__alloc_size)
, _end(__default_end_node_opened)
{}


const Node& simpleOpened::top_min_fval() const {
    if (data.empty())
        throw std::runtime_error("simpleOpened::top_min_fval() | is empty");
    
    auto min_fv = data.begin();
    for (auto it = data.begin(); it != data.end(); it++) {
        if (it->second.F < min_fv->second.F)
            min_fv = it;
    }
    return min_fv->second;
}


void simpleOpened::pop_min_fval() {
    auto top_min_fv = top_min_fval();
    erase({top_min_fv.i, top_min_fv.j});
}


const Node& simpleOpened::find(const nodeId& x) const {
    const auto it = data.find(x);
    return it == data.end() ? _end : it->second;
}


bool simpleOpened::erase(const nodeId& x) {
    return data.erase(x);
}


bool simpleOpened::empty() const {
    return data.empty();
}


void simpleOpened::clear() {
    data.clear();
}


void simpleOpened::insert(const Node& x) {
    data[{x.i, x.j}] = x;
}


const Node& simpleOpened::end() const {
    return _end;
}


size_t simpleOpened::size() const {
    return data.size();
}


/*
    closed impl
*/
simpleClosed::simpleClosed()
: _end(__default_end_node_closed)
{}


void simpleClosed::insert(const Node& x) {
    data[{x.i, x.j}] = x;
}


bool simpleClosed::erase(const nodeId& x) {
    return data.erase(x);
}


bool simpleClosed::empty() const {
    return data.empty();
}


void simpleClosed::clear() {
    data.clear();
}


const Node& simpleClosed::find(const nodeId& x) const {
    const auto it = data.find(x);
    return it == data.end() ? _end : it->second;
}


bool operator == (const Node& x, const Node& y) {
    return x.i == y.i &&
           x.j == y.j;
}


bool operator != (const Node& x, const Node& y) {
    return !(x == y);
}


const Node& simpleClosed::end() const {
    return _end;
}


size_t simpleClosed::size() const {
    return data.size();
}
