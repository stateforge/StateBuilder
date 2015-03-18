package com.stateforge.statebuilder.cpp;

import com.stateforge.statebuilder.StateBuilderAppBase;
import com.stateforge.statebuilder.StateBuilderException;
import com.stateforge.statebuilder.model.Actions;
import com.stateforge.statebuilder.model.Event;
import com.stateforge.statebuilder.model.EventSource;
import com.stateforge.statebuilder.model.ObjectType;
import com.stateforge.statebuilder.model.Observer;
import com.stateforge.statebuilder.model.Parallel;
import com.stateforge.statebuilder.model.State;
import com.stateforge.statebuilder.model.StateKind;
import com.stateforge.statebuilder.model.StateMachineModel;
import com.stateforge.statebuilder.model.Timer;
import com.stateforge.statebuilder.model.TimerStartAction;
import com.stateforge.statebuilder.model.TimerStopAction;
import com.stateforge.statebuilder.model.Transition;

import java.util.List;
import java.util.Map;
import java.util.logging.Level;

public class StateBuilderCpp extends StateBuilderCppCommon {
    public static final String FSM_INCLUDE_TEMPLATE = "fsm/Fsm.hpp";
    public static final String FSM_ASYNC_INCLUDE_TEMPLATE = "fsm/FsmAsync.hpp";
    private static final String FILE_EXTENSION = ".cpp";
	private int _stateId = 0;

    public StateBuilderCpp(StateMachineModel model) {
        super(model);      
    }

    @Override
    public void generate() throws StateBuilderException {
        writeCompilerInfo(getName() + FSM_SUFFIX + FILE_EXTENSION, commandLine);
        writeInclude();
        writeNamespaceBegin();
        writeWarningDisableBegin();
        writeStateNameToId();
        writeTransitionNameToId();
        writeStateInstantiation();
        writeContext();
        writeStateImplementation();

        if(getStateMachine().getSettings().isAsynchronous() == true){
            writeContextAsyncEventImplementation();
        }
        
        writeWarningDisableEnd();
        writeNamespaceEnd();
    }

    public String getFileExtension() {
        return FILE_EXTENSION;
    }

    /**
     * Write all includes 
     * #include <fsm/FsmAsync.hpp>
     * #include <UrlGet.h>
     * #include "UrlGetFsm.h"
     */
    private void writeInclude(){
        getStream().println("// Includes");

        getStream().print("#include \""); 
        getStream().println(getName() + 
        		StateBuilderAppBase.FSM_SUFFIX + 
                StateBuilderHpp.FILE_EXTENSION + 
                "\"");
        getStream().println();

        
        String fsmInclude = FSM_INCLUDE_TEMPLATE;
        if(getStateMachine().getSettings().isAsynchronous() == true){
            fsmInclude = FSM_ASYNC_INCLUDE_TEMPLATE;   
        }
        getStream().println("#include <" + fsmInclude + ">");

        // #include <object1Header.h>
        // #include <object2Header.h>
        for (ObjectType object : getStateMachine().getSettings().getObject()) {
            getStream().println("#include <" + object.getInclude()+ ">");
        }

        getStream().println();
    }

    private String getStateNameToId() throws StateBuilderException{
    	return "kStateNameToId" + getModel().getContextClassName(getStateMachine().getState());
    }
    
    private String getTransitionNameToId() throws StateBuilderException{
    	return "kTransitionNameToId" + getModel().getContextClassName(getStateMachine().getState());
    }

    /**
     * Write state name and id 
     * 
     * fsm::StateNameToId kStateNameToId[] = 
     * {
     *   {"Led", 0},
     *   {"SwitchedOff", 1},
     *   {"SwitchedOn", 2},
     *   {NULL, 0}
     * };
     */
    private void writeStateNameToId() throws StateBuilderException {
        getStream().println("// State names and ids");
        _stateId = 0;
        getStream().println("const fsm::StateNameToId " + getStateNameToId() + "[] = ");
        getStream().println("{");
        pushIdentLevel();
        writeStateNameToId(getStateMachine().getState());
        getStream().println(getIdent() + "{NULL, 0}");
        popIdentLevel();
        getStream().println(getIdent() + "};");
        getStream().println("");
        _stateId = 0;
    }
   
   
    /**
     *  {"SwitchedOff", 1},
     * @param state
     */
    private void writeStateNameToId(State state) throws StateBuilderException {
    	if (state.getKind() == StateKind.HISTORY) {
    		return;
    	}
    	    	
        getStream().println(getIdent() + "{\"" + state.getName() +  "\", " + _stateId + "},");
        
        _stateId++;
        // Sub states
        List<State> stateSubList = state.getState();
        for(State stateSub: stateSubList){
        	writeStateNameToId(stateSub);
        }
    
    	//Special case for orthogonal state
        Parallel parallel = state.getParallel();
        if(parallel != null){
        	List<State> stateOrthogonalList = parallel.getState();
        	for(State stateOrthogonal : stateOrthogonalList){
        		writeStateNameToId(stateOrthogonal);    		
        	}
        }	        
    }    

    private void writeTransitionNameToId() throws StateBuilderException {
        getStream().println("// Transition names and ids");
        getStream().println("const fsm::TransitionNameToId " + getTransitionNameToId() + "[] = ");
        getStream().println("{");
        pushIdentLevel();
        Map<String , Integer> transitionMap = getModel().getTransitionMap(); 
        for (Map.Entry<String, Integer> entry : transitionMap.entrySet())
        {
            getStream().println(getIdent() + "{\"" + entry.getKey() +  "\", " + entry.getValue() + "},");
        }
        
        getStream().println(getIdent() + "{NULL, 0}");
        popIdentLevel();
        getStream().println(getIdent() + "};");
        getStream().println("");
    }
    
    /**
     * Write state instantiation
     */
    private void writeStateInstantiation() throws StateBuilderException {
        getStream().println("// State Instantiation");
        writeStateInstantiation(getStateMachine().getState());
        getStream().println("");
    }
    
    /**
     * Write state static initialization for one state:
     *   static LedSwitchedOffState ledSwitchedOffState("SwitchedOff");
     *  
     * @param state
     */
    private void writeStateInstantiation(State state) throws StateBuilderException {
    	if (state.getKind() == StateKind.HISTORY) {
    		return;
    	}
    	
        String stateClassName = getStateClassName(state);
        
        getStream().println(getIdent() +
        		"static const " + stateClassName + " " + getStateVariable(state) +
        		"(" + getStateNameToId() + "[" + _stateId + "].pcName, " + _stateId + ");");
        
        _stateId++;
        // Sub states
        List<State> stateSubList = state.getState();
        for(State stateSub: stateSubList){
            writeStateInstantiation(stateSub);
        }
    
    	//Special case for orthogonal state
        Parallel parallel = state.getParallel();
        if(parallel != null){
        	m_logger.log(Level.INFO, "state " + stateClassName + " is parallel");
        	List<State> stateOrthogonalList = parallel.getState();
        	for(State stateOrthogonal : stateOrthogonalList){
        		writeStateInstantiation(stateOrthogonal);    		
        	}
        }	        
    }    
    
    /**
     * Write state implementations
     */
    private void writeStateImplementation() throws StateBuilderException {
        getStream().println(getIdent() + "/**************************************************************");
    	getStream().println(getIdent() + "  * States implementations  ");
        getStream().println(getIdent() + " **************************************************************/");
    	
    	State topState = getStateMachine().getState();         
        generateStateImplementation(topState, 
        		                    getModel().getContextClassName(topState), 
                                    getStateClassNameRoot(topState));
    } 
    
    /**
     * Write the state implementation for one machine
     * @param machineName the current machine
     * @throws StateBuilderException
     */
    private void generateStateImplementation(State state,
                                             String contextClassName,
    		                                 String stateParentName) throws StateBuilderException {
       	if((state == null) || (stateParentName == null)){
    		throw new StateBuilderException("Internal Exception");
    	}
    	
    	if (state.getKind() == StateKind.HISTORY) {
    		// Do not generate a state class for pseudo state history.
    		return;	
    	}
    	
    	String stateClassName = getStateClassName(state);
        
        getStream().println(getIdent() + "/**************************************************************");
        getStream().println(getIdent() + " * State implementation for " + stateClassName);
        getStream().println(getIdent() + " **************************************************************/");
        getStream().println();
        getStream().println(getIdent() + "// State Constructor");
        writeStateContructorImplementation(state, stateParentName, false);
        getStream().println(getIdent() + "// State Destructor");
        writeStateDestructorImplementation(state);
        
        getStream().println(getIdent() + "// State GetInstance");
        writeStateGetInstance(state);
        
        writeStateOnEntryExitImplementation(state, ONENTRY);
        writeStateOnEntryExitImplementation(state, ONEXIT);
        
        List<Event> eventList;
        
        eventList = getModel().getEventsForState(state);
        
        writeStateEventImplementation(state, contextClassName , eventList);
        
        //Do it recursively
        List<State> stateSubList = state.getState();
        for(State stateSub : stateSubList){
            generateStateImplementation(stateSub, contextClassName, getStateClassName(state));
        }        
        
    	//Special case for orthogonal states
        Parallel parallel = state.getParallel();
        if(parallel != null){
        	List<State> stateList = parallel.getState();
        	for(State stateOrthogonal : stateList){
        		generateStateImplementation(
        				stateOrthogonal, 
        				getModel().getContextClassName(stateOrthogonal), 
                        getStateClassNameRoot(stateOrthogonal));    		
        	}
        }	            
        
        getStream().println();         
    }    
    
