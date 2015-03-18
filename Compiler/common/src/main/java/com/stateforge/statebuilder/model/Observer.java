
package com.stateforge.statebuilder.model;


/**
 * <p>Java class for ObserverType.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * <p>
 * <pre>
 * &lt;simpleType name="ObserverType">
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string">
 *     &lt;enumeration value="no"/>
 *     &lt;enumeration value="string"/>
 *     &lt;enumeration value="int"/>
 *   &lt;/restriction>
 * &lt;/simpleType>
 * </pre>
 * 
 */

public enum Observer {
	
    NO("no"),
    STRING("string"),
    INT("int");
    
    private final String value;

    Observer(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static Observer fromValue(String v) {
        for (Observer c: Observer.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
