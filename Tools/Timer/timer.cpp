/**
 * File:   timer.cpp
 * Author: chris
 *
 * Created on 30 de junio de 2020, 12:23
 */

#include "timer.h"
#include <unistd.h>

Timer::Timer(TimerTimeoutHandler * timeHandler) {
    timeOutHandlerImp = timeHandler;
    m_Duration = 0;

    TimerHandler handler_cb = &timeOutHandler;
    createTimer(&timerid, handler_cb);
    //createTimer( &timerid, timeOutHandler );
}

Timer::~Timer() {
    stopTimer(timerid);
}

void Timer::setDuration(long int seconds) {
    m_Duration = seconds;
}

void Timer::start() {
    startTimer(timerid, m_Duration, 3);
}

void Timer::restart() {
    stopTimer(timerid);
    startTimer(timerid, m_Duration, 0);
}

void Timer::stop() {
    stopTimer(timerid);
}

void Timer::createTimer(timer_t *timerid, TimerHandler handler_cb) {
    sigevent sev;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    sched_param parm;

    parm.sched_priority = 255;
    pthread_attr_setschedparam(&attr, &parm);

    sev.sigev_notify_attributes = &attr;
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = handler_cb;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = this;

    timer_create(CLOCKID, &sev, timerid);
}

void Timer::startTimer(timer_t timerid, int startTimeout, int cyclicTimeout) {
    itimerspec its;

    /* Start the timer */
    its.it_value.tv_sec = startTimeout;
    its.it_value.tv_nsec = 0;

    /* for cyclic timer */
    its.it_interval.tv_sec = cyclicTimeout;
    its.it_interval.tv_nsec = 0;

    timer_settime(timerid, 0, &its, NULL);
}

void Timer::stopTimer(timer_t timerid) {
    itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    timer_settime(timerid, 0, &its, NULL);
}

void Timer::timeOutHandler(sigval_t This) {
    Timer * timer = (Timer*) This.sival_ptr;
    timer->callbackWrapper();
}

void Timer::callbackWrapper(void) {
    timeOutHandlerImp->handlerFunction();
    stopTimer(timerid);
}

void TimeTimeoutHandlerImp::handlerFunction(void) {
    cout << "time handler invoked" << endl;
}

/*int main()
{
    TimeTimeoutHandlerImp * timerImp = new TimeTimeoutHandlerImp;
    Timer * timer = new Timer( timerImp );

    timer->setDuration( 5 );
    timer->start();
    sleep( 10 );
}*/