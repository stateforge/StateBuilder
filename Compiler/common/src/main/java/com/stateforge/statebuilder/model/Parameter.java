
package com.stateforge.statebuilder.model;

/**
 * 
 *                 Define an event parameter.
 *             
 * 
 * <p>Java class for ParameterType complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ParameterType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;attribute name="type" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *       &lt;attribute name="name" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *       &lt;attribute name="passedBy" type="{http://www.stateforge.com/StateMachineCpp-v1}PassedByType" default="value" />
 *       &lt;attribute name="const" type="{http://www.w3.org/2001/XMLSchema}boolean" default="false" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */

public class Parameter {

    //@XmlAttribute(required = true)
    protected String type;
    //@XmlAttribute(required = true)
    protected String name;
    //@XmlAttribute
    protected PassedBy passedBy;
    //@XmlAttribute(name = "const")
    protected Boolean _const;
    //@XmlAttribute(name = "final")
    protected Boolean _final;    

    /**
     * Gets the value of the type property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getType() {
        return type;
    }

    /**
     * Sets the value of the type property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setType(String value) {
        this.type = value;
    }

    /**
     * Gets the value of the name property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getName() {
        return name;
    }

    /**
     * Sets the value of the name property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setName(String value) {
        this.name = value;
    }

    /**
     * Gets the value of the passedBy property.
     * 
     * @return
     *     possible object is
     *     {@link PassedBy }
     *     
     */
    public PassedBy getPassedBy() {
        if (passedBy == null) {
            return PassedBy.VALUE;
        } else {
            return passedBy;
        }
    }

    /**
     * Sets the value of the passedBy property.
     * 
     * @param value
     *     allowed object is
     *     {@link PassedBy }
     *     
     */
    public void setPassedBy(PassedBy value) {
        this.passedBy = value;
    }

    /**
     * Gets the value of the const property.
     * 
     * @return
     *     possible object is
     *     {@link Boolean }
     *     
     */
    public boolean isConst() {
        if (_const == null) {
            return false;
        } else {
            return _const;
        }
    }

    /**
     * Sets the value of the const property.
     * 
     * @param value
     *     allowed object is
     *     {@link Boolean }
     *     
     */
    public void setConst(Boolean value) {
        this._const = value;
    }

   
    public boolean isFinal() {
        if (_final == null) {
            return false;
        } else {
            return _final;
        }
    }

    
    public void setFinal(Boolean value) {
        this._final = value;
    }    
}
