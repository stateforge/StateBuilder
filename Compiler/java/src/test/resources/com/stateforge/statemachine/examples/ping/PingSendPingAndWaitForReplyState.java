
package com.stateforge.statemachine.examples.ping;


public class PingSendPingAndWaitForReplyState
    extends PingPingState
{
    private final static PingSendPingAndWaitForReplyState instance = new PingSendPingAndWaitForReplyState();

    /**
     * Protected Constructor
     */
    protected PingSendPingAndWaitForReplyState() {
        setName("SendPingAndWaitForReply");
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
        Ping ping = context.getPing();
        ping.send();
        context.timerStartTimer((ping.getTimeout()));
    }

    /**
     * onExit
     */
    @Override
    public void onExit(PingContext context) {
        context.getObserver().onExit(context.getName(), this.getName());
        Ping ping = context.getPing();
        context.timerStopTimer();
    }

    /**
     * Event id: evCancel
     */
    public void evCancel(PingContext context) {
        Ping ping = context.getPing();
        // Transition from SendPingAndWaitForReply to End triggered by evCancel
        // The next state is within the context PingContext
        context.setTransitionName("evCancel");
        com.stateforge.statemachine.algorithm.StateOperation.processTransitionBegin(context, PingEndState.getInstance());
        com.stateforge.statemachine.algorithm.StateOperation.processTransitionEnd(context, PingEndState.getInstance());
        context.onEnd();
        return ;
    }

    /**
     * Event id: evPingReply
     */
    public void evPingReply(PingContext context) {
        Ping ping = context.getPing();
        // Transition from SendPingAndWaitForReply to End triggered by evPingReply[ping.getTx() == ping.getCount()]
        // The next state is within the context PingContext
        if ((ping.getTx() == ping.getCount())) {
            context.setTransitionName("evPingReply[ping.getTx() == ping.getCount()]");
            com.stateforge.statemachine.algorithm.StateOperation.processTransitionBegin(context, PingEndState.getInstance());
            com.stateforge.statemachine.algorithm.StateOperation.processTransitionEnd(context, PingEndState.getInstance());
            context.onEnd();
            return ;
        }
    }

    /**
     * Event id: evTimer
     */
    public void evTimer(PingContext context) {
        Ping ping = context.getPing();
        // Transition from SendPingAndWaitForReply to End triggered by evTimer[ping.getTx() == ping.getCount()]
        // The next state is within the context PingContext
        if ((ping.getTx() == ping.getCount())) {
            context.setTransitionName("evTimer[ping.getTx() == ping.getCount()]");
            com.stateforge.statemachine.algorithm.StateOperation.processTransitionBegin(context, PingEndState.getInstance());
            com.stateforge.statemachine.algorithm.StateOperation.processTransitionEnd(context, PingEndState.getInstance());
            context.onEnd();
            return ;
        }
        // Self transition triggered by evTimer
        context.setTransitionName("evTimer");
        com.stateforge.statemachine.algorithm.StateOperation.processTransitionBegin(context, PingSendPingAndWaitForReplyState.getInstance());
        com.stateforge.statemachine.algorithm.StateOperation.processTransitionEnd(context, PingSendPingAndWaitForReplyState.getInstance());
        return ;
    }
}
