
package com.stateforge.statemachine.examples.ping;

import com.stateforge.statemachine.state.AbstractState;

public class PingPingState
    extends AbstractState<PingContext, PingPingState>
{

    private final static PingPingState instance = new PingPingState();

    /**
     * Protected Constructor
     * 
     */
    protected PingPingState() {
        setName("Ping");
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
