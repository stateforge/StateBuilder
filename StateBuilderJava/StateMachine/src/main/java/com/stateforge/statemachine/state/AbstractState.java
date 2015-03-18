/**
 * 
 */
package com.stateforge.statemachine.state;

import com.stateforge.statemachine.context.ContextBase;

/**
 * 
 * @author Frederic Heem
 * 
 * @param <TContext>
 *            the context
 * @param <TState>
 *            the parent state
 */
public abstract class AbstractState<TContext extends ContextBase, TState extends AbstractState<TContext, TState>>
        implements IState<TContext, TState> {
	
	/**
	 * The state name
	 */
	private String name;
	
    /**
     * the parent state.
     */
    private TState stateParent;

	/**
     * Constructor
     */
    protected  AbstractState() {
    }
    
    /**
     * Perform all necessary actions upon entering this state.
     * 
     * @param context
     *            the current context
     */
    public abstract void onEntry(final TContext context);

    /**
     * Perform all necessary actions upon leaving this state.
     *
     * @param context
     *            the current context
     */
    public abstract void onExit(final TContext context);

    /**
     * Set the parent state
     * @param stateParent
     */
    protected void setStateParent(TState stateParent) {
		this.stateParent = stateParent;
	}
    
    /**
     * @return the stateParent
     */
    public TState getStateParent() {
        return stateParent;
    }
    
    /**
     * Set the state name 
     * @param name
     */
	protected void setName(String name) {
		this.name = name;
	}

	/**
	 * Get the state name
	 * @return
	 */
	public String getName() {
		return name;
	}
}
