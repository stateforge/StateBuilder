package com.stateforge.statebuilder.cpp;

import java.io.PrintStream;
import java.text.DateFormat;
import java.util.Calendar;
import java.util.List;


import com.stateforge.statebuilder.StateBuilder;
import com.stateforge.statebuilder.StateBuilderException;
import com.stateforge.statebuilder.StateBuilderInterface;
import com.stateforge.statebuilder.model.Event;
import com.stateforge.statebuilder.model.ObjectType;
import com.stateforge.statebuilder.model.Parameter;
import com.stateforge.statebuilder.model.State;
import com.stateforge.statebuilder.model.StateMachineModel;

/**
 * This class is the base abstract class for the Cpp ang Hpp generator
 * It contains common routines for both generators
 * @author Frederic Heem
 *
 */
public abstract class StateBuilderCppCommon extends StateBuilderInterface {    
    //Class name
	public final static String LANGUAGE_QT = "qt";
	public final static String LANGUAGE_BOOST = "boost";
	public final static String LANGUAGE_STD = "std";
	public final static String LANGUAGE_LIBEV = "libev";
    public static final String FSM_NAMESPACE = "fsm";
    public static final String FSM_STATE_CLASSNAME = FSM_NAMESPACE + "::" + STATE_CLASSNAME;
    public static final String FSM_CONTEXT_CLASSNAME = FSM_NAMESPACE + "::" + CONTEXT_CLASSNAME;
    public static final String FSM_CONTEXTBASE_CLASSNAME = FSM_NAMESPACE + "::" + CONTEXTBASE_CLASSNAME;
    public static final String FSM_CONTEXTASYNC_CLASSNAME = FSM_NAMESPACE + "::" + CONTEXTASYNC_CLASSNAME;
    public static final String FSM_PARALLEL_CLASSNAME = FSM_NAMESPACE + "::" + PARALLEL_CLASSNAME;
    public static final String FSM_IPARALLEL_CLASSNAME = FSM_NAMESPACE + "::" + IPARALLEL_CLASSNAME;
    public static final String FSM_TIMER = "FsmTimer";
    public static final String FSM_PROCESSRETURN = FSM_NAMESPACE +  "::" + "ProcessReturn";
    public static final String FSM_PROCESSERROR = FSM_NAMESPACE +  "::" + "ProcessError";
    public static final String FSM_PROCESSRUNNING = FSM_NAMESPACE +  "::" + "ProcessRunning";
    public static final String FSM_PROCESSEND = FSM_NAMESPACE +  "::" + "ProcessEnd";
    public static final String FSM_PROCESSEVENT = "ProcessEvent";
    public static final String FSM_IOBSERVER = FSM_NAMESPACE +  "::" + "IObserver";
    
    public static final String MEMBER_PREFIX = "m_";
    public static final String GETEVENTLIST = "GetEventList()";
    public static final String GETTIMERLIST = "GetTimerList()";
    public static final String GETINSTANCE = "GetInstance()";
    public static final String GETSTATE = "GetState()";
    public static final String GETSTATEPREVIOUS = "GetStatePrevious()";    
    public static final String SETSTATEPREVIOUS = "SetStatePrevious(";        
    public static final String SETSTATE = "SetState(";
    public static final String GETSTATENEXT = "GetStateNext()";    
    public static final String PROCESSTRANSITION = FSM_NAMESPACE +  "::" + "ProcessTransition";
    public static final String PROCESSTRANSITIONPRE = FSM_NAMESPACE +  "::" + "ProcessTransitionPre";    
    public static final String PROCESSTRANSITIONPOST = FSM_NAMESPACE +  "::" + "ProcessTransitionPost";    
    public static final String WALKCHAINENTRY = FSM_NAMESPACE +  "::" + "WalkChainEntry";
    public static final String WALKCHAINEXIT = FSM_NAMESPACE +  "::" + "WalkChainExit"; 
    public static final String TRANSITION_TO_NEXT_STATE = "TransitionToNextState()";
    public static final String GET = "Get";
    public static final String SAVE_STATE = "SaveState()";
    public static final String GET_STATE_HISTORY = "GetStateHistory()";
    public static final String SET_INITIAL_STATE = "SetInitialState";
    public static final String ENTER_INITIAL_STATE = "EnterInitialState()";
    public static final String LEAVE_CURRENT_STATE = "LeaveCurrentState()";
    public static final String ENTER_STATE = "EnterState";
    public static final String PUSHBACK_EVENT = "PushBackEvent";
    public static final String SETOBSERVER = "SetObserver";
    public static final String GETOBSERVER = "GetObserver()";
    public static final String ONTRANSITIONPRE = "OnTransitionBegin";
    public static final String ONTRANSITIONPOST = "OnTransitionEnd";    
    public static final String ONTIMERSTART = "OnTimerStart(";
    public static final String ONTIMERSTOP = "OnTimerStop("; 
    
