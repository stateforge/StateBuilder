package com.stateforge.statebuilder.java;

import java.util.List;


import com.stateforge.statebuilder.StateBuilderException;
import com.stateforge.statebuilder.model.Actions;
import com.stateforge.statebuilder.model.Event;
import com.stateforge.statebuilder.model.ObjectType;
import com.stateforge.statebuilder.model.Parameter;
import com.stateforge.statebuilder.model.State;
import com.stateforge.statebuilder.model.StateKind;
import com.stateforge.statebuilder.model.StateMachineModel;
import com.stateforge.statebuilder.model.TimerStartAction;
import com.stateforge.statebuilder.model.TimerStopAction;
import com.stateforge.statebuilder.model.Transition;
import com.sun.codemodel.JBlock;
import com.sun.codemodel.JClass;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JConditional;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JInvocation;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;
import com.sun.codemodel.JVar;

public class CoderState extends CoderBaseJava {

    public CoderState(StateMachineModel model, JCodeModel code) {
        super(model, code);
    }

    @Override
    public void code() throws StateBuilderException
    {
        //System.out.println("CoderState");
        write(getModel().getStateMachine().getState());
    }

    private void write(State state) throws StateBuilderException{
        //System.out.println("CoderState" + state.getName());

        try {
            JDefinedClass stateClass = getCode()._class(getModel().getStateMachine().getSettings().getNamespace() + "." + getStateClassName(state));
            if(state.getParent() != null){
                stateClass._extends(getCode().directClass(getStateClassName(state.getParent())));
            } else {
                // extends AbstractState<AbstractHelloWorldContext, HelloWorldRootState>
                String abstractStateBaseClassName = "com.stateforge.statemachine.state.AbstractState<" + getContextClassName(state)  + ", " +  getStateClassName(getModel().getStateTop(state)) + ">";
                stateClass._extends(getCode().directClass(abstractStateBaseClassName));
            }

            writeContructor(state, stateClass);
            writeInstance(state, stateClass);
            writeOnEntryExit(state, stateClass, state.getOnEntry(), "onEntry");
            writeOnEntryExit(state, stateClass, state.getOnExit(), "onExit");
            writeEvents(state, stateClass);
        } catch (Exception e) {
            throw new StateBuilderException(e);
        }

        if(state.getParallel() != null){
            for(State stateOrthogonal : state.getParallel().getState()){
                write(stateOrthogonal);
            }
        }

        for(State stateChild : state.getState()){
            write(stateChild);
        }
    }

    /**
     * 	protected HelloWorldEndState(final HelloWorldRootState parent) {
     *	    setName("Idle");
     *      setStateParent(HelloWorldRootState.getInstance());
     *  }
     * @param state
     * @param stateClass
     * @throws StateBuilderException 
     */
    private void writeContructor(State state, JDefinedClass stateClass) throws StateBuilderException{
        // Create the state constructor
        JMethod constructor = stateClass.constructor(JMod.PROTECTED);
        // Doc
        constructor.javadoc().add("Protected Constructor");
        // setName("Idle");
        constructor.body().directStatement("setName(\"" + state.getName() + "\");");
        // setStateParent(HelloWorldRootState.getInstance());
        if(state.getParent() != null){
            constructor.body().directStatement("setStateParent(" + getStateClassName(state.getParent()) + ".getInstance());");
        }
    }

    /**
     *   @Override
     *   public void onEntry(HelloWorldContext context)
     *   {
     *       context.getObserver().onEntry(context.getName(), this.getName());
     *   }
     * @param state
     * @param stateClass
     * @param onEntry
     * @param string
     * @throws StateBuilderException 
     */
    private void writeOnEntryExit(
            State state, 
            JDefinedClass stateClass,
            Actions action, 
            String onEntryExitName) throws StateBuilderException {
        // Create public void onEntry
        String contextInstance = getModel().getContextInstance();
        JMethod method = stateClass.method(JMod.PUBLIC, void.class, onEntryExitName);
        //@Overrice
        method.annotate(getCode().directClass("Override"));
        // Doc
        method.javadoc().add(onEntryExitName);
        // Parameter: HelloWorldContext context
        JVar contextVar = method.param(getCode().directClass(getContextClassName(state)), contextInstance);

        JBlock block = method.body();
        if(onEntryExitName.compareTo("onExit") == 0){
            writeOnExitHistory(state, block);
            writeOnExitParallel(state, block, contextVar);
        }

        //context.getObserver().onEntry(context.getName(), this.getName());
        method.body().add(contextVar.invoke("getObserver").invoke(onEntryExitName).arg(contextVar.invoke("getName")).arg(JExpr._this().invoke("getName")));

        // Write the actions
        if(action != null){
            writeActuatorParameterDeclaration(block);
            writeActions(state, action.getAction(), action.getActionGroup(), block);
        }

        if(onEntryExitName.compareTo("onEntry") == 0){
            writeOnEntryParallel(state, block, contextVar);
        }
    }

