#include "search.h"

Search::Search()
: opened(new simpleOpened())
, closed(new simpleClosed())
{
//set defaults here
}

Search::~Search()
{
    delete opened;
    delete closed;
}


SearchResult Search::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options)
{
    std::chrono::steady_clock::time_point timeBegin = std::chrono::steady_clock::now();

    nodeId startId  = map.getStartPoint(), 
           targetId = map.getTargetPoint();    
    
    opened->insert({
        startId.first, 
        startId.second,
        calcHeuristic(map, startId, targetId) * options.hweight,
        0,
        0,
        nullptr
    });

    sresult.nodescreated++;
    sresult.pathfound = false;
    Node curr, succ_opened, succ_closed, succ;
    nodeId succId, currId;
    
    while(!opened->empty()) {
        LOG("Iteration : " << sresult.numberofsteps);
        
        curr   = breakTies(options.gtype);
        currId = {curr.i, curr.j};

        if (currId == targetId) {
            sresult.pathfound = true;
            makePrimaryPath(map, curr, options);
            break;
        }
        closed->insert(curr);
        getSuccessors(map, {curr.i, curr.j}, options);
        LOG("Start expanding node : {" << currId.first << " " << currId.second << "}");
        for (auto& succId : successors) {
            double succGVal = -1;

            succ = closed->end();
            succ_opened = opened->find(succId);
            succ_closed = closed->find(succId);
            
            bool succNodeExists = (succ_closed != closed->end()) || (succ_opened != opened->end());
            if (succ_opened != opened->end())
                succGVal = succ_opened.g;
            if (succ_closed != closed->end())
                succGVal = succ_closed.g;
            
            // double newGVal = curr.g + calcHeuristic(map, {curr.i, curr.j}, succId, options.metrictype);
            double newGVal = curr.g + getCost(map, currId, succId);
            if (!succNodeExists || (succGVal > newGVal)) {
                opened->insert({
                    succId.first,
                    succId.second, 
                    newGVal + calcHeuristic(map, succId, targetId, options.metrictype) * options.hweight,
                    newGVal, 
                    0,
                    &closed->find(currId)
                });
            }
        }
        sresult.numberofsteps++;
    }

    std::chrono::steady_clock::time_point timeEnd = std::chrono::steady_clock::now();
    sresult.time = std::chrono::duration<double>(timeEnd - timeBegin).count();
    sresult.nodescreated = opened->size() + closed->size();
    sresult.hppath = &hppath; //Here is a constant pointer
    sresult.lppath = &lppath;
    opened->clear();
    closed->clear();
    return sresult;
}


/*
Node Search::breakTies(int mode) {
    Node n = opened->top_min_fval();
    opened->pop_min_fval();
    return n;
}
*/



Node Search::breakTies(int mode) {
    for_breakties.clear();

    Node chosen = opened->top_min_fval();
    // pop nodes and save them, saving optimal node on the way
    for (
        Node curr = chosen; 
        curr.F == chosen.F;
        curr = opened->top_min_fval()
    ) {
        if (mode == CN_SP_BT_GMAX && curr.g > chosen.g) {
            chosen = curr;
        }
        if (mode == CN_SP_BT_GMIN && curr.g < chosen.g) {
            chosen = curr;
        } 
        if (mode != CN_SP_BT_GMIN && mode != CN_SP_BT_GMAX)
            throw std::invalid_argument("Search::breakTies() | unsupported mode");
        
        for_breakties.push_back(curr);
        opened->pop_min_fval();
        if (opened->empty())
            break;
    } 
    // reopening saved nodes except optimal
    for (const Node& curr : for_breakties) {
        if (curr != chosen)
            opened->insert(curr);
    }
    return chosen;
}


void Search::makePrimaryPath(const Map& map, Node node, const EnvironmentOptions& options) {
    LOG("Target reached.");
    Node bufnode = *node.parent;
    while(node.parent != nullptr) {
        bufnode = *node.parent;
        lppath.push_front(node);
        sresult.pathlength += getCost(map, {bufnode.i, bufnode.j}, {node.i, node.j});
        node = *node.parent;
    }
    hppath = lppath;
}

