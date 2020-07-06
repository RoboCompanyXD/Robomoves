/**
 * File:   UC.h
 * Author: chris
 *
 * Created on 30 de junio de 2020, 10:52
 */

#ifndef UC_H
#define	UC_H

//velocidades de las ruedas
#define VELGIR 80
#define VELCERO 0
#define VELAV 150

//maquina de estados antigua (a eliminar)
#define READY 'a'
#define WAIT 'b'
#define MOVCAM 'c'
#define RET 'd'
#define GIR 'e'
#define AV 'f'
#define AVPLUS 'g'
#define GIRPLUS 'h'
#define GIR2 'i'

//9 posibles combinaciones de las ruedas
#define STOP_UC 'r'
#define FWD 's'
#define BACK 't'
#define LEFT 'u'
#define RIGHT 'v'
#define FWD_L 'w'
#define FWD_R 'x'
#define BACK_L 'y'
#define BACK_R 'z'

//#include "../Navegador/ControlRobot.h"
class ControlRobot; // Forward Declaration of Class ControlRobot


extern bool check_btnSpot(); // TODO: documentar
extern bool check_btnClean();
extern bool check_btnDock();

////// TODO: ¿por qué no incluir estos atributos dentro de la definición de la clase UC?

extern bool cameraIsPersonInView; //## attribute cameraIsPersonInView
extern int lidarIsObstable; //## attribute lidarIsObstable
extern int computedAngle; //## attribute computedAngle
extern int computedDistance; //## attribute computedDistance
extern int sensoresbattery_percentage; //## attribute sensoresBateria
extern bool sensoresBl; //## attribute sensoresBl
extern bool sensoresBr; //## attribute sensoresBr
extern bool sensoresCliff; //## attribute sensoresCliff
extern int sensoresIsDocked; //## attribute sensoresInDock
extern bool sensoresLBumpFront; //## attribute sensoresLBumpFront
extern bool sensoresLBumpSide; //## attribute sensoresLBumpSide
extern int sensoresSumAngulo; //## attribute sensoresSumAngulo
extern int sensoresSumDistancia; //## attribute sensoresSumDistancia

/**
 * TODO: Documentar metodo
 */
void gotoDock();

/**
 * TODO: Documentar metodo (mas detallado)
 * Calcular a donde ir con la camara
 */
void computeCameraApproach();

/**
 * TODO: Documentar metodo (mas detallado)
 * Calcular a donde ir con la camara
 */
void computeCameraWithObstacle();


/**
 * Play "blocked" sound
 */
void reproducirSonidoBloqueado();

/**
 * Play "un-blocked" sound
 */
void reproducirSonidoDesbloqueado();


/**
 * Calcular a donde ir con el lidar
 */
void computeLidarTripPersonOutOfView();

//// TODO: ¿por qué no poner los metodos anteriores dentro de la definición de la clase?

/**
 *
 * TODO: documentar clase
 */
class UC {

    

public:
    
    /**
     * Class constructor
     */
    UC();

    /**
     * Class destructor
     */
    ~UC();
    
    UC(ControlRobot * contr);
    
    /**
     * TODO: documentar metodo
     */
    void statechart_process();

private:
    
    ControlRobot * control; // TODO: documentar variable

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

#endif	/* UC_H */

