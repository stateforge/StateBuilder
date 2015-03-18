package com.stateforge.statemachine.context;

import java.util.ArrayList;
import java.util.List;

import com.stateforge.statemachine.listener.IObserver;

public abstract class ContextBase {
    /**
     * The context name.
     */
    private String name;

    /**
     * The transition name.
     */
    private String transitionName;  

    /**
     * The end handler to inform that the context has reached its final state
     */
    private IContextEnd endHandler;

    /**
     * The eventual context child list
     */
    protected List<ContextBase> contextChildList;

    /**
     * the context listener.
     */
    protected IObserver observer;

    /**
     * @return the observer
     */
    public IObserver getObserver() {
        return observer;
    }

    /**
     * Set the context name
     * @param name
     */
    public void setName(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setTransitionName(String transitionName) {
        this.transitionName = transitionName;
    }

    public String getTransitionName() {
        return transitionName;
    }

    /**
     * Set the listener for this context and all its child.
     * @param the listener
     */
    public void setObserver(IObserver listener) {
        this.observer = listener;
        if(this.contextChildList != null){
            for(ContextBase contextChild : this.contextChildList){
                contextChild.setObserver(listener);
            }
        }
    }

    /**
     * Set the endHandler
     * @param endHandler
     */
    public void setEndHandler(IContextEnd endHandler) {
        this.endHandler = endHandler;
    }

    /**
     * Ge the endHandler
     * @return
     */
    public IContextEnd getEndHandler() {
        return endHandler;
    }

    /**
     * fire the end handler if it exists.
     */
    public void onEnd(Throwable throwable){
    	if(throwable != null){
    		getObserver().onActionException(getName(), throwable);
    	}
    	if(this.endHandler != null){
    		this.endHandler.end(throwable);
    	}
    }

    /**
     * fire the end handler without exception
     */
    public void onEnd(){
    	onEnd(null);
    }
    
    /**
     * Add a child context
     */
    public void addChild(ContextBase contextChild){
        if(contextChild != null){
            getContextChildList().add(contextChild);
        }
    }

    /**
     * Get the context list, lazy initialization
     * @return
     */
    private List<ContextBase> getContextChildList() {
        if(this.contextChildList == null){
            this.contextChildList = new ArrayList<ContextBase>();
        }
        return contextChildList;
    }

}
