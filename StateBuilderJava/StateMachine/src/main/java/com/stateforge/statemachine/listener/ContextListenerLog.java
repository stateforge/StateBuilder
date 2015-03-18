/**
 * Frederic Heem
 * frederic.heem@gmail.com
 * Copyright 2007, All right reserved
 */
package com.stateforge.statemachine.listener;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @author heefre
 * 
 */
public final class ContextListenerLog<TContext, TState> implements ContextListener<TContext, TState> {

    /**
     * @param logger
     *            to use, if null, an anonymous loggee will be used
     */
    public ContextListenerLog(final Logger logger) {
        super();
        if (logger == null) {
            this.logger = Logger.getAnonymousLogger();
        } else {
            this.logger = logger;
        }
    }

    /**
     * @return the logger
     */
    public Logger getLogger() {
        return logger;
    }

    /* (non-Javadoc)
     * @see com.genit.statecc.core.listener.ContextListener#onEntry(java.lang.Object, java.lang.Object)
     */
    public void onEntry(final TContext context, final TState state) {
        getLogger().log(
                Level.INFO,
                CONTEXT + context.getClass().getSimpleName() + ", state = "
                        + state.getClass().getSimpleName());        
    }

    /* (non-Javadoc)
     * @see com.genit.statecc.core.listener.ContextListener#onExit(java.lang.Object, java.lang.Object)
     */
    public void onExit(final TContext context, final TState state) {
        getLogger().log(
                Level.INFO,
                CONTEXT + context.getClass().getSimpleName() + ", state = "
                        + state.getClass().getSimpleName());        
        
    }

    /* (non-Javadoc)
     * @see com.genit.statecc.core.listener.ContextListener#onTimerStart(java.lang.Object, java.lang.String, int)
     */
    public void onTimerStart(final TContext context, final String name, final int duration) {
        getLogger().log(Level.INFO,
                CONTEXT + context.getClass().getSimpleName() + ", timer = " + name + ", duration " + duration);
        
    }

    /* (non-Javadoc)
     * @see com.genit.statecc.core.listener.ContextListener#onTimerStop(java.lang.Object, java.lang.String)
     */
    public void onTimerStop(final TContext context, final String name) {
        getLogger().log(Level.INFO,
                CONTEXT + context.getClass().getSimpleName() + ", timer = " + name);
        
    }

    /* (non-Javadoc)
     * @see com.genit.statecc.core.listener.ContextListener#onTransitionPost(java.lang.Object, java.lang.Object, java.lang.Object, java.lang.String)
     */
    public void onTransitionPost(final TContext context, final TState statePrevious, final TState stateNext, final String transition) {
        getLogger().log(
                Level.INFO,
                CONTEXT + context.getClass().getSimpleName() + ", state previous = "
                        + statePrevious.getClass().getSimpleName() + ", state next = "
                        + stateNext.getClass().getSimpleName() + ", transition = " + transition);
        
    }

    /* (non-Javadoc)
     * @see com.genit.statecc.core.listener.ContextListener#onTransitionPre(java.lang.Object, java.lang.Object, java.lang.Object, java.lang.String)
     */
    public void onTransitionPre(final TContext context, final TState statePrevious, final TState stateNext, final String transition) {
        getLogger().log(
                Level.INFO,
                CONTEXT + context.getClass().getSimpleName() + ", state previous = "
                        + statePrevious.getClass().getSimpleName() + ", state next = "
                        + stateNext.getClass().getSimpleName() + ", transition = " + transition);
        
        
    }

    private final static String CONTEXT = "context = "; 
    /**
     * the logger to use.
     */
    private final Logger logger;
}
