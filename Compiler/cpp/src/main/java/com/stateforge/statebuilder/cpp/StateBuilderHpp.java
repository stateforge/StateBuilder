package com.stateforge.statebuilder.cpp;

import java.util.List;
import java.util.Map;
import java.util.logging.Level;

import com.stateforge.statebuilder.StateBuilderException;
import com.stateforge.statebuilder.model.Event;
import com.stateforge.statebuilder.model.EventSource;
import com.stateforge.statebuilder.model.ObjectType;
import com.stateforge.statebuilder.model.Observer;
import com.stateforge.statebuilder.model.Parallel;
import com.stateforge.statebuilder.model.State;
import com.stateforge.statebuilder.model.StateKind;
import com.stateforge.statebuilder.model.StateMachineModel;
import com.stateforge.statebuilder.model.Timer;

public class StateBuilderHpp extends StateBuilderCppCommon {
    public static final String FILE_EXTENSION = ".h";
    public static final String FSM_INCLUDE = "fsm/Fsm.h";
    public static final String FSM_ASYNC_INCLUDE = "fsm/FsmAsync.h";
    
    private String m_hashDefine;
    
    public StateBuilderHpp(StateMachineModel model) {
        super(model);      
    }

    @Override
    public void generate() throws StateBuilderException {
        writeCompilerInfo(getName() + FSM_SUFFIX + FILE_EXTENSION, commandLine);
        writeHashIfnDefBegin();
        writeDefine();
        writeInclude();
        //writeFriendClassMacro();
        writeForwardDeclaration();
        
        writeNamespaceBegin();
        writeForwardDeclarationInNamespace();
        writeContext();
        writeState();
        writeNamespaceEnd();
        writeHashIfnDefEnd();        
    }

    @Override
    public String getFileExtension() {
        return FILE_EXTENSION;
    }
    
    /**
     * Write the following code :
     *  #ifdef  __NAMEFSM_H__
     *  #define __NAMEFSM_H__
     */
    private void writeHashIfnDefBegin(){
        m_hashDefine = getName().toUpperCase() + "FSM_H";
        getStream().println("#ifndef " + m_hashDefine);
        getStream().println("#define " + m_hashDefine);
        getStream().println();
    }
    
    /**
     * Write the following code :
     *  #endif // __NAMEFSM_H__
     */    
    
    private void writeHashIfnDefEnd(){
        getStream().println("#endif //" + m_hashDefine);
    }  
    
    /**
     * #define FSM_TIMERS_INTERNAL
     */
    private void writeDefine(){
        if(getLibrary().compareTo(LANGUAGE_QT) == 0){
        } else if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
        } else if(getLibrary().compareTo(LANGUAGE_LIBEV) == 0){
                
        } else {
        	if(getStateMachine().getSettings().isAsynchronous() == true){
        		getStream().println("#define FSM_TIMERS_INTERNAL");
        	}
        } 	
        
        if(getStateMachine().getSettings().getObserver().equals(Observer.NO)){
        	getStream().println("#define STATEFORGE_OBSERVER_NO");
        } else if(getStateMachine().getSettings().getObserver().equals(Observer.INT)){
        	getStream().println("#define STATEFORGE_OBSERVER_INT");
        }
        
