package com.stateforge.statebuilder.java;

import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

import com.stateforge.statebuilder.StateBuilderException;
import com.stateforge.statebuilder.model.Event;
import com.stateforge.statebuilder.model.ObjectType;
import com.stateforge.statebuilder.model.Parameter;
import com.stateforge.statebuilder.model.State;
import com.stateforge.statebuilder.model.StateMachineModel;
import com.stateforge.statebuilder.model.Timer;
import com.sun.codemodel.JBlock;
import com.sun.codemodel.JCatchBlock;
import com.sun.codemodel.JClass;
import com.sun.codemodel.JCodeModel;
import com.sun.codemodel.JDefinedClass;
import com.sun.codemodel.JExpr;
import com.sun.codemodel.JFieldRef;
import com.sun.codemodel.JInvocation;
import com.sun.codemodel.JMethod;
import com.sun.codemodel.JMod;
import com.sun.codemodel.JTryBlock;
import com.sun.codemodel.JVar;
import com.sun.codemodel.JTypeVar;

public class CoderContext extends CoderBaseJava {

    public CoderContext(StateMachineModel model, JCodeModel code) {
        super(model, code);
    }

    @Override
    public void code() throws StateBuilderException {
        //System.out.println("CoderContext");
        write(getModel().getStateMachine().getState());
    }

    /**
    Root context:

 public class MicrowaveContext
    extends AbstractContext<MicrowaveTopState, MicrowaveContext>

    Top context:

public class MicrowaveEngineContext
    extends AbstractContext<MicrowaveEngineState, MicrowaveContext>

     * */
    private void write(State state) throws StateBuilderException{
        //System.out.println("CoderContext" + state.getName());

        if(state.isTop() == true){
            try {
                String contextClassName = getModel().getStateMachine().getSettings().getNamespace() + "." + getContextClassName(state);
                JDefinedClass contextClass = getCode()._class(contextClassName);
                String contextParentClassName = getContextParentClassName(state);
                String stateTopClassName = getStateClassName(getModel().getStateTop(state));
                JClass parentClass = getCode().directClass("com.stateforge.statemachine.context.AbstractContext");
                JClass parentContextTypeVar = getCode().directClass(contextParentClassName);
                JClass topStateTypeVar = getCode().directClass(stateTopClassName);

                contextClass._extends(parentClass.narrow(topStateTypeVar, parentContextTypeVar));
                
                JMethod constructor = writeContructor(state, contextClass);
                writeEnterInitialState(state, contextClass);
                writeLeaveCurrentState(state, contextClass);
                writeEvents(state, contextClass);
                writeTimers(state, contextClass, constructor);
                writeParallel(state, contextClass, constructor);
            } catch (Exception e) {
                throw new StateBuilderException(e);
            }
        }

        if(state.getParallel() != null){
            for(State stateOrthogonal : state.getParallel() .getState()){
                write(stateOrthogonal);
            }
        }

        for(State stateChild : state.getState()){
            write(stateChild);
        }
    }

    private void writeTimers(State state, JDefinedClass contextClass, JMethod constructor) {

        if(state.isRoot() == false){
            return;
        }

        for(Timer timer : getModel().getTimersAll()){
            writeTimer(contextClass, timer, constructor);
        }
    }

    private void writeTimer(JDefinedClass contextClass, Timer timer, JMethod contructor) {
        // private ScheduledFuture<?> myTimerRedScheduledFuture
        JClass scheduledFutureClass = getCode().ref(ScheduledFuture.class);
        JVar timerScheduledFuture = contextClass.field(JMod.PRIVATE, scheduledFutureClass, "my" + timer.getName() + "ScheduledFuture");

        writeTimerStart(contextClass, timer, timerScheduledFuture);
        writeTimerStop(contextClass, timer, timerScheduledFuture);
    }

