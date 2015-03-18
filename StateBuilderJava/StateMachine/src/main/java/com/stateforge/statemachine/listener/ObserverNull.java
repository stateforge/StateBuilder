package com.stateforge.statemachine.listener;

public class ObserverNull implements IObserver {

    private static ObserverNull instanceNull = new ObserverNull();
    
    private ObserverNull(){};
    
    public static ObserverNull getInstance(){
        return instanceNull;
    }
    
    public void onEntry(String context, String state) {
    }

    public void onExit(String context, String state) {
    }

    public void onTransitionBegin(String context, String statePrevious,
            String stateNext, String transition) {
    }

    public void onTransitionEnd(String context, String statePrevious,
            String stateNext, String transition) {
    }

    public void onTimerStart(String context, String name, long duration) {
    }

    public void onTimerStop(String context, String name) {
    }
    
    public void onActionException(String context, Throwable throwable){
    	System.err.println(context + ": exception during action: " + throwable.getMessage());
    }
}
