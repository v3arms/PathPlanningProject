#include "search.h"


Search::Search()
{
    //set defaults here
    adjBuf = std::vector<Node>(8);
}


Search::~Search() {}


SearchResult Search::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options)
{
    //need to implement


    /*sresult.pathfound = ;
    sresult.nodescreated =  ;
    sresult.numberofsteps = ;
    sresult.time = ;
    sresult.hppath = &hppath; //Here is a constant pointer
    sresult.lppath = &lppath;*/
    return sresult;
}

/*void Search::makePrimaryPath(Node curNode)
{
    //need to implement
}*/

/*void Search::makeSecondaryPath()
{
    //need to implement
}*/


inline Node Search::GetSuccessorNode(const Map& Map, int i, int j) {
    Node retNode;
    if (Map.CellIsTraversable(i, j) && Map.CellOnGrid(i, j)) {
        retNode = nodeContainer.At(i, j);
        return retNode == nodeContainer.NullNode ? Node({i, j, 0, 0, 0, NULL}) : retNode;
    } else {
        return nodeContainer.NullNode;
    }
}


void Search::getSuccessors(const Map& Map, const Node& x) {
    adjBuf.resize(0);
    Node tmp;
    int resizeTo = 0,
            i = x.i,
            j = x.j;
    if (!((tmp = GetSuccessorNode(Map, i - 1, j - 1)) == nodeContainer.NullNode)) adjBuf.push_back(tmp);
    if (!((tmp = GetSuccessorNode(Map, i - 1, j))     == nodeContainer.NullNode)) adjBuf.push_back(tmp);
    if (!((tmp = GetSuccessorNode(Map, i - 1, j + 1)) == nodeContainer.NullNode)) adjBuf.push_back(tmp);
    if (!((tmp = GetSuccessorNode(Map, i,     j - 1)) == nodeContainer.NullNode)) adjBuf.push_back(tmp);
    if (!((tmp = GetSuccessorNode(Map, i,     j + 1)) == nodeContainer.NullNode)) adjBuf.push_back(tmp);
    if (!((tmp = GetSuccessorNode(Map, i + 1, j - 1)) == nodeContainer.NullNode)) adjBuf.push_back(tmp);
    if (!((tmp = GetSuccessorNode(Map, i + 1, j))     == nodeContainer.NullNode)) adjBuf.push_back(tmp);
    if (!((tmp = GetSuccessorNode(Map, i + 1, j + 1)) == nodeContainer.NullNode)) adjBuf.push_back(tmp);

}