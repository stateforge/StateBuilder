
package com.stateforge.statemachine.examples.ping;


public class PingIdleState
    extends PingPingState
{
    private final static PingIdleState instance = new PingIdleState();

    /**
     * Protected Constructor
     */
    protected PingIdleState() {
        setName("Idle");
        setStateParent(PingPingState.getInstance());
    }

    /**
     * Get the State Instance
     */
    public static PingPingState getInstance() {
        return instance;
    }

    /**
     * onEntry
     */
    @Override
    public void onEntry(PingContext context) {
        context.getObserver().onEntry(context.getName(), this.getName());
    }

    /**
     * onExit
     */
    @Override
    public void onExit(PingContext context) {
        context.getObserver().onExit(context.getName(), this.getName());
    }

    /**
     * Event id: evStart
     */
    public void evStart(PingContext context) {
        Ping ping = context.getPing();
        // Transition from Idle to SendPingAndWaitForReply triggered by evStart
        // The next state is within the context PingContext
        context.setTransitionName("evStart");
        com.stateforge.statemachine.algorithm.StateOperation.processTransitionBegin(context, PingSendPingAndWaitForReplyState.getInstance());
        com.stateforge.statemachine.algorithm.StateOperation.processTransitionEnd(context, PingSendPingAndWaitForReplyState.getInstance());
        return ;
    }
}
