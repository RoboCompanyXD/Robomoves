
#include "UC.h"

UC::UC() {
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
    PersonInView_timeout = 0;
    DodgeObstacle_subState = OMNonState;
    CrashAlgorithm_subState = OMNonState;
    CliffAhead_subState = OMNonState;
}

UC::statechart_process() {

    switch (rootState_active) {
            // State Initializing
            // Description: Estado de iniciacilación.
            // Mediante la funcion Init se prepara todo el sistema.
        case Initializing:
        {

            //## transition 1 
            if (initStatus == 1) {

                rootState_subState = Idle;
                rootState_active = Idle;

            } else if (initStatus > 1) {

                rootState_subState = InitFailed;
                rootState_active = InitFailed;

            }
            else if (check_btnSpot()) {

                rootState_subState = Shutdown;
                rootState_active = Shutdown;

            }

        }
            break;
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
                res = IdleTakebtnClean();
            } else if (check_btnSpot()) {
                
                rootState_subState = Shutdown;
                rootState_active = Shutdown;
               
            
        }
            break;
    }

}