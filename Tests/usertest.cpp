#include "catch.hpp"
#include <iostream>
#include "container.h"


TEST_CASE("Container") {
    /*
    auto oc = nodeContainer();

    Node b({0, 1, 4.56, 1, 0, NULL}),
         a({0, 0, 5, 0, 0, NULL}),
         c({1, 0, 4.56, 1, 0, NULL});

    oc.open({0, 1}, 1, 4.56, NULL);
    oc.open({0, 0}, 0, 5, NULL);
    oc.open({1, 0}, 1, 4.56, NULL);


    REQUIRE(oc.isOpened({0, 0}));
    REQUIRE(oc.isOpened({0, 1}));
    REQUIRE(oc.isOpened({1, 0}));

    oc.close({0, 0});
    try {
        oc.close({0, 0});
    } catch(std::exception& e) {
        INFO(e.what());
    }

    REQUIRE(oc.isClosed({0, 0}));
    REQUIRE_FALSE(oc.isOpened({0, 0}));

    auto node = oc.topOpenedMinFVal();
    REQUIRE((node == nodeId{0, 1} || node == nodeId{1, 0}));
    REQUIRE(oc.at(node)->g == 1);

    oc.popOpenedMinFVal();
    oc.popOpenedMinFVal();
    REQUIRE(oc.openedEmpty());


    oc = nodeContainer();

    for (int i = 1; i < 1000000; i++) {
        if (i % 10 == 0) {
            auto n = oc.topOpenedMinFVal();
            const Node* node = oc.at(n);
            node->g;
            node->F;
            std::cout << node->i << " " << node->j << '\n';
            oc.popOpenedMinFVal();
            oc.close(n);
        }
        oc.open({i - 1, i}, 0, 0, NULL);
    }

    while(!oc.openedEmpty())
        oc.popOpenedMinFVal();
    
    REQUIRE(!oc.closedEmpty());
    oc.clear();
    REQUIRE(oc.openedEmpty());
    REQUIRE(oc.closedEmpty());
    */
}


/*
TEST_CASE("Search") {
    Search s;
    Map m;
    m.getMap("../../TestData/2_test.xml");

    // auto n = s.GetSuccessorNode(m, 1, 1);

    // std::cout << n.i << " " << n.j << std::endl;

    
    s.getSuccessors(m, {2, 2, 0, 0, 0, 0});

    for (auto& i : s.adjBuf) {
        std::cout << i.i << " " << i.j << std::endl;
    }
    
    
}
*/



