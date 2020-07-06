/**
 * File:   UC.h
 * Author: chris
 *
 * Created on 30 de junio de 2020, 10:52
 */

#ifndef RobotStateMachine_H
#define	RobotStateMachine_H

//#include "../ControlRobot/ControlRobot.h"
class ControlRobot; // Forward Declaration of Class ControlRobot

/**
 *
 * TODO: documentar clase
 */
class RobotStateMachine {

    

public:
    
    /**
     * Class constructor
     */ 
    RobotStateMachine(ControlRobot * contr);
    
    /**
     * Class destructor
     */
    virtual ~RobotStateMachine();
    
    /**
     * TODO: documentar metodo
     */
    void statechart_process();

private:
    
    ControlRobot * robot; // TODO: documentar variable

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

    /** State IDs */
    enum UC_Enum {

        Disabled = 0, // TODO: What does OMN mean?
        UnDock = 1,
        UnDock_Rotate180 = 2,
        ExitDock = 3,
        EndUndoParkHelperState = 4,
        Shutdown = 5,
        NormalOperate = 6,
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
        Idle = 32,
        Dock = 33,
        Statechart_End = 1000
    };

    /** Define the flags for the state-machine */
    
    int currentSuperState; // Estado mas exterior: superestado
    int currentState; // Estado activo principal
    int currentUnDock_subState; // Subestados del estado "UnDock": "ExitDock" y "UnDock_Rotate180"
    int currentNormalOperate_subState; // Subestados del estado "NormalOperate": TrackingByCamera, CliffAhead y DodgeObstacle
    int currentTrackingByCamera_subState; // TODO
    int currentPersonOutView_subState; // TODO
    int currentPersonInView_subState; // TODO
    int currentDodgeObstacle_subState; // TODO
    int currentCrashAlgorithm_subState; // TODO
    int currentCliffAhead_subState; // TODO

    int PersonInView_timeout; // TODO

}; // end class UC

#endif	/* RobotStateMachine_H */

