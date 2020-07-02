
#include "UC.h"

UC::UC(ControlRobot c) {

    this->control = c;

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
    PersonInView_timeout = 0; // BORRAR?
    DodgeObstacle_subState = OMNonState;
    CrashAlgorithm_subState = OMNonState;
    CliffAhead_subState = OMNonState;
    rootState_entDef();
}
void UC::rootState_entDef() {
    {
        rootState_subState = Idle;
        rootState_active = Idle;
    }
}
void UC::UnDock_entDef() {
    rootState_subState = UnDock;
    //#[ transition UnDock.0 
    sensoresSumDistancia = 0;
    //#]
    UnDock_subState = ExitDock;
    rootState_active = ExitDock;
}
void UC::NormalOperate_entDef() {
    rootState_subState = NormalOperate;
    NormalOperateEntDef();
}
void UC::NormalOperateEntDef() {
    TrackingByCamera_entDef();
}
void UC::TrackingByCamera_entDef() {
    NormalOperate_subState = TrackingByCamera;
    TrackingByCameraEntDef();
}
void UC::TrackingByCameraEntDef() {
    //## transition 14 
    if (cameraIsPersonInView == true) {
        PersonInView_entDef();
    } else {
        PersonOutView_entDef();
    }
}
void UC::PersonInView_entDef() {
    TrackingByCamera_subState = PersonInView;
    PersonInView_subState = PersonInView_ApproachUser;
    rootState_active = PersonInView_ApproachUser;
    //#[ state NormalOperate.TrackingByCamera.PersonInView.PersonInView.PersonInView_ApproachUser.(Entry) 
    computeCameraApproach();
    //#]
    //PersonInView_timeout = scheduleTimeout(ComputeStepTime, "ROOT.NormalOperate.TrackingByCamera.PersonInView.ROOT.PersonInView.PersonInView_ApproachUser");
}
void UC::PersonOutView_entDef() {
    TrackingByCamera_subState = PersonOutView;
    //#[ transition NormalOperate.TrackingByCamera.PersonOutView.4 
    sensoresSumAngulo = 0;
    sensoresSumDistancia = 0;
    //#]
    PersonOutView_subState = PersonOutView_Rotate360;
    rootState_active = PersonOutView_Rotate360;
}
void UC::DodgeObstacle_entDef() {
    NormalOperate_subState = DodgeObstacle;
    //#[ transition 9 
    sensoresSumAngulo = 0;
    sensoresSumDistancia = 0;
    //#]
    DodgeObstacle_subState = Dodge_MoveBack;
    rootState_active = Dodge_MoveBack;
}
void UC::CliffAhead_entDef() {
    NormalOperate_subState = CliffAhead;
    //#[ transition 12 
    sensoresSumAngulo = 0;
    sensoresSumDistancia = 0;
    //#]
    CliffAhead_subState = CliffAhead_Rotate180;
    rootState_active = CliffAhead_Rotate180;
}
void UC::CrashAlgorithm_entDef() {
    DodgeObstacle_subState = CrashAlgorithm;
    CrashAlgorithm_subState = CrashAlgorithm_Dodge;
    rootState_active = CrashAlgorithm_Dodge;
}
void UC::statechart_process() {
    
    switch (rootState_active) {
            // State Idle
            // Description: Modo de espera.
            // Se pulsa el boton "Clean" para empezar con el modo normal de operación.
            // El boton "Dock" o un nivel de batería inferior al 15% enviarán el robot a la base de carga.
            // Una vez el robot está en la base de carga se retorna a este estado.
            // En el modo de espera se puede estar cargando o sin cargar
        case Idle:
        {
            if (sensoresBateria < 15 || check_btnDock()) {
                rootState_subState = Dock;
                rootState_active = Dock;
                //#[ state Dock.(Entry) 
                gotoDock();
                //#]
            } else if (check_btnClean()) {
                if (sensoresInDock == true) {
                    UnDock_entDef();
                } else {
                    NormalOperate_entDef();
                }
            } else if (check_btnSpot()) {
                rootState_subState = Shutdown;
                rootState_active = Shutdown;
            }
        }
            break;
            // State Dock
            // Description: Dirigirse a la base.
        case Dock:
        {
            
            //## transition 29 
            if (sensoresInDock == true) {
                rootState_subState = Idle;
                rootState_active = Idle;
            }
        }
            break;
            // State Shutdown
            // Description: Fin de operacion
            // Terminar la ejecucion del algoritmo principal, liberar recursos y salir.
        case Shutdown:
        {
            endBehavior();
        }
            break;
            // State UnDock
            // Description: Salir de la base.
            // Salir marcha atras y dar la vuelta para empezar a funcionar.
        case UnDock:
        {
            if (check_btnSpot()) {
                UnDock_subState = OMNonState;
                rootState_subState = Shutdown;
                rootState_active = Shutdown;
            }
            switch (UnDock_subState) {
                    // State ExitDock
                case ExitDock:
                {
                    //## transition UnDock.1 
                    if (sensoresSumDistancia<-300) {
                        //#[ transition UnDock.1 
                        sensoresSumAngulo = 0;
                        //#]
                        UnDock_subState = UnDock_Rotate180;
                        rootState_active = UnDock_Rotate180;
                    }
                }
                    break;
                    // State UnDock_Rotate180
                case UnDock_Rotate180:
                {
                    
                    //## transition UnDock.2 
                    if (sensoresSumAngulo > 180) {
                        UnDock_subState = OMNonState;
                        NormalOperate_entDef();
                    }
                }
                    break;
                default:
                    break;
            }
        }
            break;
            // State NormalOperate
            // Description: Modo de operacion normal.
        case NormalOperate:
        {
            if (sensoresBateria < 15 || check_btnDock()) {
                //NormalOperate_exit();
                rootState_subState = Dock;
                rootState_active = Dock;
                //#[ state Dock.(Entry) 
                gotoDock();
                //#]
            } else if (check_btnClean()) {
                //NormalOperate_exit();
                rootState_subState = Idle;
                rootState_active = Idle;
            } else if (check_btnSpot()) {
                //NormalOperate_exit();
                rootState_subState = Shutdown;
                rootState_active = Shutdown;
            }
            switch (NormalOperate_subState) {
                    // State TrackingByCamera
                    // Description: Seguimiento mediante la camara.
                case TrackingByCamera:
                {
                    if (sensoresBl == true || sensoresBr == true) {
                        TrackingByCamera_subState = OMNonState;
                        PersonInView_subState = OMNonState;
                        PersonOutView_subState = OMNonState;
                        DodgeObstacle_entDef();
                    } else if (sensoresCliff == true) {
                        TrackingByCamera_subState = OMNonState;
                        PersonInView_subState = OMNonState;
                        PersonOutView_subState = OMNonState;
                        CliffAhead_entDef();
                    }
                    switch (TrackingByCamera_subState) {
                            // State PersonInView
                            // Description: La persona está a la vista.
                            // Acercarse todo lo que se pueda sin invadir su espacio.
                        case PersonInView:
                        {
                            if (cameraIsPersonInView == false) {
                                PersonInView_subState = OMNonState;
                                PersonOutView_entDef();
                            }
                            switch (PersonInView_subState) {
                                    // State PersonInView_ApproachUser
                                    // Description: Acercarse a la persona
                                    // El frente está libre, el robot se acerca a la persona.
                                    // //
                                case PersonInView_ApproachUser:
                                {
                                    //Ejecutar funcion de calculo de aproximacion
                                    // computeCameraApproach();
                                    if (lidarIsObstable == true) {
                                        //#[ transition NormalOperate.TrackingByCamera.PersonInView.1 
                                        reproducirSonidoBloqueado();
                                        //#]
                                        PersonInView_subState = PersonInView_PathBlocked;
                                        rootState_active = PersonInView_PathBlocked;
                                        //#[ state NormalOperate.TrackingByCamera.PersonInView.PersonInView.PersonInView_PathBlocked.(Entry) 
                                        //#]
                                    }
                                }
                                    break;
                                    // State PersonInView_PathBlocked
                                    // Description: Obstaculo en el camino
                                    // El obstaculo puede ser una persona o un onstaculo real
                                    // Puedo rotar si la persona se mueve y retroceder
                                case PersonInView_PathBlocked:
                                {
                                    // Ejecutar funcion de calculo de aproximacion con obstaculo
                                    //computeCameraWithObstacle();
                                    if (lidarIsObstable == false) {
                                        //#[ transition NormalOperate.TrackingByCamera.PersonInView.1 
                                        reproducirSonidoDesbloqueado();
                                        //#]
                                        PersonInView_subState = PersonInView_ApproachUser;
                                        rootState_active = PersonInView_ApproachUser;
                                        //#[ state NormalOperate.TrackingByCamera.PersonInView.PersonInView.PersonInView_PathBlocked.(Entry) 
                                        //#]
                                    }
                                }
                                    break;
                                default:
                                    break;
                            }
                        }
                            break;
                            // State PersonOutView
                            // Description: La persona no está a la vista.
                            // Ejecutar algoritmo de búsqueda.
                        case PersonOutView:
                        {
                            switch (PersonOutView_subState) {
                                    // State PersonOutView_RotateToMove
                                case PersonOutView_RotateToMove:
                                {
                                    if (sensoresSumAngulo > computedAngle) {
                                        PersonOutView_subState = PersonOutView_GoForward;
                                        rootState_active = PersonOutView_GoForward;
                                    }
                                }
                                    break;
                                    // State PersonOutView_ComputePosition
                                    // Description: Computar posicion a la que dirigirse.
                                    // Encontrar sector con mayor media de distancia.
                                case PersonOutView_ComputePosition:
                                {
                                    // Calcular la posicion a la que dirigirse
                                    //#[ transition NormalOperate.TrackingByCamera.PersonOutView.0 
                                    sensoresSumAngulo = 0;
                                    //#]
                                    PersonOutView_subState = PersonOutView_RotateToMove;
                                    rootState_active = PersonOutView_RotateToMove;
                                }
                                    break;
                                    // State PersonOutView_GoForward
                                case PersonOutView_GoForward:
                                {
                                    if (sensoresSumDistancia > computedDistance) {
                                        //#[ transition NormalOperate.TrackingByCamera.PersonOutView.2 
                                        sensoresSumAngulo = 0;
                                        //#]
                                        PersonOutView_subState = PersonOutView_Rotate360;
                                        rootState_active = PersonOutView_Rotate360;
                                    }
                                }
                                    break;
                                    // State PersonOutView_Rotate360
                                case PersonOutView_Rotate360:
                                {
                                    if (sensoresSumAngulo > 360) {
                                        PersonOutView_subState = PersonOutView_ComputePosition;
                                        rootState_active = PersonOutView_ComputePosition;
                                    }
                                }
                                    break;
                                default:
                                    break;
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                    // State DodgeObstacle
                    // Description: Se ha colisionado con un obstaculo.
                    // Rodear el obstaculo y volver a encontrar a la persona.
                case DodgeObstacle:
                {
                    if (sensoresCliff == true) {
                        CrashAlgorithm_subState = OMNonState;
                        DodgeObstacle_subState = OMNonState;
                        CliffAhead_entDef();
                    }
                    switch (DodgeObstacle_subState) { // State Dodge_MoveBack
                        case Dodge_MoveBack:
                        { //## transition 10 
                            if (sensoresSumDistancia<-30) {
                                CrashAlgorithm_entDef();
                            }
                        }
                            break;
                            // State CrashAlgorithm
                            // Description: Algoritmo de rodeo.
                            // (El del TFG)
                        case CrashAlgorithm:
                        {
                            if (sensoresBl == true || sensoresBr == true) {
                                CrashAlgorithm_subState = OMNonState;
                                //#[ transition 11 
                                sensoresSumDistancia = 0;
                                //#]
                                DodgeObstacle_subState = Dodge_MoveBack;
                                rootState_active = Dodge_MoveBack;
                            }
                            switch (CrashAlgorithm_subState) {
                                    // State CrashAlgorithm_Dodge
                                case CrashAlgorithm_Dodge:
                                {
                                    if (sensoresSumAngulo > 25) {
                                        CrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        rootState_active = CrashAlgorithm_DodgeParallel;
                                    }
                                }
                                    break;
                                    // State CrashAlgorithm_DodgeParallel
                                case CrashAlgorithm_DodgeParallel:
                                {
                                    if (sensoresLBumpFront == false) {
                                        CrashAlgorithm_subState = CrashAlgorithm_GoForward;
                                        rootState_active = CrashAlgorithm_GoForward;
                                    }
                                }
                                    break;
                                    // State CrashAlgorithm_GoForward
                                case CrashAlgorithm_GoForward:
                                {
                                    if (sensoresLBumpSide == false) {
                                        //#[ transition NormalOperate.DodgeObstacle.CrashAlgorithm.5 
                                        sensoresSumDistancia = 0;
                                        //#]
                                        CrashAlgorithm_subState = CrashAlgorithm_GoForwardExtended;
                                        rootState_active = CrashAlgorithm_GoForwardExtended;
                                    } else if (sensoresLBumpFront == true) {
                                        CrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        rootState_active = CrashAlgorithm_DodgeParallel;
                                    }
                                }
                                    break;
                                    // State CrashAlgorithm_GoForwardExtended
                                case CrashAlgorithm_GoForwardExtended:
                                {
                                    if (sensoresSumDistancia > 300) {
                                        CrashAlgorithm_subState = CrashAlgorithm_RecoverTrajectory;
                                        rootState_active = CrashAlgorithm_RecoverTrajectory;
                                    } else if (sensoresLBumpFront == false && sensoresLBumpSide == true) {
                                        CrashAlgorithm_subState = CrashAlgorithm_GoForward;
                                        rootState_active = CrashAlgorithm_GoForward;
                                    } else if (sensoresLBumpFront == true) {
                                        CrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        rootState_active = CrashAlgorithm_DodgeParallel;
                                    }
                                }
                                    break;
                                    // State CrashAlgorithm_RecoverTrajectory
                                case CrashAlgorithm_RecoverTrajectory:
                                {
                                    if (sensoresSumAngulo < 25) {
                                        DodgeObstacle_subState = OMNonState;
                                        TrackingByCamera_entDef();
                                    } else if (sensoresLBumpFront == true) {
                                        CrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        rootState_active = CrashAlgorithm_DodgeParallel;
                                    } else if (sensoresLBumpFront == false && sensoresLBumpSide == true) {
                                        CrashAlgorithm_subState = CrashAlgorithm_GoForward;
                                        rootState_active = CrashAlgorithm_GoForward;
                                    }
                                }
                                    break;
                                default:
                                    break;
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                    // State CliffAhead
                    // Description: Barranco detectado.
                    // Devolver el robot a un estado seguro.
                case CliffAhead:
                {
                    switch (CliffAhead_subState) {
                            // State CliffAhead_Rotate180
                        case CliffAhead_Rotate180:
                        {
                            if (sensoresSumAngulo > 180) {
                                CliffAhead_subState = CliffAhead_GoForward;
                                rootState_active = CliffAhead_GoForward;
                            }
                        }
                            break;
                            // State CliffAhead_GoForward
                        case CliffAhead_GoForward:
                        {
                            if (sensoresSumDistancia > 300) {
                                CliffAhead_subState = OMNonState;
                                TrackingByCamera_entDef();
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                default:
                    break;
            }
        }
            break;
    }
}