    public static final String SETPARALLEL = "SetParallel";
    public static final String GETPARALLEL = "GetParallel()";
    public static final String PARALLEL_VAR = "pParallel";
    
    public static final String SET_TRANSITION_NAME = "SetTransitionName";
    public static final String SET_TRANSITION_ID = "SetTransitionId";
    
    public static final String SETSTATEACTIVE = "SetStateActive";
    public static final String GETSTATEACTIVE = "GetStateActive()";
    public static final String STATEACTIVEDECREMENT = "StateActiveDecrement()";
    
    public static final String CONTEXT_LIST = "m_contextList";
    public static final String GETPARENTCONTEXT = "GetParentContext()";
    
    public static final String GETNAME = "GetName()";
    public static final String GETID = "GetId()";
    public static final String SETNAME = "SetName";  
    public static final String E_STATE_FINAL = "STATE_FINAL";
    public static final String E_STATE_ERROR = "STATE_ERROR";
    public static final String MEMBER_STATEKIND = "m_eStateKind";
    public static final String MEMBER_STATE_PARENT = "m_pStateParent";
    public static final String ONENTRY = "OnEntry";
    public static final String ONEXIT = "OnExit";
    public static final String EVDEFAULT = "EvDefault";
    public static final String SYNC_SUFFIX = "Sync";
    
    public static final String TIMERSTART = "TimerStart";
    public static final String TIMERSTOP = "TimerStop";
    
    public static final String LOCK = "Lock()";
    public static final String UNLOCK = "UnLock()";

    public static final String ISCHEDULE = FSM_NAMESPACE +  "::ISchedule";
    public static final String VARIABLE_SCHEDULE = "schedule";
    public static final String MEMBER_SCHEDULE = "m_schedule";
    public static final String SCHEDULE_FUNCTION = "Schedule(";

    public static final String IMANAGEMENT = FSM_NAMESPACE +  "::IManagement";
    public static final String MANAGEMENTGET = "GetManagement()";
    public static final String MANAGEMENTSET = "SetManagement(";
    public static final String ON_END = "OnEnd()";
    
    protected StateBuilderCppCommon(StateMachineModel model) {
        super(model);
    }
    
    /* (non-Javadoc)
     * @see com.stateforge.fsm.StateBuilderInterface#setParam(java.lang.String, org.w3c.dom.Document, com.stateforge.fsm.fsmxml.StateMachineType, java.io.PrintStream)
     */
    protected void setParam(
    		String name, 
    		StateMachineModel model,
            PrintStream stream){
        super.setParam(name, model, stream);        
    }    
    
    /**
     * Write the beginning of the package declaration if present
     * namespace mynamespace {
     */    
    protected void writeNamespaceBegin() throws StateBuilderException {
    	String namespace = getStateMachine().getSettings().getNamespace();
    	if(namespace.isEmpty() == false){
    		getStream().println();
    		String[] namespaces = namespace.split("::");
    		getStream().print(getIdent());
    		for(String namespaceSplitted : namespaces){
    			getStream().print("namespace " + namespaceSplitted + " { ");
    		}
    	
    		//pushIdentLevel();
    		getStream().println();
    	}	
    }    

    /**
     * Write the beginning of the package declaration if present
     * } // namespace mynamespace {
     */    
    protected void writeNamespaceEnd() throws StateBuilderException {
    	String namespace = getStateMachine().getSettings().getNamespace();
    	if(namespace.isEmpty() == false){
    		//popIdentLevel();
    		String[] namespaces = namespace.split("::");
    		getStream().print(getIdent());
    		for(int i = 0; i < namespaces.length; i++){
    			getStream().print("}");
    		}
    		
    		getStream().println("// namespace " + namespace);
        	getStream().println();
    	}
    }        
    
