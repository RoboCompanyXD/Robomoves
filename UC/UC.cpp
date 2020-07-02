/**
 * File:   UC.cpp
 * Author: chris
 *
 * Created on 30 de junio de 2020, 10:52
 */

#include "UC.h"
#include <stdexcept>

UC::UC(ControlRobot c) {

    this->control = c;

    initStatechart();
}

void UC::initStatechart() {
    rootState_subState = OMNonState; // TODO: change to UC_Enum.OMNonState for better readability
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

    /** 
     * First level "super-case" machine states: 
     *  - Idle
     *  - Dock
     *  - Shutdown
     *  - Undock
     *  - Normal Operate
     */
    switch (rootState_active) {

        case Idle:
        {
            // State Idle
            // Description: Modo de espera.
            // Se pulsa el boton "Clean" para empezar con el modo normal de operación.
            // El boton "Dock" o un nivel de batería inferior al 15% enviarán el robot a la base de carga.
            // Una vez el robot está en la base de carga se retorna a este estado.
            // En el modo de espera se puede estar cargando o sin cargar

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

            break; // end super-case "Idle"
        } //end Idle

        case Dock:
        {
            // State Dock
            // Description: Dirigirse a la base.

            //## transition 29 
            if (sensoresInDock == true) {
                rootState_subState = Idle;
                rootState_active = Idle;
            }

            break; // end super-case "Dock"
        } // end Dock

        case Shutdown:
        {
            // State Shutdown
            // Description: Fin de operacion
            // Terminar la ejecucion del algoritmo principal, liberar recursos y salir.

            endBehavior();

            break; // end case "Shutdown"
        } // end Shutdown

        case UnDock:
        {
            // State UnDock
            // Description: Salir de la base.
            // Salir marcha atras y dar la vuelta para empezar a funcionar.

            if (check_btnSpot()) {
                UnDock_subState = OMNonState;
                rootState_subState = Shutdown;
                rootState_active = Shutdown;
            }

            /**
             * "UnDock" sub-states (second level):
             *  - ExitDock
             *  - UnDock: Rotate 180º
             */
            switch (UnDock_subState) {
                case ExitDock:
                {
                    // State UnDock >> ExitDock

                    //## transition UnDock.1 
                    if (sensoresSumDistancia<-300) {
                        //#[ transition UnDock.1 
                        sensoresSumAngulo = 0;
                        //#]
                        UnDock_subState = UnDock_Rotate180;
                        rootState_active = UnDock_Rotate180;
                    }

                    break; // end case "ExitDock"
                } // end ExitDock

                case UnDock_Rotate180:
                {
                    // State UnDock >> Rotate180

                    //## transition UnDock.2 
                    if (sensoresSumAngulo > 180) {
                        UnDock_subState = OMNonState;
                        NormalOperate_entDef();
                    }

                    break; // end case "UnDock_Rotate180"
                } // end UnDock_Rotate180

                default:
                    throw std::logic_error("Invalid switch-case of the Robomoves state-machine");
                    break;
            } // end "UnDock" (second level) sub-states

            break; // end super-case "UnDock"
        } // end Undock

        case NormalOperate:
        {
            // State NormalOperate
            // Description: Modo de operacion normal.

            // TODO: Resumir Qué se hace en los siguientes if-else:
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

            /**
             * "Normal Operate" sub-states (second level):
             *  - TrackingByCamera
             *  - Dodge Obstacle
             *  - Cliff Ahead
             */
            switch (NormalOperate_subState) {
                    // State TrackingByCamera
                    // Description: Seguimiento mediante la camara.
                case TrackingByCamera:
                {
                    // State NormalOperate >> TrackingByCamera

                    // TODO: Resumir Qué se hace en los siguientes if-else:
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

                    /**
                     * "NormalOperate >> Tracking By Camera" sub-sub-states (third level):
                     *  - Person in view
                     *  - Person out of view
                     */
                    switch (TrackingByCamera_subState) {
                        case PersonInView:
                        {
                            // State: NormalOperate >> TrackingByCamera >> PersonInView
                            // Description: La persona está a la vista.
                            // Acercarse todo lo que se pueda sin invadir su espacio.

                            if (cameraIsPersonInView == false) {
                                PersonInView_subState = OMNonState;
                                PersonOutView_entDef();
                            }

                            /**
                             * "Person in view" sub-sub-sub-states (fourth level):
                             *  - Approach user
                             *  - Path blocked
                             */
                            switch (PersonInView_subState) {
                                case PersonInView_ApproachUser:
                                {
                                    // State: NormalOperate >> TrackingByCamera >> PersonInView >> ApproachUser
                                    // Description: Acercarse a la persona
                                    // El frente está libre, el robot se acerca a la persona.

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

                                    break; // end case "PersonInView_ApproachUser"
                                } // end ApproachUser

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

                                    break; // end case "PersonInView_PathBlocked"
                                } // end PathLocked

                                default:
                                    throw std::logic_error("Invalid switch-case of the Robomoves state-machine");
                                    break;
                            } // end switch "PersonInView_subState"
                            break; // end case "PersonInView"
                        } // end "PersonInView"

                        case PersonOutView:
                        {
                            // State PersonOutView
                            // Description: La persona no está a la vista.
                            // Ejecutar algoritmo de búsqueda.

                            /**
                             * "Person out of view" sub-sub-sub-states (fourth level):
                             *  - Rotate to move
                             *  - Compute position
                             *  - Go forward
                             *  - Rotate 360º
                             */
                            switch (PersonOutView_subState) {
                                case PersonOutView_RotateToMove:
                                {
                                    // State PersonOutView_RotateToMove

                                    if (sensoresSumAngulo > computedAngle) {
                                        PersonOutView_subState = PersonOutView_GoForward;
                                        rootState_active = PersonOutView_GoForward;
                                    }

                                    break; // end case "PersonOutView_RotateToMove"
                                } // end PersonOutView_RotateToMove

                                case PersonOutView_ComputePosition:
                                {
                                    // State PersonOutView_ComputePosition
                                    // Description: Computar posicion a la que dirigirse.
                                    // Encontrar sector con mayor media de distancia.

                                    // Calcular la posicion a la que dirigirse
                                    //#[ transition NormalOperate.TrackingByCamera.PersonOutView.0 
                                    sensoresSumAngulo = 0;
                                    //#]
                                    PersonOutView_subState = PersonOutView_RotateToMove;
                                    rootState_active = PersonOutView_RotateToMove;

                                    break; // end case "PersonOutView_ComputePosition"
                                } // end PersonOutView_ComputePosition

                                case PersonOutView_GoForward:
                                {
                                    // State PersonOutView_GoForward

                                    // TODO: resumir lo que se hace en este if
                                    if (sensoresSumDistancia > computedDistance) {
                                        //#[ transition NormalOperate.TrackingByCamera.PersonOutView.2 
                                        sensoresSumAngulo = 0;
                                        //#]
                                        PersonOutView_subState = PersonOutView_Rotate360;
                                        rootState_active = PersonOutView_Rotate360;
                                    }

                                    break; // end case "PersonOutView_GoForward"
                                } // end PersonOutView_GoForward

                                case PersonOutView_Rotate360:
                                {
                                    // State PersonOutView_Rotate360

                                    // TODO: resumir lo que se hace dentro de este if
                                    if (sensoresSumAngulo > 360) {
                                        PersonOutView_subState = PersonOutView_ComputePosition;
                                        rootState_active = PersonOutView_ComputePosition;
                                    }

                                    break; // end case "PersonOutView_GoForward"
                                } // end PersonOutView_GoForward

                                default:
                                    throw std::logic_error("Invalid switch-case of the Robomoves state-machine");
                                    break;
                            } //end switch (PersonOutView_subState)

                            break; // end case PersonOutView
                        } //end PersonOutView

                        default:
                            throw std::logic_error("Invalid switch-case of the Robomoves state-machine");
                            break;
                    } // end switch (TrackingByCamera_subState)

                    break; // end case "TrackingByCamera"
                } // end TrackingByCamera

                case DodgeObstacle:
                {
                    // State NormalOperate >> DodgeObstacle
                    // Description: Se ha colisionado con un obstaculo.
                    // Rodear el obstaculo y volver a encontrar a la persona.

                    if (sensoresCliff == true) {
                        CrashAlgorithm_subState = OMNonState;
                        DodgeObstacle_subState = OMNonState;
                        CliffAhead_entDef();
                    }

                    /**
                     * "Dodge Obstacle" sub-sub-states (third level):
                     *  - Move back
                     *  - Crash algorithm
                     */
                    switch (DodgeObstacle_subState) { // State Dodge_MoveBack
                        case Dodge_MoveBack:
                        {
                            // State NormalOperate >> DodgeObstacle >> MoveBack

                            //## transition 10 
                            if (sensoresSumDistancia<-30) {
                                CrashAlgorithm_entDef();
                            }

                            break; // end case Dodge_MoveBack
                        } // end Dodge_MoveBack

                        case CrashAlgorithm:
                        {
                            // State NormalOperate >> DodgeObstacle >> CrashAlgorithm
                            // Description: Algoritmo de rodeo.
                            // (El del TFG)

                            if (sensoresBl == true || sensoresBr == true) {
                                CrashAlgorithm_subState = OMNonState;
                                //#[ transition 11 
                                sensoresSumDistancia = 0;
                                //#]
                                DodgeObstacle_subState = Dodge_MoveBack;
                                rootState_active = Dodge_MoveBack;
                            }

                            /**
                             * "Crash Algorithm" sub-sub-sub-states (fourth level):
                             *  - Dodge
                             *  - Dodge Parallel
                             *  - Go Forward
                             *  - Go Forward Extended
                             *  - Recover Trajectory
                             */
                            switch (CrashAlgorithm_subState) {
                                case CrashAlgorithm_Dodge:
                                {
                                    // State NormalOperate >> DodgeObstacle >> CrashAlgorithm >> Dodge

                                    if (sensoresSumAngulo > 25) {
                                        CrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        rootState_active = CrashAlgorithm_DodgeParallel;
                                    }

                                    break; // end case CrashAlgorithm_Dodge
                                } // end CrashAlgorithm_Dodge

                                case CrashAlgorithm_DodgeParallel:
                                {
                                    // State NormalOperate >> DodgeObstacle >> CrashAlgorithm >> DodgeParallel

                                    if (sensoresLBumpFront == false) {
                                        CrashAlgorithm_subState = CrashAlgorithm_GoForward;
                                        rootState_active = CrashAlgorithm_GoForward;
                                    }

                                    break; // end CrashAlgorithm_DodgeParallel
                                } // end CrashAlgorithm_DodgeParallel

                                case CrashAlgorithm_GoForward:
                                {
                                    // State NormalOperate >> DodgeObstacle >> CrashAlgorithm >> GoForward

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

                                    break; // end CrashAlgorithm_GoForward
                                } // end CrashAlgorithm_GoForward

                                case CrashAlgorithm_GoForwardExtended:
                                {
                                    // State CrashAlgorithm_GoForwardExtended

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

                                    break; // end CrashAlgorithm_GoForwardExtended
                                } // end CrashAlgorithm_GoForwardExtended

                                case CrashAlgorithm_RecoverTrajectory:
                                {
                                    // State CrashAlgorithm_RecoverTrajectory
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

                                    break; // end case CrashAlgorithm_RecoverTrajectory
                                } // end CrashAlgorithm_RecoverTrajectory

                                default:
                                    throw std::logic_error("Invalid switch-case of the Robomoves state-machine");
                                    break;

                            } // end switch (CrashAlgorithm_subState)

                            break; // end case CrashAlgorithm:
                        } // end CrashAlgorithm:

                        default:
                            throw std::logic_error("Invalid switch-case of the Robomoves state-machine");
                            break;
                    } // end switch DodgeObstacle_subState

                    break; // end case DodgeObstacle
                }// end DodgeObstacle

                case CliffAhead:
                {
                    // State CliffAhead
                    // Description: Barranco detectado.
                    // Devolver el robot a un estado seguro.

                    /**
                     * "Cliff Ahead" sub-states:
                     *  - Rotate 180º
                     *  - Go Forward
                     */
                    switch (CliffAhead_subState) {
                        case CliffAhead_Rotate180:
                        {
                            // State CliffAhead_Rotate180
                            if (sensoresSumAngulo > 180) {
                                CliffAhead_subState = CliffAhead_GoForward;
                                rootState_active = CliffAhead_GoForward;
                            }

                            break; // end CliffAhead_Rotate180
                        } // end CliffAhead_Rotate180

                        case CliffAhead_GoForward:
                        {
                            // State CliffAhead_GoForward
                            if (sensoresSumDistancia > 300) {
                                CliffAhead_subState = OMNonState;
                                TrackingByCamera_entDef();
                            }

                            break; // end CliffAhead_GoForward
                        } // end CliffAhead_GoForward

                        default:
                            throw std::logic_error("Invalid switch-case of the Robomoves state-machine");
                            break;

                    } // end switch (CliffAhead_subState)

                    break; // end case CliffAhead
                }

                default:
                    throw std::logic_error("Invalid switch-case of the Robomoves state-machine");
                    break;

            } //end switch (NormalOperate_subState)

            break; // end case "NormalOperate"
        } // end NormalOperate

        default:
            throw std::logic_error("Invalid switch-case of the Robomoves state-machine");
            break;

    } // end TOP-level switch (rootState_active)

} // end void UC::statechart_process()