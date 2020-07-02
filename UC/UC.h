/* 
 * File:   RobotIA.h
 * Author: chris
 *
 * Created on 30 de junio de 2020, 10:52
 */

#ifndef UC_H
#define	UC_H

extern bool check_btnSpot();
extern bool check_btnClean();
extern bool check_btnDock();

////    Attributes    ////
    
    extern int BumpObstacle;		//## attribute BumpObstacle
    
    extern int ComputeStepTime;		//## attribute ComputeStepTime
    
    extern bool cameraIsPersonInView;		//## attribute cameraIsPersonInView
    
    extern int initStatus;		//## attribute initStatus
    
    extern int lidarIsObstable;		//## attribute lidarIsObstable
    
    extern int sensoresBateria;		//## attribute sensoresBateria
    
    extern bool sensoresBl;		//## attribute sensoresBl
    
    extern bool sensoresBr;		//## attribute sensoresBr
    
    extern bool sensoresCliff;		//## attribute sensoresCliff
    
    extern int sensoresInDock;		//## attribute sensoresInDock
    
    extern bool sensoresLBumpFront;		//## attribute sensoresLBumpFront
    
    extern bool sensoresLBumpSide;		//## attribute sensoresLBumpSide
    
    extern int sensoresSumAngulo;		//## attribute sensoresSumAngulo
    
    extern int sensoresSumDistancia;		//## attribute sensoresSumDistancia


void gotoDock();

// Funciones para calcular a donde ir con la camara

void computeCameraApproach();
void computeCameraWithObstacle();

// Funciones sonidos

void reproducirSonidoBloqueado();
void reproducirSonidoDesbloqueado();

// Funciones para calcular a donde ir con el lidar

void computeLidarSectorAngleDistance();

// Variables resultado de computeLidarSectorAngleDistance()
int computedAngle;
int computedDistance;



class UC {
    UC();
    ~UC();

public:

    void statechart_process();

private:
    
    void rootState_entDef();
    void UnDock_entDef();
    void NormalOperate_entDef();
    void NormalOperateEntDef();
    
    void TrackingByCamera_entDef();
    void TrackingByCameraEntDef();
    
    void PersonInView_entDef();
    void PersonOutView_entDef();
    
    void DodgeObstacle_entDef();
    void CliffAhead_entDef();
    
    void CrashAlgorithm_entDef();
    
    void endBehavior();
    
protected:

    void initStatechart();

    void cancelTimeouts();

    enum UC_Enum {
        OMNonState = 0,
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

    int rootState_subState;
    
    int rootState_active;
    
    int UnDock_subState;
    
    int NormalOperate_subState;
    
    int TrackingByCamera_subState;
    
    int PersonOutView_subState;
    
    int PersonInView_subState;
    
    int PersonInView_timeout;
    
    int DodgeObstacle_subState;
    
    int CrashAlgorithm_subState;
    
    int CliffAhead_subState;
    
    
    
};

#endif	/* ROBOTIA_H */

