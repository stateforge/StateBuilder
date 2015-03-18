package com.stateforge.statebuilder.java;

import com.stateforge.statebuilder.CoderBase;
import com.stateforge.statebuilder.StateBuilderException;
import com.stateforge.statebuilder.model.Parameter;
import com.stateforge.statebuilder.model.State;
import com.stateforge.statebuilder.model.StateKind;
import com.stateforge.statebuilder.model.StateMachineModel;
import com.sun.codemodel.JBlock;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JExpression;
import com.sun.codemodel.JInvocation;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;
import com.sun.codemodel.JPackage;
import com.sun.codemodel.JCodeModel;

public abstract class CoderBaseJava extends CoderBase {
    private JCodeModel code;
    private JPackage m_package;

    public CoderBaseJava(StateMachineModel model, JCodeModel code){
        super(model);
        this.code = code;
        this.m_package = code._package(getModel().getStateMachine().getSettings().getNamespace());
    }

    protected JCodeModel getCode() {
        return code;
    }

    protected JPackage getPackage() {
        return m_package;
    }

    protected void addParameter(JMethod method, Parameter parameter) throws StateBuilderException
    {
    	try {
    		int mod = 0;
    		if(parameter.isConst() == true){
    			mod = JMod.FINAL;
    		}
    		method.param(mod, getCode().parseType(parameter.getType()), parameter.getName());
    	} catch (ClassNotFoundException e) {
    		throw new StateBuilderException(e);
    	}
    }

    protected String getAbstractContextClassName(State state) throws StateBuilderException
    {
        return "Abstract" + getContextClassName(state);
    }

    /**
     * parallelOperating
     * @param state
     * @return
     */
    protected String getParallelLocalVariableName(State state){
        return "parallel" + state.getName();
    }
    
    /**
     * _parallelOperating
     * @param state
     * @return
     */
    protected String getParallelFieldName(State state){
        return "_parallel" + state.getName();
    }
    
    /**
     * MicrowaveOperatingParallel
     * @param state
     * @return
     */
    protected String getParallelClassName(State state){
        return getModel().getStateMachine().getSettings().getName() + state.getName() + "Parallel";
    }
    
    /**
     * StateOperation.processTransitionPre(context, HelloWorldEndState.getInstance());
     * @param state
     * @param stateNext
     * @param suffix
     * @throws StateBuilderException
     */
    protected void writeProcessTransition(State state, State stateNext, JBlock block, String suffix) throws StateBuilderException
    {
        JExpression contextExpression = getContextParentExpression(state, stateNext);

        JInvocation paramInvokation = getCode().directClass("com.stateforge.statemachine.algorithm.StateOperation").staticInvoke("processTransition" + suffix).arg(contextExpression);

        if(stateNext == null){	        
            paramInvokation = paramInvokation.arg( JExpr._null());
        } else {
            if(stateNext.getKind() == StateKind.HISTORY){
                paramInvokation = paramInvokation.arg(JExpr.ref(getModel().getContextInstance()).invoke("getStateHistory"));
            } else {
                State stateLeaf = getModel().getStateLeaf(stateNext);
                String stateNextClassName = getStateClassName(stateLeaf);
                paramInvokation = paramInvokation.arg(JExpr.ref(stateNextClassName).invoke("getInstance"));
            }
        }

        block.add(paramInvokation);
    }

    /**
     * context.onEnd() or context.getContextParent().onEnd() ...
     * @param state
     * @param stateNext
     * @param block
     * @throws StateBuilderException
     */
    protected void writeContextOnEnd(State state, State stateNext, JBlock block) throws StateBuilderException {
        if((stateNext != null) && (stateNext.getKind() == StateKind.FINAL)){
            block.add(getContextParentExpression(state, stateNext).invoke("onEnd"));
        }
    }
    
    /**
     * context
     * context.getContextParent()
     * context.getContextParent().getContextParent()
     * 
     * @param state
     * @param stateNext
     * @return
     * @throws StateBuilderException
     */
    public JExpression getContextParentExpression(State state, State stateNext) throws StateBuilderException
    {
        JExpression invokation =  JExpr.ref(getModel().getContextInstance());
        
        int contextDepth = getModel().contextDepth(state, stateNext);
        
        for (int i = 1; i <= contextDepth; i++) {
            invokation =  invokation.invoke("getContextParent");
        }
            
        return invokation;
    }
}
