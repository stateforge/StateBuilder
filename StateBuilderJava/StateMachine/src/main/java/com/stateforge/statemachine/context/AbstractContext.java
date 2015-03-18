/**
 * Frederic Heem
 * frederic.heem@gmail.com
 * Copyright 2007, All right reserved
 */
package com.stateforge.statemachine.context;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;

import com.stateforge.statemachine.listener.ObserverNull;

/**
 * @author Frederic Heem
 * @param <TState>
 *            the top state

 * @param <TContext>
 *            the parent context
 */

public abstract class AbstractContext<TState, TContextParent extends ContextBase>
extends ContextBase {

    /**
     * the current state.
     */
    private TState stateCurrent;

    /**
     * the previous state.
     */
    private TState statePrevious;

    /**
     * the next state.
     */
    private TState stateNext;

    /**
     * the parent context.
     */
    private TContextParent contextParent;

    /**
     * the history state.
     */
    private TState stateHistory;

    /**
     * The eventual parallel context
     */
    private ContextParallel contextParallel;

    /**
     * The class that executes actions
     */
    private ScheduledExecutorService executorService;   

    /**
     * 
     * @param object
     *            the controlled object
     * @param contextParent
     *            the parent context
     */
    public AbstractContext() {
        this.observer = ObserverNull.getInstance();
        this.executorService = Executors.newSingleThreadScheduledExecutor();
    }

    /**
     * 
     * @param object
     *            the controlled object
     * @param contextParent
     *            the parent context
     */
    public AbstractContext(final TContextParent contextParent) {
        super();
        this.contextParent = contextParent;
        contextParent.addChild(this);
    }

    /**
     * @return the stateCurrent
     */
    public final TState getStateCurrent() {
        return stateCurrent;
    }

    /**
     * @param stateCurrent
     *            the current state to set
     */
    public final void setStateCurrent(final TState stateCurrent) {
        this.stateCurrent = stateCurrent;
    }

    /**
     * @return the statePrevious
     */
    public final TState getStatePrevious() {
        return statePrevious;
    }

    /**
     * @param statePrevious
     *            the statePrevious to set
     */
    public final void setStatePrevious(final TState statePrevious) {
        this.statePrevious = statePrevious;
    }

    /**
     * Set the next state
     * @param stateNext
     */
    public void setStateNext(TState stateNext) {
        this.stateNext = stateNext;
    }

    /**
     * get the next state
     * @return
     */
    public TState getStateNext() {
        return stateNext;
    }

    /**
     * @return the contextParent
     */
    public final TContextParent getContextParent() {
        return contextParent;
    }

    /**
     * @return the executorService
     */
    public final ScheduledExecutorService getExecutorService() {
        return executorService;
    }
    
    /**
     * @param executorService the executorService to set
     */
    public final void setExecutorService(ScheduledExecutorService executorService) {
        this.executorService = executorService;
    }  



    /**
     * Set the history state
     * @param stateHistory
     */
    public void setStateHistory(TState stateHistory) {
        this.stateHistory = stateHistory;
    }

    /**
     * Get the history state
     * @return
     */
    public TState getStateHistory() {
        return stateHistory;
    }   

    /**
     * Save the previous state to the history state
     */
    public void saveState(){
        this.stateHistory = this.statePrevious;
    }

    public void setContextParallel(ContextParallel contextParallel) {
        this.contextParallel = contextParallel;
    }

    public ContextParallel getContextParallel() {
        return contextParallel;
    }

    /**
     * Set the initial state.
     * @param stateInitial
     */
    public void setInitialState(TState stateInitial) {
        setStatePrevious(stateInitial);
        setStateCurrent(stateInitial);
    }
}
