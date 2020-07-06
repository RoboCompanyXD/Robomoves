/**
 * File:   UC.cpp
 * Author: chris
 *
 * Created on 30 de junio de 2020, 10:52
 */

#include "UC.h"
#include "../Navegador/ControlRobot.h"
#include <stdexcept>

UC::UC() {
}

UC::UC(ControlRobot * contr) {

    this->control = contr;

    initStatechart();
}

void UC::initStatechart() {
    currentSuperState = Disabled; // TODO: change to UC_Enum.OMNonState for better readability
    currentState = Disabled;
    currentUnDock_subState = Disabled;
    currentNormalOperate_subState = Disabled;
    currentTrackingByCamera_subState = Disabled;
    currentPersonOutView_subState = Disabled;
    currentPersonInView_subState = Disabled;
    PersonInView_timeout = 0; // BORRAR?
    currentDodgeObstacle_subState = Disabled;
    currentCrashAlgorithm_subState = Disabled;
    currentCliffAhead_subState = Disabled;
    rootState_entDef();
}

void UC::rootState_entDef() {
    {
        currentSuperState = Idle;
        currentState = Idle;
    }
}

void UC::UnDock_entDef() {
    currentSuperState = UnDock;
    //#[ transition UnDock.0 
    sensoresSumDistancia = 0;
    //#]
    currentUnDock_subState = ExitDock;
    currentState = ExitDock;
}

void UC::NormalOperate_entDef() {
    currentSuperState = NormalOperate;
    NormalOperateEntDef();
}

void UC::NormalOperateEntDef() {
    TrackingByCamera_entDef();
}

void UC::TrackingByCamera_entDef() {
    currentNormalOperate_subState = TrackingByCamera;
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
    currentTrackingByCamera_subState = PersonInView;
    currentPersonInView_subState = PersonInView_ApproachUser;
    currentState = PersonInView_ApproachUser;
    //#[ state NormalOperate.TrackingByCamera.PersonInView.PersonInView.PersonInView_ApproachUser.(Entry) 
    computeCameraApproach();
    //#]
    //PersonInView_timeout = scheduleTimeout(ComputeStepTime, "ROOT.NormalOperate.TrackingByCamera.PersonInView.ROOT.PersonInView.PersonInView_ApproachUser");
}

void UC::PersonOutView_entDef() {
    currentTrackingByCamera_subState = PersonOutView;
    //#[ transition NormalOperate.TrackingByCamera.PersonOutView.4 
    sensoresSumAngulo = 0;
    sensoresSumDistancia = 0;
    //#]
    currentPersonOutView_subState = PersonOutView_Rotate360;
    currentState = PersonOutView_Rotate360;
}

void UC::DodgeObstacle_entDef() {
    currentNormalOperate_subState = DodgeObstacle;
    //#[ transition 9 
    sensoresSumAngulo = 0;
    sensoresSumDistancia = 0;
    //#]
    currentDodgeObstacle_subState = Dodge_MoveBack;
    currentState = Dodge_MoveBack;
}

void UC::CliffAhead_entDef() {
    currentNormalOperate_subState = CliffAhead;
    //#[ transition 12 
    sensoresSumAngulo = 0;
    sensoresSumDistancia = 0;
    //#]
    currentCliffAhead_subState = CliffAhead_Rotate180;
    currentState = CliffAhead_Rotate180;
}

