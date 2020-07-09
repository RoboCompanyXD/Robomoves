/**
 * File:   RobotStateMachine.h
 * Author: chris
 *
 * Created on 30 de junio de 2020, 10:52
 */

#ifndef ROBOT_STATE_MACHINE_H
#define	ROBOT_STATE_MACHINE_H

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
    enum stateMachineStates {

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
        END_STATE_MACHINE // TODO CRITICAL: REMOVE???
    };

    /** Define the flags for the state-machine */

    int currentSuperState; // Current top-level super-state [Idle, Dock, Shutdown, Undock, NormalOperate]
    int currentState; // Current global state (amongst all possible states)
    int currentUnDocking_subState; // Current sub-state within UnDock: [ExitDock, UnDock_Rotate180]
    int currentRoaming_subState; // Current sub-state within NormalOperate: [TrackingByCamera, CliffAhead, DodgeObstacle]
    int currentTrackingByCamera_subState; // Current sub-state within TrackingByCamera: [ \\TODO ]
    int currentSearchingUser_subState; // Current sub-state within PersonOutOfView: \\TODO
    int currentFollowingUser_subState; // Current sub-state within PersonInView: \\TODO
    int currentDodgingObstacle_subState; // Current sub-state within DodgeObstacle: \\TODO
    int currentCrashAlgorithm_subState; // Current sub-state within CrashAlgorithm: \\TODO
    int currentAvoidingCliff_subState; // Current sub-state within CliffAhead: \\TODO

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

