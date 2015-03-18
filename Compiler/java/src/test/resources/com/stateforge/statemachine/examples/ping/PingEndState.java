
package com.stateforge.statemachine.examples.ping;


public class PingEndState
    extends PingPingState
{

    private final static PingEndState instance = new PingEndState();

    /**
     * Protected Constructor
     * 
     */
    protected PingEndState() {
        setName("End");
        setStateParent(PingPingState.getInstance());
    }

    /**
     * Get the State Instance
     * 
     */
    public static PingPingState getInstance() {
        return instance;
    }

    /**
     * onEntry
     * 
     */
    @Override
    public void onEntry(PingContext context) {
        context.getObserver().onEntry(context.getName(), this.getName());
    }

    /**
     * onExit
     * 
     */
    @Override
    public void onExit(PingContext context) {
        context.getObserver().onExit(context.getName(), this.getName());
    }

    /**
     * Event id: evStart
     * 
     */
    public void evStart(PingContext context) {
    }

    /**
     * Event id: evCancel
     * 
     */
    public void evCancel(PingContext context) {
    }

    /**
     * Event id: evError
     * 
     */
    public void evError(PingContext context) {
    }

    /**
     * Event id: evPingReply
     * 
     */
    public void evPingReply(PingContext context) {
    }

    /**
     * Event id: evTimer
     * 
     */
    public void evTimer(PingContext context) {
    }

}