    /**
    public void timerStartTimerLight(long duration) {
        getObserver().onTimerStart(this.getName(), "TimerLight", duration);
        final TrafficLightContext me = this;
        myTimerLightScheduledFuture = getExecutorService().schedule(new Runnable() {


            public void run() {
                getStateCurrent().evTimerLight(me);
            }

        }
        , duration, TimeUnit.MILLISECONDS);
    }

     * @param contextClass
     * @param timer
     */
    private void writeTimerStart(JDefinedClass contextClass, Timer timer, JVar timerField) {
        JMethod timerStartMethod = contextClass.method(JMod.PUBLIC, void.class, "timerStart" + timer.getName());
        timerStartMethod.javadoc().add("Start the timer " + timer.getName());
        timerStartMethod.param(getCode().directClass("long"), "duration");

        JBlock block = timerStartMethod.body();

        // context.getObserver().onTimerStart(getName(), "TimerRed", duration);
        block.add(JExpr.invoke("getObserver").invoke("onTimerStart").arg(JExpr._this().invoke("getName")).arg(JExpr.lit(timer.getName())).arg(JExpr.ref("duration")));

        // final TrafficLightContext me = this; 
        block.decl(JMod.FINAL, contextClass, "me", JExpr._this());

        JDefinedClass anon = getCode().anonymousClass(java.lang.Runnable.class);

        /*   public void run() {
                getStateCurrent().EvTimerMaxDuration(me);

            }
         */
        JMethod runMethod = anon.method(JMod.PUBLIC, void.class, "run");
        JBlock runBlock = runMethod.body();
        JTryBlock tryBlock = runBlock._try();
        tryBlock.body().invoke(JExpr.invoke("getStateCurrent"), timer.getId()).arg(JExpr.ref("me"));

        addCatchBlockPrintException(tryBlock);
        JInvocation invokation = JExpr.invoke("getExecutorService").invoke("schedule").arg(JExpr._new(anon)).arg(JExpr.ref("duration")).arg(getCode().ref(TimeUnit.class).staticRef("MILLISECONDS"));
        block.assign(timerField, invokation);
    }

    /**
    public void timerStopTimerLight() {
        getObserver().onTimerStop(this.getName(), "TimerLight");
        if (myTimerLightScheduledFuture!= null) {
            myTimerLightScheduledFuture.cancel(false);
        }
    }

     * @param contextClass
     * @param timer
     */

    private void writeTimerStop(JDefinedClass contextClass, Timer timer, JVar timerField) {
        JMethod timerStopMethod = contextClass.method(JMod.PUBLIC, void.class, "timerStop" + timer.getName());
        timerStopMethod.javadoc().add("Stop the timer " + timer.getName());

        JBlock block = timerStopMethod.body();
        // context.getObserver().onTimerStart(getName(), "TimerRed", duration);
        block.add(JExpr.invoke("getObserver").invoke("onTimerStop").arg(JExpr._this().invoke("getName")).arg(JExpr.lit(timer.getName())));
        /*
        if (myTimerLightScheduledFuture!= null) {
            myTimerLightScheduledFuture.cancel(false);
        }
         */
        block._if(timerField.ne(JExpr._null()))._then().add(timerField.invoke("cancel").arg(JExpr.FALSE));
    }

    /**
     * Root context:

    public MicrowaveContext(MicrowaveAction action) {
        super();
        _action = action;
        setName("MicrowaveContext");
        setInitialState(MicrowaveInitialState.getInstance());
    }

      Top context:

    public MicrowaveEngineContext(MicrowaveAction action, MicrowaveContext contextParent) {
        super(contextParent);
        _action = action;
        setName("MicrowaveEngineContext");
        setInitialState(MicrowaveOffState.getInstance());
    }

     * @param state
     * @param contextClass
     * @throws StateBuilderException
     */
    private JMethod writeContructor(State state, JDefinedClass contextClass) throws StateBuilderException{
        JMethod constructor = contextClass.constructor(JMod.PUBLIC);
        String contextParentClassName = getContextParentClassName(state);
        constructor.javadoc().add("Context constructor");

        String contextClassName = getContextClassName(state);

        JBlock block = constructor.body();

        JInvocation superInvokation = block.invoke("super");

        for(ObjectType obj : getModel().getStateMachine().getSettings().getObject()){
            //Object object1
            //Add constructor parameters
            String objFullClassName = obj.getInclude().isEmpty() == false ? obj.getInclude() + "." + obj.getClazz() : obj.getClazz();
            JVar objParam = constructor.param(getCode().directClass(objFullClassName), obj.getInstance());
            JVar objField = contextClass.field(JMod.PRIVATE, getCode().directClass(objFullClassName), "_" + obj.getInstance()); 
            constructor.body().assign(objField, objParam);
            //Add Object getter
            JMethod getter = contextClass.method(JMod.PUBLIC, 
                    getCode().directClass(objFullClassName), 
                    "get" + obj.getClazz());
            getter.body()._return(objField);
        }

        if(state.isRoot() == false){
            superInvokation.arg(constructor.param(getCode().directClass(contextParentClassName), "contextParent"));
        }

        // setName("HelloWorldContext");
        block.invoke("setName").arg(contextClassName);

        // setInitialState(HelloWorldIdleState.getInstance());
        block.invoke("setInitialState").arg(JExpr.ref(getStateClassName(getModel().getStateLeaf(state))).invoke("getInstance"));
        return constructor;

    }

