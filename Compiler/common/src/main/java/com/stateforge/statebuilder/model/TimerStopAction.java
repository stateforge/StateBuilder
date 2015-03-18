
package com.stateforge.statebuilder.model;


/**
 * <p>Java class for TimerStopActionType complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="TimerStopActionType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;attribute name="timer" use="required" type="{http://www.w3.org/2001/XMLSchema}string" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */

public class TimerStopAction {

    protected String timer;

    /**
     * Gets the value of the timer property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getTimer() {
        return timer;
    }

    /**
     * Sets the value of the timer property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setTimer(String value) {
        this.timer = value;
    }

}
