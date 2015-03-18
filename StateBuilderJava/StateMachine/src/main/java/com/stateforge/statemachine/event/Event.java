/**
 * Frederic Heem
 * frederic.heem@gmail.com
 * Copyright 2007, All right reserved
 */
package com.stateforge.statemachine.event;

/**
 * @author heefre
 *
 */
public abstract class Event<TContext> implements IEvent {

    /**
     * Default constructor
     */
    protected Event(){
        //Do nothing
    };
    
    /* (non-Javadoc)
     * @see com.genit.statecc.core.event.IEvent#dispatch()
     */
    public void dispatch() throws EventException {
        // TODO Auto-generated method stub

    }

}
