
package com.stateforge.statebuilder.model;

import java.util.ArrayList;
import java.util.List;

/**
 * 
 *                 When an event is received and the eventual condition is true,
 *                 a transition occurs from the state that contains this transition to the state specified in transtion@nextState.
 *                 Actions are performed during this transition
 *             
 * 
 * <p>Java class for TransitionType complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="TransitionType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="condition" type="{http://www.w3.org/2001/XMLSchema}string" minOccurs="0"/>
 *         &lt;group ref="{http://www.stateforge.com/StateMachineCpp-v1}ActionGroup" maxOccurs="unbounded" minOccurs="0"/>
 *       &lt;/sequence>
 *       &lt;attribute name="description" type="{http://www.w3.org/2001/XMLSchema}string" default="" />
 *       &lt;attribute name="event" use="required" type="{http://www.w3.org/2001/XMLSchema}IDREF" />
 *       &lt;attribute name="condition" type="{http://www.w3.org/2001/XMLSchema}string" />
 *       &lt;attribute name="action" type="{http://www.w3.org/2001/XMLSchema}string" />
 *       &lt;attribute name="nextState" type="{http://www.w3.org/2001/XMLSchema}IDREF" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */

public class Transition {


    protected String conditionElement;
    protected List<Object> actionGroup;

    protected String description;

    protected Event event;

    protected String condition;

    protected String action;

    protected String nextState;

    /**
     * Gets the value of the conditionElement property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getConditionElement() {
        return conditionElement;
    }

    /**
     * Sets the value of the conditionElement property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setConditionElement(String value) {
        this.conditionElement = value;
    }

    /**
     * Gets the value of the actionGroup property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the actionGroup property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getActionGroup().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link TimerStartAction }
     * {@link String }
     * {@link TimerStopAction }
     * 
     * 
     */
    public List<Object> getActionGroup() {
        if (actionGroup == null) {
            actionGroup = new ArrayList<Object>();
        }
        return this.actionGroup;
    }

    /**
     * Gets the value of the description property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getDescription() {
        if (description == null) {
            return "";
        } else {
            return description;
        }
    }

    /**
     * Sets the value of the description property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setDescription(String value) {
        this.description = value;
    }

    /**
     * Gets the value of the event property.
     * 
     * @return
     *     possible object is
     *     {@link Object }
     *     
     */
    public Event getEvent() {
        return event;
    }

    /**
     * Sets the value of the event property.
     * 
     * @param value
     *     allowed object is
     *     {@link Object }
     *     
     */
    public void setEvent(Event value) {
        this.event = value;
    }

    /**
     * Gets the value of the condition property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getCondition() {
        return condition;
    }

    /**
     * Sets the value of the condition property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setCondition(String value) {
        this.condition = value;
    }

    /**
     * Gets the value of the action property.
     * 
     * @return
     *     possible object is
     *     {@link String }
     *     
     */
    public String getAction() {
        return action;
    }

    /**
     * Sets the value of the action property.
     * 
     * @param value
     *     allowed object is
     *     {@link String }
     *     
     */
    public void setAction(String value) {
        this.action = value;
    }

    /**
     * Gets the value of the nextState property.
     * 
     * @return
     *     possible object is
     *     {@link Object }
     *     
     */
    public String getNextState() {
        return nextState;
    }

    /**
     * Sets the value of the nextState property.
     * 
     * @param value
     *     allowed object is
     *     {@link Object }
     *     
     */
    public void setNextState(String value) {
        this.nextState = value;
    }

}
