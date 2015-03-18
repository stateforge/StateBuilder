
package com.stateforge.statebuilder.model;

import java.util.ArrayList;
import java.util.List;

/**
 * <p>Java class for ParallelType complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ParallelType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="state" type="{http://www.stateforge.com/StateMachineCpp-v1}StateType" maxOccurs="unbounded"/>
 *       &lt;/sequence>
 *       &lt;attribute name="nextState" use="required" type="{http://www.w3.org/2001/XMLSchema}IDREF" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */

/*@XmlType(name = "ParallelType", propOrder = {
    "state"
})*/
public class Parallel {

    //@XmlElement(required = true)
    protected List<State> state;
    //@XmlAttribute(required = true)
    //@XmlIDREF
    //@XmlSchemaType(name = "IDREF")
    protected String nextState;

    /**
     * Gets the value of the state property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the state property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getState().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link State }
     * 
     * 
     */
    public List<State> getState() {
        if (state == null) {
            state = new ArrayList<State>();
        }
        return this.state;
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
