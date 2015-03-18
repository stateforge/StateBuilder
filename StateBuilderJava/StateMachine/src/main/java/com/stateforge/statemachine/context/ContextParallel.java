package com.stateforge.statemachine.context;

/**
 * ContextParallel is an abstract class used to hold child contexts, ContextParallel knows about the number of active context.
 * @author Frederic Heem
 *
 */
public abstract class ContextParallel {
    /**
     * The number of active context
     */
    private int activeContext;

    /**
     * Go the next state
     */
    public abstract void transitionToNextState();

    /**
     * Set the number of active contexts
     * @param activeContext
     */
    public void setActiveContext(int activeContext) {
        this.activeContext = activeContext;
    }

    /**
     * Get the number of active contexts.
     * @return
     */
    public int getActiveContext() {
        return activeContext;
    }
}