    /**
            context.setTransitionName("evTimer[ping.getTx() == ping.getCount()]");
            com.stateforge.statemachine.algorithm.StateOperation.processTransitionBegin(context, PingEndState.getInstance());
            com.stateforge.statemachine.algorithm.StateOperation.processTransitionEnd(context, PingEndState.getInstance());
            context.onEnd();
            
     * @param state
     * @param transition
     * @return
     * @throws StateBuilderException 
     */
    protected void processTransitionStatements(State state, Transition transition, JBlock block, JVar contextVar) throws StateBuilderException
    {
        State stateNext = getModel().getState(transition.getNextState());
        block.invoke(contextVar, "setTransitionName").arg(getModel().getTransitionName(transition));

        writeProcessTransition(state, stateNext, block, "Begin");
        writeActionsFromTransition(state, transition, block);
        writeProcessTransition(state, stateNext, block, "End");
        writeContextOnEnd(state, stateNext, block);
        block._return();
    }

    private void writeActions(
            State state,
            String actionAttribute,
            List<Object> actionList,
            JBlock block) throws StateBuilderException {

        if(actionAttribute.isEmpty() == false){
            block.directStatement(actionAttribute + ";");
        }

        writeActionGroup(state, actionList, block);
    }

    private void writeActionGroup(
            State state,
            List<Object> actionList,
            JBlock block) throws StateBuilderException {
        if(actionList == null){
            return;
        }		

        for(Object action : actionList){
            if(action instanceof String){
                String actionstring = (String)action;
                if(actionstring.isEmpty() == false){
                    block.directStatement(action + ";");
                }
            } else if(action instanceof TimerStartAction){
                writeTimerStart(state, (TimerStartAction)action, block);
            } else if(action instanceof TimerStopAction){
                writeTimerStop(state, (TimerStopAction)action, block);
            }
        }
    }

    private void writeActionsFromTransition(State state, Transition transition, JBlock block) throws StateBuilderException {
        writeActions(state, transition.getAction(), transition.getActionGroup(), block);
    }

    /**
     *  context.saveState();
     * @param state
     * @param method
     */
    private void writeOnExitHistory(State state, JBlock block) {
        if(state.isHasHistory() == true){
            block.directStatement(getModel().getContextInstance() + ".saveState();");
        }
    }
        
    /**
        MicrowaveOperatingParallel parallelOperating = context.getMicrowaveOperatingParallel();
        context.setContextParallel(parallelOperating);
        parallelOperating.setActiveContext(2);
        parallelOperating.getMicrowaveEngineContext().enterInitialState();
        parallelOperating.getMicrowaveDoorContext().enterInitialState();

     */
    private void writeOnEntryParallel(State state, JBlock block, JVar contextVar) throws StateBuilderException {
        if(state.getParallel() == null){
            return;
        }
        
        String parallelClassName = getParallelClassName(state);
        String parallelLocalVariableName = getParallelLocalVariableName(state);
        
        // MicrowaveOperatingParallel parallelOperating = context.getMicrowaveOperatingParallel();
        JClass parallelClass = getCode().ref(parallelClassName);
        JVar parallelVar = block.decl(parallelClass, parallelLocalVariableName);
        parallelVar.init(contextVar.invoke("get" + parallelClassName));
        
        // context.setContextParallel(parallelRunning);
        block.add(contextVar.invoke("setContextParallel").arg(parallelVar));
     
        // parallelRunning.setActiveContext(2);
        block.add(parallelVar.invoke("setActiveContext").arg(JExpr.ref(String.valueOf(state.getParallel().getState().size()))));
        
        for(State stateOrthogonal : state.getParallel().getState()){
            block.add(parallelVar.invoke("get" + getContextClassName(stateOrthogonal)).invoke("enterInitialState"));
        }
    }
 
