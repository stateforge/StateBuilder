
package com.stateforge.statebuilder.model;

/**
 * Xml description of a finite state machine.
 * 
 * <p>Java class for StateMachineType complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="StateMachineType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="settings" type="{http://www.stateforge.com/StateMachineCpp-v1}SettingsType"/>
 *         &lt;element name="eventList" type="{http://www.stateforge.com/StateMachineCpp-v1}EventListType"/>
 *         &lt;element name="state" type="{http://www.stateforge.com/StateMachineCpp-v1}StateType"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */

public class StateMachine {

    //@XmlElement(required = true)
    protected Settings settings;
    //@XmlElement(required = true)
    protected Events events;
    //@XmlElement(required = true)
    protected State state;
    
    /**
     * Gets the value of the settings property.
     * 
     * @return
     *     possible object is
     *     {@link Settings }
     *     
     */
    public Settings getSettings() {
        return settings;
    }

    /**
     * Sets the value of the settings property.
     * 
     * @param value
     *     allowed object is
     *     {@link Settings }
     *     
     */
    public void setSettings(Settings value) {
        this.settings = value;
    }

    /**
     * Gets the value of the eventList property.
     * 
     * @return
     *     possible object is
     *     {@link Events }
     *     
     */
    public Events getEvents() {
        return events;
    }

    /**
     * Sets the value of the eventList property.
     * 
     * @param value
     *     allowed object is
     *     {@link Events }
     *     
     */
    public void setEvents(Events value) {
        this.events = value;
    }

    /**
     * Gets the value of the state property.
     * 
     * @return
     *     possible object is
     *     {@link State }
     *     
     */
    public State getState() {
        return state;
    }

    /**
     * Sets the value of the state property.
     * 
     * @param value
     *     allowed object is
     *     {@link State }
     *     
     */
    public void setState(State value) {
        this.state = value;
    }

}
