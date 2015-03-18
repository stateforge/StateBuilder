package com.stateforge.statemachine.listener;

public class ObserverConsole implements IObserver {

    private static ObserverConsole instance = new ObserverConsole();
    
    private ObserverConsole(){};
    
    public static ObserverConsole getInstance(){
        return instance;
    }
    
    public void onEntry(String context, String state) {
        System.out.println(context + ": entering in state " + state);
    }

    public void onExit(String context, String state) {
        System.out.println(context + ": leaving from state " + state);
    }

    public void onTransitionBegin(
            String context,
            String statePrevious,
            String stateNext,
            String transition) {
        System.out.println(context + ": transition begins from state " + statePrevious + " to " + stateNext + ", event " + transition);
    }

    public void onTransitionEnd(
            String context, 
            String statePrevious,
            String stateNext,
            String transition) {
        System.out.println(context + ": transition has ended from state " + statePrevious + " to " + stateNext + ", event " + transition);
    }

    public void onTimerStart(String context, String name, long duration) {
        System.out.println(context + ": start timer " + name + " for " + duration + " msec");
    }

    public void onTimerStop(String context, String name) {
        System.out.println(context + ": start timer " + name);
    }
    
    public void onActionException(String context, Throwable throwable){
    	System.err.println(context + ": exception during action: " + throwable.getMessage());
    }
}
