
package com.stateforge.statebuilder.model;


/**
 * <p>Java class for LibraryType.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * <p>
 * <pre>
 * &lt;simpleType name="LibraryType">
 *   &lt;restriction base="{http://www.w3.org/2001/XMLSchema}string">
 *     &lt;enumeration value="qt"/>
 *     &lt;enumeration value="boost"/>
 *     &lt;enumeration value="std"/>
 *   &lt;/restriction>
 * &lt;/simpleType>
 * </pre>
 * 
 */

public enum Library {
	
    QT("qt"),
    BOOST("boost"),
    STD("std"),
    LIBEV("libev");
    
    private final String value;

    Library(String v) {
        value = v;
    }

    public String value() {
        return value;
    }

    public static Library fromValue(String v) {
        for (Library c: Library.values()) {
            if (c.value.equals(v)) {
                return c;
            }
        }
        throw new IllegalArgumentException(v);
    }

}