    /**
     * Write compiler informations, the date and time when the files are generated
     * @param fileName the generated file name
     */    
    protected void writeCompilerInfo(final String fileName, final String[] commandLine) throws StateBuilderException {
    	getStream().print("// " + fileName + " has been generated automatically ");
    	//TODO HEEFRE application name
    	getStream().println("by StateBuilderCpp " + getVersion());
    	
    	getStream().println("// Executable: ");
    	String path = StateBuilder.class.getProtectionDomain().getCodeSource().getLocation().getPath();
    	// file:/C:/Users/frederic/Documents/Visual%20Studio%202008/Projects/dev/StateBuilderCpp/bin/StateBuilderCpp.exe!/main/StateBuilderCpp-1.0.22.jar
    	int index = path.indexOf("!");
    	if(index > 0){
    		path = path.substring(0, index);	
    	}
    	
    	getStream().println("// " + path);
    	
    	getStream().println("// Command line options: ");
    	getStream().print("// ");
    	for(int i = 0; i < commandLine.length; i++){
    		getStream().print(" " + commandLine[i]);
    	}
    	getStream().println("");
    	DateFormat.getDateTimeInstance().format(Calendar.getInstance().getTime());
    	getStream().println("// Date: " + DateFormat.getDateTimeInstance().format(Calendar.getInstance().getTime()));
    	getStream().println("");
    }      
    
    /**
     * Write the parameter declaration:
     * Param1 param1, const Param2& param2, Param3 *pParam3
     * @param event
     */
    protected void writeParameterDeclaration(Event event){
        if(event == null){
            return;
        }
        
        List<Parameter> parameterList = event.getParameter();
        for(int i = 0; i < parameterList.size(); i++){
            Parameter parameter = parameterList.get(i);
            // Const 
            if(parameter.isConst() == true){
                getStream().print("const ");
            }
            // Type
            getStream().print(parameter.getType());
            
            // PassedBy
            if(parameter.getPassedBy().value().compareTo("reference") == 0){
                getStream().print("& ");
            } else  if(parameter.getPassedBy().value().compareTo("pointer") == 0){
                getStream().print("* ");
            } else {
                getStream().print(" ");
            }        

            // Value
            getStream().print(parameter.getName());
          
            //Add a coma except for the last parameter
            if(i < parameterList.size() - 1){
                getStream().print(", ");
            }
        }
    } 
    
    /**
     * Write the parameter declaration:
     * param1, param2, pParam3
     * @param event
     */
    protected void writeParameterList(Event event){
        if(event == null){
            return;
        }   
        
        // Add a coma if there is at least one parameter
        if(event.getParameter().isEmpty() == false){
            getStream().print(", ");
        }
        
        List<Parameter> parameterList = event.getParameter();
        for(int i = 0; i < parameterList.size(); i++){
            Parameter parameter = parameterList.get(i);
            // Value
            getStream().print(parameter.getName());
            //Add a coma except for the last parameter
            if(i < parameterList.size() - 1){
                getStream().print(", ");
            }
        }
    }     
    
