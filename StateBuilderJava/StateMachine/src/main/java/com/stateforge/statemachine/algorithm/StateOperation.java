/**
 * Frederic Heem
 * frederic.heem@gmail.com
 * Copyright 2007, All right reserved
 */
package com.stateforge.statemachine.algorithm;

import com.stateforge.statemachine.context.AbstractContext;
import com.stateforge.statemachine.context.ContextBase;
import com.stateforge.statemachine.state.AbstractState;
import com.stateforge.statemachine.state.IState;

/**
 * @author Frederic Heem
 * 
 */
public class StateOperation {
    /**
     * @param stateChild
     *            the state that may be the child state
     * @param stateParent
     *            the state that may be the parent state
     * @return true if stateChild is the child of stateParent or if the two
     *         states are equal
     */
    static public <TState extends IState<?, TState>> 
    boolean isChild(
            TState stateChild, TState stateFather) {
        TState stateParent = stateChild.getStateParent();
        return ((stateChild.equals(stateFather) == true) || 
                ((stateParent != null) && (isChild(stateParent, stateFather))));
    }

    /** 
     * Walk the entry chain from the state father of both current and next state to the next state
     * The onEntry of the father is not executed 
     * This method is recursive.
     * @param <TState>
     * @param <TContext>
     * @param context
     * @param statePrevious
     * @param stateNext
     */

    static public <TState extends IState<TContext, TState>, TContext extends ContextBase>    
    void walkTreeEntry(final TContext context, final TState statePrevious, final TState stateNext)
    {
        if(stateNext == null){
            return;
        }

        if(statePrevious == stateNext){
            stateNext.onEntry(context);
        }

        TState stateParent = stateNext.getStateParent();
        if(stateParent == null){
            return;
        }

        if (StateOperation.isChild(statePrevious, stateNext) == false) {
            StateOperation.walkTreeEntry(context, statePrevious, stateParent);
            stateNext.onEntry(context);
        }
    }    

    /** Walk the exit chain from the current state to the state father of both the current 
     *  and next state excluding the father state
     *  This method is recursive.
     * @param <TState> the top state class
     * @param <TContext> the context class 
     * @param context the context
     * @param statePrevious the previous state
     * @param stateNext the next state
     */
    static public <TState extends IState<TContext, TState>, TContext extends ContextBase>   

    void walkTreeExit(final TContext context, final TState statePrevious, final TState stateNext){
        if (statePrevious == stateNext){
            stateNext.onExit(context);
            return;
        }

        TState stateParent = statePrevious.getStateParent();
        if ((stateParent == null) || (stateNext == null)){
            return;
        }

        if ((statePrevious.equals(stateNext) == true) ||
                (StateOperation.isChild(stateNext, statePrevious) == false)) {
            statePrevious.onExit(context);
            StateOperation.walkTreeExit(context, stateParent, stateNext);
        }        
    } 

    /** Process the pre transition
     *   1.Set the the previous state to the current state 
     *   2.Se the current state to null 
     *   3.Advise the listener about the pre transition
     *   4.Walk the exit chain from the current state to the state father of both current and next state
     * @param <TObject> the controlled object class
     * @param <TState> the top state class 
     * @param <TContext> the context class 
     * @param context the context
     * @param statePrevious the previous state
     * @param stateNext the next state 
     * @param transitionName the transition name
     */
    static public <TState extends AbstractState<TContext, TState>, 
    TContext extends AbstractContext<TState, ?>>   
    void processTransitionBegin(final TContext context,            
            final TState stateNext)
    {
        TState statePrevious = context.getStateCurrent();
        context.setStatePrevious(statePrevious);
        context.setStateCurrent(null);
        context.setStateNext(stateNext);

        context.getObserver().onTransitionBegin(context.getName(),
                statePrevious.getName(),
                stateNext != null ? stateNext.getName() : statePrevious.getName(),
                        context.getTransitionName());

        StateOperation.walkTreeExit(context, statePrevious, stateNext);

    }

    /** Process the post transition
     *    1.Walk the entry chain from the state father of both current and next state to the next state
     *    2.Set the current state to the next state  
     * @param <TObject> the controlled object class
     * @param <TContext> the context class 
     * @param context the context
     * @param statePrevious the previous state
     * @param stateNext the next state 
     * @param transitionName the transition name
     */
    static public <TState extends AbstractState<TContext , TState>, 
    TContext extends AbstractContext<TState, ?>>  
    void processTransitionEnd(final TContext context,            
            final TState stateNext)
    {
        TState statePrevious = context.getStatePrevious(); 
        StateOperation.walkTreeEntry(context, statePrevious, stateNext);

        context.getObserver().onTransitionEnd(context.getName(),
                statePrevious.getName(),
                stateNext != null ? stateNext.getName() : statePrevious.getName(),
                        context.getTransitionName());

        context.setStatePrevious(null);
        context.setStateCurrent(stateNext != null ? stateNext : statePrevious);
        context.setStateNext(null);
    }
}


