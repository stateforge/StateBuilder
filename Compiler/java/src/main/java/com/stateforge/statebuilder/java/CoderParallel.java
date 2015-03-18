 

/*
public class MicrowaveOperatingParallel
    extends ContextParallel
{
    private MicrowaveContext context;
    private MicrowaveEngineContext myMicrowaveEngineContext;
    private MicrowaveDoorContext myMicrowaveDoorContext;
    
    public MicrowaveOperatingParallel(MicrowaveContext contextToSet, MicrowaveAction action) {
        context = contextToSet;
        myMicrowaveEngineContext = new MicrowaveEngineContext(action, contextToSet);
        myMicrowaveDoorContext = new MicrowaveDoorContext(action, contextToSet);
    }
    
    public MicrowaveEngineContext getMicrowaveEngineContext() {
        return myMicrowaveEngineContext;
    }

    public MicrowaveDoorContext getMicrowaveDoorContext() {
        return myMicrowaveDoorContext;
    }
    
    public void transitionToNextState() {
        com.stateforge.statemachine.algorithm.StateOperation.processTransitionBegin(context, MicrowaveEndState.getInstance());
        com.stateforge.statemachine.algorithm.StateOperation.processTransitionEnd(context, MicrowaveEndState.getInstance());
    }
*/

package com.stateforge.statebuilder.java;

import com.stateforge.statebuilder.StateBuilderException;
import com.stateforge.statebuilder.model.ObjectType;
import com.stateforge.statebuilder.model.State;
import com.stateforge.statebuilder.model.StateMachineModel;
import com.sun.codemodel.JBlock;
import com.sun.codemodel.JClass;
import com.sun.codemodel.JClassAlreadyExistsException;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JInvocation;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;
import com.sun.codemodel.JVar;

public class CoderParallel extends CoderBaseJava {

    public CoderParallel(StateMachineModel model, JCodeModel code) {
        super(model, code);
    }

    @Override
    public void code() throws StateBuilderException {
        try {
            writeClass(getModel().getStateMachine().getState());
        } catch (Exception e) {
            throw new StateBuilderException(e);
        }
    }
    
    private void writeClass(State state) throws JClassAlreadyExistsException, StateBuilderException {
        for(State stateParallel : state.getParallelList()){
            String parallelClassName = getModel().getStateMachine().getSettings().getNamespace() + "." + getParallelClassName(stateParallel);
            // Create parallel class
            JDefinedClass parallelClass = getCode()._class(parallelClassName);
            // Add doc
            parallelClass.javadoc().add("Parallel class " + parallelClassName);
            // extends com.stateforge.statemachine.context.ContextParallel
            parallelClass._extends(getCode().directClass("com.stateforge.statemachine.context.ContextParallel"));
            // Create constructor
            JMethod constructor =  parallelClass.constructor(JMod.PUBLIC);
            // HellowWorldContext context;
            JVar contextField = parallelClass.field(JMod.PRIVATE, getCode().directClass(getContextClassName(state)), getModel().getContextInstance()); 
            JVar contextParam = constructor.param(getCode().directClass(getContextClassName(state)), getModel().getContextInstance() + "ToSet");
            
            JBlock blockContructor = constructor.body();
            blockContructor.assign(contextField, contextParam);
            
            for(State stateOrthogonal : stateParallel.getParallel().getState()){
                String contextVarName = "my" + getContextClassName(stateOrthogonal);
                JClass contextOrthognalClass =  getCode().directClass(getContextClassName(stateOrthogonal));
                JVar contextOrthogonalField = parallelClass.field(JMod.PRIVATE, contextOrthognalClass, contextVarName); 
                
                JInvocation contextOrthogonalInvokation = JExpr._new(contextOrthognalClass);
                
                for(ObjectType obj : getModel().getStateMachine().getSettings().getObject()){
                    contextOrthogonalInvokation = contextOrthogonalInvokation.arg(JExpr.ref(obj.getInstance()));
                }
                contextOrthogonalInvokation = contextOrthogonalInvokation.arg(contextParam);
                
                blockContructor.assign(contextOrthogonalField, contextOrthogonalInvokation);
                
                JMethod getter = parallelClass.method(JMod.PUBLIC, 
                        getCode().directClass(getContextClassName(stateOrthogonal)), 
                        "get" + getContextClassName(stateOrthogonal));
                getter.body()._return(contextOrthogonalField);
            }
            
            for(ObjectType obj : getModel().getStateMachine().getSettings().getObject()){
                //Add constructor parameters
                String objFullClassName = obj.getInclude().isEmpty() == false ? obj.getInclude() + "." + obj.getClazz() : obj.getClazz();
                constructor.param(getCode().directClass(objFullClassName), obj.getInstance());
            }
            
            writeTransitionToNextState(state, stateParallel, parallelClass);
        }
        
        if(state.getParallel() != null){
            for(State stateOrthogonal : state.getParallel() .getState()){
                writeClass(stateOrthogonal);
            }
        }

        for(State stateChild : state.getState()){
            writeClass(stateChild);
        }
    }

    /**
     * 
     * public void transitionToNextState() {
     *    com.stateforge.statemachine.algorithm.StateOperation.processTransitionBegin(context, MicrowaveEndState.getInstance());
     *    com.stateforge.statemachine.algorithm.StateOperation.processTransitionEnd(context, MicrowaveEndState.getInstance());
    *  }
     * @param state
     * @param stateParallel
     * @param parallelClass
     * @throws StateBuilderException
     */
    private void writeTransitionToNextState(
            State state, 
            State stateParallel,
            JDefinedClass parallelClass) throws StateBuilderException {
        JMethod transitionToNextState = parallelClass.method(JMod.PUBLIC, void.class, "transitionToNextState");
        transitionToNextState.javadoc().add("Transition to next the next");
        JBlock block = transitionToNextState.body();
        State stateNext =  getModel().getState(stateParallel.getParallel().getNextState());
        writeProcessTransition(state, stateNext, block, "Begin");
        writeProcessTransition(state, stateNext, block, "End");
         
        writeContextOnEnd(stateParallel, stateNext, block);
    }
}
