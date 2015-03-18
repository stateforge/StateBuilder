

package com.stateforge.statebuilder.model;

import java.util.ArrayList;
import java.util.List;


/**
 * <p>Java class for EventListType complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="EventListType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="eventProvider" type="{http://www.stateforge.com/StateMachineCpp-v1}EventProviderType" maxOccurs="unbounded"/>
 *       &lt;/sequence>
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */

public class Events {

    protected List<EventSource> eventProvider;

    /**
     * Gets the value of the eventProvider property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the eventProvider property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getEventProvider().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link EventSource }
     * 
     * 
     */
    public List<EventSource> getEventSource() {
        if (eventProvider == null) {
            eventProvider = new ArrayList<EventSource>();
        }
        return this.eventProvider;
    }

}