    /**
     * Write the OnEntry or Exit implementation,   
     * @param state the current state 
     * @throws StateBuilderException
     */
    private void writeStateOnEntryExitImplementation(State state, String onEntryExit) throws StateBuilderException {
        getStream().println(getIdent() + "// " + onEntryExit);
        String stateName = getStateClassName(state);
        String contextClassName = getModel().getContextClassName(state);
        Actions actions;
        boolean onEntry;
        
        if(onEntryExit.equals(ONENTRY)){
        	actions = state.getOnEntry();
        	onEntry = true;
        } else {
        	actions = state.getOnExit();
        	onEntry = false;
        }
        
        writeFunctionImplementationBegin("void", stateName, onEntryExit, contextClassName, null, true);
        
        if(onEntry == false){
            writeOnExitHistory(state);
        	writeStateOnExitParallel(state);
        }
        
        getStream().println(getIdent() + "// " + onEntryExit + " for state " + stateName);
        
        if(getStateMachine().getSettings().getObserver().equals(Observer.NO)){
        } else if(getStateMachine().getSettings().getObserver().equals(Observer.INT)){
            getStream().println(getIdent() + "fsm::IObserverInt *pObserver = " + 
            		getModel().getContextInstance() + "." + GETOBSERVER + ";");
            getStream().println(getIdent() + "if(pObserver){");
            pushIdentLevel();
            
            getStream().print(getIdent() + "pObserver->" + onEntryExit + "(");

            getStream().println(getModel().getContextInstance() + "." + GETID + ", " +
                    getStateClassName(state) + "::" + GETINSTANCE + "." + GETID  + ");");
            
            popIdentLevel();
            getStream().println(getIdent() + "}");
        } else {
            //IObserver *pObserver = context.GetObserver();
            
            getStream().println(getIdent() + "fsm::IObserver *pObserver = " + 
            		getModel().getContextInstance() + "." + GETOBSERVER + ";");
            getStream().println(getIdent() + "if(pObserver){");
            pushIdentLevel();
            
            getStream().print(getIdent() + "pObserver->" + onEntryExit + "(");
            
                getStream().println(getModel().getContextInstance() + "." + GETNAME + ", " +
                        getStateClassName(state) + "::" + GETINSTANCE + "." + GETNAME  + ");");

            popIdentLevel();
            getStream().println(getIdent() + "}");
        }   
                
        if(actions != null){
            writeActions(state, actions.getAction(), actions.getActionGroup(), true);        	
        }
            
        if(onEntry == true){ 
            writeStateOnEntryParallel(state);
        }
        
        popIdentLevel();
        getStream().println(getIdent() + "}");
        getStream().println();
    }    
    
    /**
     * When getting out of state which has child history state, save the current state.
     * context.SaveState()
     * @param state
     */
    private void writeOnExitHistory(State state) {
    	if(state.isHasHistory()){
    		getStream().println(getIdent() + "// Save the current state.");
    		getStream().println(getIdent() + getModel().getContextInstance() + "." + SAVE_STATE + ";");
    	}
    }

	/**
     * Write the code to handle the onEntry of a parallel state : 
     *
     *   UrlGetParallelRunning &parallelRunning = context.GetParallelRunning();
     *   context.SetParallel(&parallelRunning);
     *   parallelRunning.SetStateActive(2);
     *   parallelRunning.GetContextHttp().EnterInitialState();
     *   parallelRunning.GetContextFtp().EnterInitialState();
     * 
     * @param state
     * @throws StateBuilderException
     */
    private void writeStateOnEntryParallel(State state) throws StateBuilderException {
    	Parallel parallel = state.getParallel();
    	if(parallel == null){
            return;
        }
        
        String parallelVar = PARALLEL + state.getName();
        // UrlGetParallelRunning &parallelRunning = context.GetParallelRunning();
        getStream().println(getIdent() + getParallelClassName(state) + " &" + parallelVar + " = " + 
        		getModel().getContextInstance() + "." + getGetParallel(state) + "();");
        
        // context.SetParallel(&parallelRunning);
        getStream().println(getIdent() + getModel().getContextInstance() + "." + SETPARALLEL + "(&" + parallelVar + ");");
        
        // parallelRunning.SetStateActive(2);
        getStream().println(getIdent() + parallelVar + "." + SETSTATEACTIVE + "(" + parallel.getState().size() + ");");
        
        // parallelRunning.GetContextHttp().EnterInitialState();
        // parallelRunning.GetContextFtp().EnterInitialState();
    	List<State> stateList = parallel.getState();
    	for(State stateOrthogonal : stateList){
            String nameGetContext = getGetContext(stateOrthogonal);
            getStream().println(getIdent() + parallelVar + "." + nameGetContext + "()." + ENTER_INITIAL_STATE + ";");
    	}
    }
    
    /**
     * Write the code to handle the onExit of a parallel state : 
     * 
     * UrlGetParallelRunning &parallelUrlGetRunning = context.GetParallelRunning();
     * context.SetParallel(NULL);
     * parallelUrlGetRunning.GetContextHttp().LeaveCurrentState();
     * parallelUrlGetRunning.GetContextFtp().LeaveCurrentState();
     *    
     * @param state
     * @throws StateBuilderException
     */
    private void writeStateOnExitParallel(State state) throws StateBuilderException {
        Parallel parallel = state.getParallel();
        if(parallel == null){
            return;
        }
        m_logger.log(Level.INFO, "state " + state.getName() + " is parallel");
        String parallelVar = PARALLEL + getStateClassName(state);
        // UrlGetParallelRunning &parallelUrlGetRunning = context.GetParallelRunning();
        getStream().println(getIdent() + getParallelClassName(state) + " &" + parallelVar + " = " + 
        		getModel().getContextInstance() + "." + getGetParallel(state) + "();");
        
        // context.SetParallel(NULL);
        getStream().println(getIdent() + getModel().getContextInstance() + "." + SETPARALLEL + "(NULL);");
        
        // parallelUrlGetRunning.GetContextHttp().LeaveCurrentState();
        // parallelUrlGetRunning.GetContextFtp().LeaveCurrentState();
        List<State> stateList = parallel.getState();
        for(State stateOrthogonal : stateList){
            String nameGetContext = getGetContext(stateOrthogonal);
            getStream().println(getIdent() + parallelVar + "." + nameGetContext + "()." + LEAVE_CURRENT_STATE + ";");            
        }
    }    
       
    /**
     * Write the state implementation for the given machine, state and event list
     * @param state the current state
     * @param eventList the event list
     * @throws StateBuilderException
     */
    private void writeStateEventImplementation(State state, 
    		                                      String contextClassName, 
                                                  List<Event> eventList) throws StateBuilderException {
        String stateClassName = getStateClassName(state);
        
        getStream().println(getIdent() + "// Events implementation for state  " + stateClassName);
        
        for(Event event : eventList){
            writeStateEventImplementation(state, contextClassName, event);
        }                    
        getStream().println();
    }    
    
    /**
     * Write the state constructor implementation
     * @param className the state class 
     * @param classBaseName the base state class
     * @param bFinal true if the state is a final state
     * @throws StateBuilderException
     */
    private void writeStateContructorImplementation(State state,
    		                                            String stateParentName, 
                                                        boolean bFinal) throws StateBuilderException{
        String stateClassName = getStateClassName(state);
        getStream().println(getIdent() + 
        		            stateClassName + 
                            "::" + 
                            stateClassName + 
                            "(const char* pName, int id) : " + 
                            stateParentName + 
                            "(pName, id)");
        getStream().println(getIdent() + "{");
        pushIdentLevel();
        if(state.getKind() == StateKind.FINAL){
        	getStream().println(getIdent() + MEMBER_STATEKIND + " = " + FSM_NAMESPACE + "::" + E_STATE_FINAL + ";");
        } 

        // Set the parent state if any
        State stateParent = state.getParent();
        if(stateParent != null){
            getStream().println(getIdent() + MEMBER_STATE_PARENT + 
                    " = &" + getStateClassName(stateParent) + "::" + GETINSTANCE + ";");
        }
        popIdentLevel();
        getStream().println(getIdent() + "}");
        getStream().println();
    }
    
    /**
     * Write the state constructor implementation
     * const LedLedState& LedLedState::GetInstance()
     * {
     *    return ledLedState;
     * }
     * @param className the state class 
     * @param classBaseName the base state class
     * @param bFinal true if the state is a final state
     * @throws StateBuilderException
     */
    private void writeStateGetInstance(State state) throws StateBuilderException{
        String stateName = getStateClassName(state);
        getStream().println(getIdent() +
                            "const " + stateName + "& " +
                            stateName + 
                            "::" + 
                            GETINSTANCE);
        getStream().println(getIdent() + "{");
        pushIdentLevel();
        /*
        getStream().println(getIdent() + "if(" + getStateVariable(state) + " == NULL){");
        pushIdentLevel();
        
        getStream().println(getIdent() + getStateVariable(state) + 
                            " = new " + stateName + "(\"" + state.getName() + "\");");
        
        popIdentLevel();
        getStream().println(getIdent() + "}");
        getStream().println(getIdent() + "return *" + getStateVariable(state) + ";");
        */
        getStream().println(getIdent() + "return " + getStateVariable(state) + ";");
        popIdentLevel();        
        getStream().println(getIdent() + "}");
        getStream().println();
    }    
    
    /**
     * Write the state destructor implementation
     * @param className the state class
     * @throws StateBuilderException
     */
    private void writeStateDestructorImplementation(State state) throws StateBuilderException {
    		                                            
    	String stateClassName = getStateClassName(state);
    	getStream().println(getIdent() + stateClassName + "::~" + stateClassName + "()");
        getStream().println(getIdent() + "{");
        pushIdentLevel();
        //getStream().println(getIdent() + "delete " + getStateVariable(state) + ";");
        popIdentLevel();
        getStream().println(getIdent() + "}");
        getStream().println();
    }
    
