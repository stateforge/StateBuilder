/**
 * Frederic Heem
 * frederic.heem@gmail.com
 * Copyright 2007, All right reserved
 */
package com.stateforge.statemachine.event;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 * @author heefre
 *
 */
public class Event0<TContext> extends Event<TContext> {
    private final TContext context;
    private final String methodName;
    /**
     * @param context
     * @param method
     */
    public Event0(final TContext context, final String methodName) {
        super();
        this.context = context;
        this.methodName = methodName;
    }
    /* (non-Javadoc)
     * @see com.genit.statecc.event.IEvent#dispatch()
     */
    public final void dispatch() throws EventException {
       try {
           final Method method = getContext().getClass().getMethod(getMethodName()); 
           method.invoke(getContext());
       } catch(IllegalAccessException iae){
           throw new EventException(iae);
       } catch (InvocationTargetException ite){
           throw new EventException(ite);           
       } catch (NoSuchMethodException nsme){
           throw new EventException(nsme);
       }
    }


    /**
     * @return the context
     */
    protected TContext getContext() {
        return context;
    }
 
    /**
     * @return the methodName
     */
    private String getMethodName() {
        return methodName;
    }

}
