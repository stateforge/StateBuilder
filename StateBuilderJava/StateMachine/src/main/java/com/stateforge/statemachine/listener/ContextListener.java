/**
 * Frederic Heem
 * frederic.heem@gmail.com
 * Copyright 2007, All right reserved
 */
package com.stateforge.statemachine.listener;

/**
 * @author heefre
 * 
 */
public interface ContextListener<TContext, TState> {
    /**
     * advise that a transition is about to happen
     * 
     * @param context
     *            the context
     * @param statePrevious
     *            the previous state
     * @param stateNext
     *            the next state
     * @param transition
     *            the transition name
     */
    void onTransitionPre(
            final TContext context, 
            final TState statePrevious,
            final TState stateNext, 
            final String transition);

    /**
     * advise that a transition is about to finish
     * 
     * @param context
     *            the context
     * @param statePrevious
     *            the previous state
     * @param stateNext
     *            the next state
     * @param transition
     *            the transition name
     */
    void onTransitionPost(
            final TContext context, 
            final TState statePrevious,
            final TState stateNext, 
            final String transition);
    

    /**
     * invoked when a timer is started.
     * 
     * @param context
     *            the context
     * @param name
     *            the timer name
     * @param duration
     *            the duration in msec
     */
    void onTimerStart(final TContext context, final String name, int duration);

    /**
     * Invoked when a timer is stopped.
     * 
     * @param context
     *            the context
     * @param name
     *            the timer name
     */
    void onTimerStop(final TContext context, final String name);

    /**
     * Invoked when the context enters a new state.
     * 
     * @param context
     *            the context
     * @param state
     *            the entered state
     */
    void onEntry(final TContext context, final TState state);

    /**
     * Invoked when a context leaves a state.
     * 
     * @param context
     *            the context
     * @param state
     *            the leaving state
     */
    void onExit(final TContext context, final TState state);
}