    /**
     * Write event member functions: 
     *     void State1::EvEvent2(contextClassName& context, Param2 param2, Param3 *pParam){
     *       return;
     *     }
     * @param state the current state
     * @param contextClassName  the context class name
     * @param event the events to generate
     * @throws StateBuilderException
     */
    private void writeStateEventImplementation(State state,
    		                                      String contextClassName, 
                                                  Event event) throws StateBuilderException {        
        String eventId = event.getId();
        String stateClassName = getStateClassName(state);

        List<Transition> transitionList = getModel().getTransitionList(state, event);

        // If this state doesn't handle totally this event, so pass it to its father 
        // if it handles it        
        State stateParent = state.getParent();
        
        boolean isEventHandledByParent = false;
        
        if(((getModel().isAllTransitionGuarded(state, stateClassName, transitionList) == true) || (transitionList.isEmpty())) &&
                (stateParent != null) &&
                (state.getKind() != StateKind.FINAL) &&  
                (getModel().isEventHandledByState(stateParent, event) == true)){
        	isEventHandledByParent = true;
        }  
        
        //If the event is handled by the parent state, the parameters must not be commented
        writeFunctionImplementationBegin("void", 
                                            stateClassName, 
                                            eventId, 
                                            contextClassName, 
                                            event, 
                                            true);
        
        getStream().println(getIdent() + "// " + transitionList.size() + " transition(s)");
         
        writeStateEventTransitionImplementation(state, stateClassName, transitionList);
                
        //handle parallel state
        if(state.getParallel() != null){  
        	getStream().println(getIdent() + "// State " + state.getName() + " is parallel");
            writeStateEventParallelImplementation(state, event);
        }
        
        if(isEventHandledByParent == true){
            // StateParent::GetInstance().EvEvent(context, param1, param2);
            getStream().println(getIdent() + "// pass the event to the parent state");
            getStream().print(getIdent() + getStateClassName(stateParent));
            getStream().print("::" + GETINSTANCE + "." + eventId + "(");
            getStream().print(getModel().getContextInstance());
            writeParameterList(event);
            getStream().println(");");
            
        }
                
        popIdentLevel();
        getStream().println(getIdent() +  "}");
        getStream().println();
    }    
    
    /**
     * Write the state event implementation
     * @param state the state 
     * @param transitionList the transition's list
     * @throws StateBuilderException
     */
    private void writeStateEventTransitionImplementation(State state,
    		                                                String stateClassName,
                                                            List<Transition> transitionList) throws StateBuilderException {
    	if(transitionList.isEmpty() == false){
    	    writeObjectsDeclaration();
    	}
    	
        for(Transition transition : transitionList){
            writeStateEventTransitionImplementation(state, stateClassName, transition);
        }
    }
    
    /**
     * Write event implementation for parallel state, events are forwarded to all orthogonal state
     * FsmNameParralelS1 &parallelS1 = context.GetParralelS1();
     *   parallelS1.GetContextS11().GetState()->EvStart(parallelS1.GetContextS11());
     *   parallelS1.GetContextS12().GetState()->EvStart(parallelS1.GetContextS12());
     * @param state
     * @param event
     * @throws StateBuilderException
     */
    private void writeStateEventParallelImplementation(State state, 
                                                         Event event) throws StateBuilderException {
        //handle parallel state
        if(state.getParallel() == null){         
            return;
        }
        List<State> stateList = state.getParallel().getState();

        getStream().println(getIdent() + "// Forward events to all orthogonal states");
        
        String parallelVar = PARALLEL + getStateClassName(state);
        // FsmNameParralelS1 &parallelS1 = context.GetParralelS1();
        getStream().println(getIdent() + getParallelClassName(state) + " &" + parallelVar + " = " + 
        		getModel().getContextInstance() + "." + getGetParallel(state) + "();");
        
        for(State stateOrthogonal : stateList){
            String nameGetContext = getGetContext(stateOrthogonal);
            getStream().print(getIdent() + parallelVar + "." + nameGetContext + 
                                "()." + GETSTATE + "." + event.getId() + "(" + parallelVar + "." + nameGetContext + "()");
            writeParameterList(event);
            getStream().println(");");
        }
    }

    /**
     * Write the state event implementation :
     *   if((guardCondition) == true){
     *     ProcessTransitionPre<ObjectContext, ObjectState>(context, stateNext);
     *     action1();
     *     action2();
     *     ProcessTransitionPost<ObjectContext, ObjectState>(context, stateNext);
     *     return;
     *   }
     *   
     * @param state the state
     * @param transition the transition
     * @return true if the transition has a guard condition
     * @throws StateBuilderException
     */
    private boolean writeStateEventTransitionImplementation(State state, 
    		                                                   String contextClassName,
                                                               Transition transition) throws StateBuilderException {
    	String stateName = getStateClassName(state);
    	String condition = getModel().getCondition(transition);
        State stateNext = getModel().getState(transition.getNextState());
        String stateNameNext = null;
        if(stateNext != null){
        	// stateNext is null for internal transition
        	stateNameNext = stateNext.getName();          
        }
        //find the next state's leaf
        State stateNextLeaf = getModel().getStateLeaf(stateNext);
        
        m_logger.log(Level.INFO, "state " + stateName + ", next " + 
                     stateNameNext + ", guard " + condition);
        
        if(condition.isEmpty() == false){
            getStream().println(getIdent() + "if((" + condition + ") == true){");
            pushIdentLevel();
        }     
        if(getStateMachine().getSettings().getObserver().equals(Observer.NO)){
        } else if(getStateMachine().getSettings().getObserver().equals(Observer.INT)){
        	// Set the transition id
        	int transitionId = getModel().getTransitionMap().get(getModel().getTransitionName(transition));
            getStream().println(getIdent() +
            		getModel().getContextInstance() + "." + SET_TRANSITION_ID +
            		"(" + transitionId + ");");
        } else {
        	// Set the transition name
            getStream().println(getIdent() +
            		getModel().getContextInstance() + "." + SET_TRANSITION_NAME +
            		"(\"" + getModel().getTransitionName(transition) + "\");");
        }
        
       
        writeProcessTransition(state, stateNextLeaf, "Pre");
        
        // Perform action's list
        writeActions(state, transition.getAction(), transition.getActionGroup(), false);
        
        writeProcessTransition(state, stateNextLeaf, "Post");
        
        writeContextOnEnd(state, stateNextLeaf);
        
        // return now
        getStream().println(getIdent() + "return;");
        
        if(condition.isEmpty() == false){
            popIdentLevel();
            getStream().println(getIdent() +  "}");
            return true;
        } else {
            return false;
        }
    }
    
    /*
     *  If next state is final, invoke:
     *  context.OnEnd();
     *  or
     *  context.ContextParent.OnEnd(); 
     */ 
    private void writeContextOnEnd(State state, State stateNext) throws StateBuilderException {
    	if((stateNext != null) && (stateNext.getKind() == StateKind.FINAL)){
    	    String contextInstance = getModel().getContextParentString(state, stateNext);
    	    getStream().println(getIdent() + contextInstance + "." + ON_END + ";");  
    	}	
	}

	/*
     * fsm::ProcessTransitionPre<UrlGetContext, UrlGetTopState>(context, &UrlGetErrorState::GetInstance(), "TimerMaxDuration");
     * fsm::ProcessTransitionPre<UrlGetHttpContext, UrlGetHttpState>(context, &UrlGetHttpEndState::GetInstance(), "evHttpDone");
     * fsm::ProcessTransitionPre<UrlGetContext, UrlGetTopState>(context.GetParentContext(), &UrlGetEndState::GetInstance(), "evFtpDone");
     */
    public void writeProcessTransition(State state, State nextState, String suffix) throws StateBuilderException
    {
    	State stateTop;
        String contextExpression = getModel().getContextParentString(state, nextState);
        
    	String effectiveNextState;
		if (nextState == null) {
			stateTop = getModel().getStateTop(state);
			effectiveNextState = "NULL";
		} else {
			stateTop = getModel().getStateTop(nextState);

			if (nextState.getKind() == StateKind.HISTORY) {
				getStream().println(
						getIdent() + "// Next state is an history state");
				effectiveNextState = getModel().getContextInstance()	+ "." + GET_STATE_HISTORY;
			} else {
				effectiveNextState = "&" + getStateClassName(nextState) + "::" + GETINSTANCE;	
			}
		}
    
        getStream().println(getIdent() +   
                PROCESSTRANSITION + suffix +
                getTemplateContextState(stateTop) +
                "(" + 
                contextExpression + ", " +
                effectiveNextState + ");");
    }
    
    /**
     * generate all parallel states for a given state
     * @param state
     * @throws StateBuilderException
     */
    private void writeParallelAll(State state) throws StateBuilderException {
    	List<State> parallelList = state.getParallelList();
        for(State parallel : parallelList){
            writeParallel(parallel);
        }
    }    
    
    /**
     * Write a parallel state implementation
     * 
     * UrlGetRunningParallel::UrlGetRunningParallel(UrlGet& urlGet, UrlGetContext *pContext) : 
     *     fsm::Parallel(),
     *     m_contextHttp(urlGet, pContext), 
     *     m_contextFtp(urlGet, pContext), 
     *     context(*pContext)
     * {
     *     m_contextList.push_back(&m_contextHttp);
     *     m_contextList.push_back(&m_contextFtp);
     * }
     * 
     * @param state
     * @throws StateBuilderException
     */    
    private void writeParallel(State state) throws StateBuilderException {
        Parallel parallel = state.getParallel();
        if(parallel == null){
            return;
        }   
        String constructor = getParallelClassName(state); 
        String contextClassName = getModel().getContextClassName(state);                 

        // UrlGetRunningParallel::UrlGetRunningParallel
        getStream().print(getIdent() + 
                          constructor + 
                          "::" + 
                          constructor + 
                          "(");

        writeObjectsParameterDeclaration();
        String contextVar = "p" + CONTEXT_CLASSNAME;
        getStream().print(", " + contextClassName + " *" + contextVar);        
        
        getStream().println(") : ");
        
        pushIdentLevel();

        getStream().print(getIdent() + FSM_PARALLEL_CLASSNAME + "()");
        writeContextConstructorInitList(state, contextVar);
        popIdentLevel();
        getStream().println(getIdent() + "{");
        pushIdentLevel();
        List<State> stateOrthogonalList = state.getParallel().getState();
        
        for(State stateOrthogonal : stateOrthogonalList){
            getStream().println(getIdent() + CONTEXT_LIST + ".push_back(&" + getContextMemberName(stateOrthogonal) + ");");
        }
              
        popIdentLevel();
        getStream().println(getIdent() + "}");
        getStream().println();
        writeTransitionToNextState(state);
    }   
    
