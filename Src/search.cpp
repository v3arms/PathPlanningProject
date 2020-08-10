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

        // breaking ties
        
        currId = breakTies(CN_SP_BT_GMIN);

        getSuccessors(map, currId, options);
        LOG("Start expanding node : {" << currId.first << " " << currId.second << "}");
        for (auto& succId : successors) {
            double succGVal = -1;
            
            bool   succNodeExists = node_container.isClosed(succId) || node_container.isOpened(succId);
            if (succNodeExists)
                succGVal = node_container.at(succId)->g;
            
            double newGVal = currNode->g + getCost(map, currId, succId);
            if (!succNodeExists || (succGVal > newGVal)) {
                node_container.open(
                    succId, 
                    newGVal, 
                    newGVal + calcHeuristic(map, succId, targetId, options.metrictype),
                    currNode
                );
                if (!succNodeExists)
                    sresult.nodescreated++;
            }
        }
        sresult.numberofsteps++;
    }

    std::chrono::steady_clock::time_point timeEnd = std::chrono::steady_clock::now();
    sresult.time = std::chrono::duration<double>(timeEnd - timeBegin).count();
    sresult.hppath = &hppath; //Here is a constant pointer
    sresult.lppath = &lppath;
    sresult.pathlength = lppath.size() - 1;
    return sresult;
}


nodeId Search::breakTies(int mode) {
    nodeId id = node_container.topOpenedMinFVal();
    node_container.popOpenedMinFVal();
    return id;
}


/*
nodeId Search::breakTies(int mode) {
    // reusing successors buffer for tied nodes
    successors.clear();

    nodeId chosenId = node_container.topOpenedMinFVal();
    double F = node_container.at(chosenId)->F;
    double g = node_container.at(chosenId)->g;
    // pop nodes and save them, saving optimal node on the way
    for (
        nodeId currId = chosenId; 
        node_container.at(currId)->F == F;
        currId = node_container.topOpenedMinFVal()
    ) {
        if (mode == CN_SP_BT_GMAX && node_container.at(currId)->g > g && !node_container.isClosed(currId)) {
            g = node_container.at(currId)->g;
            chosenId = currId;
        }
        if (mode == CN_SP_BT_GMIN && node_container.at(currId)->g < g && !node_container.isClosed(currId)) {
            g = node_container.at(currId)->g;
            chosenId = currId;
        } 
        
        if (mode != CN_SP_BT_GMIN && mode != CN_SP_BT_GMAX)
            throw std::invalid_argument("Search::breakTies() | unsupported mode");
        
        if (!node_container.isClosed(currId))
            successors.push_back(currId);
        node_container.popOpenedMinFVal();
        if (node_container.openedEmpty())
            break;
    } 
    // reopening saved nodes except optimal
    for (const nodeId& currId : successors) {
        if (currId != chosenId)
            node_container.open(
                currId,
                node_container.at(currId)->g,
                node_container.at(currId)->F,
                node_container.at(currId)->parent,
                true
            );
    }
    return chosenId;
}
*/


void Search::AStarSearch(const std::string& algo_params, const Map &map, const EnvironmentOptions &options) {

}


void Search::makePrimaryPath(const Node* node) {
    LOG("Target reached.");
    while(node != nullptr) {
        lppath.push_front(*node);
        node = node->parent;
    }
    LOG("Path length : " << lppath.size() - 1);
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
    if ((x.first == y.first) || x.second == y.second)
        return map.getCellSize();
    else
        return map.getCellSize() * sqrt(2.0);
}
