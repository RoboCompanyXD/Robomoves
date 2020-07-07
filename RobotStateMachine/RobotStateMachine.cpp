/**
 * File:   UC.cpp
 * Author: chris
 *
 * Created on 30 de junio de 2020, 10:52
 */

#include "RobotStateMachine.h"
#include "../ControlRobot/ControlRobot.h"
#include <stdexcept>


/**
 * RobotStateMachine Class constructor
 * @param cr Pointer to an instance of this system's robot controller interface.
 */
RobotStateMachine::RobotStateMachine(ControlRobot::ControlRobot * cr) {
    this->robot = cr;
    initStatechart();
}

/**
 * RobotStateMachine Class destructor
 */
RobotStateMachine::~RobotStateMachine(){
    
}

/**
 * TODO: documentar
 */
void RobotStateMachine::initStatechart() {
    currentSuperState = Disabled; // TODO: change to UC_Enum.OMNonState for better readability
    currentState = Disabled;
    currentUnDock_subState = Disabled;
    currentNormalOperate_subState = Disabled;
    currentTrackingByCamera_subState = Disabled;
    currentPersonOutView_subState = Disabled;
    currentPersonInView_subState = Disabled;
    PersonInView_timeout = 0; // TODO: BORRAR?
    currentDodgeObstacle_subState = Disabled;
    currentCrashAlgorithm_subState = Disabled;
    currentCliffAhead_subState = Disabled;
    rootState_entDef();
}

/**
 * TODO: documentar
 */
void RobotStateMachine::rootState_entDef() {
    {
        currentSuperState = Idle;
        currentState = Idle;
    }
}

/**
 * TODO: documentar
 */
void RobotStateMachine::UnDock_entDef() {
    currentSuperState = UnDock;
    //#[ transition UnDock.0 
    robot->sensores.sum_distance = 0;
    //#]
    currentUnDock_subState = ExitDock;
    currentState = ExitDock;
}

/**
 * TODO: documentar
 */
void RobotStateMachine::NormalOperate_entDef() {
    currentSuperState = NormalOperate;
    NormalOperateEntDef();
}

/**
 * TODO: documentar
 */
void RobotStateMachine::NormalOperateEntDef() {
    TrackingByCamera_entDef();
}

/**
 * TODO: documentar
 */
void RobotStateMachine::TrackingByCamera_entDef() {
    currentNormalOperate_subState = TrackingByCamera;
    TrackingByCameraEntDef();
}

/**
 * TODO: documentar
 */
void RobotStateMachine::TrackingByCameraEntDef() {
    //## transition 14 
    if (robot->cam.isPersonInView == true) {
        PersonInView_entDef();
    } else {
        PersonOutView_entDef();
    }
}

/**
 * TODO: documentar
 */
void RobotStateMachine::PersonInView_entDef() {
    currentTrackingByCamera_subState = PersonInView;
    currentPersonInView_subState = PersonInView_ApproachUser;
    currentState = PersonInView_ApproachUser;
    //#[ state NormalOperate.TrackingByCamera.PersonInView.PersonInView.PersonInView_ApproachUser.(Entry) 
    robot->computeCameraApproach();
    //#]
    //PersonInView_timeout = scheduleTimeout(ComputeStepTime, "ROOT.NormalOperate.TrackingByCamera.PersonInView.ROOT.PersonInView.PersonInView_ApproachUser");
}

/**
 * TODO: documentar
 */
void RobotStateMachine::PersonOutView_entDef() {
    currentTrackingByCamera_subState = PersonOutView;
    //#[ transition NormalOperate.TrackingByCamera.PersonOutView.4 
    robot->sensores.sum_angle = 0;
    robot->sensores.sum_distance = 0;
    //#]
    currentPersonOutView_subState = PersonOutView_Rotate360;
    currentState = PersonOutView_Rotate360;
}

/**
 * TODO: documentar
 */
void RobotStateMachine::DodgeObstacle_entDef() {
    currentNormalOperate_subState = DodgeObstacle;
    //#[ transition 9 
    robot->sensores.sum_angle = 0;
    robot->sensores.sum_distance = 0;
    //#]
    currentDodgeObstacle_subState = Dodge_MoveBack;
    currentState = Dodge_MoveBack;
}

/**
 * TODO: documentar
 */
void RobotStateMachine::CliffAhead_entDef() {
    currentNormalOperate_subState = CliffAhead;
    //#[ transition 12 
    robot->sensores.sum_angle = 0;
    robot->sensores.sum_distance = 0;
    //#]
    currentCliffAhead_subState = CliffAhead_Rotate180;
    currentState = CliffAhead_Rotate180;
}

