package com.stateforge.statemachine.listener;

public interface IObserver {

    public  void onEntry(
            final String context,
            final String state);

    public  void onExit(
            final String context,
            final String state);   

    void onTransitionBegin(
            final String context, 
            final String statePrevious,
            final String stateNext, 
            final String transition);

    void onTransitionEnd(
            final String context, 
            final String statePrevious,
            final String stateNext, 
            final String transition);
    
    void onTimerStart(
            final String context, 
            final String name, 
            long duration);
    
    void onTimerStop(
            final String context, 
            final String name);
    
    void onActionException(
    		final String context, 
    		final Throwable throwable);
}