        getStream().println("");
    }
    
    /**
     * generate the include code for synchronous fsm:
     *   #include <objectHeader.h>
     *   #include <fsm/Fsm.h>
     * or the following code  for asynchronous fsm
     *   #include <objectHeader.h>
     *   #include <fsm/FsmAsync.h>
     * @throws StateBuilderException 
     */
    private void writeInclude() throws StateBuilderException{
        getStream().println("// Includes");
        
        //getStream().println("#include <" + getFsm().getControlledObjectHeader() + ">");
        String fsmInclude = FSM_INCLUDE;
        if(getStateMachine().getSettings().isAsynchronous() == true){
            fsmInclude = FSM_ASYNC_INCLUDE;   
        }
        
        getStream().println("#include <cstring>");
        
        if(getLibrary().compareTo(LANGUAGE_QT) == 0){
            getStream().println("#include <QtCore/QObject>");
            getStream().println("#include <QtCore/QTimer>");
            getStream().println("#include <QtCore/QTimerEvent>");
        } else if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
            getStream().println("#ifdef _MSC_VER");
            getStream().println("#pragma warning(push, 0)");
            getStream().println("#endif /*#ifdef _MSC_VER*/");
            getStream().println("#include <boost/bind.hpp>");
            getStream().println("#include <boost/asio.hpp>");
            getStream().println("#include <boost/date_time/posix_time/posix_time_types.hpp>");
            getStream().println("#ifdef _MSC_VER");
            getStream().println("#pragma warning(pop)");
            getStream().println("#endif /*#ifdef _MSC_VER*/");
        } else if(getLibrary().compareTo(LANGUAGE_LIBEV) == 0){
        	getStream().println("#include <ev++.h>");
        }
        
        generateLockInclude();
        
        getStream().println("#include <" + fsmInclude + ">");
        
        // #include <include1.h>
        // #include <include2.h>
        List<String> includeList = getStateMachine().getSettings().getInclude();
        for(String include : includeList){
            getStream().println("#include <" + include + ">");
        }
        
        getStream().println();
    }
    /**
     * Generate the friend class macro to be used in the 
     * controlled object declaration located in its header file.
     * The state class declared as the friend classes can therefore 
     * access the controlled object private members.
     * Example:
     * #define FRIEND_CLASS_FSM friend class HelloWorldStateFoo; \
	 *                          friend class HelloWorldStateBar;
     * @throws StateBuilderException if the number of state is not is less or equal than 3
     * 
     */
    /*
    private void writeFriendClassMacro() throws StateBuilderException{
    	getStream().print("#define " + getName().toUpperCase() + "_SM_FRIEND_CLASS");
    	Set<String> mapSet = getModel().getStateMap().keySet();
    	String[] stateNameArray = mapSet.toArray(new String[0]);
    	if(stateNameArray.length < 3){
    		throw new StateBuilderException("The number of state cannot be lower than 3: the root state, the final state and one user state");
    	}
    	getStream().println(" friend class " + getStateClassName(stateNameArray[0]) + "; \\");
    	for(int i = 1; i < stateNameArray.length - 1 ; i++){
    		getStream().println("                         " +
    				"friend class " + getStateClassName(stateNameArray[i]) + "; \\");
    	}
    	getStream().println("                         friend class " + 
    			getStateClassName(stateNameArray[stateNameArray.length - 1]) + ";");
    	getStream().println("");
    }
    */
    /**
     * Write the forward declaration code :
     * 
     *  //Context classes forward declaration
     *  class UrlGetContext;
     *  // Parallel classes forward declaration
     *  class UrlGetRunningParallel;
     *  // State classes forward declaration
     *  class UrlGetEndState;
     *  class UrlGetTopState;
     *  class UrlGetIdleState;
     *  class UrlGetFtpEndState;
     *  class UrlGetRunningState;
     *  class UrlGetHttpState;
     *  class UrlGetFtpState;
     *  class UrlGetFtpGetState;
     *  class UrlGetHttpGetState;
     *  class UrlGetHttpEndState;
     */
    private void writeForwardDeclaration() throws StateBuilderException {
        getStream().println(getIdent() +  "// Object classes forward declaration");
        for (ObjectType object : getStateMachine().getSettings().getObject()) {
        	if(!object.getClazz().isEmpty()){
        		String[] namespaces = object.getClazz().split("::");

        		if(namespaces.length >= 2){
        			for(int i = 0; i < namespaces.length -1; i++){
        				getStream().println(getIdent() + "namespace " + namespaces[i] + " { ");
        				pushIdentLevel();
        			}
        			
            		getStream().println(getIdent() + "class " + namespaces[namespaces.length - 1] + ";");
            		
            		for(int i = 0; i < namespaces.length -1; i++){
        				popIdentLevel();
        				getStream().print("}");	
        			}
        		}
        		getStream().println();
        	}
        }
        
        writeNamespaceBegin();
        getStream().println(getIdent() +  "// Context classes forward declaration");

        for(Map.Entry<String, State> stateEntry  : getModel().getStateMap().entrySet()){
        	State state = stateEntry.getValue();
        	if(state.isTop() == true){
        		getStream().println(getIdent() + 
                		"class " + getModel().getContextClassName(state) + ";");
        	}
        }
        
        getStream().println(getIdent() +  "// Parallel classes forward declaration");
        
        for(Map.Entry<String, State> stateEntry  : getModel().getStateMap().entrySet()){
        	State state = stateEntry.getValue();
        	if(state.getParallel() != null){
        		getStream().println(getIdent() + 
                		"class " + getParallelClassName(state) + ";");
        	}
        }
        
        getStream().println(getIdent() +  "// State classes forward declaration");
        for(Map.Entry<String, State> stateEntry  : getModel().getStateMap().entrySet()){
        	if (stateEntry.getValue().getKind() == StateKind.HISTORY) {
        		// Do not generate a state class for pseudo state history.
        	} else {
        		String stateName = getStateClassName(stateEntry.getValue());
        		getStream().println(getIdent() + "class " + stateName + ";");
        	}
        }
        
        writeNamespaceEnd();
        getStream().println("");
    }

    /*
     * Forward declaration for object without namespace, which will be under the eventual machine namespace
     */
    private void writeForwardDeclarationInNamespace() throws StateBuilderException {
        getStream().println(getIdent() +  "// Object classes forward declaration within the custom namepace");
        for (ObjectType object : getStateMachine().getSettings().getObject()) {
        	if(!object.getClazz().isEmpty()){
        		String[] namespaces = object.getClazz().split("::");

        		if(namespaces.length == 1){
        			getStream().println(getIdent() + "class " + namespaces[0] + ";");
        		}
        		
        		getStream().println();
        	}
        }
        
        getStream().println("");
    }

    /**
     * Write all states
     * 
     * class State1 : public StateParent {
     *   public:
     *     State1(const char* pName, int iStateId) : StateParent(pName, stateId){}
     *     virtual void EvEvent2(ObjectContext& context, State &state, Param2 param2, Param3 *pParam3);
     *     virtual void EvEvent3(ObjectContext& context, State &state, Param4 *pParam4);
     * }
     * 
     * class State2 : public StateParent {
     *   public:
     *     State2(const char* pName, int iStateId) : StateParent(pName, stateId){}
     *     virtual void EvEvent1(ObjectContext& context, State &state, Param2 param2, Param3 *pParam3);
     * } 
     * 
     */        
    private void writeState() throws StateBuilderException { 
        writeState(getStateMachine().getState(),
                      getStateClassNameRoot(getStateMachine().getState()));
    }
    
    /** generate the states for one state:
     * 
     * class State1 : public StateParent {
     *   public:
     *     State1(const char* pName) : StateParent(pName){}
     *     virtual void EvEvent2(ObjectContext& context, Param2 param2, Param3 *pParam3);
     *     virtual void EvEvent3(ObjectContext& context, Param4 *pParam4);
     * 
     * }
     */    
    private void writeState(State state, String stateParentName) throws StateBuilderException {
    	if((state == null) || (stateParentName == null)){
    		throw new StateBuilderException("Internal Exception");
    	}
    	
    	if (state.getKind() == StateKind.HISTORY) {
    		// Do not generate a state class for pseudo state history.
    		return;	
    	}
    	
        String contextClassName = getModel().getContextClassName(state);
        
        String stateName = getStateClassName(state);
        getStream().println();
        getStream().println(getIdent() + "// State " + stateName);

        m_logger.log(Level.FINE, "state " + stateName + " parent state " + stateParentName  + ", contextClassName " + contextClassName);
        // class State1 : public StateParent {
        writeClassDeclaration(stateName, "public " + stateParentName);
        //pushIdentLevel();
        //  public:
        getStream().println(getIdent() + "public:");
        pushIdentLevel();
        getStream().println(getIdent() + "// Constructor");
        // State1();
        writeStateConstructorDeclaration(state);
        
        writeStateGetInstance(stateName);
        
        getStream().println(getIdent() + "// Destructor");
        // ~State1() {}
        writeStateDestructorDeclaration(stateName);
        getStream().println();

        // OnEntry and OnExit declaration if any
        getStream().println(getIdent() + "// OnEntry and OnExit");
        
        // virtual void OnEntry(StateContext&);
        
        writeOnEntryExitFunctionDeclaration(ONENTRY, contextClassName);
        getStream().println(";");            

        // virtual void OnExit(StateContext&);
        
        writeOnEntryExitFunctionDeclaration(ONEXIT, contextClassName);
        getStream().println(";");            

        getStream().println();
        
        // virtual void EvEvent2(StateContext& context, Param2 param2, Param3 *pParam3);
        // virtual void EvEvent3(StateContext& context, Param3 *pParam3);
        List<Event> eventList = getModel().getEventsForState(state);
        
        getStream().println(getIdent() + "// Events");
        
        writeStateEventDeclaration(eventList, contextClassName);
        popIdentLevel();
        getStream().println();
        
        // Not used due to static const classes
        //  private :
        //getStream().println(getIdent() + "protected:");
        //pushIdentLevel();
        
        //getStream().println(getIdent() + "// static class ");
        //getStream().println(getIdent() + "static " + stateName + " const *" + getStateVariable(state) + ";"); 
        
        //getStream().println();        
        
        //popIdentLevel();
        getStream().println(getIdent() + "};");
        getStream().println();
        
        List<State> stateSubList = state.getState();
        
        //Sanity check, cannot have both sub and parallel state  
        if((stateSubList.size() > 0) && state.getParallel() != null){
        	throw new StateBuilderException("state " + state.getName());
        }
        
        //Do it recursively
    	for(State stateSub : stateSubList){
    		m_logger.log(Level.INFO, "state " + stateName + " has substates");
            writeState(stateSub, getStateClassName(state));    		
    	}
    	
    	//Special case for orthogonal state
        Parallel parallel = state.getParallel();
        if(parallel != null){
        	m_logger.log(Level.INFO, "state " + stateName + " is parallel");
        	List<State> stateList = parallel.getState();
        	for(State stateOrthogonal : stateList){                 
                writeState(stateOrthogonal, getStateClassNameRoot(stateOrthogonal));    		
        	}
        }	
    }
    
    
    /**
     * Write all parallel states for a given state
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
     * generate a parallel state
     * @param state
     * @throws StateBuilderException
     */
    private void writeParallel(State state) throws StateBuilderException {
        Parallel parallel = state.getParallel();
        if(parallel == null){
            return;
        }
        
        getStream().println(getIdent() + "// Parallel declaration for " + state.getName());
        
        writeClassDeclaration(getParallelClassName(state), "public " + FSM_PARALLEL_CLASSNAME);
        pushIdentLevel();
        //  public:
        getStream().println(getIdent() + "public:");
        pushIdentLevel();
  
        writeContextConstructor(state, getParallelClassName(state), getModel().getContextParentClassName(state));
        
        writeContextGetContext(parallel);
        
        // void TransitionToNextState();
        getStream().println(getIdent() + "void " + TRANSITION_TO_NEXT_STATE + ";");
        
        getStream().println();
        popIdentLevel();
        //  private:
        getStream().println(getIdent() + "private:");
        pushIdentLevel();
        
        writeContextMemberDeclaration(state, parallel);
        
        popIdentLevel();
        popIdentLevel();
        getStream().println(getIdent() + "};");
        getStream().println();
    }    
    
    /**
     * Write all context classes
     */
    private void writeContext() throws StateBuilderException{
        writeContext(getStateMachine().getState());        
    }
    
    /**
     * Write a context class
     */
    private void writeContext(State state) throws StateBuilderException {
    	if(state.isTop() == true){
    		getStream().println(getIdent() + "// Context declaration for state " + state.getName());
    		String contextClassName = getModel().getContextClassName(state); 
    		String contextParentClassName = getModel().getContextParentClassName(state);
    		String stateName = getStateClassName(state);    
    		String baseClass = "";

    		if(getLibrary().compareTo(LANGUAGE_QT) == 0){
    			baseClass = " public QObject, ";
    		}

    		if((getStateMachine().getSettings().isAsynchronous() == true) && (state.isRoot() == true)){
    			baseClass += "public " + FSM_CONTEXTASYNC_CLASSNAME + 
    			"<" + 
    			stateName + ", " + 
    			contextClassName + ", " +
    			contextParentClassName + 
    			">";
    		} else {
    			baseClass += "public " + FSM_CONTEXT_CLASSNAME +                         
    			"<" +  
    			stateName + ", " + 
    			contextParentClassName + 
    			">";;
    		}

    		if((getStateMachine().getSettings().isAsynchronous() == true) && (state.isRoot() == true)){
    			if(getLibrary().compareTo(LANGUAGE_QT) == 0){
    				baseClass += ", public " + ISCHEDULE + ", public " + IMANAGEMENT;
    			} else if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
    				baseClass += ", public " + ISCHEDULE;
    			} else if(getLibrary().compareTo(LANGUAGE_LIBEV) == 0){
    				baseClass += ", public " + ISCHEDULE;
    			}
    		}

    		if((isLock() == true) && ((state.isRoot() == true)) &&
    				((getLibrary().compareTo(LANGUAGE_QT) == 0) || (getLibrary().compareTo(LANGUAGE_BOOST) == 0))){
    			baseClass += ", public fsm::ILock";
    		}

    		writeClassDeclaration(contextClassName, baseClass);

    		//  public:
    		if(getLibrary().compareTo(LANGUAGE_QT) == 0){
    			pushIdentLevel();
    			getStream().println(getIdent() + "Q_OBJECT");
    			getStream().println("");
    			popIdentLevel();
    		}

    		getStream().println("public:");
    		pushIdentLevel();

    		writeContextConstructor(state, contextClassName, contextParentClassName);
    		writeContextDestructor(state, contextClassName);

    		writeContextGetStateNameToId(state);
    		
    		if((getLibrary().compareTo(LANGUAGE_QT) == 0)){
    			popIdentLevel();
    			getStream().println("public slots:");
    			pushIdentLevel();
    		}

    		writeContextEventDeclaration("");

    		if((getLibrary().compareTo(LANGUAGE_QT) == 0)){
    			popIdentLevel();
    			getStream().println("public:");
    			pushIdentLevel();
    		}

    		// Timer start and Timer stop only for root state.  
    		if((getStateMachine().getSettings().isAsynchronous() == true) && (state.isRoot() == true)){
    			writeTimersStartStop(state);
    		}            

    		writeContextgetParallel(state);

    		//void EnterInitialState();
    		getStream().println(getIdent()+ "// Enter the initial state: walk the onEntry chain from the top state to the initial state.");
    		getStream().println(getIdent() + "void " + ENTER_INITIAL_STATE + ";");
    		getStream().println();

    		//void LeaveCurrentState();
    		getStream().println(getIdent()+ "// Leave the current state: walk the onExit chain from the current state to the top state.");
    		getStream().println(getIdent() + "void " + LEAVE_CURRENT_STATE + ";");
    		getStream().println();

    		//bool EnterState(const char* pStateName);
    		if(getStateMachine().getSettings().isSerialize() == true){
    			getStream().println(getIdent()+ "// Enter the state given by its name");
    			getStream().println(getIdent() + "bool " + ENTER_STATE + "(const char* pStateName);");
    			getStream().println();
    		}
    		
    		if((getStateMachine().getSettings().isAsynchronous() == true) &&
    				(getStateMachine().getSettings().isSerialize() == true)){
    	 		//void PushBackEvent(int eventId);
        		getStream().println(getIdent()+ "// Create the event and push it back to the event queue");
        		getStream().println(getIdent() + "void " + PUSHBACK_EVENT + "(int eventId);");
        		getStream().println();
        	}
   
    		//void OnEnd(bool error) { emit end(error);};
    		//Only root state in qt shall invoke this function
    		if((getLibrary().compareTo(LANGUAGE_QT) == 0) && (state.isRoot() == true)){
    			getStream().println(getIdent() + "// Wrap OnEnd()"); 
    			getStream().println(getIdent() + "void " + 
    					ON_END + " {emit end();}");
    			getStream().println();
    		}  

    		//Object1& getObject1(){return m_object1;}
    		//Object2& getObject2(){return m_object2;}
    		writeObjectGetters();

    		popIdentLevel();

    		if((isLock() == true) && ((state.isRoot() == true)) &&
    				((getLibrary().compareTo(LANGUAGE_QT) == 0) || (getLibrary().compareTo(LANGUAGE_BOOST) == 0))){
    			// void Lock();
    			// void UnLock();

    			getStream().println(getIdent() + "protected:");
    			pushIdentLevel();

    			getStream().println(getIdent()+ "// Lock and Unlock, implement ILock ");
    			getStream().println(getIdent() + "void Lock();");
    			getStream().println(getIdent() + "void UnLock();");
    			getStream().println();

    			popIdentLevel();
    		}

    		//  private:
    		getStream().println(getIdent() + "private:");
    		pushIdentLevel();

    		// Object1& m_object1;
    		// Object2& m_object2;
    		writeObjectsMemberDeclaration();

    		// auto_ptr<Microwave02OperatingParallel> m_parallelOperating;
    		writeParallelMemberDeclaration(state);

    		// Only generate function with the sync suffix for asynchronous fsm and for the top context 
    		if((getStateMachine().getSettings().isAsynchronous() == true) && (state.isRoot() == true)){
    			writeContextEventDeclaration(SYNC_SUFFIX);
    			writeContextTimerVariableDeclaration(state);
    			writeContextTimerEventDeclaration(state);
    			//Qt only
    			writeTimerEventDeclaration(state);
    			//Boost only
    			writeProcessDeclaration(state);
    			//All
    			writeContextSchedule(state); 
    		}    

    		writeLockVariableDeclaration(state);

    		getStream().println();

    		popIdentLevel();

    		//Root Context only
    		if(state.isRoot() == true){
    			//Qt only
    			generateSignals(state);
    		}

    		getStream().println(getIdent() + "};");  
    		getStream().println("");  

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
    /**
     * Write the context constructor
     *   Qt:
     *     Context(Object1& object1, Object2& object2, QObject *pParent = NULL);
     *   Boost Async:
     *     Context(Object1& object1, Object2& object2, boost::asio::io_service &io_service);
     *   Boost Sync:
     *     Context(HelloWorld& object);
     *   StdCpp Async:
     *     Context(Object1& object1, Object2& object2, ISchedule &schedule, ContextParent *pContext);
     *   StdCpp Sync:
     *     Context(Object1& object1, Object2& object2, ISchedule &schedule, ContextParent *pContext);
     */
    
    private void writeContextConstructor(State state, String contextClassName, String contextParentClassName){
        getStream().println(getIdent() + "// Constructor");
        getStream().print(getIdent() + contextClassName + 
                            "(");

        writeObjectsParameterDeclaration();

        if((getLibrary().compareTo(LANGUAGE_QT) == 0) && (state.isRoot() == true)){
        	getStream().print(", QObject *pParent = NULL");
        }
        
        if(getStateMachine().getSettings().isAsynchronous() && (state.isRoot() == true)){
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
        
        getStream().println(");");
        getStream().println();        
    }
    
    private void writeContextDestructor(State state, String contextClassName){
        getStream().println(getIdent() + "// Destructor");
        getStream().println(getIdent() + "virtual ~" + contextClassName + "();");
        getStream().println();        
    }
    /**
     * Write event declaration
     */
    private void writeContextEventDeclaration(String functionSuffix){
        List<EventSource> eventProviderList = getStateMachine().getEvents().getEventSource();
        for(EventSource eventProvider : eventProviderList){
            List<Event> eventList =  eventProvider.getEvent();
            getStream().println(getIdent() + "// " + eventProvider.getName() + "'s events ");
            writeContextEventDeclaration(eventList,functionSuffix);        
        }
    }
    
    /**
     * Write timer start and stop declaration.
     * 
     * void TimerStartMyTimer1();
     * void TimerStopMyTimer1();
     * 
     * void TimerStartMyTimer2();
     * void TimerStopMyTimer2(); 
     */
    private void writeTimersStartStop(State state) throws StateBuilderException {
        List<Timer> timerList = getModel().getTimersAll();
        
        if(timerList.size() == 0){
            return;
        }
        
        //TODO add this when parsing the file to get the line number
        if(getStateMachine().getSettings().isAsynchronous() == false){
        	throw new StateBuilderException("A timer element is present but the Fsm is synchronous"
        			+ ", please add the element <asynchronous>true<asynchronous> to specify an asynchronous Fsm");
        }
        
        getStream().println(getIdent() + "// Timers declarations");
        
        for(Timer timer : timerList){
        	String timerName = timer.getName();
        	if(timerName.isEmpty() == true){
        		//TODO add this when parsing the file to get the line number
        		throw new StateBuilderException("timer 's name is empty for state " + state.getName());
        	}
            getStream().println(getIdent() + "void " + TIMERSTART + timerName + "(long duration);");
            getStream().println(getIdent() + "void " + TIMERSTOP + timerName + "();");
            getStream().println();
        }
        
        getStream().println();
    }
    
    /**
     * Write event member functions: 
     *     void EvEvent1(Param1 param1, Param3 *pParam3);
     *     void EvEvent2(Param2 param2, Param3 *pParam3);
     *     void EvEvent3(Param4 *pParam4);
     * @param eventList the list of events to generate
     * @param functionSuffix the suffix to add to the event function
     */
    private void writeContextEventDeclaration(List<Event> eventList, String functionSuffix){
        for(Event event : eventList){
            writeContextEventDeclaration(event,functionSuffix);
        }    
        
        getStream().println();
    }    
    
    /**
     * Write event member functions: 
     *     
     *     void EvEvent2(Param2 param2, Param3 *pParam3);
     *     
     * @param event the event to generate
     * @param functionSuffix the suffix to add to the event function
     */
    private void writeContextEventDeclaration(Event event, String functionSuffix){
        getStream().print(getIdent() + 
                "void " + 
                event.getId() + functionSuffix);
        getStream().print("(");
        writeParameterDeclaration(event);
        getStream().println(");");        
    }        
    
    /**
     * Write the context get function
     *   Microwave02EngineContext& GetContextEngine() {return m_contextEngine;}
     *   Microwave02DoorContext& GetContextDoor() {return m_contextDoor;}
     * @param state
     */
    private void writeContextGetContext(Parallel parallel) throws StateBuilderException {
    	List<State> stateOrthogonalList = parallel.getState();
    	for(State stateOrthogonal : stateOrthogonalList){
            String nameGetContext = getGetContext(stateOrthogonal);
            getStream().println(getIdent() + getModel().getContextClassName(stateOrthogonal) + "& " + nameGetContext + 
                    "() {return " + getContextMemberName(stateOrthogonal) + ";}");
    	} 
    }
    
    /**
     * Write the context get parallel
     * FsmNameParralelS1& getParallelS1() {return *m_pParallelS1;}
     * FsmNameParralelS3& getParallelS3() {return *m_pParallelS3;}
     * @param state
     */
    private void writeContextgetParallel(State state) throws StateBuilderException {
    	List<State> parallelList = state.getParallelList();
    	for(State parallel : parallelList){
    		String namegetParallel = getGetParallel(parallel);
    		getStream().println(getIdent() + getParallelClassName(parallel) + "& " + namegetParallel + 
    				"() {return *" + getParallelMemberName(parallel) + ";}");
    	}
    	getStream().println();

    }    
    
    /**
     * Write the context members declaration:
     * 
     *   UrlGetHttpContext m_contextHttp;
     *   UrlGetFtpContext m_contextFtp;
     *  
     *   UrlGetContext &context;
     *  
     * @param state the current state.
     * @param parallel the parallel state.
     */
    private void writeContextMemberDeclaration(State state, Parallel parallel) throws StateBuilderException {
    	List<State> stateOrthogonalList = parallel.getState();
    	for(State stateOrthogonal : stateOrthogonalList){
    		getStream().println(getIdent() + getModel().getContextClassName(stateOrthogonal) + " " + 
    				getContextMemberName(stateOrthogonal) + ";");
    	}
    	
    	getStream().println();
    	getStream().println(getIdent() + getModel().getContextClassName(state) + " &" + getModel().getContextInstance() + ";");
    }

    private void writeContextGetStateNameToId(State state)
    {
    	if(state.isRoot() == true){
    		getStream().println();
    		getStream().println(getIdent() + "static const fsm::StateNameToId* GetStateNameToId();");
    	}
    }
    /**
     * generate the parallel member declaration
     * std::auto_ptr<FsmNameParallelS1>  m_parallelS1;
     * std::auto_ptr<FsmNameParallelS2>  m_parallelS2;
     * @param parallel the parallel state
     */
    private void writeParallelMemberDeclaration(State state) throws StateBuilderException {
        List<State> parallelList = state.getParallelList();
        for(State parallel : parallelList){
            getStream().println(getIdent() + "std::auto_ptr<" + getParallelClassName(parallel) + "> " + 
                                getParallelMemberName(parallel) + ";");
        }
        
        getStream().println();
    }    
    /**
     * StdCpp and Qt
     * void evTimerMaxDuration();
     * 
     * Boost 
     * void evTimerConnected(const boost::system::error_code &error);
     * 
     * @throws StateBuilderException
     */
    private void writeContextTimerEventDeclaration(State state) throws StateBuilderException {
        getStream().println(getIdent() + "// Timer's event declarations"); 
        
        List<Timer> timerList = getModel().getTimersAll();
        
        if(timerList.size() == 0){
            return;
        }
        
        if(getLibrary().compareTo(LANGUAGE_QT) == 0){
        	getStream().println("private slots:");
        }
        
        for(Timer timer : timerList){
        	String timerEventId = timer.getId();
        	if(timerEventId.isEmpty() == true){
        		throw new StateBuilderException("state " + state.getName() + " has a timer with an empty id");
        	}
        	String param = "";
            if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
            	param = "const boost::system::error_code &error";
            }
            generateFunctionDeclaration("void", timerEventId, param);
            getStream().println();
        }
    }
    
    /**
     * QTimer *m_pTimerMaxDutation;
     * QTimer *m_pTimerConnected;
     * 
     * @throws StateBuilderException
     */
    private void writeContextTimerVariableDeclaration(State state) throws StateBuilderException {
    	getStream().println(getIdent() + "// Timer variables declaration");
    	
    	// For Boost in asynchronous mode, declare a timer to invoke the process() function 
        if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
    		getStream().println(getIdent() + "boost::asio::deadline_timer m_timerProcess;");
    	} else if(getLibrary().compareTo(LANGUAGE_LIBEV) == 0){
    		getStream().println(getIdent() + "ev::timer m_timerProcess;");
    	}
        
        List<Timer> timerList = getModel().getTimersAll();
        
        if(timerList.size() == 0){
            return;
        }
       
        for(Timer timer : timerList){
        	String timerName = timer.getName();
        	if(getLibrary().compareTo(LANGUAGE_QT) == 0){
        		getStream().println(getIdent() + "QTimer *m_pTimer" + timerName + ";");
        	} else if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
        		getStream().println(getIdent() + "boost::asio::deadline_timer m_timer" + timerName + ";");
        	} else if(getLibrary().compareTo(LANGUAGE_LIBEV) == 0){
        		getStream().println(getIdent() + "ev::timer m_timer" + timerName + ";");
        	}
        }
        
        getStream().println();
    }
    
    /**
     * Qt: #include <QMutex>;
     * Boost: #include <boost/thread/mutex.hpp>
     * Stdcpp: #include <fsm/ILock>
     * 
     * @throws StateBuilderException
     */
    private void generateLockInclude() throws StateBuilderException {
    	if(isLock() == false){
    		return;
    	}
 
        if(getLibrary().compareTo(LANGUAGE_QT) == 0){
            getStream().println(getIdent() + "#include <QtCore/QMutex>;");
            getStream().println(getIdent() + "#include <QtCore/QMutexLocker>");
        } else if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
           getStream().println(getIdent() + "#include <boost/thread/mutex.hpp>");  
        } else {
        }
  
        getStream().println();
    }
    /**
     * Qt: QMutex m_mutex;
     * Boost: boost::mutex m_mutex;
     * 
     * @throws StateBuilderException
     */
    private void writeLockVariableDeclaration(State state) throws StateBuilderException {
    	if(isLock() == false){
    		return;
    	}
    	getStream().println(getIdent() + "// Lock variables declaration");
        if(getLibrary().compareTo(LANGUAGE_QT) == 0){
    		getStream().println(getIdent() + "QMutex m_mutex;");
        } else if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
        	getStream().println(getIdent() + "boost::mutex m_mutex;");
        } else {
        }
  
        getStream().println();
    }
    /**
     * Write the class declaration:
     *   class className : public classBaseName {
     * @param className name of the class 
     * @param classBaseName name of the base class
     */
    private void writeClassDeclaration(String className, String classBaseName){
        getStream().println(getIdent() + "class " + 
                className + 
                " : " + 
                classBaseName + 
                " {");        
    }
    /**
     * Write the state contructor
     *  State(const char* pName);
     *  
     *  @param state the state 
     *  @throws StateBuilderException
     */
    
     private void writeStateConstructorDeclaration(State state) throws StateBuilderException{
         getStream().println(getIdent() + 
                 getStateClassName(state) +  
                 "(const char* pName, int id);");
         getStream().println();
     }
    
     /**
      * generate state destructor 
      *  virtual ~State();
      */
      private void writeStateDestructorDeclaration(String state){
          getStream().println(getIdent() + 
                  "virtual ~" + 
                  state +  
                  "();");
      }     
     
      /**
       * generate GetInstance:
       *  const State& GetInstance();
       */
       private void writeStateGetInstance(String state){
           getStream().println(getIdent() + "// Singleton pattern");
           getStream().println(getIdent() + "static const " + state + "& " + GETINSTANCE + ";");
           getStream().println();
       }
       
      
       /**
        * Write the function declaration for OnEntry and OnExit
        *  virtual void functionName(StateContext& context) const;
        * @param functionName
        * @param contextClassName the context's class name
        */
       private void writeOnEntryExitFunctionDeclaration(String functionName, String contextClassName){
    	   getStream().print(getIdent() + 
    			   "virtual void " +
    			   functionName + 
    	   "(");
    	   getStream().print(contextClassName + "& " + getModel().getContextInstance() + ") const");
       }

       /**
        * generate event member functions: 
        *     virtual void EvEvent2(StateContext& context, Param2 param2, Param3 *pParam3);
        *     virtual void EvEvent3(StateContext& context, Param4 *pParam4);
        * @param eventList the list of events to generate
        * @param contextClassName the context's class name
        * 
        */
       private void writeStateEventDeclaration(List<Event> eventList, String contextClassName){
    	   for(Event event : eventList){ 
    		   writeStateEventDeclaration(event, contextClassName);
    	   }                    
       }

    /**
     * Write the event member functions: 
     *     virtual void EvEvent2(StateContext &context, Param2 param2, Param3 *pParam3) const;
     *     
     * @param event the event to generate
     * @param contextClassName
     */
    private void writeStateEventDeclaration(Event event, String contextClassName){   
        String eventId = event.getId();
        getStream().print(getIdent() + 
                "virtual void " + 
                eventId);
        getStream().print("(");
        getStream().print(contextClassName + " &" + getModel().getContextInstance());
        
        // Add a comma if there is at least one parameter
        if(event.getParameter().isEmpty() == false){
            getStream().print(", ");
        }
        
        writeParameterDeclaration(event);
        getStream().println(") const ;");
    }   
    
    /**
     * Qt and Boost only, and asynchronous
     * virtual void Schedule(long);
     * @param state
     * @throws StateBuilderException
     */
    public void writeContextSchedule(State state) throws StateBuilderException {
    	if(getStateMachine().getSettings().isAsynchronous() == false){
    		return;
    	}
    	
        if((getLibrary().compareTo(LANGUAGE_QT) == 0) || getLibrary().compareTo(LANGUAGE_BOOST) == 0 || getLibrary().compareTo(LANGUAGE_LIBEV) == 0){
        	getStream().println(getIdent() + "// Overrides void ISchedule::" + SCHEDULE_FUNCTION + "long);");
        	getStream().println(getIdent() + "virtual void " + SCHEDULE_FUNCTION + "long);");
        	getStream().println("");
        }
    }     
    
    /**
     * Qt specific and asynchronous
     * virtual void timerEvent(QTimerEvent *pTimerEvent)
     * @param state
     * @throws StateBuilderException
     */
    public void writeTimerEventDeclaration(State state) throws StateBuilderException {
    	if(getStateMachine().getSettings().isAsynchronous() == false){
    		return;
    	}
    	
        if(getLibrary().compareTo(LANGUAGE_QT) == 0){
        	getStream().println(getIdent() + "// Overrides virtual void QObject::timerEvent(QTimerEvent *pTimerEvent)");
        	getStream().println(getIdent() + "virtual void timerEvent(QTimerEvent *pTimerEvent);");
        	getStream().println("");
        }
    }   

    /**
     * Boost specific
     * void process(const boost::system::error_code &error);
     * @param state
     * @throws StateBuilderException
     */
    public void writeProcessDeclaration(State state) throws StateBuilderException {
    	if(getStateMachine().getSettings().isAsynchronous() == false){
    		return;
    	}
    	
        if(getLibrary().compareTo(LANGUAGE_BOOST) == 0){
        	getStream().println(getIdent() + "// process is called by the io_service main loop");
        	getStream().println(getIdent() + "void process(const boost::system::error_code &error);");
        	getStream().println("");
        } else if(getLibrary().compareTo(LANGUAGE_LIBEV) == 0){
        	getStream().println(getIdent() + "// process is called by the libev loop");
        	getStream().println(getIdent() + "void process();");
        	getStream().println("");
        }
    }   
    
    /**
     * Qt only
     * signal:
     *   void end();
     * @param state
     * @throws StateBuilderException
     */
    public void generateSignals(State state) throws StateBuilderException{
        if(getLibrary().compareTo(LANGUAGE_QT) == 0){
        	getStream().println(getIdent() + "signals:");
        	pushIdentLevel();
        	getStream().println(getIdent() + "void end();");
        	popIdentLevel();
        	getStream().println("");
        }
    } 
    
    /**
     * generate a function declaration :
     *  returnType functionName(parameters);
     * @param returnType
     * @param functionName
     * @param parameters
     */
    private void generateFunctionDeclaration(String returnType, String functionName, String parameters){
        getStream().println(getIdent() + returnType + " " + functionName + "(" + parameters + ");");
    }
}