    private void writeParallel(State state, JDefinedClass abstractContext, JMethod constructor) {        
        for(State stateParallel : state.getParallelList()){
            JClass parallelClass = getCode().directClass(getParallelClassName(stateParallel));
            //Field
            JVar parallelField = abstractContext.field(JMod.PRIVATE, parallelClass, getParallelFieldName(stateParallel));

            // _parallelOperating = new MicrowaveContextOperatingParallel(this, microwave);
            JInvocation parallelCreateInvokation = JExpr._new(parallelClass);

            // this
            parallelCreateInvokation = parallelCreateInvokation.arg(JExpr._this());

            for(ObjectType obj : getModel().getStateMachine().getSettings().getObject()){
                parallelCreateInvokation = parallelCreateInvokation.arg(JExpr.ref(obj.getInstance()));
            }

            constructor.body().assign(parallelField, parallelCreateInvokation);

            // Getter
            JMethod getter = abstractContext.method(
                    JMod.PUBLIC, 
                    parallelClass, 
                    "get" + getParallelClassName(stateParallel));
            getter.body()._return(parallelField);
        }
    }

    private void writeEvents(State state, JDefinedClass contextClass) throws ClassNotFoundException{
        for(Event event : getModel().getEventsAll()){
            //if(event instanceof Timer){
            //    continue; 
            //} else {
                writeEventAsync(state, event, contextClass);
                writeEventSync(state, event, contextClass);
           // }
        }
    }

    /**
     *  public void evStart(String msg, int value) {
     *      getStateCurrent().evStart(this, msg, value);
     *  }
     * @param state
     * @param event
     * @param contextClass
     * @throws ClassNotFoundException
     */
    private void writeEventSync(
            State state, 
            Event event,
            JDefinedClass contextClass) throws ClassNotFoundException{
        if((getModel().getStateMachine().getSettings().isAsynchronous() == true) && (state.isRoot() == true)){
            return;
        } 
        
        int mod = JMod.PUBLIC;
        String eventMethodName = event.getId();;

        JMethod eventMethod = contextClass.method(mod, void.class, eventMethodName);
        eventMethod.javadoc().add("Event " + event.getId());

        JBlock block = eventMethod.body();

        JInvocation superInvokation = block.invoke(JExpr.invoke("getStateCurrent"), event.getId()).arg(JExpr._this());

        for(Parameter parameter : event.getParameter()){
            eventMethod.param(getCode().parseType(parameter.getType()), parameter.getName());
            superInvokation = superInvokation.arg(JExpr.ref(parameter.getName()));
        }
    }

