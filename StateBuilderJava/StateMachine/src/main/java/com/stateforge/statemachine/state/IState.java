/**
 * Frederic Heem
 * frederic.heem@gmail.com
 * Copyright 2007, All right reserved
 */
package com.stateforge.statemachine.state;

import com.stateforge.statemachine.context.ContextBase;

/**
 * @author Frederic Heem
 */
public interface IState<TContext extends ContextBase, TState extends IState<?, TState>> {    
    /**
     * @return the stateParent
     */    
    TState getStateParent();
    
    /**
     * Perform all necessary actions upon entering this state.
     * 
     * @param context
     *            the current context
     */
    void onEntry(final TContext context);

    /**
     * Perform all necessary actions upon leaving this state.
     *
     * @param context
     *            the current context
     */
    void onExit(final TContext context);
}