    /**
     * Get the state class name from the given state: MicrowaveDoorOpenedState 
     * @param the state
     * @return the state class name
     * @throws StateBuilderException
     */
    String getStateClassName(State state) throws StateBuilderException {
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
    String getStateClassName(String stateName) throws StateBuilderException {
        return getName() + stateName + "State";
    }

    /**
     * Get the parallel class name from the given state: MicrowaveOperatingParallel.
     * @param state
     * @return the parallel class name
     * @throws StateBuilderException
     */
    String getParallelClassName(State state) throws StateBuilderException {
        return getName() + state.getName() + PARALLEL_CLASSNAME;
    }    
    
    /**
     * get the context member name from the given state:
     *   m_contextStateName 
     * @param stateName 
     * @return the context member name
     * @throws StateBuilderException
     */
    String getContextMemberName(State state) throws StateBuilderException {
        return MEMBER_PREFIX + getModel().getContextInstance() + state.getName();    
    }    
    
    /**
     * get the parallel member name from the given state:
     *   m_parralelStateName 
     * @param stateName 
     * @return the parallel member name
     * @throws StateBuilderException
     */
    String getParallelMemberName(State state) throws StateBuilderException {
        return MEMBER_PREFIX + PARALLEL + state.getName();    
    }       
    
    /**
     * get the name of the function to get the context of a state
     *   GetContextS1
     * @param stateName
     * @return
     */
    protected String getGetContext(State state) throws StateBuilderException{
        if(state.isRoot() == true){
            return GET + CONTEXT_CLASSNAME;
        } else {
            return GET + CONTEXT_CLASSNAME + state.getName();
        }
    }
    
    /**
     * get the name of the function to get the parallel of a state:
     *   GetParallelS1
     * @param stateName
     * @return
     */
    protected String getGetParallel(State state) throws StateBuilderException {
    	return GET + PARALLEL_CLASSNAME + state.getName();
    }    

    /**
     * get the template Context-State, e.g 
     * @param state
     * @return the string representing the template <ObjectContext, ObjectState>
     */
    protected String getTemplateContextState(State state) throws StateBuilderException{
        return "<" + getModel().getContextClassName(state) + ", " + getStateClassName(state) +">";
    }

    /**
     * get the name of the root state class name: 
     *   fsm::State<ObjectContext, ObjectState>
     * @param state
     * @return the name of the root state class name
     */
    protected String getStateClassNameRoot(State state) throws StateBuilderException{
        return FSM_STATE_CLASSNAME + getTemplateContextState(state);
    }  
  
    /**
     * Not used any longer with static const states
     * get the name of the state member variable:
     *  m_pStateName 
     * @param state
     * @return
     */
    protected String getStateMember(State state){
        return MEMBER_PREFIX + "p" + state.getName();
    }
  
    /**
     * get the name of the state static variable:
     *  kLedSwitchedOffState
     * @param state
     * @return
     * @throws StateBuilderException 
     */
    protected String getStateVariable(State state) throws StateBuilderException{
    	String className = getStateClassName(state);
    	return  "k" + className;
    }
    
    public String getClassNameWithoutNamespace(String className){
    	if(!className.isEmpty()){
    		String[] namespaces = className.split("::");
    		return namespaces[namespaces.length - 1];
    	} else {
    		return "";
    	}
    }
    /**
     * getObject1()
     */
    public String getContextObjectGetter(ObjectType object){
        return GETTER_PREFIX + getClassNameWithoutNamespace(object.getClazz());
    }
    

    /**
     * Object1& object1
     */
    public void writeObjectDeclaration(ObjectType object) {
        getStream().print(object.getClazz()
                + "& "
                + object.getInstance());
    }

    /**
     * object1, object2
     */
    public void writeObjectsParameterInstance() {
    	List<ObjectType> objectList = getStateMachine().getSettings().getObject();
        ObjectType objectLast = objectList.get(objectList.size() - 1);
        for (ObjectType object : getStateMachine().getSettings().getObject()) {
        	getStream().print(object.getInstance());
            if(object.equals(objectLast) == false){
            	getStream().print(", ");
            }   
        }
    }
    
    /**
     * Object1& object1, Object2& object2
     */
    public void writeObjectsParameterDeclaration() {
    	List<ObjectType> objectList = getStateMachine().getSettings().getObject();
        ObjectType objectLast = objectList.get(objectList.size() - 1);
        for (ObjectType object : getStateMachine().getSettings().getObject()) {
            writeObjectDeclaration(object);
            if(object.equals(objectLast) == false){
            	getStream().print(", ");
            }   
        }
    }
    /**
     * Object1& object1 = context.getObject1();
     * Object2& object1 = context.getObject2();
     */
    public void writeObjectsDeclaration() {
        for (ObjectType object : getStateMachine().getSettings().getObject()) {
        	getStream().print(getIdent());
        	writeObjectDeclaration(object);
            getStream().println(" = "
                    + getModel().getContextInstance()
                    + "."
                    + getContextObjectGetter(object)
                    + "();");
        }
    }

    /**
     * Object1& m_object1;
     * Object2& m_object2;
     */
    public void writeObjectsMemberDeclaration() {
        for (ObjectType object : getStateMachine().getSettings().getObject()) {
        	getStream().println(getIdent() +
        			object.getClazz() + "& "+ MEMBER_PREFIX + object.getInstance() + ";"); 
        }
    }

    /**
     * Object1& getObject1(){return m_object1;}
     * Object2& getObject2(){return m_object2;}
     */
    public void writeObjectGetters() {
        for (ObjectType object : getStateMachine().getSettings().getObject()) {
        	getStream().println(getIdent() +
        			object.getClazz() + "& "+ getContextObjectGetter(object)
        			+ "(){return " + MEMBER_PREFIX + object.getInstance() + ";}"); 
        }
        getStream().println("");
    }

    /**
     * , m_object1(object1)
     * , m_object2(object2)
     */
    public void writeObjectsInitialization() {
        for (ObjectType object : getStateMachine().getSettings().getObject()) {
        	getStream().print(getIdent() + ", "
        			+ MEMBER_PREFIX + object.getInstance() + "(" + object.getInstance() + ")"); 
        }
    }
}
