
package com.stateforge.statebuilder.model;

/**
 * <p>Java class for PassedByType.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * <p>
 * <pre>
 * &lt;simpleType name="PassedByType">
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string">
 *     &lt;enumeration value="reference"/>
 *     &lt;enumeration value="pointer"/>
 *     &lt;enumeration value="value"/>
 *   &lt;/restriction>
 * &lt;/simpleType>
 * </pre>
 * 
 */

public enum PassedBy {

    REFERENCE("reference"),
    POINTER("pointer"),
    VALUE("value");
    private final String value;

    PassedBy(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static PassedBy fromValue(String v) {
        for (PassedBy c: PassedBy.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
