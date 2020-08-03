/**
 * File:   RobotStateMachine.h
 * Author: chris
 *
 * Created on 30 de junio de 2020, 10:52
 */

#ifndef ROBOT_STATE_MACHINE_H
#define	ROBOT_STATE_MACHINE_H

#define toname( name ) # name

//#include "../ControlRobot/ControlRobot.h"

namespace ControlRobot {
    class ControlRobot; // Forward Declaration of Class ControlRobot
}

/**
 * Class RobotStateMachine
 * 
 * This class encapsulates the logic behind the robot's different states through a finite state machine.
 * 
 */
class RobotStateMachine {
public:

    /**
     * RobotStateMachine Class constructor
     * @param cr Pointer to an instance of this system's robot controller interface.
     */
    RobotStateMachine(ControlRobot::ControlRobot * cr);

    /**
     * RobotStateMachine Class destructor
     */
    ~RobotStateMachine();

    /**
     * TODO: documentar metodo
     */
    void statechart_process();

    /**
     * TODO: Documentar
     */
    bool isEndCondition();
    
    /**
     * TODO: Documentar
     */
    void printCurrentStates();

private:

    ControlRobot::ControlRobot * robot; // reference to the controller interface of this system's robot

    /**
     * TODO: Documentar
     */
    void rootState_entDef();

    /**
     * TODO: Documentar
     */
    void UnDock_entDef();

    /**
     * TODO: Documentar
     */
    void roaming_entDef();

    /**
     * TODO: Documentar
     */
    void RoamingEntDef();

    /**
     * TODO: Documentar
     */
    void TrackingByCamera_entDef();

    /**
     * TODO: Documentar
     */
    void TrackingByCameraEntDef();

    /**
     * TODO: Documentar
     */
    void followingUser_entDef();

    /**
     * TODO: Documentar
     */
    void searchingUser_entDef();

    /**
     * TODO: Documentar
     */
    void dodgingObstacle_entDef();

    /**
     * TODO: Documentar
     */
    void avoidingCliff_entDef();

    /**
     * TODO: Documentar
     */
    void CrashAlgorithm_entDef();

    /**
     * TODO: Documentar
     */
    void endBehavior(); // TODO BORRAR?



protected:

    /**
     * TODO: Documentar
     */
    void initStatechart();

    //void cancelTimeouts();

    /** Unique identifiers for all the different states of the state machine */
    enum class RobotStates {
        DISABLED = 0,
        IDLE = 1,
        SHUTDOWN,
        DOCKING,
        UN_DOCKING,
        /**/TurnAwayFromDock,
        /**/ExitDock,
        ROAMING,
        /**/TrackingByCamera,
        /*****/SearchingUser,
        /********/SearchingUser_TurnToEmptyHallway,
        /********/SearchingUser_Look360Around,
        /********/SearchingUser_TraverseEmptyHallway,
        /********/SearchingUser_FindEmptyHallway,
        /*****/FollowingUser,
        /********/FollowingUser_GettingCloser,
        /********/FollowingUser_ObstacleInBetween,
        /***/DodgingObstacle,
        /*****/MovingBackFromObstacle,
        /*****/CrashAlgorithm,
        /********/CrashAlorithmEndHelperState,
        /********/CrashAlgorithm_RecoverTrajectory,
        /********/CrashAlgorithm_GoForwardExtended,
        /********/CrashAlgorithm_GoForward,
        /********/CrashAlgorithm_DodgeParallel,
        /********/CrashAlgorithm_Dodge,
        /***/AvoidingCliff,
        /*****/TurnAwayFromCliff,
        /*****/LeaveCliffBehind,
        END_STATE_MACHINE
    };

    const char* RobotStateNames[] ={
        toname(DISABLED),
        toname(IDLE),
        toname(SHUTDOWN),
        toname(DOCKING),
        toname(UN_DOCKING),
        /**/toname(TurnAwayFromDock),
        /**/toname(ExitDock),
        toname(ROAMING),
        /**/toname(TrackingByCamera),
        /*****/toname(SearchingUser),
        /********/toname(SearchingUser_TurnToEmptyHallway),
        /********/toname(SearchingUser_Look360Around),
        /********/toname(SearchingUser_TraverseEmptyHallway),
        /********/toname(SearchingUser_FindEmptyHallway),
        /*****/toname(FollowingUser),
        /********/toname(FollowingUser_GettingCloser),
        /********/toname(FollowingUser_ObstacleInBetween),
        /***/toname(DodgingObstacle),
        /*****/toname(MovingBackFromObstacle),
        /*****/toname(CrashAlgorithm),
        /********/toname(CrashAlorithmEndHelperState),
        /********/toname(CrashAlgorithm_RecoverTrajectory),
        /********/toname(CrashAlgorithm_GoForwardExtended),
        /********/toname(CrashAlgorithm_GoForward),
        /********/toname(CrashAlgorithm_DodgeParallel),
        /********/toname(CrashAlgorithm_Dodge),
        /***/toname(AvoidingCliff),
        /*****/toname(TurnAwayFromCliff),
        /*****/toname(LeaveCliffBehind),
        toname(END_STATE_MACHINE)
    };

    /** Define the flags for the state-machine */

    RobotStates currentSuperState; // Current top-level super-state [Idle, Dock, Shutdown, Undock, NormalOperate]
    RobotStates currentState; // Current global state (amongst all possible states)
    RobotStates currentUnDocking_subState; // Current sub-state within UnDock: [ExitDock, UnDock_Rotate180]
    RobotStates currentRoaming_subState; // Current sub-state within NormalOperate: [TrackingByCamera, CliffAhead, DodgeObstacle]
    RobotStates currentTrackingByCamera_subState; // Current sub-state within TrackingByCamera: [ \\TODO ]
    RobotStates currentSearchingUser_subState; // Current sub-state within PersonOutOfView: \\TODO
    RobotStates currentFollowingUser_subState; // Current sub-state within PersonInView: \\TODO
    RobotStates currentDodgingObstacle_subState; // Current sub-state within DodgeObstacle: \\TODO
    RobotStates currentCrashAlgorithm_subState; // Current sub-state within CrashAlgorithm: \\TODO
    RobotStates currentAvoidingCliff_subState; // Current sub-state within CliffAhead: \\TODO

}; // end class RobotStateMachine

#define motores_STOP 'r'
#define motores_FWD 's'
#define motores_BACK 't'
#define motores_LEFT 'u'
#define motores_RIGHT 'v'
#define motores_FWD_L 'w'
#define motores_FWD_R 'x'
#define motores_BACK_L 'y'
#define motores_BACK_R 'z'

#endif	/* ROBOT_STATE_MACHINE_H */