    /**
     * void UrlGetRunningParallel::TransitionToNextState()
     * {
     *   fsm::ProcessTransitionPre<UrlGetContext, UrlGetTopState>(m_context, &UrlGetEndState::GetInstance());
     *   fsm::ProcessTransitionPost<UrlGetContext, UrlGetTopState>(m_context, &UrlGetEndState::GetInstance());
     *   context.OnEnd();
     *   m_contextAlice.SetInitialState(PersistenceParallel01AliceIdleState::GetInstance());
     *   m_contextBob.SetInitialState(PersistenceParallel01BobIdleState::GetInstance());
     *   
     * }
     * @param state
     * @throws StateBuilderException
     */
    private void writeTransitionToNextState(State state) throws StateBuilderException {
    	
    	getStream().println(getIdent() + "void " + getParallelClassName(state) + "::" + TRANSITION_TO_NEXT_STATE );
    	getStream().println(getIdent() + "{");
    	pushIdentLevel();
    	
    	State stateNext = getModel().getState(state.getParallel().getNextState());
    	
    	writeProcessTransition(state, stateNext, "Pre");
    	writeProcessTransition(state, stateNext, "Post");
    	
    	if(stateNext != null){
    		writeContextOnEnd(state, stateNext);
    	} else {
    		String contextInstance = getModel().getContextInstance();
    	    getStream().println(getIdent() + contextInstance + "." + ON_END + ";");  
    	}
    	
        for(State stateOrthogonal : state.getParallel().getState()){
        	// m_contextAlice.SetInitialState(PersistenceParallel01AliceIdleState::GetInstance());
            getStream().println(getIdent() + 
            		getContextMemberName(stateOrthogonal) + "." + 
            		SET_INITIAL_STATE + "(" + getStateClassName(getModel().getStateLeaf(stateOrthogonal)) + "::" + GETINSTANCE +");");
        }
        
        popIdentLevel();
        getStream().println(getIdent() + "}");
        getStream().println();
    }
    
    /**
     * Write all contexts
     * @throws StateBuilderException
     */    
    private void writeContext() throws StateBuilderException {
        getStream().println(getIdent() + "/**************************************************************");
    	getStream().println(getIdent() + "  * Contexts implementations  ");
        getStream().println(getIdent() + "  *************************************************************/");
        writeContext(getStateMachine().getState());
    }    
    
    /**
     * Write the context associated with the given state
     * @param state
     * @throw StateBuilderException
     */    
    private void writeContext(State state) throws StateBuilderException {
    	if(state.isTop() == true){
            getStream().println(getIdent() + "/**************************************************************");
        	getStream().println(getIdent() + "  * Context " + getModel().getContextClassName(state));
            getStream().println(getIdent() + "  *************************************************************/");
            
    		writeContextConstructor(state);
    		writeContextDestructor(state);
    		writeContextGetStateNameToId(state);
    		writeMethodEnterInitialState(state);
    		writeMethodLeaveCurrentState(state);
    		
    		writeEnterState(state);
    		writePushBackEventFromId(state);
    		writeContextSyncEventImplementation(state);

    		if(state.isRoot() == true){
    			writeContextTimersStartImplemention(state);
    			writeContextTimersStopImplemention(state);
    		}

    		if(state.isRoot() == true){
    			if(getLibrary().compareTo(LANGUAGE_QT) == 0){
    				writeContextTimerEventImplementation(state);
    				writeContextSchedule(state);
    			} else if (getLibrary().compareTo(LANGUAGE_BOOST) == 0) {
    				writeContextBoostProcessImplementation(state);
    				writeContextSchedule(state);
    			} else if (getLibrary().compareTo(LANGUAGE_LIBEV) == 0) {
    				writeContextLibevProcessImplementation(state);
    				writeContextSchedule(state);
    			}
    		}

    		writeLockFunction(state);
    		writeUnLockFunction(state);
    	}
    	
    	// Sub context
        for(State stateChild : state.getState()){
        	writeContext(stateChild);
        }
        
        // Parallel 
        Parallel parallel = state.getParallel();
        if(parallel != null){
        	List<State> stateOrthogonalList = parallel.getState();
        	for(State stateOrthogonal : stateOrthogonalList){                 
                writeContext(stateOrthogonal);    		
        	}        	
        }
        
        writeParallelAll(state);        
    }        
    
    private void writeContextGetStateNameToId(State state) throws StateBuilderException
    {
    	if(state.isRoot() == false){
    		return;
    	}
    	
    	getStream().println();
    	String contextClassName = getModel().getContextClassName(state);
    	getStream().println(getIdent() + "const fsm::StateNameToId* " + contextClassName + "::GetStateNameToId()");
    	getStream().println(getIdent() + "{");
    	pushIdentLevel();
    	getStream().println(getIdent() + "return " + getStateNameToId() + ";");
    	popIdentLevel();
    	getStream().println(getIdent() + "}");
    	getStream().println("");
    }
    
    /**
     * Write the context constructor
     *  FsmTestM1S3Context::FsmTestM1S3Context(Object1& object1, Object2& object2, ISchedule &schedule) : 
     *    fsm::ContextAsync<FsmTestM1S3Context, Top, FsmTest>(object1, object2, schedule) ,
     *    m_contextS11(object1, object2),
     *    m_contextS12(object1, object2) 
     *  {
     *    SetName("ObjectContext");
     *    // Timer's registration
     *    FsmTimer<ObjectContext> timer1(TM_RETRY, &GetTimerList(), this, &ObjectContext::EvTimerRetry);
     *    // machine's registration
     *    // registering machine
     *    SetState(_S0);
     *  }
     * @param state
     * @throws StateBuilderException
     */
    private void writeContextConstructor(State state) throws StateBuilderException {
    	//Begin the real work here
        String contextClassName = getModel().getContextClassName(state);
        String contextParentClassName = getModel().getContextParentClassName(state);
        String stateName = getStateClassName(state);
        getStream().print(getIdent() +  
                contextClassName + 
                "::" + 
                contextClassName +
                "(");
        
        writeObjectsParameterDeclaration();
        
        //Qt only
        if((getLibrary().compareTo(LANGUAGE_QT) == 0) && (state.isRoot() == true)){
        	getStream().print(", QObject *pParent");
        }
        
        if(getStateMachine().getSettings().isAsynchronous() && state.equals(getStateMachine().getState())){
            if((getLibrary().compareTo(LANGUAGE_QT) == 0)){
            } else if((getLibrary().compareTo(LANGUAGE_BOOST) == 0)){
                getStream().print(", boost::asio::io_service &io_service");
            } else if((getLibrary().compareTo(LANGUAGE_LIBEV) == 0)){
                getStream().print(", ev::default_loop &loop");
            } else {
            	getStream().print(", " + ISCHEDULE + " &" + VARIABLE_SCHEDULE);
            }           
        }
 
        if((contextParentClassName != null) && (contextParentClassName.compareTo(contextClassName) != 0)){
            getStream().print(", " + contextParentClassName + " *p" + CONTEXT_CLASSNAME);
        } 
        
        getStream().println(") : ");
        
        pushIdentLevel();
        
        //Qt only
        if((getLibrary().compareTo(LANGUAGE_QT) == 0) && (state.isRoot() == true)){
        	getStream().println(getIdent() + "QObject(pParent), ");
        }
        
        String baseClass;
        if((getStateMachine().getSettings().isAsynchronous() == true) && (state.equals(getStateMachine().getState()))){
            baseClass = FSM_CONTEXTASYNC_CLASSNAME + 
                        "<" + 
                        stateName + ", " + 
                        contextClassName + ", " + 
                        contextParentClassName + 
                        ">";
        } else {
            baseClass = FSM_CONTEXT_CLASSNAME +
                        "<" +  
                        stateName + ", " + 
                        contextParentClassName + 
                        ">";
        }   
                
        getStream().print(getIdent() + baseClass + "(");

        if((getStateMachine().getSettings().isAsynchronous() == true) && (state.equals(getStateMachine().getState()))){
        	if((getLibrary().compareTo(LANGUAGE_QT) == 0) 
        			|| (getLibrary().compareTo(LANGUAGE_BOOST) == 0) 
        			|| (getLibrary().compareTo(LANGUAGE_LIBEV) == 0)){ 
        			getStream().print("*this, NULL");
        	} else {
        	    getStream().print(VARIABLE_SCHEDULE);
        	} 
        } else {
        	if((contextParentClassName != null) && (contextParentClassName.compareTo(contextClassName) != 0)){
        		getStream().print("p" + CONTEXT_CLASSNAME);
        	} 
        }
        
        getStream().println(")");
        
        // , m_object1(object1),
        // , m_object1(object1)
        writeObjectsInitialization();

        writeParallelConstructorInitList(state);
        
        if((state.isRoot() == true) 
        		&& (getStateMachine().getSettings().isAsynchronous() == true)){
        	if((getLibrary().compareTo(LANGUAGE_BOOST) == 0)){
        		getStream().println(getIdent() + ", m_timerProcess(io_service)");

        		List<Timer> timerList = getModel().getTimersAll();
        		for(Timer timer : timerList){
        			getStream().println(getIdent() + ", m_timer" + timer.getName() + "(io_service)");
        		}
        	} else  if((getLibrary().compareTo(LANGUAGE_LIBEV) == 0)){
        		getStream().println(getIdent() + ", m_timerProcess(loop)");

        		List<Timer> timerList = getModel().getTimersAll();
        		for(Timer timer : timerList){
        			getStream().println(getIdent() + ", m_timer" + timer.getName() + "(loop)");
        		}
        	}
        }

        popIdentLevel();        
        getStream().println(getIdent() + "{");
        pushIdentLevel();

        getStream().println(getIdent() + SETNAME + "(\"" + contextClassName + "\");");
        
        if((getLibrary().compareTo(LANGUAGE_QT) == 0) && (state.isRoot() == true)){
        	// SetManagement(this);
            getStream().println(getIdent() + MANAGEMENTSET + "this);");
            getStream().println();
        } 

        if((isLock() == true) && 
        		(contextClassName.equals(contextParentClassName) == true) &&
        		((getLibrary().compareTo(LANGUAGE_QT) == 0) || (getLibrary().compareTo(LANGUAGE_BOOST) == 0))){
            getStream().println(getIdent() + "SetLock(this);");
            getStream().println();
        }
        
        writeTimerRegistration(state, contextClassName);
        writeContextSetInitialState(state);
        popIdentLevel();        
        getStream().println(getIdent() + "}");      
        getStream().println("");
    }
    
