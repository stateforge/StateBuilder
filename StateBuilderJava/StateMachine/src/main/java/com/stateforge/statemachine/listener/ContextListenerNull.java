/**
 * 
 */
package com.stateforge.statemachine.listener;

/**
 * @author heefre
 * 
 */
public final class ContextListenerNull<TContext, TState> implements ContextListener<TContext, TState> {

    /**
     * Empty private contructor.
     */
    public ContextListenerNull() {
    }

    /* (non-Javadoc)
     * @see com.genit.statecc.core.listener.ContextListener#onEntry(java.lang.Object, java.lang.Object)
     */
    public void onEntry(final TContext context, final TState state) {
    }

    /* (non-Javadoc)
     * @see com.genit.statecc.core.listener.ContextListener#onExit(java.lang.Object, java.lang.Object)
     */
    public void onExit(final TContext context, final TState state) {
    }

    /* (non-Javadoc)
     * @see com.genit.statecc.core.listener.ContextListener#onTimerStart(java.lang.Object, java.lang.String, int)
     */
    public void onTimerStart(final TContext context, final String name, final int duration) {
    }

    /* (non-Javadoc)
     * @see com.genit.statecc.core.listener.ContextListener#onTimerStop(java.lang.Object, java.lang.String)
     */
    public void onTimerStop(final TContext context, final String name) {
    }

    /* (non-Javadoc)
     * @see com.genit.statecc.core.listener.ContextListener#onTransitionPost(java.lang.Object, java.lang.Object, java.lang.Object, java.lang.String)
     */
    public void onTransitionPost(final TContext context, final TState statePrevious, final TState stateNext, final String transition) {
    }

    /* (non-Javadoc)
     * @see com.genit.statecc.core.listener.ContextListener#onTransitionPre(java.lang.Object, java.lang.Object, java.lang.Object, java.lang.String)
     */
    public void onTransitionPre(final TContext context, final TState statePrevious, final TState stateNext, final String transition) {
    }
}
