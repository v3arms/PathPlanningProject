#include "search.h"

Search::Search()
{
//set defaults here
}

Search::~Search() {}


SearchResult Search::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options)
{
    //need to implement
    std::chrono::steady_clock::time_point timeBegin = std::chrono::steady_clock::now();

    nodeId startId  = map.getStartPoint(), 
           targetId = map.getTargetPoint();    
    node_container.open(startId, 0, calcHeuristic(map, startId, targetId), nullptr);
    
    sresult.nodescreated++;
    sresult.pathfound = false;
    
    while(!node_container.openedEmpty()) {
        LOG("Iteration : " << sresult.numberofsteps);

        nodeId currId   = node_container.topOpenedMinFVal();
        Node*  currNode = node_container.at(currId);
        if (currId == targetId) {
            sresult.pathfound = true;
            makePrimaryPath(currNode);
            break;
        }
            
        
        node_container.popOpenedMinFValAndClose();
        node_container.close(currId);
        getSuccessors(map, currId, options);
        LOG("Start expanding node : {" << currId.first << " " << currId.second << "}");
        for (auto& succId : successors) {

            double succGVal = -1;
            if (node_container.isClosed(succId) || node_container.isOpened(succId))
                succGVal = node_container.at(succId)->g;
            
            double newGVal = currNode->g + getCost(map, currId, succId);
            if (
                (!node_container.isClosed(succId) &&
                !node_container.isOpened(succId)) ||
                (succGVal > newGVal))
            {
                node_container.open(
                    succId, 
                    newGVal, 
                    newGVal + calcHeuristic(map, succId, targetId, options.metrictype),
                    currNode  
                );
                sresult.nodescreated++;
            }
        }
        sresult.numberofsteps++;
    }

    std::chrono::steady_clock::time_point timeEnd = std::chrono::steady_clock::now();
    sresult.time = std::chrono::duration<double, std::milli>(timeEnd - timeBegin).count();
    sresult.hppath = &hppath; //Here is a constant pointer
    sresult.lppath = &lppath;
    sresult.pathlength = lppath.size() - 1;
    return sresult;
}


void Search::makePrimaryPath(const Node* node) {
    LOG("Target reached.");
    while(node != nullptr) {
        lppath.push_front(*node);
        node = node->parent;
    }
    LOG("Path length : " << lppath.size());
    hppath = lppath;
}

/*void Search::makeSecondaryPath()
{
    //need to implement
}*/

float Search::calcHeuristic(const Map& map, const nodeId& x, const nodeId& y, int metrictype) {
    double ds, df;
    switch(metrictype) {
        case CN_SP_MT_EUCL :
            return map.getCellSize()*sqrt(
                (x.first - y.first)*(x.first - y.first) + (x.second - y.second)*(x.second - y.second)
            );
        case CN_SP_MT_MANH :
            return map.getCellSize()*(abs(x.first - y.first) + abs(x.second - y.second));
        case CN_SP_MT_CHEB :
            return std::max(abs(x.first - y.first), abs(x.second - y.second));
        case CN_SP_MT_DIAG :
            df = abs(x.first - y.first),
            ds = abs(x.second - y.second);
            return std::min(df, ds)*sqrt(2) + abs(df - ds);
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
        if ((cellAvailable(map, {x - 1, y}) || cellAvailable(map, {x, y - 1})) && cellAvailable(map, {x - 1, y - 1})) successors.push_back({x - 1, y - 1});
        if ((cellAvailable(map, {x - 1, y}) || cellAvailable(map, {x, y + 1})) && cellAvailable(map, {x - 1, y + 1})) successors.push_back({x - 1, y + 1});
        if ((cellAvailable(map, {x + 1, y}) || cellAvailable(map, {x, y - 1})) && cellAvailable(map, {x + 1, y - 1})) successors.push_back({x + 1, y - 1});
        if ((cellAvailable(map, {x + 1, y}) || cellAvailable(map, {x, y + 1})) && cellAvailable(map, {x + 1, y + 1})) successors.push_back({x + 1, y + 1});
    }

    if (opts.allowdiagonal && opts.cutcorners && !opts.allowsqueeze) {
        if (cellAvailable(map, {x - 1, y}) && cellAvailable(map, {x, y - 1}) && cellAvailable(map, {x - 1, y - 1})) successors.push_back({x - 1, y - 1});
        if (cellAvailable(map, {x - 1, y}) && cellAvailable(map, {x, y + 1}) && cellAvailable(map, {x - 1, y + 1})) successors.push_back({x - 1, y + 1});
        if (cellAvailable(map, {x + 1, y}) && cellAvailable(map, {x, y - 1}) && cellAvailable(map, {x + 1, y - 1})) successors.push_back({x + 1, y - 1});
        if (cellAvailable(map, {x + 1, y}) && cellAvailable(map, {x, y + 1}) && cellAvailable(map, {x + 1, y + 1})) successors.push_back({x + 1, y + 1});
    } 

    if (opts.allowdiagonal && opts.cutcorners && opts.allowsqueeze) {
        if (cellAvailable(map, {x - 1, y - 1})) successors.push_back({x - 1, y - 1});
        if (cellAvailable(map, {x + 1, y - 1})) successors.push_back({x + 1, y - 1});
        if (cellAvailable(map, {x + 1, y + 1})) successors.push_back({x + 1, y + 1});
        if (cellAvailable(map, {x - 1, y + 1})) successors.push_back({x - 1, y + 1});
    }
}


float Search::getCost(const Map& map, const nodeId& x, const nodeId& y) {
    return map.getCellSize()*sqrt(
        (x.first - y.first)*(x.first - y.first) + (x.second - y.second)*(x.second - y.second)
    );
}