    /**
        final TrafficLightContext me = this;
        getExecutorService().execute(new Runnable() {
            public void run() {
                getStateCurrent().evStop(me);
            }
        }
        );

     * @param state
     * @param event
     * @param contextClass
     * @throws ClassNotFoundException
     */
    private void writeEventAsync(State state, Event event, JDefinedClass contextClass) throws ClassNotFoundException {
        if ((getModel().getStateMachine().getSettings().isAsynchronous() == false) || (state.isRoot() == false)) {
            return;
        }

        if(event instanceof Timer){
            return; 
        }
        
        JMethod eventMethod = contextClass.method(JMod.PUBLIC, void.class, event.getId());
        eventMethod.javadoc().add("Asynchronous event " + event.getId());

        JBlock block = eventMethod.body();
        // final TrafficLightContext me = this; 
        block.decl(JMod.FINAL, contextClass, "me", JExpr._this());

        JDefinedClass anon = getCode().anonymousClass(Runnable.class);

        JMethod runMethod = anon.method(JMod.PUBLIC, void.class, "run");
        JBlock runBlock = runMethod.body();

        JTryBlock tryBlock = runBlock._try();
        
        JInvocation syncInvokation = JExpr.invoke("getStateCurrent").invoke(event.getId()).arg(JExpr.ref("me"));
        for(Parameter parameter : event.getParameter()){
            eventMethod.param(JMod.FINAL, getCode().parseType(parameter.getType()), parameter.getName());
            syncInvokation = syncInvokation.arg(JExpr.ref(parameter.getName()));
        }
        
        tryBlock.body().add(syncInvokation);
        addCatchBlockPrintException(tryBlock);
        
        JInvocation invokation = JExpr.invoke("getExecutorService").invoke("execute").arg(JExpr._new(anon));

        block.add(invokation);
    }

    /**
     * catch(Exception exception){
     *     onEnd(exception);
     * }
     * @param tryBlock
     */
    private void addCatchBlockPrintException(JTryBlock tryBlock){
    	JCatchBlock catchBlock = tryBlock._catch(getCode().ref(Exception.class));
		JVar exceptionParam = catchBlock.param("exception");
		JBlock catchBody = catchBlock.body();
		catchBody.add(JExpr.invoke("onEnd").arg(exceptionParam));
    }
    /**
     * 
     *  public void enterInitialState() {
     *      StateOperation.walkTreeEntry(this, HelloWorldRootState.getInstance(), HelloWorldIdleState.getInstance());
     *  }
     *  
     * WriteMethodEnterInitialState
     * @param state
     * @throws StateBuilderException 
     */
    private void writeEnterInitialState(State state, JDefinedClass contextClass) throws StateBuilderException{
        JMethod enterInitialStateMethod = contextClass.method(JMod.PUBLIC, void.class, "enterInitialState");
        enterInitialStateMethod.javadoc().add("Enter the initial state");
        JBlock block = enterInitialStateMethod.body();
        // StateOperation.walkTreeEntry
        JInvocation invokation = block.staticInvoke(getCode().directClass("com.stateforge.statemachine.algorithm.StateOperation"), "walkTreeEntry");
        // this
        invokation = invokation.arg(JExpr._this());
        // HelloWorldRootState.getInstance()
        invokation = invokation.arg(JExpr.ref(getStateClassName(getModel().getStateTop(state))).invoke("getInstance"));
        // HelloWorldIdleState.getInstance()
        invokation = invokation.arg(JExpr.ref(getStateClassName(getModel().getStateLeaf(state))).invoke("getInstance"));
    }

    /**
     * protected void leaveCurrentState(){
     *     StateOperation.walkTreeEntry(this, getStateCurrent(), HelloWorldRootState.getInstance());
     * }
     * 
     * WriteMethodEnterInitialState
     * @param state
     * @throws StateBuilderException 
     */
    private void writeLeaveCurrentState(State state, JDefinedClass contextClass) throws StateBuilderException{
        JMethod leaveCurrentStateMethod = contextClass.method(JMod.PUBLIC, void.class, "leaveCurrentState");
        leaveCurrentStateMethod.javadoc().add("Leave the current state");
        JBlock block = leaveCurrentStateMethod.body();
        // StateOperation.walkTreeEntry
        JInvocation invokation = block.staticInvoke(getCode().directClass("com.stateforge.statemachine.algorithm.StateOperation"), "walkTreeExit");
        // this
        invokation = invokation.arg(JExpr._this());
        // getStateCurrent()
        invokation = invokation.arg(JExpr._this().invoke("getStateCurrent"));
        // HelloWorldTopState.getInstance()
        invokation = invokation.arg(JExpr.ref(getStateClassName(getModel().getStateTop(state))).invoke("getInstance")); 
    }
}
