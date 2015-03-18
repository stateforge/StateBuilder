
package com.stateforge.statebuilder.model;

import java.util.ArrayList;
import java.util.List;



/**
 * <p>Java class for ActionsType complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ActionsType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;group ref="{http://www.stateforge.com/StateMachineCpp-v1}ActionGroup" maxOccurs="unbounded" minOccurs="0"/>
 *       &lt;attribute name="action" type="{http://www.w3.org/2001/XMLSchema}string" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
/*
@XmlType(name = "ActionsType", propOrder = {
    "actionGroup"
})*/
public class Actions {

    /*@XmlElements({
        @XmlElement(name = "action", type = String.class),
        @XmlElement(name = "timerStop", type = TimerStopActionType.class),
        @XmlElement(name = "timerStart", type = TimerStartActionType.class)
    })*/
    protected List<Object> actionGroup;

    protected String action;

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
     * {@link String }
     * {@link TimerStopAction }
     * {@link TimerStartAction }
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

}