    private void writeContextDestructor(State state) throws StateBuilderException {
        String contextClassName = getModel().getContextClassName(state);
        getStream().println(getIdent() +  
                contextClassName + 
                "::~" + 
                contextClassName +
                "(){}");
        
        getStream().println("");
    }
    /**
     * Write all timer's registration
     *   FsmTimer<ObjectContext> timer0(TM_TIMER1, &GetTimerList(), this, &ObjectContext::EvTimer1);
     *   FsmTimer<ObjectContext> timer1(TM_TIMER2, &GetTimerList(), this, &ObjectContext::EvTimer2);
     */
    private void writeTimerRegistration(State state, String contextClassName) throws StateBuilderException {
        List<Timer> timerList = getModel().getTimersAll();
        
        if(timerList.size() == 0){
            return;
        }

        if(state.isRoot() == false){
        	return;
        }
        
        getStream().println(getIdent() + "// Timers registration");
        
        if(getLibrary().compareTo(LANGUAGE_LIBEV) == 0){
			//m_timerProcess.set<BlinkContext,&BlinkContext::process>(this);
			getStream().println(getIdent() +
					"m_timerProcess.set<" + contextClassName + ",&" + contextClassName + "::process>(this);");
		}
        
        for(Timer timer : timerList){
            writeTimerRegistration(state, 
            		                  contextClassName, 
            		                  timer);
        }
        
        getStream().println();
    }
  
