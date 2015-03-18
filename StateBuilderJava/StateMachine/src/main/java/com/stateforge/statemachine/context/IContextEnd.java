package com.stateforge.statemachine.context;

/**
 * IContextEnd is an interface which is invoked when the context ends.
 * @author Frederic Heem
 *
 */
public interface IContextEnd {
    void end(Throwable throwable);
}
