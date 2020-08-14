#include "environmentoptions.h"

EnvironmentOptions::EnvironmentOptions()
{
    hweight = 1.0;
    gtype = CN_SP_BT_GMIN;
    metrictype = CN_SP_MT_EUCL;
    allowsqueeze = false;
    allowdiagonal = true;
    cutcorners = false;
}

EnvironmentOptions::EnvironmentOptions(bool AS, bool AD, bool CC, int MT, int GT, double HW)
{
    hweight = HW;
    metrictype = MT;
    gtype = GT;
    allowsqueeze = AS;
    allowdiagonal = AD;
    cutcorners = CC;
}

