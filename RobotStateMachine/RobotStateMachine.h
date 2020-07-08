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
    void NormalOperate_entDef();

    /**
     * TODO: Documentar
     */
    void NormalOperateEntDef();

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
    void PersonInView_entDef();

    /**
     * TODO: Documentar
     */
    void PersonOutView_entDef();

    /**
     * TODO: Documentar
     */
    void DodgeObstacle_entDef();

    /**
     * TODO: Documentar
     */
    void CliffAhead_entDef();

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
    enum UC_Enum {

        DISABLED = 0, // The robot is disabled. Expected behavior: the robot remains disabled.
        UN_DOCKING = 1, // The `undock` button has been pressed while being docked, and the robot should be stepping back from the dock-station.
        UnDock_Rotate180 = 2, // The robot has stepped back from the dock-station and should be rotating 180º to face towards the room.
        ExitDock = 3, // TODO
        EndUndoParkHelperState = 4, // TODO
        SHUTDOWN = 5, // TODO
        NORMAL_ROAMING = 6, // Super-state
        TrackingByCamera = 7,
        PersonOutView = 8,
        PersonOutView_RotateToMove = 9,
        PersonOutView_Rotate360 = 10,
        PersonOutView_GoForward = 11,
        PersonOutView_ComputePosition = 12,
        PersonInView = 13,
        PersonInView_PathBlocked = 14,
        PersonInView_ApproachUser = 15,
        ComputeStepBlocked = 16,
        ComputeStepApproach = 17,
        DodgeObstacle = 18,
        Dodge_MoveBack = 19,
        CrashAlgorithm = 20,
        CrashAlorithmEndHelperState = 21,
        CrashAlgorithm_RecoverTrajectory = 22,
        CrashAlgorithm_GoForwardExtended = 23,
        CrashAlgorithm_GoForward = 24,
        CrashAlgorithm_DodgeParallel = 25,
        CrashAlgorithm_Dodge = 26,
        CliffAhead = 27,
        CliffAhead_Rotate180 = 28,
        CliffAhead_GoForward = 29,
        Initializing = 30,
        InitFailed = 31,
        IDLE = 32,
        DOCKING = 33,
        Statechart_End = 1000
    };

    /** Define the flags for the state-machine */

    int currentSuperState; // Current top-level super-state [Idle, Dock, Shutdown, Undock, NormalOperate]
    int currentState; // Current global state (amongst all possible states)
    int currentUnDock_subState; // Current sub-state within UnDock: [ExitDock, UnDock_Rotate180]
    int currentNormalOperate_subState; // Current sub-state within NormalOperate: [TrackingByCamera, CliffAhead, DodgeObstacle]
    int currentTrackingByCamera_subState; // Current sub-state within TrackingByCamera: [ \\TODO ]
    int currentPersonOutView_subState; // Current sub-state within PersonOutOfView: \\TODO
    int currentPersonInView_subState; // Current sub-state within PersonInView: \\TODO
    int currentDodgeObstacle_subState; // Current sub-state within DodgeObstacle: \\TODO
    int currentCrashAlgorithm_subState; // Current sub-state within CrashAlgorithm: \\TODO
    int currentCliffAhead_subState; // Current sub-state within CliffAhead: \\TODO

    int PersonInView_timeout; // TODO

}; // end class UC

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

