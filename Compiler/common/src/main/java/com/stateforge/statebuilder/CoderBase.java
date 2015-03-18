package com.stateforge.statebuilder;

import com.stateforge.statebuilder.model.Context;
import com.stateforge.statebuilder.model.State;
import com.stateforge.statebuilder.model.StateMachineModel;

public abstract class CoderBase {
    private StateMachineModel model;

    public CoderBase(StateMachineModel model){
        this.model = model;
    }

    public abstract void code() throws StateBuilderException;

    public void setModel(StateMachineModel model) {
        this.model = model;
    }

    public StateMachineModel getModel() {
        return model;
    }

    /**
     * Get the state class name from the given state: MicrowaveDoorOpenedState 
     * @param the state
     * @return the state class name
     * @throws StateBuilderException
     */
    protected String getStateClassName(State state) throws StateBuilderException {
        if(state == null){
            return null;
        }
        return getStateClassName(state.getName());   
    }

    /**
     * Get the state class name from the given state name: Microwave02InitialState
     * @param the state name
     * @return the state class name
     * @throws StateBuilderException
     */
    protected String getStateClassName(String stateName) throws StateBuilderException {
        return this.model.getStateMachine().getSettings().getName() + stateName + "State";
    }


    public String getContextClassName(State state) throws StateBuilderException {
        Context context = getModel().getStateMachine().getSettings().getContext();
        State stateTop = getModel().getStateTop(state);
        if(stateTop.isRoot() == true){
            if((context != null) && 
                    (context.getClazz() != null) &&
                    (context.getClazz().isEmpty() == false)){
                return context.getClazz();
            } else {
                return getModel().getStateMachine().getSettings().getName() + "Context";
            }
        } else {
            return getModel().getStateMachine().getSettings().getName() + stateTop.getName() +  "Context";
        }    
    }

    public String getContextParentClassName(State state) throws StateBuilderException {
        if(state.getStateParallel() == null){
            return getContextClassName(state);
        } else {
            return getContextClassName(state.getStateParallel());
        }
    }
}
