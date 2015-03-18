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
public interface IEvent {

    /**
     * Dispatch the event by invoking the object´s method with the stored parameters
     * @throws EventException
     */
    void dispatch() throws EventException;
}
