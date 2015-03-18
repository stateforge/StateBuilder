package com.stateforge.statebuilder.model;

import java.util.ArrayList;
import java.util.List;

/**
 * 
 *                 Define a state. A state can have any number of child state.
 *                 A state usually has transitions that allows to go to another state.
 *             
 * 
 * <p>Java class for StateType complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="StateType">
 *   &lt;complexContent>
 *     &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *       &lt;sequence>
 *         &lt;element name="onEntry" type="{http://www.stateforge.com/StateMachineCpp-v1}ActionsType" minOccurs="0"/>
 *         &lt;element name="onExit" type="{http://www.stateforge.com/StateMachineCpp-v1}ActionsType" minOccurs="0"/>
 *         &lt;element name="transition" type="{http://www.stateforge.com/StateMachineCpp-v1}TransitionType" maxOccurs="unbounded" minOccurs="0"/>
 *         &lt;element name="state" type="{http://www.stateforge.com/StateMachineCpp-v1}StateType" maxOccurs="unbounded" minOccurs="0"/>
 *         &lt;element name="parallel" type="{http://www.stateforge.com/StateMachineCpp-v1}ParallelType" minOccurs="0"/>
 *       &lt;/sequence>
 *       &lt;attribute name="name" use="required" type="{http://www.w3.org/2001/XMLSchema}ID" />
 *       &lt;attribute name="kind" type="{http://www.stateforge.com/StateMachineCpp-v1}StateKindType" />
 *       &lt;attribute name="description" type="{http://www.w3.org/2001/XMLSchema}string" />
 *     &lt;/restriction>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */

public class State {

	private State stateParent;
	private State stateParallel;
	private List<State> parallelList;
	private boolean isRoot;
	private boolean isTop;
	private boolean isLeaf;
	private boolean hasHistory;
	
    protected Actions onEntry;
    protected Actions onExit;
    protected List<Transition> transition;
    protected List<State> state;
    protected Parallel parallel;
    protected String name;

    protected StateKind kind;

    protected String description;

    /**
     * Gets the value of the onEntry property.
     * 
     * @return
     *     possible object is
     *     {@link Actions }
     *     
     */
    public Actions getOnEntry() {
        return onEntry;
    }

    /**
     * Sets the value of the onEntry property.
     * 
     * @param value
     *     allowed object is
     *     {@link Actions }
     *     
     */
    public void setOnEntry(Actions value) {
        this.onEntry = value;
    }

    /**
     * Gets the value of the onExit property.
     * 
     * @return
     *     possible object is
     *     {@link Actions }
     *     
     */
    public Actions getOnExit() {
        return onExit;
    }

    /**
     * Sets the value of the onExit property.
     * 
     * @param value
     *     allowed object is
     *     {@link Actions }
     *     
     */
    public void setOnExit(Actions value) {
        this.onExit = value;
    }

    /**
     * Gets the value of the transition property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the transition property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getTransition().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link Transition }
     * 
     * 
     */
    public List<Transition> getTransition() {
        if (transition == null) {
            transition = new ArrayList<Transition>();
        }
        return this.transition;
    }

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
     * Gets the value of the parallel property.
     * 
     * @return
     *     possible object is
     *     {@link Parallel }
     *     
     */
    public Parallel getParallel() {
        return parallel;
    }

    /**
     * Sets the value of the parallel property.
     * 
     * @param value
     *     allowed object is
     *     {@link Parallel }
     *     
     */
    public void setParallel(Parallel value) {
        this.parallel = value;
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
     * Gets the value of the kind property.
     * 
     * @return
     *     possible object is
     *     {@link StateKind }
     *     
     */
    public StateKind getKind() {
        return kind;
    }

    /**
     * Sets the value of the kind property.
     * 
     * @param value
     *     allowed object is
     *     {@link StateKind }
     *     
     */
    public void setKind(StateKind value) {
        this.kind = value;
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
        return description;
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
    
	public void setParent(State stateParent) {
		this.stateParent = stateParent;
	}

	public State getParent() {
		return stateParent;
	}

	public void setStateParallel(State stateParallel) {
		this.stateParallel = stateParallel;
	}

	public State getStateParallel() {
		return stateParallel;
	}

	public void setRoot(boolean isRoot) {
		this.isRoot = isRoot;
	}

	public boolean isRoot() {
		return isRoot;
	}

	public void setTop(boolean isTop) {
		this.isTop = isTop;
	}

	public boolean isTop() {
		return isTop;
	}

	public void setLeaf(boolean isLeaf) {
		this.isLeaf = isLeaf;
	}

	public boolean isLeaf() {
		return isLeaf;
	}

	public void setHasHistory(boolean hasHistory) {
		this.hasHistory = hasHistory;
	}

	public boolean isHasHistory() {
		return hasHistory;
	}

	public List<State> getParallelList() {
		if(this.parallelList == null){
			this.parallelList = new ArrayList<State>();
		}
		return parallelList;
	}
}