/**
 * TODO: documentar
 */
void RobotStateMachine::CrashAlgorithm_entDef() {
    currentDodgeObstacle_subState = CrashAlgorithm;
    currentCrashAlgorithm_subState = CrashAlgorithm_Dodge;
    currentState = CrashAlgorithm_Dodge;
}

/**
 * TODO: documentar
 */
void RobotStateMachine::statechart_process() {

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

            if (robot->sensores.battery_percentage < 15 || robot->check_btnDock()) {
                currentSuperState = Dock;
                currentState = Dock;
                //#[ state Dock.(Entry) 
                robot->gotoDock();
                //#]
                break; // Salir inmediatamente del sub-estado
            } else if (robot->check_btnClean()) {
                if (robot->sensores.IsDocked == true) {
                    UnDock_entDef();
                    break; // Salir inmediatamente del sub-estado
                } else {
                    NormalOperate_entDef();
                    break; // Salir inmediatamente del sub-estado
                }
            } else if (robot->check_btnSpot()) {
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
            if (robot->sensores.IsDocked == true) {
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
            if (robot->check_btnSpot()) {
                
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
                    robot->setMotores_actual(motores_BACK);
                    //## transition UnDock.1 
                    if (robot->sensores.sum_distance<-300) {
                        //#[ transition UnDock.1 
                        robot->sensores.sum_angle = 0;
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
                    robot->setMotores_actual(motores_LEFT);
                    //## transition UnDock.2 
                    if (robot->sensores.sum_angle > 180) {
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
            if (robot->sensores.battery_percentage < 15 || robot->check_btnDock()) {
                //NormalOperate_exit();
                currentSuperState = Dock;
                currentState = Dock;
                //#[ state Dock.(Entry) 
                robot->gotoDock();
                //#]
                break; // Salir inmediatamente del sub-estado
            } else if (robot->check_btnClean()) {
                //NormalOperate_exit();
                currentSuperState = Idle;
                currentState = Idle;
                break; // Salir inmediatamente del sub-estado
            } else if (robot->check_btnSpot()) {
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
                    if (robot->sensores.bl == true || robot->sensores.br == true) {
                        currentTrackingByCamera_subState = Disabled;
                        currentPersonInView_subState = Disabled;
                        currentPersonOutView_subState = Disabled;
                        DodgeObstacle_entDef();
                        break; // Salir inmediatamente del sub-estado
                    } else if (robot->sensores.cliff == true) {
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

                            if (robot->cam.isPersonInView == false) {
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
                                    robot->computeCameraApproach(); 
                                    if (robot->lidar.isObstable == true) {
                                        //#[ transition NormalOperate.TrackingByCamera.PersonInView.1 
                                        robot->reproducirSonidoBloqueado();
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
                                    robot->computeCameraWithObstacle();
                                    if (robot->lidar.isObstable == false) {
                                        //#[ transition NormalOperate.TrackingByCamera.PersonInView.1 
                                        robot->reproducirSonidoDesbloqueado();
                                        //#]
                                        currentPersonInView_subState = PersonInView_ApproachUser;
                                        currentState = PersonInView_ApproachUser;
                                        //#[ state NormalOperate.TrackingByCamera.PersonInView.PersonInView.PersonInView_Pathblocked.(Entry) 
                                        //#]
                                        break; // Salir inmediatamente del sub-estado
                                    }

                                    break; // end case "PersonInView_Pathblocked"
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
                                    robot->setMotores_actual(motores_LEFT);
                                    if (robot->sensores.sum_angle > robot->lidar.computedAngle) {
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
                                    robot->sensores.sum_angle = 0;
                                    robot->lidar.computeLidarTripPersonOutOfView();
                                    //#]
                                    currentPersonOutView_subState = PersonOutView_RotateToMove;
                                    currentState = PersonOutView_RotateToMove;

                                    break; // end case "PersonOutView_ComputePosition"
                                } // end PersonOutView_ComputePosition

                                case PersonOutView_GoForward:
                                {
                                    // State PersonOutView_GoForward
                                    robot->setMotores_actual(motores_FWD);
                                    // TODO: resumir lo que se hace en este if
                                    if (robot->sensores.sum_distance > robot->lidar.computedDistance) {
                                        //#[ transition NormalOperate.TrackingByCamera.PersonOutView.2 
                                        robot->sensores.sum_angle = 0;
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
                                    robot->setMotores_actual(motores_LEFT);
                                    // TODO: resumir lo que se hace dentro de este if
                                    if (robot->sensores.sum_angle > 360) {
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

                    if (robot->sensores.cliff == true) {
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
                            robot->setMotores_actual(motores_BACK);
                            //## transition 10 
                            if (robot->sensores.sum_distance<-30) {
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

                            if (robot->sensores.bl == true || robot->sensores.br == true) {
                                currentCrashAlgorithm_subState = Disabled;
                                //#[ transition 11 
                                robot->sensores.sum_distance = 0;
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
                                    robot->setMotores_actual(motores_LEFT);
                                    if (robot->sensores.sum_angle > 25) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        currentState = CrashAlgorithm_DodgeParallel;
                                        break; // Salir inmediatamente del sub-estado
                                    }

                                    break; // end case CrashAlgorithm_Dodge
                                } // end CrashAlgorithm_Dodge

                                case CrashAlgorithm_DodgeParallel:
                                {
                                    // State NormalOperate >> DodgeObstacle >> CrashAlgorithm >> DodgeParallel
                                    robot->setMotores_actual(motores_LEFT);
                                    if (robot->sensores.lbump_front == false) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_GoForward;
                                        currentState = CrashAlgorithm_GoForward;
                                        break; // Salir inmediatamente del sub-estado
                                    }

                                    break; // end CrashAlgorithm_DodgeParallel
                                } // end CrashAlgorithm_DodgeParallel

                                case CrashAlgorithm_GoForward:
                                {
                                    // State NormalOperate >> DodgeObstacle >> CrashAlgorithm >> GoForward
                                    robot->setMotores_actual(motores_FWD);
                                    if (robot->sensores.lbump_side == false) {
                                        //#[ transition NormalOperate.DodgeObstacle.CrashAlgorithm.5 
                                        robot->sensores.sum_distance = 0;
                                        //#]
                                        currentCrashAlgorithm_subState = CrashAlgorithm_GoForwardExtended;
                                        currentState = CrashAlgorithm_GoForwardExtended;
                                        break; // Salir inmediatamente del sub-estado
                                    } else if (robot->sensores.lbump_front == true) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        currentState = CrashAlgorithm_DodgeParallel;
                                        break; // Salir inmediatamente del sub-estado
                                    }

                                    break; // end CrashAlgorithm_GoForward
                                } // end CrashAlgorithm_GoForward

                                case CrashAlgorithm_GoForwardExtended:
                                {
                                    // State CrashAlgorithm_GoForwardExtended
                                    robot->setMotores_actual(motores_FWD);
                                    if (robot->sensores.sum_distance > 300) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_RecoverTrajectory;
                                        currentState = CrashAlgorithm_RecoverTrajectory;
                                        break; // Salir inmediatamente del sub-estado
                                    } else if (robot->sensores.lbump_front == false && robot->sensores.lbump_side == true) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_GoForward;
                                        currentState = CrashAlgorithm_GoForward;
                                        break; // Salir inmediatamente del sub-estado
                                    } else if (robot->sensores.lbump_front == true) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        currentState = CrashAlgorithm_DodgeParallel;
                                        break; // Salir inmediatamente del sub-estado
                                    }

                                    break; // end CrashAlgorithm_GoForwardExtended
                                } // end CrashAlgorithm_GoForwardExtended

                                case CrashAlgorithm_RecoverTrajectory:
                                {
                                    // State CrashAlgorithm_RecoverTrajectory
                                    robot->setMotores_actual(motores_RIGHT);
                                    if (robot->sensores.sum_angle < 25 && robot->sensores.sum_angle > -25) {
                                        currentDodgeObstacle_subState = Disabled;
                                        TrackingByCamera_entDef();
                                        break; // Salir inmediatamente del sub-estado
                                    } else if (robot->sensores.lbump_front == true) {
                                        currentCrashAlgorithm_subState = CrashAlgorithm_DodgeParallel;
                                        break; // Salir inmediatamente del sub-estado
                                        currentState = CrashAlgorithm_DodgeParallel;
                                    } else if (robot->sensores.lbump_front == false && robot->sensores.lbump_side == true) {
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
                            if (robot->sensores.sum_angle > 180) {
                                currentCliffAhead_subState = CliffAhead_GoForward;
                                currentState = CliffAhead_GoForward;
                                break; // Salir inmediatamente del sub-estado
                            }

                            break; // end CliffAhead_Rotate180
                        } // end CliffAhead_Rotate180

                        case CliffAhead_GoForward:
                        {
                            // State CliffAhead_GoForward
                            if (robot->sensores.sum_distance > 300) {
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

/**
 * TODO: documentar
 */
void RobotStateMachine::endBehavior(){
    
}