/*void Search::makeSecondaryPath()
{
    //need to implement
}*/

double Search::calcHeuristic(const Map& map, const nodeId& x, const nodeId& y, int metrictype) {
    double ds = abs(x.second - y.second), 
           df = abs(x.first - y.first);

    switch(metrictype) {
        case CN_SP_MT_EUCL :
            return sqrt(df*df + ds*ds);

        case CN_SP_MT_MANH :
            return (df + ds);

        case CN_SP_MT_CHEB :
            return std::max(df, ds);

        case CN_SP_MT_DIAG :
            return std::min(df, ds)*sqrt(2.0) + abs(df - ds);

        default :
            return 0;
    }
}


bool Search::cellAvailable(const Map& map, const nodeId& id) {
    return map.CellOnGrid(id.first, id.second) && map.CellIsTraversable(id.first, id.second);
}


void Search::getSuccessors(const Map& map, const nodeId& id, const EnvironmentOptions& opts) {
    successors.clear();

    int x = id.first, y = id.second;

    // nondiagonal
    if (cellAvailable(map, {x - 1, y    })) successors.push_back({x - 1, y    });
    if (cellAvailable(map, {x    , y - 1})) successors.push_back({x    , y - 1});
    if (cellAvailable(map, {x + 1, y    })) successors.push_back({x + 1, y    });
    if (cellAvailable(map, {x    , y + 1})) successors.push_back({x    , y + 1});

    if (opts.allowdiagonal && !opts.cutcorners) {
        if (cellAvailable(map, {x - 1, y}) && cellAvailable(map, {x, y - 1}) && cellAvailable(map, {x - 1, y - 1})) successors.push_back({x - 1, y - 1});
        if (cellAvailable(map, {x - 1, y}) && cellAvailable(map, {x, y + 1}) && cellAvailable(map, {x - 1, y + 1})) successors.push_back({x - 1, y + 1});
        if (cellAvailable(map, {x + 1, y}) && cellAvailable(map, {x, y - 1}) && cellAvailable(map, {x + 1, y - 1})) successors.push_back({x + 1, y - 1});
        if (cellAvailable(map, {x + 1, y}) && cellAvailable(map, {x, y + 1}) && cellAvailable(map, {x + 1, y + 1})) successors.push_back({x + 1, y + 1});
    }

    if (opts.allowdiagonal && opts.cutcorners && !opts.allowsqueeze) {
        if ((cellAvailable(map, {x - 1, y}) || cellAvailable(map, {x, y - 1})) && cellAvailable(map, {x - 1, y - 1})) successors.push_back({x - 1, y - 1});
        if ((cellAvailable(map, {x - 1, y}) || cellAvailable(map, {x, y + 1})) && cellAvailable(map, {x - 1, y + 1})) successors.push_back({x - 1, y + 1});
        if ((cellAvailable(map, {x + 1, y}) || cellAvailable(map, {x, y - 1})) && cellAvailable(map, {x + 1, y - 1})) successors.push_back({x + 1, y - 1});
        if ((cellAvailable(map, {x + 1, y}) || cellAvailable(map, {x, y + 1})) && cellAvailable(map, {x + 1, y + 1})) successors.push_back({x + 1, y + 1});
    } 

    if (opts.allowdiagonal && opts.cutcorners && opts.allowsqueeze) {
        if (cellAvailable(map, {x - 1, y - 1})) successors.push_back({x - 1, y - 1});
        if (cellAvailable(map, {x + 1, y - 1})) successors.push_back({x + 1, y - 1});
        if (cellAvailable(map, {x + 1, y + 1})) successors.push_back({x + 1, y + 1});
        if (cellAvailable(map, {x - 1, y + 1})) successors.push_back({x - 1, y + 1});
    }
}


double Search::getCost(const Map& map, const nodeId& x, const nodeId& y) {
    if ((x.first == y.first) || (x.second == y.second))
        return 1;
    else
        return sqrt(2.0);
}
