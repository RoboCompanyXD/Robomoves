/* 
 * File:   RobotIA.h
 * Author: chris
 *
 * Created on 30 de junio de 2020, 10:52
 */

#ifndef UC_H
#define	UC_H

class UC{
    UC();
    ~UC();

public:
    
    inline bool rootState_IN() const;
    inline bool UnDock_IN() const;
    inline bool UnDock_Rotate180_IN() const;
    inline bool ExitDock_IN() const;
    inline bool EndUndoParkHelperState_IN() const;
    inline bool Shutdown_IN() const;
    inline bool NormalOperate_IN() const;
    inline bool TrackingByCamera_IN() const;
    inline bool PersonOutView_IN() const;
    inline bool PersonOutView_RotateToMove_IN() const;
    inline bool PersonOutView_Rotate360_IN() const;
    inline bool PersonOutView_GoForward_IN() const;
    inline bool PersonOutView_ComputePosition_IN() const;
    inline bool PersonInView_IN() const;
    inline bool PersonInView_PathBlocked_IN() const;
    inline bool PersonInView_ApproachUser_IN() const;
    inline bool ComputeStepBlocked_IN() const;
    inline bool ComputeStepApproach_IN() const;
    inline bool DodgeObstacle_IN() const;
    inline bool Dodge_MoveBack_IN() const;
    inline bool CrashAlgorithm_IN() const;
    inline bool CrashAlorithmEndHelperState_IN() const;
    inline bool CrashAlgorithm_RecoverTrajectory_IN() const;
    inline bool CrashAlgorithm_GoForwardExtended_IN() const;
    inline bool CrashAlgorithm_GoForward_IN() const;
    inline bool CrashAlgorithm_DodgeParallel_IN() const;
    inline bool CrashAlgorithm_Dodge_IN() const;
    inline bool CliffAhead_IN() const;
    inline bool CliffAhead_Rotate180_IN() const;
    inline bool CliffAhead_GoForward_IN() const;
    inline bool Initializing_IN() const;
    inline bool InitFailed_IN() const;
    inline bool Idle_IN() const;
    inline bool Dock_IN() const;
    
private:
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
        Dock = 33
    };
    
    int rootState_subState;
    
    int rootState_active;
    
    int UnDock_subState;
    
    int NormalOperate_subState;
    
    int TrackingByCamera_subState;
    
    int PersonOutView_subState;
    
    int PersonInView_timeout;
    
    int PersonInView_subState;
    
    int DodgeObstacle_subState;
    
    int CrashAlgorithm_subState;
    
    int CliffAhead_subState;

};

inline bool UC::rootState_IN() const {
    return true;
}

inline bool UC::UnDock_IN() const {
    return rootState_subState == UnDock;
}

inline bool UC::UnDock_Rotate180_IN() const {
    return UnDock_subState == UnDock_Rotate180;
}

inline bool UC::ExitDock_IN() const {
    return UnDock_subState == ExitDock;
}

inline bool UC::EndUndoParkHelperState_IN() const {
    return UnDock_subState == EndUndoParkHelperState;
}

inline bool UC::Shutdown_IN() const {
    return rootState_subState == Shutdown;
}

inline bool UC::NormalOperate_IN() const {
    return rootState_subState == NormalOperate;
}

inline bool UC::TrackingByCamera_IN() const {
    return NormalOperate_subState == TrackingByCamera;
}

inline bool UC::PersonOutView_IN() const {
    return TrackingByCamera_subState == PersonOutView;
}

inline bool UC::PersonOutView_RotateToMove_IN() const {
    return PersonOutView_subState == PersonOutView_RotateToMove;
}

inline bool UC::PersonOutView_Rotate360_IN() const {
    return PersonOutView_subState == PersonOutView_Rotate360;
}

inline bool UC::PersonOutView_GoForward_IN() const {
    return PersonOutView_subState == PersonOutView_GoForward;
}

inline bool UC::PersonOutView_ComputePosition_IN() const {
    return PersonOutView_subState == PersonOutView_ComputePosition;
}

inline bool UC::PersonInView_IN() const {
    return TrackingByCamera_subState == PersonInView;
}

inline bool UC::PersonInView_PathBlocked_IN() const {
    return PersonInView_subState == PersonInView_PathBlocked;
}

inline bool UC::PersonInView_ApproachUser_IN() const {
    return PersonInView_subState == PersonInView_ApproachUser;
}

inline bool UC::ComputeStepBlocked_IN() const {
    return PersonInView_subState == ComputeStepBlocked;
}

inline bool UC::ComputeStepApproach_IN() const {
    return PersonInView_subState == ComputeStepApproach;
}

inline bool UC::DodgeObstacle_IN() const {
    return NormalOperate_subState == DodgeObstacle;
}

inline bool UC::Dodge_MoveBack_IN() const {
    return DodgeObstacle_subState == Dodge_MoveBack;
}

inline bool UC::CrashAlgorithm_IN() const {
    return DodgeObstacle_subState == CrashAlgorithm;
}

inline bool UC::CrashAlorithmEndHelperState_IN() const {
    return CrashAlgorithm_subState == CrashAlorithmEndHelperState;
}

inline bool UC::CrashAlgorithm_RecoverTrajectory_IN() const {
    return CrashAlgorithm_subState == CrashAlgorithm_RecoverTrajectory;
}

inline bool UC::CrashAlgorithm_GoForwardExtended_IN() const {
    return CrashAlgorithm_subState == CrashAlgorithm_GoForwardExtended;
}

inline bool UC::CrashAlgorithm_GoForward_IN() const {
    return CrashAlgorithm_subState == CrashAlgorithm_GoForward;
}

inline bool UC::CrashAlgorithm_DodgeParallel_IN() const {
    return CrashAlgorithm_subState == CrashAlgorithm_DodgeParallel;
}

inline bool UC::CrashAlgorithm_Dodge_IN() const {
    return CrashAlgorithm_subState == CrashAlgorithm_Dodge;
}

inline bool UC::CliffAhead_IN() const {
    return NormalOperate_subState == CliffAhead;
}

inline bool UC::CliffAhead_Rotate180_IN() const {
    return CliffAhead_subState == CliffAhead_Rotate180;
}

inline bool UC::CliffAhead_GoForward_IN() const {
    return CliffAhead_subState == CliffAhead_GoForward;
}

inline bool UC::Initializing_IN() const {
    return rootState_subState == Initializing;
}

inline bool UC::InitFailed_IN() const {
    return rootState_subState == InitFailed;
}

inline bool UC::Idle_IN() const {
    return rootState_subState == Idle;
}

inline bool UC::Dock_IN() const {
    return rootState_subState == Dock;
}



#endif	/* ROBOTIA_H */

