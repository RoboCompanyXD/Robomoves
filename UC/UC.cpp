
#include "UC.h"

UC::UC()
{
    initStatechart();
}

void UC::initStatechart() {
    rootState_subState = OMNonState;
    rootState_active = OMNonState;
    UnDock_subState = OMNonState;
    NormalOperate_subState = OMNonState;
    TrackingByCamera_subState = OMNonState;
    PersonOutView_subState = OMNonState;
    PersonInView_subState = OMNonState;
    PersonInView_timeout = 0;
    DodgeObstacle_subState = OMNonState;
    CrashAlgorithm_subState = OMNonState;
    CliffAhead_subState = OMNonState;
}

UC::statechart_process(){}