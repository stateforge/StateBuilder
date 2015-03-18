
package com.stateforge.statebuilder.model;

/**
 * <p>Java class for StateKindType.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * <p>
 * <pre>
 * &lt;simpleType name="StateKindType">
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string">
 *     &lt;enumeration value="final"/>
 *     &lt;enumeration value="history"/>
 *   &lt;/restriction>
 * &lt;/simpleType>
 * </pre>
 * 
 */

public enum StateKind {
    /**
     * 
     *                         When the state machine reaches a final state,
     *                         the object which hold the context is informed about the end of the state machine.
     *                         A final state cannot have child states or transitions
     *                     
     * 
     */
    FINAL("final"),

    /**
     * 
     *                         An history state allows to remember in which state the machine was before a transition to this state.
     *                         When the opposite transition happens, the machines goes into the remembered state intead of the initial state.
     *                     
     * 
     */
    HISTORY("history");
    private final String value;

    StateKind(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static StateKind fromValue(String v) {
        for (StateKind c: StateKind.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
