/**
 * File:   timer.h
 * Author: chris
 *
 * Created on 30 de junio de 2020, 12:00
 */

#ifndef TIMERHELPER_H
#define TIMERHELPER_H

#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <iostream>

using namespace std;

#define CLOCKID CLOCK_REALTIME
#define SIG SIGUSR1

typedef void (*TimerHandler)(sigval_t signum); // TODO: Documentar

/**
 * TODO: Documentar clase (breve)
 */
class TimerTimeoutHandler {

public:
    virtual void handlerFunction(void) = 0;
};

/**
 * TODO: Documentar clase (mas extenso)
 */
class Timer {

public:

    /**
     * Class Constructor
     */
    Timer(TimerTimeoutHandler * timeHandler);

    /**
     * Class Destructor
     */
    ~Timer();

    /**
     * TODO: Documentar metodo
     */
    void setDuration(long int seconds);

    /**
     * TODO: Documentar metodo
     */
    void start();

    /**
     * TODO: Documentar metodo
     */
    void restart();

    /**
     * TODO: Documentar metodo
     */
    void timeout();

    /**
     * TODO: Documentar metodo
     */

    /**
     * TODO: Documentar metodo
     */
    void stop();

    /**
     * TODO: Documentar metodo
     */
    void callbackWrapper(void);

    /**
     * TODO: Documentar metodo
     */
    static void timeOutHandler(sigval_t This);

private:

    /**
     * TODO: Documentar metodo
     */
    void createTimer(timer_t *timerid, TimerHandler handler_cb);

    /**
     * TODO: Documentar metodo
     */
    void startTimer(timer_t timerid, int startTimeout, int cyclicTimeout);

    /**
     * TODO: Documentar metodo
     */
    void stopTimer(timer_t timerid);

    long int m_Duration; // TODO: Documentar
    TimerTimeoutHandler * timeOutHandlerImp; // TODO: Documentar
    timer_t timerid; // TODO: Documentar
};

/**
 * Documentar clase (breve)
 */
class TimeTimeoutHandlerImp : public TimerTimeoutHandler {

public:

        /**
     * Class constructor
     */
    TimeTimeoutHandlerImp() {
    }

        /**
     * Class destructor
     */
    ~TimeTimeoutHandlerImp() {
    }

    /**
     * TODO: Documentar metodo
     */
    void handlerFunction(void);
};

#endif /* TIMERHELPER_H */