void UC::CrashAlgorithm_entDef() {
    currentDodgeObstacle_subState = CrashAlgorithm;
    currentCrashAlgorithm_subState = CrashAlgorithm_Dodge;
    currentState = CrashAlgorithm_Dodge;
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
    switch (currentState) {

        case Idle:
        {
            // State Idle
            // Description: Modo de espera.
            // Se pulsa el boton "Clean" para empezar con el modo normal de operación.
            // El boton "Dock" o un nivel de batería inferior al 15% enviarán el robot a la base de carga.
            // Una vez el robot está en la base de carga se retorna a este estado.
            // En el modo de espera se puede estar cargando o sin cargar

            if (sensoresbattery_percentage < 15 || check_btnDock()) {
                currentSuperState = Dock;
                currentState = Dock;
                //#[ state Dock.(Entry) 
                gotoDock();
                //#]
                break; // Salir inmediatamente del sub-estado
            } else if (check_btnClean()) {
                if (sensoresIsDocked == true) {
                    UnDock_entDef();
                    break; // Salir inmediatamente del sub-estado
                } else {
                    NormalOperate_entDef();
                    break; // Salir inmediatamente del sub-estado
                }
            } else if (check_btnSpot()) {
                currentSuperState = Shutdown;
                currentState = Shutdown;
                break; // Salir inmediatamente del sub-estado
            }

            break; // end super-case "Idle"
        } //end Idle

        case Dock:
        {
            // State Dock
            // Description: Dirigirse a la base.

            //## transition 29 
            if (sensoresIsDocked == true) {
                currentSuperState = Idle;
                currentState = Idle;
                break; // Salir inmediatamente del sub-estado
            }

            break; // end super-case "Dock"
        } // end Dock

        case Shutdown:
        {
            // State Shutdown
            // Description: Fin de operacion
            // Terminar la ejecucion del algoritmo principal, liberar recursos y salir.
            currentState = Statechart_End;
            currentSuperState = Statechart_End;
            endBehavior();

            break; // end case "Shutdown"
        } // end Shutdown

        case UnDock:
        {
            // State UnDock
            // Description: Salir de la base.
            // Salir marcha atras y dar la vuelta para empezar a funcionar.

            /** Si mientras se está haciendo el Un-Dock pulsamos el boton Spot (apagar) */
            if (check_btnSpot()) {
                
                // Desactivar UnDock substates
                
                currentUnDock_subState = Disabled;
                currentSuperState = Shutdown;
                currentState = Shutdown;
                
                break; // Salida inmediata del subestado UnDock
            }

            /**
             * "UnDock" sub-states (second level):
             *  - ExitDock
             *  - UnDock: Rotate 180º
             */
            switch (currentUnDock_subState) {
                case ExitDock:
                {
                    // State UnDock >> ExitDock
                    control->setMotores_actual(BACK);
                    //## transition UnDock.1 
                    if (sensoresSumDistancia<-300) {
                        //#[ transition UnDock.1 
                        sensoresSumAngulo = 0;
                        //#]
                        currentUnDock_subState = UnDock_Rotate180;
                        currentState = UnDock_Rotate180;
                        break; // Salir inmediatamente del sub-estado
                    }

                    break; // end case "ExitDock"
                } // end ExitDock

                case UnDock_Rotate180:
                {
                    // State UnDock >> Rotate180
                    control->setMotores_actual(LEFT);
                    //## transition UnDock.2 
                    if (sensoresSumAngulo > 180) {
                        currentUnDock_subState = Disabled;
                        NormalOperate_entDef();
                        break; // Salir inmediatamente del sub-estado UnDock >> Rotate180º para entrar en NormalOperate
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
            if (sensoresbattery_percentage < 15 || check_btnDock()) {
                //NormalOperate_exit();
                currentSuperState = Dock;
                currentState = Dock;
                //#[ state Dock.(Entry) 
                gotoDock();
                //#]
                break; // Salir inmediatamente del sub-estado
            } else if (check_btnClean()) {
                //NormalOperate_exit();
                currentSuperState = Idle;
                currentState = Idle;
                break; // Salir inmediatamente del sub-estado
            } else if (check_btnSpot()) {
                //NormalOperate_exit();
                currentSuperState = Shutdown;
                currentState = Shutdown;
                break; // Salir inmediatamente del sub-estado
            }
            

            /**
             * "Normal Operate" sub-states (second level):
             *  - TrackingByCamera
             *  - Dodge Obstacle
             *  - Cliff Ahead
             */
            switch (currentNormalOperate_subState) {
                    // State TrackingByCamera
                    // Description: Seguimiento mediante la camara.
                case TrackingByCamera:
                {
                    // State NormalOperate >> TrackingByCamera

                    // TODO: Resumir Qué se hace en los siguientes if-else:
                    if (sensoresBl == true || sensoresBr == true) {
                        currentTrackingByCamera_subState = Disabled;
                        currentPersonInView_subState = Disabled;
                        currentPersonOutView_subState = Disabled;
                        DodgeObstacle_entDef();
                        break; // Salir inmediatamente del sub-estado
                    } else if (sensoresCliff == true) {
                        currentTrackingByCamera_subState = Disabled;
                        currentPersonInView_subState = Disabled;
                        currentPersonOutView_subState = Disabled;
                        CliffAhead_entDef();

                    }

                    /**
                     * "NormalOperate >> Tracking By Camera" sub-sub-states (third level):
                     *  - Person in view
                     *  - Person out of view
                     */
                    switch (currentTrackingByCamera_subState) {
                        case PersonInView:
                        {
                            // State: NormalOperate >> TrackingByCamera >> PersonInView
                            // Description: La persona está a la vista.
                            // Acercarse todo lo que se pueda sin invadir su espacio.

                            if (cameraIsPersonInView == false) {
                                currentPersonInView_subState = Disabled;
                                PersonOutView_entDef();
                                break; // Salir inmediatamente del sub-estado
                            }

                            /**
                             * "Person in view" sub-sub-sub-states (fourth level):
                             *  - Approach user
                             *  - Path blocked
                             */
                            switch (currentPersonInView_subState) {
                                case PersonInView_ApproachUser:
                                {
                                    // State: NormalOperate >> TrackingByCamera >> PersonInView >> ApproachUser
                                    // Description: Acercarse a la persona
                                    // El frente está libre, el robot se acerca a la persona.
                                    //Ejecutar funcion de calculo de aproximacion
                                    control->computeCamaraApproach();
                                    if (lidarIsObstable == true) {
                                        //#[ transition NormalOperate.TrackingByCamera.PersonInView.1 
                                        reproducirSonidoBloqueado();
                                        //#]
                                        currentPersonInView_subState = PersonInView_PathBlocked;
                                        currentState = PersonInView_PathBlocked;
                                        //#[ state NormalOperate.TrackingByCamera.PersonInView.PersonInView.PersonInView_PathBlocked.(Entry) 
                                        //#]
                                        break; // Salir inmediatamente del sub-estado
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
                                    
                                    control->computeCamaraWithObstacle();
                                    if (lidarIsObstable == false) {
                                        //#[ transition NormalOperate.TrackingByCamera.PersonInView.1 
                                        reproducirSonidoDesbloqueado();
                                        //#]
                                        currentPersonInView_subState = PersonInView_ApproachUser;
                                        currentState = PersonInView_ApproachUser;
                                        //#[ state NormalOperate.TrackingByCamera.PersonInView.PersonInView.PersonInView_PathBlocked.(Entry) 
                                        //#]
                                        break; // Salir inmediatamente del sub-estado
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
                            switch (currentPersonOutView_subState) {
                                case PersonOutView_RotateToMove:
                                {
                                    // State PersonOutView_RotateToMove
                                    control->setMotores_actual(LEFT);
                                    if (sensoresSumAngulo > computedAngle) {
                                        currentPersonOutView_subState = PersonOutView_GoForward;
                                        currentState = PersonOutView_GoForward;
                                        break; // Salir inmediatamente del sub-estado
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
                                    currentPersonOutView_subState = PersonOutView_RotateToMove;
                                    currentState = PersonOutView_RotateToMove;

                                    break; // end case "PersonOutView_ComputePosition"
                                } // end PersonOutView_ComputePosition

                                case PersonOutView_GoForward:
                                {
                                    // State PersonOutView_GoForward
                                    control->setMotores_actual(FWD);
                                    // TODO: resumir lo que se hace en este if
                                    if (sensoresSumDistancia > computedDistance) {
                                        //#[ transition NormalOperate.TrackingByCamera.PersonOutView.2 
                                        sensoresSumAngulo = 0;
                                        //#]
                                        currentPersonOutView_subState = PersonOutView_Rotate360;
                                        currentState = PersonOutView_Rotate360;
                                        break; // Salir inmediatamente del sub-estado
                                    }

                                    break; // end case "PersonOutView_GoForward"
                                } // end PersonOutView_GoForward

                                case PersonOutView_Rotate360:
                                {
                                    // State PersonOutView_Rotate360
                                    control->setMotores_actual(LEFT);

                                    // TODO: resumir lo que se hace dentro de este if
                                    if (sensoresSumAngulo > 360) {
                                        currentPersonOutView_subState = PersonOutView_ComputePosition;
                                        currentState = PersonOutView_ComputePosition;
                                        break; // Salir inmediatamente del sub-estado
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
                        currentCrashAlgorithm_subState = Disabled;
                        currentDodgeObstacle_subState = Disabled;
                        CliffAhead_entDef();
                        break; // Salir inmediatamente del sub-estado
                    }

                    /**
                     * "Dodge Obstacle" sub-sub-states (third level):
                     *  - Move back
                     *  - Crash algorithm
                     */
                    switch (currentDodgeObstacle_subState) { // State Dodge_MoveBack
                        case Dodge_MoveBack:
                        {
                            // State NormalOperate >> DodgeObstacle >> MoveBack
                            control->setMotores_actual(BACK);

                            //## transition 10 
                            if (sensoresSumDistancia<-30) {
                                CrashAlgorithm_entDef();
                                break; // Salir inmediatamente del sub-estado
                            }

                            break; // end case Dodge_MoveBack
                        } // end Dodge_MoveBack

                        case CrashAlgorithm:
                        {
                            // State NormalOperate >> DodgeObstacle >> CrashAlgorithm
                            // Description: Algoritmo de rodeo.
                            // (El del TFG)

                            if (sensoresBl == true || sensoresBr == true) {
                                currentCrashAlgorithm_subState = Disabled;
                                //#[ transition 11 
                                sensoresSumDistancia = 0;
                                //#]
                                currentDodgeObstacle_subState = Dodge_MoveBack;
                                currentState = Dodge_MoveBack;
                                break; // Salir inmediatamente del sub-estado
                            }

                            /**
                             * "Crash Algorithm" sub-sub-sub-states (fourth level):
                             *  - Dodge
                             *  - Dodge Parallel
                             *  - Go Forward
                             *  - Go Forward Extended
                             *  - Recover Trajectory
                             */
                            switch (currentCrashAlgorithm_subState) {
                                case CrashAlgorithm_Dodge:
                                {
                                    // State NormalOperate >> DodgeObstacle >> CrashAlgorithm >> Dodge

                                    if (sensoresSumAngulo > 25) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        currentState = CrashAlgorithm_DodgeParallel;
                                        break; // Salir inmediatamente del sub-estado
                                    }

                                    break; // end case CrashAlgorithm_Dodge
                                } // end CrashAlgorithm_Dodge

                                case CrashAlgorithm_DodgeParallel:
                                {
                                    // State NormalOperate >> DodgeObstacle >> CrashAlgorithm >> DodgeParallel

                                    if (sensoresLBumpFront == false) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_GoForward;
                                        currentState = CrashAlgorithm_GoForward;
                                        break; // Salir inmediatamente del sub-estado
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
                                        currentCrashAlgorithm_subState = CrashAlgorithm_GoForwardExtended;
                                        currentState = CrashAlgorithm_GoForwardExtended;
                                        break; // Salir inmediatamente del sub-estado
                                    } else if (sensoresLBumpFront == true) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        currentState = CrashAlgorithm_DodgeParallel;
                                        break; // Salir inmediatamente del sub-estado
                                    }

                                    break; // end CrashAlgorithm_GoForward
                                } // end CrashAlgorithm_GoForward

                                case CrashAlgorithm_GoForwardExtended:
                                {
                                    // State CrashAlgorithm_GoForwardExtended

                                    if (sensoresSumDistancia > 300) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_RecoverTrajectory;
                                        currentState = CrashAlgorithm_RecoverTrajectory;
                                        break; // Salir inmediatamente del sub-estado
                                    } else if (sensoresLBumpFront == false && sensoresLBumpSide == true) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_GoForward;
                                        currentState = CrashAlgorithm_GoForward;
                                        break; // Salir inmediatamente del sub-estado
                                    } else if (sensoresLBumpFront == true) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        currentState = CrashAlgorithm_DodgeParallel;
                                        break; // Salir inmediatamente del sub-estado
                                    }

                                    break; // end CrashAlgorithm_GoForwardExtended
                                } // end CrashAlgorithm_GoForwardExtended

                                case CrashAlgorithm_RecoverTrajectory:
                                {
                                    // State CrashAlgorithm_RecoverTrajectory
                                    if (sensoresSumAngulo < 25) {
                                        currentDodgeObstacle_subState = Disabled;
                                        TrackingByCamera_entDef();
                                        break; // Salir inmediatamente del sub-estado
                                    } else if (sensoresLBumpFront == true) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        break; // Salir inmediatamente del sub-estado
                                        currentState = CrashAlgorithm_DodgeParallel;
                                    } else if (sensoresLBumpFront == false && sensoresLBumpSide == true) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_GoForward;
                                        currentState = CrashAlgorithm_GoForward;
                                        break; // Salir inmediatamente del sub-estado
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
                    switch (currentCliffAhead_subState) {
                        case CliffAhead_Rotate180:
                        {
                            // State CliffAhead_Rotate180
                            if (sensoresSumAngulo > 180) {
                                currentCliffAhead_subState = CliffAhead_GoForward;
                                currentState = CliffAhead_GoForward;
                                break; // Salir inmediatamente del sub-estado
                            }

                            break; // end CliffAhead_Rotate180
                        } // end CliffAhead_Rotate180

                        case CliffAhead_GoForward:
                        {
                            // State CliffAhead_GoForward
                            if (sensoresSumDistancia > 300) {
                                currentCliffAhead_subState = Disabled;
                                TrackingByCamera_entDef();
                                break; // Salir inmediatamente del sub-estado
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