    /**
     * Write one timer's registration
     * For C++:
     *   new FsmTimer<ObjectContext>(TM_TIMER1, &GetTimerList(), this, &ObjectContext::EvTimer1)
     * For Qt:
     *   m_pTimerMaxDutation = new QTimer(this);
     *   m_pTimerMaxDutation->setSingleShot(true);
     *   QObject::connect(m_pTimerMaxDutation, SIGNAL(timeout()),
     *              this, SLOT(evTimerMaxDuration()));  
     *   
     */    
    private void writeTimerRegistration(State state, 
    		                               String contextClassName, 
    		                               Timer timer){
    	String timerName = timer.getName();
        String timerEventName = timer.getId();
    	
    	if(state.isRoot() == true){
    		if(getLibrary().compareTo(LANGUAGE_QT) == 0){
    			getStream().println(getIdent() + "m_pTimer" + timerName + " = new QTimer(this);");
    			getStream().println(getIdent() + "m_pTimer" + timerName + "->setSingleShot(true);");
    			getStream().println(getIdent() +
    					"QObject::connect(m_pTimer" + timerName + ", SIGNAL(timeout()), " + 
    					"this, SLOT(" + timerEventName + "()));");
    		} else if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
    		} else if(getLibrary().compareTo(LANGUAGE_LIBEV) == 0){
    			//m_timerTimerRetry.set<BlinkContext,&BlinkContext::EvTimerRetry>(this);
    			getStream().println(getIdent() +
    					"m_timer" + timerName + ".set<" + contextClassName + ",&" + contextClassName + "::" + timerEventName + ">(this);");
    		} else {
    			getStream().print(getIdent() + "new " + FSM_TIMER + "<" + contextClassName + ">");
    			getStream().print("(\"" + timerName + "\", ");
    			getStream().print("&" + GETTIMERLIST + ", this, &");
    			getStream().println(contextClassName + "::" + timerEventName + ");");
    		}
    	}
    }
    
    /**
     * Generate the setting of the initial state: 
     *     SetInitialState(MicrowaveEngineOffState::GetInstance()); 
     *     
     * @param state the root state 
     * @throws StateBuilderException
     */
    private void writeContextSetInitialState(State state) throws StateBuilderException {        
        String stateNameInitial = getStateClassName(getModel().getStateLeaf(state));
                
        getStream().println(getIdent() + SET_INITIAL_STATE + 
        		"(" + stateNameInitial + "::"+ GETINSTANCE + ");");       
    }    
    
    /**
     * Write context events asynchronous implementation
     */
    private void writeContextAsyncEventImplementation() throws StateBuilderException {
        List<EventSource> eventProviderList = getStateMachine().getEvents().getEventSource();
        for(EventSource eventProvider : eventProviderList){
            List<Event> eventList =  eventProvider.getEvent();
            getStream().println(getIdent() + "// " + eventProvider.getName() + " asynchronous events ");
            writeContextEventAsyncImplementation(getModel().getContextClassName(getStateMachine().getState()), eventList, SYNC_SUFFIX);        
        }
        getStream().println();
    }      
    
    /**
     * Write context asynchronous event member implementation: 
     *     void Context::EvEvent1(Param1 param1, Param3 *pParam3)
     *     {
     *        GetEventList().push_back(Event(&Context::EvAlertingSync, connection)); 
     *     }
     *     
     * @param eventList the list of events to generate
     * @param functionSuffix the suffix to add to the event function
     */
    private void writeContextEventAsyncImplementation(String contextClassName,
                                                         List<Event> eventList, 
                                                         String functionSuffix) throws StateBuilderException {
        for(Event event : eventList){ 
            String eventName = event.getId();
            
            getStream().print(getIdent() + 
                                "void " +
                                contextClassName + 
                                "::" + 
                                eventName);
            getStream().print("(");
            writeParameterDeclaration(event);
            getStream().println(")");
            getStream().println(getIdent() + "{");
            pushIdentLevel();

            //Lock
            writeLock();
            
            writePushBackEventToQueue(contextClassName, functionSuffix, event);
            
            //UnLock
            writeUnlock();

            getStream().println(getIdent() + MEMBER_SCHEDULE + "." + SCHEDULE_FUNCTION + "0);");

            popIdentLevel();
            getStream().println(getIdent() + "}");
            getStream().println();            
        }                    
    }

    /**
     * GetEventList().push_back(Event(&Context::EvAlertingSync, connection)); 
     * @param contextClassName
     * @param functionSuffix
     * @param event
     * @param eventName
     */
	private void writePushBackEventToQueue(String contextClassName,
			String functionSuffix, Event event) {
		getStream().print(getIdent() + GETEVENTLIST + ".push_back(Event(*this");
		
		String eventName = event.getId();
		
		if(event.getParameter().isEmpty()){
			getStream().print(", " + event.getNumberId());
		}
		
		getStream().print(", &" + contextClassName);
		getStream().print("::" + eventName + functionSuffix);
		
		writeParameterList(event);
		
		getStream().println("));");
	}   
    
    /**
     * Write context synchronous events member implementation
     */
    private void writeContextSyncEventImplementation(State state) throws StateBuilderException {
        String contextClassName = getModel().getContextClassName(state);
        String syncSuffix = "";
        //Only the root state has SYNC_SUFFIX
        if(state.isRoot() == true){
            syncSuffix = SYNC_SUFFIX;
        }
        List<EventSource> eventProviderList = getStateMachine().getEvents().getEventSource();
        for(EventSource eventProvider : eventProviderList){
            List<Event> eventList =  eventProvider.getEvent();
            getStream().println(getIdent() + "// " + eventProvider.getName() + " synchronous events ");
            for(Event event : eventList){ 
              writeContextEventSyncImplementation(contextClassName, event, syncSuffix, null);
            }
        }
        
        List<Timer> timerList = getModel().getTimersAll();
        if(timerList.size() == 0){
            return;
        }

        getStream().println(getIdent() + "// Timer's event implementation");
        
        if(state.isRoot() == true){
        	for(Timer timer : timerList){
            	String param = null;
                if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
                	param = "const boost::system::error_code &error";
                }
                
        		writeContextEventSyncImplementation(contextClassName, timer, "", param); 
        		getStream().println();
        	}        

        	getStream().println();
        }
    }     
    
    /**
     * Write the synchronous event member functions: 
     *     void ObjectContext::EvEvent1Sync(Param1 param1, Param2 param2)
     *     {
     *       GetState().EvEvent1(*this, param1, param2);
     *     }
     *     
     * @param event the event to generate
     * @param functionSuffix the suffix to add to the event function
     */
    private void writeContextEventSyncImplementation(String contextClassName,
                                                        Event event, 
                                                        String functionSuffix,
                                                        String param) throws StateBuilderException {
         
        String eventId = event.getId();
        getStream().print(getIdent() + 
                "void " +
                contextClassName + 
                "::" + 
                eventId);
        if(getStateMachine().getSettings().isAsynchronous() == true){
            getStream().print(functionSuffix);
        }
        
        getStream().print("(");
        if(param == null){
            writeParameterDeclaration(event);
        } else {
        	getStream().print(param);
        }
        
        getStream().println(")");
        getStream().println(getIdent() + "{");
        pushIdentLevel();

        //Lock
        if((isLock() == true) && (getStateMachine().getSettings().isAsynchronous() == false)){
        	writeLock();
        }
    
        if((getLibrary().compareTo(LANGUAGE_BOOST) == 0) && (param != null)){
        	//It's a timer
        	getStream().println(getIdent() + "if(error != boost::asio::error::operation_aborted){");
            pushIdentLevel();
            getStream().print(getIdent() + GETSTATE + "." + eventId + "(*this");
            writeParameterList(event);
            getStream().println(");");
            popIdentLevel();
            getStream().println(getIdent() + "}");
        } else {
            getStream().print(getIdent() + GETSTATE + "." + eventId + "(*this");
            writeParameterList(event);
            getStream().println(");");
        }

        //UnLock
        if((isLock() == true) && (getStateMachine().getSettings().isAsynchronous() == false)){
        	writeUnlock();
        }  
        
        popIdentLevel();
        getStream().println(getIdent() + "}");
        getStream().println();
    }      
    
    /**
     * Write the beginning of the function implementation
     *    returnType className::functionName(contextClassName &context, Param1 param1, Param2 &param2){
     * @param returnType 
     * @param className
     * @param functionName
     * @param contextClassName
     * @param event name of the event which contains parameters
     * @param bConstant true if function is constant
     * @throws StateBuilderException
     */
    private void writeFunctionImplementationBegin(String returnType,
                                                     String className, 
                                                     String functionName,
                                                     String contextClassName,
                                                     Event event,
                                                     boolean bConstant) throws StateBuilderException {
            
        getStream().print(getIdent() + returnType + " " + className + "::" + functionName + "(");
        getStream().print(contextClassName + " &");
        
        // comment parameter if not used

        getStream().print(getModel().getContextInstance());          
        
        if(event != null){
            // Add a comma if there is at least one parameter
            if(event.getParameter().size() != 0){
                getStream().print(", ");
            }
            
            writeParameterDeclaration(event);
        }
        getStream().print(")");
        // add const ?
        if(bConstant == true){
            getStream().println(" const");
        } else {
            getStream().println();
        }
        
        getStream().println(getIdent() + "{");
        pushIdentLevel();        
    }   
    
    /**
     * Write the actions for <onEntry> and <onExit>
     * @param state the current state
     * @param actions the actions
     * @param bDeclareObject declare object if needed : Object &object = context.GetObject(); 
     * @throws StateBuilderException
     */
    private void writeActions(State state,
    		                  String actionInAttribute,
    		                  List<Object> actionObjectList,
    		                  boolean bDeclareObject) throws StateBuilderException {
        //How many actions to perform ?
        int actionNumber = 0;

        if(actionObjectList != null){
        	actionNumber = actionObjectList.size();
        }
        
        if(actionInAttribute != null && (actionInAttribute.isEmpty() == false)){
        	actionNumber += 1;
        }
        
        if(actionNumber == 0){
        	getStream().println(getIdent() + "// No action to do");
        	return;
        } 

        if(bDeclareObject == true){
        	writeObjectsDeclaration();
        }

        getStream().println(getIdent() + "// " + actionNumber + " action(s) to do");
    
        // Generate the eventual action which can be set as an attribute of <onEntry> or <onExit> 
        if(actionInAttribute != null && (actionInAttribute.isEmpty() == false)){
        	getStream().println(getIdent() + actionInAttribute + ";");
        }
        
        // Generate the eventual action which can be set as children of <onEntry> or <onExit>
        writeActionGroup(state, actionObjectList);
    }

    /**
     * Write the actions: action, timerStart and timerStop
     * @param state the current state
     * @param actionObjectList
     * @throws StateBuilderException
     */
    void writeActionGroup(State state, 
    		                 List<Object> actionObjectList) throws StateBuilderException{
        // Generate the eventual action which can be set as children of <transition> 
    	if(actionObjectList != null){
    		for(Object actionAction : actionObjectList){
    			if(actionAction.getClass() == String.class){
    				//Normal code goes here
    				String actionString  = (String)actionAction;
    				getStream().println(getIdent() + actionString + ";");
    			} else if(actionAction.getClass() == TimerStartAction.class){
    				//Start a timer
    				writeTimerStart(state, (TimerStartAction)actionAction);
    			} else if(actionAction.getClass() == TimerStopAction.class){
    				//Stop a timer
    				writeTimerStop(state, (TimerStopAction)actionAction);
    			} else {
    				// Should never happened anyway if XML validation is on.
    				throw new StateBuilderException("S1001", "Action must be either code, timerStart or TimerStop");
    			}
    		}  
    	}
    }
    
    /**
     * Write the code related to start a timer, duration is in milliseconds
     * @param state the current state
     * @param timerStartAction  the action to perform
     *   
     *   context.TimerStartTimerMaxDuration(duration);
     *   or
     *   context.GetParentContext().TimerStartTimerMaxDuration(duration);
     *   
     * @throws NoSuchStateException 
     */
    private void writeTimerStart(State state,
    		                        TimerStartAction timerStartAction) throws StateBuilderException {
    	String timerName = timerStartAction.getTimer();
    	
    	String contextInstance = getModel().getContextParentString(state, getStateMachine().getState());
		getStream().println(getIdent() + 
				contextInstance +
				"." +
				TIMERSTART +  timerName + "(" + 
				timerStartAction.getDuration() + ");");
    }
    
    /**
     * Write the code related to stop a timer, duration is in milliseconds:
     *
     *   context.TimerStopTimerMaxDuration(duration);
     *   or
     *   context.GetParentContext().TimerStopTimerMaxDuration(duration);
     *   
     * @param timerStartAction
     * @throws NoSuchStateException 
     */
    private void writeTimerStop(
    		State state,
    		TimerStopAction timerStopAction) throws StateBuilderException {
    	String timerName = timerStopAction.getTimer(); 
    	String contextInstance = getModel().getContextParentString(state, getStateMachine().getState());
    	getStream().println(getIdent() + 
    			contextInstance +
    			"." +
    			TIMERSTOP + timerName + "();");
    }
    
    /**
     * Write the context constructor initialisation list:
     *   m_contextHttp(urlGet, pContext), 
     *   m_contextFtp(urlGet, pContext), 
     *   context(*pContext)
     *   
     * @param state
     */
    private void writeContextConstructorInitList(State state, String contextVar) throws StateBuilderException {
    	Parallel parallel = state.getParallel();
    	if(parallel != null){
    		getStream().println(",");
    		List<State> stateOrthogonalList = parallel.getState();
    		for(State stateOrthogonal : stateOrthogonalList){
    			getStream().print(getIdent() + getContextMemberName(stateOrthogonal) +         "(");
    			writeObjectsParameterInstance();
    			getStream().print(", " + contextVar + ")");
    			getStream().println(", ");
    		}
    		getStream().println(getIdent() + getModel().getContextInstance() + "(*pContext)");
    	} else {
    		getStream().println();
    	}           
    }
    
    /**
     * Write the parallel constructor initialisation list
     *   m_parallelRunning(new UrlGetRunningParallel(urlGet, this)),
     *   
     * @param state
     */
    private void writeParallelConstructorInitList(State state) throws StateBuilderException {
    	List<State> stateList = state.getParallelList();
        if(stateList.size() > 0){
            getStream().println(",");
            for(int i = 0; i < stateList.size(); i++){
                getStream().print(getIdent() + getParallelMemberName(stateList.get(i)) +
                                  "(new " + getParallelClassName(stateList.get(i)) + "(");
                writeObjectsParameterInstance();
                getStream().print(", this))");
                if(i < stateList.size() - 1){
                    getStream().println(", ");
                } else {
                    getStream().println();
                }
            }
        } else {
            getStream().println();
        }   
    }    
    
    /**
     * Write timers start implementations
     *  
     */
    private void writeContextTimersStartImplemention(State state) throws StateBuilderException {
        getStream().println(getIdent() + "// Timer start implementation");
        
        for(Timer timer : getModel().getTimersAll()){
        	writeTimerStartImplementation(state, timer);
        }
        
        getStream().println();
    }
    
   /**
    * StdCpp: 
    * void TimerStartMyTimer1(long duration)
    * {
    *   TimerStart("MyTimer1", duration);
    *   m_schedule.schedule(duration);
    * }
    * 
    * Qt:
    * void TimerStartMyTimer1(long duration)
    * {
    *   GetObserver().OnTimerStart("TimerStart", duration);
    *   m_pTimerRetry->start(duration);
    * }

    * Boost:
    * void TimerStartMyTimer1(long duration)
    * {
    *   GetObserver().OnTimerStart("TimerStart", duration);
    *   m_timerRetry.cancel();
    *   m_timerRetry.expires_from_now(boost::posix_time::milliseconds(duration));
    *   m_timerRetry.async_wait(boost::bind(&HelloWorldContext::evTimerRetry, this, _1)); 
    * }
    */
    private void writeTimerStartImplementation(State state, Timer timer) throws StateBuilderException {

    	String timerName = timer.getName();
    	String timerEvent = timer.getId();
    	getStream().println(getIdent() + "void " + getModel().getContextClassName(state) + "::" + TIMERSTART + timerName + "(long duration)");
    	getStream().println(getIdent() + "{");
    	pushIdentLevel();
    	
        if(getStateMachine().getSettings().getObserver().equals(Observer.NO)){
        } else if(getStateMachine().getSettings().getObserver().equals(Observer.INT)){
        } else {            
    		getStream().println(getIdent() + "if(m_pObserver){");
    		pushIdentLevel();
    		getStream().println(getIdent() + "m_pObserver->" + ONTIMERSTART + "\"" + getModel().getContextClassName(state) + "\", \"" + timerName + "\", " + "duration);");
    		popIdentLevel();
    		getStream().println(getIdent() + "}");
    	}
    	
    	if(getLibrary().compareTo(LANGUAGE_QT) == 0){
    		getStream().println(getIdent() + "m_pTimer" + timerName + "->start(duration);");
    	} else if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
    		String timerMemberName = "m_timer" + timerName;
    		getStream().println(getIdent() + timerMemberName + ".cancel();");
    		getStream().println(getIdent() + timerMemberName + ".expires_from_now(boost::posix_time::milliseconds(duration));");
    		getStream().println(getIdent() + 
    				timerMemberName + ".async_wait(boost::bind(&" + getModel().getContextClassName(state) + "::" + timerEvent + ", this, _1)); ");
    	} else if(getLibrary().compareTo(LANGUAGE_LIBEV) == 0){
    		String timerMemberName = "m_timer" + timerName;
    		getStream().println(getIdent() + timerMemberName + ".start(((double)duration)/1000);");
        } else {
    		getStream().println(getIdent() + TIMERSTART + "(\"" + timerName + "\", duration);");
    		getStream().println(getIdent() + MEMBER_SCHEDULE + "." + SCHEDULE_FUNCTION + "duration);");
    	}
    	popIdentLevel();
    	getStream().println(getIdent() + "}");            
    	getStream().println();

    }

    /**
     * Write timer stop declaration
     *  
     * void TimerStopMyTimer1()
     * {
     *   GetObserver().OnTimerStop("MaxDutation");
     *   TimerStop("MyTimer");
     * } 
     *
     * For Qt:
     * void TimerStopMyTimer1()
     * {
     *   GetObserver().OnTimerStop("MaxDutation");
     *   m_pTimerMaxDutation->stop();
     * } 
     *  
     * For Boost:
     * void TimerStopMyTimer1()
     * {
     *   GetObserver().OnTimerStop("MaxDutation");
     *   m_timerMaxDutation.cancel();
     * } 
     * 
     * For Libev:
     * void TimerStopMyTimer1()
     * {
     *   GetObserver().OnTimerStop("MaxDutation");
     *   m_timerMaxDutation.stop();
     * } 
     * 
     */
    private void writeContextTimersStopImplemention(State state) throws StateBuilderException {
        List<Timer> timerList = getModel().getTimersAll();
        if(timerList.size() == 0){
            return;
        }

        getStream().println(getIdent() + "// Timer stop implementation");
        
        for(Timer timer : timerList){
        	writeContextTimerStopImplemention(state, timer);
        }      
    }    
    
    private void writeContextTimerStopImplemention(State state, Timer timer) throws StateBuilderException {
    	String timerName = timer.getName();
    	getStream().println(getIdent() + "void " + getModel().getContextClassName(state) + "::" + TIMERSTOP + timerName + "()");
    	getStream().println(getIdent() + "{");
    	pushIdentLevel();
    	
        if(getStateMachine().getSettings().getObserver().equals(Observer.NO)){
        	
        } else if(getStateMachine().getSettings().getObserver().equals(Observer.INT)){
        } else {
    		getStream().println(getIdent() + "if(m_pObserver){");
    		pushIdentLevel();
    		getStream().println(getIdent() + "m_pObserver->" + ONTIMERSTOP + "\"" + getModel().getContextClassName(state) + "\", \"" + timerName + "\");");
    		popIdentLevel();
    		getStream().println(getIdent() + "}");
    	}
    	
    	if(state.isRoot() == true){
        	if(getLibrary().compareTo(LANGUAGE_QT) == 0){
        		getStream().println(getIdent()  + "m_pTimer" + timerName + "->stop();");
        	} else if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){ 
        		getStream().println(getIdent()  + "m_timer" + timerName + ".cancel();");
        	} else if(getLibrary().compareTo(LANGUAGE_LIBEV) == 0){ 
        		getStream().println(getIdent()  + "m_timer" + timerName + ".stop();");
        	} else {
        		getStream().println(getIdent() + TIMERSTOP + "(\"" + timerName + "\");");
        	}
    	} else {
    		getStream().println(getIdent() + GETPARENTCONTEXT + "." +TIMERSTOP + timerName + "();");
    	}
    	    	
    	popIdentLevel();
    	getStream().println(getIdent() + "}");
    	getStream().println();    
    }
    
    /**
     * void LightContext::EnterInitialState()
     * {
     *   WalkChainEntry<LightContext, LightTop>(*this, &LightTop::GetInstance(), &GetState());
     * }
     * @param state
     * @throws StateBuilderException
     */
    private void writeMethodEnterInitialState(State state) throws StateBuilderException {
        getStream().println(getIdent() + "void " + getModel().getContextClassName(state) + "::" + 
                ENTER_INITIAL_STATE );
        getStream().println(getIdent() + "{");
        pushIdentLevel(); 
   
        String stateTop = getStateClassName(getModel().getStateTop(state).getName());
        
        // WalkChainEntry<LightContext, LightTop>(*this, &LightTop::GetInstance(), GetState());
        getStream().println(getIdent() + WALKCHAINENTRY + "<" + getModel().getContextClassName(state) + ", "
        		+ stateTop + ">(*this, " +
        		"&" + stateTop + "::" + GETINSTANCE + " , " + 
        		"&" + GETSTATE + ");");
        
        popIdentLevel();
        getStream().println(getIdent() + "}");
        getStream().println();        
    }
    
    /**
     * void LightContext::LeaveCurrentState()
     * {
     *   WalkChainExit<LightContext, LightTop>(*this, GetState(), &LightTop::GetInstance());
     * }
     * @param state
     * @throws StateBuilderException
     */
    private void writeMethodLeaveCurrentState(State state) throws StateBuilderException {
        getStream().println(getIdent() + "void " + getModel().getContextClassName(state) + "::" + 
        		LEAVE_CURRENT_STATE);
        getStream().println(getIdent() + "{");
        pushIdentLevel(); 
   
        String stateTop = getStateClassName(getModel().getStateTop(state).getName());
        getStream().println(getIdent() + WALKCHAINEXIT + "<" + getModel().getContextClassName(state) + ", "
        		+ stateTop + ">(*this, " +
        		"&" + GETSTATE + ", " + 
        		"&" + stateTop + "::" + GETINSTANCE + ");" 
        		);
        
        popIdentLevel();
        getStream().println(getIdent() + "}");
        getStream().println();        
    }  
   
    /**
      bool EnterState(const char* pStateName);
      {
         if(strcmp(pStateName, "End") == 0){
            SetState(&PersistenceAsyncEndState::GetInstance());
		  }
		  else if(strcmp(pStateName, "Idle") == 0){
		    SetState(&PersistenceAsyncIdleState::GetInstance());
		  }
		  else if(strcmp(pStateName, "Running") == 0){
		    SetState(&PersistenceAsyncRunningState::GetInstance());
		  }
		  else if(strcmp(pStateName, "PersistenceAsync") == 0){
		    SetState(&PersistenceAsyncPersistenceAsyncState::GetInstance());
		  }
		  else {return false;}
		  return true;
      }
     * @param state
     * @throws StateBuilderException
     */
    private void writeEnterState(State stateCurrent) throws StateBuilderException {
    	if(getStateMachine().getSettings().isSerialize() == false){
    		return;
    	}
    	
        getStream().println(getIdent() + "bool " + getModel().getContextClassName(stateCurrent) + "::" + 
        		ENTER_STATE + "(const char* pStateName)");
        getStream().println(getIdent() + "{");
        pushIdentLevel(); 
       
        boolean isFirst = true;
        for(Map.Entry<String, State> stateEntry  : getModel().getStateMap().entrySet()){
        	State state = stateEntry.getValue();
        	// Do not set to history state
        	if(state.getKind() == StateKind.HISTORY) continue;
        	
        	// Only when state belongs to the same context.
        	String contextName = getModel().getContextClassName(state);
        	String contextNameCurrent = getModel().getContextClassName(stateCurrent);
        	if(contextName.compareTo(contextNameCurrent) != 0) continue;
        	
        	String ifelseif;
        	if(isFirst == true){
        		ifelseif = "if(";
        		isFirst = false;
        	} else {
        		ifelseif = "else if(";
        	}
        	
        	getStream().println(getIdent() + ifelseif + "strcmp(pStateName, \"" + state.getName() +"\") == 0){");
        	pushIdentLevel();
        	getStream().println(getIdent() + "SetState(&" + getStateClassName(state) + "::" + GETINSTANCE + ");");
        	popIdentLevel();	
        	getStream().println(getIdent() + "}");
        }
        getStream().println(getIdent() + "else {return false;}");
        getStream().println(getIdent() + "return true;");
                
        popIdentLevel();
        getStream().println(getIdent() + "}");
        getStream().println();           
    } 
    
    private void writePushBackEventFromId(State stateCurrent) throws StateBuilderException {
    	if((getStateMachine().getSettings().isAsynchronous() == false) ||
    			(getStateMachine().getSettings().isSerialize() == false)){
    		return;
    	}
    	
        getStream().println(getIdent() + "void " + getModel().getContextClassName(stateCurrent) + "::" + 
        		PUSHBACK_EVENT + "(int eventId)");
        
        getStream().println(getIdent() + "{");
        pushIdentLevel(); 
       
        getStream().println(getIdent() + "switch(eventId){");
        
        int eventid = 0;
        for(Event event : getModel().getEventsAll()){
        	if(event instanceof Timer){
        		continue;
        	}
        	eventid++;
        	pushIdentLevel();
        	getStream().println(getIdent() + "case " + eventid + ": ");
        	pushIdentLevel();
        	
        	String eventName = event.getId();
        	getStream().println(getIdent() + eventName + "();");

        	getStream().println(getIdent() + "break;");
        	popIdentLevel();
        	popIdentLevel();
        }
        
        getStream().println(getIdent() +"}");
        
        popIdentLevel();
        getStream().println(getIdent() + "}");
        getStream().println();
    } 
    
    private void writeWarningDisableBegin(){
    	// Disable some MSVC warnings
    	getStream().println("// Disable some MS compiler warnings");
    	getStream().println(getIdent() + "#ifdef _MSC_VER");
    	getStream().println(getIdent() + "#pragma warning(push)");
    	// 'this' : used in base member initialiser list
    	getStream().println(getIdent() + "#pragma warning(disable:4355) // 'this' : used in base member initialiser list");
    	// local variable is initialised but not referenced
    	getStream().println(getIdent() + "#pragma warning(disable:4189) // local variable is initialised but not referenced");
    	// unreferenced formal parameter
    	getStream().println(getIdent() + "#pragma warning(disable:4100) // unreferenced formal parameter");
    	getStream().println(getIdent() + "#endif");
    	getStream().println();
    	getStream().println("// Disable some GCC compiler warnings");
    	getStream().println(getIdent() + "#if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 402");
    	getStream().println(getIdent() + "#pragma GCC diagnostic ignored \"-Wunused-variable\"");
    	getStream().println(getIdent() + "#endif");
    	getStream().println();
    }
    
    private void writeWarningDisableEnd(){
        // Disable MSVC warning, end
    	getStream().println("// Reenable some compiler warnings");
        getStream().println(getIdent() + "#ifdef _MSC_VER");
        getStream().println(getIdent() + "#pragma warning(pop)");
        getStream().println(getIdent() + "#endif");
        getStream().println(); 
    }
    
    /**
     * Generate the object declaration
     *   Object &object = context.GetObject();
     * @param contextInstance
     * @param objectClass
     * @param objectInstance
     * @param getter
     */
    public void writeDeclareObject(final String contextInstance,
    		                          final String objectClass, 
    		                          final String objectInstance, 
    		                          final String getter){
    
    	getStream().println(getIdent() + objectClass + 
    			"& "  + objectInstance + 
    			" = " + contextInstance + "." + getter + ";");
    }
    
    /**
     * Generate "void ObjectContext::timerEvent(QTimerEvent *pTimerEvent)" implementation
     * Qt specific, asynchronous
     * @param state
     * @throws StateBuilderException
     */
    public void writeContextTimerEventImplementation(State state) throws StateBuilderException {
    	if(getStateMachine().getSettings().isAsynchronous() == false){
    		return;
    	}
    	getStream().println(getIdent() + "void " + getModel().getContextClassName(state)+ "::timerEvent(QTimerEvent *pTimerEvent)");
    	getStream().println(getIdent() + "{");
    	pushIdentLevel();
    	getStream().println(getIdent() + "killTimer(pTimerEvent->timerId());");
    	getStream().println(getIdent() + "long nextTick = 0;");
    	getStream().println(getIdent() + "ProcessEvent(nextTick);");
    	popIdentLevel();
    	getStream().println(getIdent() + "}");
    	getStream().println();
    }
    
    /**
     * Boost specific, asynchronous
     * @param state
     * @throws StateBuilderException
     */
    public void writeContextBoostProcessImplementation(State state) throws StateBuilderException{
    	if(getStateMachine().getSettings().isAsynchronous() == false){
    		return;
    	}
    	getStream().println(getIdent() + "void " + getModel().getContextClassName(state)+ "::process(const boost::system::error_code &error)");
    	getStream().println(getIdent() + "{");
    	pushIdentLevel();
    	getStream().println(getIdent() + "if(error && error != boost::asio::error::operation_aborted){");
    	pushIdentLevel();
    	getStream().println(getIdent() + "return;");
    	popIdentLevel();
    	getStream().println(getIdent() + "}");
    	getStream().println(getIdent() + "long nextTick = 0;");
    	getStream().println(getIdent() + "switch (ProcessEvent(nextTick))");
    	getStream().println(getIdent() + "{");
    	getStream().println(getIdent() + "case fsm::ProcessRunning:");
    	getStream().println(getIdent() + "    break;");
    	getStream().println(getIdent() + "case fsm::ProcessEnd:");
    	getStream().println(getIdent() + "    m_timerProcess.cancel();");
    	getStream().println(getIdent() + "    break;");
    	getStream().println(getIdent() + "default:");
    	getStream().println(getIdent() + "    break;");
    	getStream().println(getIdent() + "}");
    	popIdentLevel();
    	getStream().println(getIdent() + "}");
    }    
    
    /**
     * libev specific, asynchronous
     * @param state
     * @throws StateBuilderException
     */
    public void writeContextLibevProcessImplementation(State state) throws StateBuilderException{
    	if(getStateMachine().getSettings().isAsynchronous() == false){
    		return;
    	}
    	getStream().println(getIdent() + "void " + getModel().getContextClassName(state)+ "::process()");
    	getStream().println(getIdent() + "{");
    	pushIdentLevel();
    	getStream().println(getIdent() + "long nextTick = 0;");
    	getStream().println(getIdent() + "switch (ProcessEvent(nextTick))");
    	getStream().println(getIdent() + "{");
    	getStream().println(getIdent() + "case fsm::ProcessRunning:");
    	getStream().println(getIdent() + "    break;");
    	getStream().println(getIdent() + "case fsm::ProcessEnd:");
    	getStream().println(getIdent() + "    m_timerProcess.stop();");
    	getStream().println(getIdent() + "    break;");
    	getStream().println(getIdent() + "default:");
    	getStream().println(getIdent() + "    break;");
    	getStream().println(getIdent() + "}");
    	popIdentLevel();
    	getStream().println(getIdent() + "}");
    }
    /**
     * Qt:
     * void ObjectContext::schedule(long nextTick)
     * {
     *     startTimer(nextTick);
     * }
     * 
     * Boost:
     * void ObjectContext::schedule(long nextTick)
     * {
     *   m_timer.cancel();
     *   m_timer.expires_from_now(boost::posix_time::milliseconds(nexTick));
     *   m_timer.async_wait(boost::bind(&HelloWorldContext::process, this, _1)); 
     * }  
     * @param state
     * @throws StateBuilderException
     */
    public void writeContextSchedule(State state) throws StateBuilderException{
    	if(getStateMachine().getSettings().isAsynchronous() == false){
    		return;
    	}
    	getStream().println(getIdent() +
    			"void " + getModel().getContextClassName(state)+ "::" + SCHEDULE_FUNCTION + "long nextTick)");
    	getStream().println(getIdent() + "{");
    	pushIdentLevel();
    	
    	if(getLibrary().compareTo(LANGUAGE_QT) == 0){
    	    getStream().println(getIdent() + "startTimer(nextTick);");
    	} else if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
    		getStream().println(getIdent() + "if(m_timerProcess.expires_from_now(boost::posix_time::milliseconds(nextTick)) == 0){");
    		pushIdentLevel();
    		getStream().println(getIdent() + "m_timerProcess.async_wait(boost::bind(&" + getModel().getContextClassName(state)+ "::process, this, _1));");
    		popIdentLevel();
    		getStream().println(getIdent() + "}");
    	} else if(getLibrary().compareTo(LANGUAGE_LIBEV) == 0){
    		getStream().println(getIdent() + "m_timerProcess.start(0);");
    	}
    	
    	popIdentLevel();
    	getStream().println(getIdent() + "}");
    	getStream().println("");
    }    
    
    /**
     * Lock function
     * Lock
     * Qt:     m_mutex.lock();
     * Boost:  m_mutex.lock();
     */
    public void writeLockFunction(State state) throws StateBuilderException{
    	if((isLock() == false) || (state.isRoot() == false)){
    		return;
    	}
    	
    	getStream().println(getIdent() + "void " + getModel().getContextClassName(state)+ "::Lock()");
    	getStream().println(getIdent() + "{");
		pushIdentLevel();

		if (getLibrary().compareTo(LANGUAGE_QT) == 0) {
			getStream().println(
					getIdent() + "m_mutex.lock();");
		} else if (getLibrary().compareTo(LANGUAGE_BOOST) == 0) {
			getStream().println(
					getIdent() + "m_mutex.lock();");
		} else {
			
		}
		
		popIdentLevel();
		getStream().println(getIdent() + "}");
		getStream().println("");
    }

    /**
     * UnLock function
     * Lock
     * Qt:     m_mutex.unlock();
     * Boost:  m_mutex.unlock();
     */
    public void writeUnLockFunction(State state) throws StateBuilderException{
    	if((isLock() == false) || ((state.isRoot() == false))){
    		return;
    	}
    	
    	getStream().println(getIdent() + "void " + getModel().getContextClassName(state)+ "::UnLock()");
    	getStream().println(getIdent() + "{");
		pushIdentLevel();

		if (getLibrary().compareTo(LANGUAGE_QT) == 0) {
			getStream().println(
					getIdent() + "m_mutex.unlock();");
			
		} else if (getLibrary().compareTo(LANGUAGE_BOOST) == 0) {
			getStream().println(
					getIdent() + "m_mutex.unlock();");
		} else {
		}
		
		popIdentLevel();
		getStream().println(getIdent() + "}");
		getStream().println("");
    }    
    
    /**
     * Lock()
     */
    public void writeLock(){
        if(isLock() == true){
    		getStream().println(getIdent() + "Lock();");
        }
    }
    
    /**
     * Unlock() 
     */    
    public void writeUnlock(){
        if(isLock() == true){
        	getStream().println(getIdent() + "UnLock();");
        }
    }
}