    /**
        MicrowaveOperatingParallel parallelOperating = context.getMicrowaveOperatingParallel();
        context.setContextParallel(null);
        parallelOperating.getMicrowaveEngineContext().leaveCurrentState();
        parallelOperating.getMicrowaveDoorContext().leaveCurrentState();

     * @param state
     * @param block
     * @param contextVar
     * @throws StateBuilderException
     */
    private void writeOnExitParallel(State state, JBlock block, JVar contextVar) throws StateBuilderException {
        if(state.getParallel() == null){
            return;
        }
        
        String parallelClassName = getParallelClassName(state);
        String parallelLocalVariableName = getParallelLocalVariableName(state);
        
        // MicrowaveOperatingParallel parallelOperating = context.getMicrowaveOperatingParallel();
        JClass parallelClass = getCode().ref(parallelClassName);
        JVar parallelVar = block.decl(parallelClass, parallelLocalVariableName);
        parallelVar.init(contextVar.invoke("get" + parallelClassName));
        
        // context.setContextParallel(null);
        block.add(contextVar.invoke("setContextParallel").arg(JExpr._null()));
        
        for(State stateOrthogonal : state.getParallel().getState()){
            block.add(parallelVar.invoke("get" + getContextClassName(stateOrthogonal)).invoke("leaveCurrentState"));
        } 
    }
    
    /**
     * 
      context.timerStartTimerLight((light.getYellowDuration()));

     * @param state
     * @param timerStart
     * @param block
     * @throws StateBuilderException
     */
    private void writeTimerStart(
            State state, 
            TimerStartAction timerStart,
            JBlock block) throws StateBuilderException {
        block.add(getContextParentExpression(state, getModel().getStateMachine().getState()).invoke("timerStart" + timerStart.getTimer()).arg(JExpr.direct(timerStart.getDuration())));
    }

    /**
      context.timerStopTimerLight();
     * @param state
     * @param timerStop
     * @param block
     * @throws StateBuilderException
     */
    private void writeTimerStop(
            State state,
            TimerStopAction timerStop,
            JBlock block) throws StateBuilderException {
        block.add(getContextParentExpression(state, getModel().getStateMachine().getState()).invoke("timerStop" + timerStop.getTimer()));
    }
    
    /**
     * Object1 object = context.getObject1();
     */
    private void writeActuatorParameterDeclaration(JBlock block){
        for(ObjectType obj : getModel().getStateMachine().getSettings().getObject()){
            String objFullClassName = obj.getInclude().isEmpty() == false ? obj.getInclude() + "." + obj.getClazz() : obj.getClazz();
            block.directStatement(objFullClassName + " " + obj.getInstance() +
                    " = " + getModel().getContextInstance() + ".get" + obj.getClazz() + "();");
        }
    }

    /**
     * 
     * private static final HelloWorldEndState instance = new HelloWorldEndState(HelloWorldRootState.getInstance());
     * 
     * public static HelloWorldRootState getInstance() {
     *     return instance;
     * }
     * @throws StateBuilderException 
     */
    private void writeInstance(State state, JDefinedClass stateClass) throws StateBuilderException{
        // private static final HelloWorldEndState INSTANCE = new HelloWorldEndState(HelloWorldRootState.getInstance());
        JVar instanceField = stateClass.field(
                JMod.PRIVATE | JMod.STATIC | JMod.FINAL, 
                stateClass, 
                "instance", 
                JExpr._new(stateClass));

        JMethod getInstanceMethod = stateClass.method(JMod.PUBLIC | JMod.STATIC, getCode().directClass(getStateClassName(getModel().getStateTop(state))), "getInstance");
        getInstanceMethod.javadoc().add("Get the State Instance");
        getInstanceMethod.body()._return(instanceField);

    }	

