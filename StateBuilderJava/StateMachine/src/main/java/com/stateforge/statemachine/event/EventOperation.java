/**
 * Frederic Heem
 * frederic.heem@gmail.com
 * Copyright 2007, All right reserved
 */
package com.stateforge.statemachine.event;

/**
 * Event Operation is a utility class to create events with different parameters
 * @author heefre
 *
 */
public final class EventOperation {

    private EventOperation(){
        //Empty
    }
    /** Create a new event without parameter
     * @param <TContext>
     * @param context the context 
     * @param methodName the method name
     * @return
     */
    public static <TContext> Event<TContext> 
    eventCreate(final TContext context, final String methodName){
        return new Event0<TContext>(context, methodName);
    }
}