    /**
     * @throws ClassNotFoundException 
     * @throws StateBuilderException 
     *  
     */
    private void writeEvents(State state, JDefinedClass stateClass) throws ClassNotFoundException, StateBuilderException{
        for(Event event : getModel().getEventsForState(state)){
            writeEvent(state, stateClass, event);
        }
    }

    /**
     * 
     * @param state
     * @param stateClass
     * @param event
     * @throws ClassNotFoundException
     * @throws StateBuilderException
     */
    private void writeEvent(State state, JDefinedClass stateClass, Event event) throws ClassNotFoundException, StateBuilderException{
        JMethod eventMethod = stateClass.method(JMod.PUBLIC, void.class, event.getId());
        JVar contextVar = eventMethod.param(getCode().parseType(getContextClassName(state)), "context");
        eventMethod.javadoc().add("Event id: " + event.getId());
        for(Parameter parameter : event.getParameter()){
            addParameter(eventMethod, parameter);
        }

        JBlock block = eventMethod.body();
        writeEventParallel(state, event, block, contextVar);
        writeEventLeaf(state, event, block, contextVar);
    }

    private void writeEventLeaf(State state, Event event, JBlock block, JVar contextVar) throws ClassNotFoundException, StateBuilderException{
        List<Transition> transitions = getModel().getTransitionList(state, event);
        if(transitions.size() != 0){
            writeActuatorParameterDeclaration(block);
            for(Transition transition : transitions){
                writeTransition(state, transition, block, contextVar);
            }
        }
    }

    private void writeTransition(
            State state,
            Transition transition,
            JBlock block,
            JVar contextVar) throws StateBuilderException {

        State stateNext = getModel().getState(transition.getNextState());
        if(stateNext == null){
            block.directStatement("// Internal transition triggered by " + getModel().getTransitionName(transition));
        } else if(stateNext.equals(state)){
            block.directStatement("// Self transition triggered by " + getModel().getTransitionName(transition));
        } else {
            block.directStatement("// Transition from " + state.getName() + " to " + stateNext.getName() + " triggered by " + getModel().getTransitionName(transition));
            if(stateNext.getParallel() == state.getParallel()){
                block.directStatement("// The next state is within the context " + getContextClassName(state));
                if(stateNext.getKind() == StateKind.HISTORY){
                    block.directStatement("// The next state is historical ");
                }
            } else {
                block.directStatement("// The next state belonging to context " + getContextClassName(stateNext) + " is outside the current context " + getContextClassName(state));
            }
        }

        String condition = getModel().getCondition(transition);

        if(condition.isEmpty() == false){
            JConditional conditional = block._if(JExpr.direct(condition));
            processTransitionStatements(stateNext, transition, conditional._then(), contextVar);
        } else {
            processTransitionStatements(stateNext, transition, block, contextVar);
        }
    }

    /**
        MicrowaveOperatingParallel parallelOperating = context.getMicrowaveOperatingParallel();
        parallelOperating.getMicrowaveEngineContext().evStart();
        parallelOperating.getMicrowaveDoorContext().evStart();
     * @param state
     * @param event
     * @param block
     * @param contextVar
     * @throws ClassNotFoundException
     * @throws StateBuilderException
     */
    private void writeEventParallel(State state, Event event, JBlock block, JVar contextVar) throws ClassNotFoundException, StateBuilderException{
        if(state.getParallel() == null){
            return;
        }
        
        String parallelClassName = getParallelClassName(state);
        String parallelLocalVariableName = getParallelLocalVariableName(state);
        
        // MicrowaveOperatingParallel parallelOperating = context.getMicrowaveOperatingParallel();
        JClass parallelClass = getCode().ref(parallelClassName);
        JVar parallelVar = block.decl(parallelClass, parallelLocalVariableName);
        parallelVar.init(contextVar.invoke("get" + parallelClassName));
        
        // parallelOperating.getMicrowaveEngineContext().evStart(); 
        // parallelOperating.getMicrowaveDoorContext().evStart();
        for(State stateOrthogonal : state.getParallel().getState()){
            JInvocation eventInvokation = parallelVar.invoke("get" + getContextClassName(stateOrthogonal)).invoke(event.getId());
            for(Parameter parameter : event.getParameter()){
                eventInvokation.arg(JExpr.ref(parameter.getName()));
            }
            
            block.add(eventInvokation);
        } 
    }
}