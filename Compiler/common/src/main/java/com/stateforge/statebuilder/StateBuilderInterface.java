package com.stateforge.statebuilder;

import java.io.PrintStream;
import java.util.logging.Level;
import java.util.logging.Logger;

import com.stateforge.statebuilder.StateBuilderException;
import com.stateforge.statebuilder.model.StateMachine;
import com.stateforge.statebuilder.model.StateMachineModel;


public abstract class StateBuilderInterface {
    //Variable name
    //private String m_contextInstance = "context";
    public static final String STATE = "state";
    public static final String OBSERVER = "observer";
    public static final String PARALLEL = "parallel";
    
    //Class name
    public static final String STATE_CLASSNAME = "State";
    public static final String CONTEXTBASE_CLASSNAME = "Context<TState, TContextParent>";
    public static final String CONTEXT_CLASSNAME = "Context";
    public static final String CONTEXT_INSTANCENAME = "context";
    public static final String CONTEXTASYNC_CLASSNAME = "ContextAsync";        
    public static final String PARALLEL_CLASSNAME = "Parallel";
    public static final String IPARALLEL_CLASSNAME = "IParallel";
    public static final String TOPSTATE = "TopState";

    //Getter and setter prefix
    public static final String GETTER_PREFIX = "get";
    public static final String SETTER_PREFIX = "set";

    //All finite state machine tags
    protected final static String FSM_SUFFIX = "Fsm";
    protected static final String PARALLEL_TAG = "parallel";
    protected static final String STATE_TAG = "state";
    protected static final String ONENTRY_TAG = "onEntry";
    protected static final String ONEXIT_TAG = "onExit";
    protected static final String NAME_TAG = "name";
    protected static final String ID_TAG = "id";
    protected static final String TRANSITION_TAG = "transition";
    protected static final String EVENT_TAG = "event";
    protected static final String EVENTID_TAG = "id";
    protected static final String ACTIONLIST_TAG = "actionList";
    protected static final String ACTION_TAG = "action";
    protected static final String GUARDCONDITION_TAG = "guardCondition";
    protected static final String TIMER_TAG = "timer";
    
    protected Logger m_logger;
    protected String m_version;
    protected String m_name;
    protected String outputDirectory;
	protected StateMachine m_stateMachine;
    private StateMachineModel model;
    protected PrintStream m_stream;
    protected String m_ident;
    protected int m_identLevel;
    protected int m_identLength;
    protected boolean m_bObserver;
    protected boolean m_bLock;
    protected String m_library;
    protected String[] commandLine;
    


    protected StateBuilderInterface(StateMachineModel model) {
        m_logger = Logger.getLogger(getClass().getName());
        m_logger.setLevel(Level.OFF);         
      
        setIdent("");
        setIdentLength(2);
        m_identLevel = 0;  
        m_bObserver = true;
        setModel(model);
        setStateMachine(model.getStateMachine()); 
        setName(model.getStateMachine().getSettings().getName());
    }
    
    public String getOutputDirectory() {
		return outputDirectory;
	}
    
	public void setOutputDirectory(String outputDirectory) {
		this.outputDirectory = outputDirectory;
	}
	
    /**
     * generate the code 

     * @throws StateBuilderException
     */
    public abstract void generate() throws StateBuilderException;
    
    /**
     * get the file extension, i.e .cpp, .h, .java etc...
     * @return the file extension 
     */
    public abstract String getFileExtension();

    
    public String getVersion() {
		return m_version;
	}
    
	public void setVersion(String version) {
		this.m_version = version;
	}
	/**
     * @return Returns the stream.
     */
    public PrintStream getStream() {
        return m_stream;
    }

    /**
     * @param stream The stream to set.
     */
    public void setStream(PrintStream stream) {
        this.m_stream = stream;
    }

    /**
     * @return true if the compiler generate code for lock/unlock
     */
    public boolean isLock() {
        return m_bLock;
    }
    /**
     * @param lock the additional include to set
     */
    public void setLock(boolean lock) {
        this.m_bLock = lock;
    }
    /**
     * set parameters
     * @param name the name
     * @param fsmDom the dom document
     * @param stream the code is written from this stream
     */    
    
    protected void setParam(String name, 
    						StateMachineModel model,
                            PrintStream stream){
        setName(name);
        setStream(stream);        
    }

    /**
     * @return Returns the fsm.
     */
    protected StateMachine getStateMachine() {
        return m_stateMachine;
    }

    /**
     * @param fsm The fsm to set.
     */
    protected void setStateMachine(StateMachine fsm) {
        this.m_stateMachine = fsm;
    }

    /**
     * @return Returns the name.
     */
    protected String getName() {
        return m_name;
    }

    /**
     * @param name The name to set.
     */
    protected void setName(String name) {
        this.m_name = name;
    }
    
    public String getLibrary() {
		return m_library;
	}
    
	public void setLibrary(String library) {
		this.m_library = library;
	}
	
    /**
     * @return Returns the indentation.
     */
    protected String getIdent() {
        return m_ident;
    }
    /**
     * @param ident The indentation to set.
     */
    protected void setIdent(String ident) {
        this.m_ident = ident;
    }
    /**
     * @return Returns the indentation level.
     */
    protected int getIdentLevel() {
        return m_identLevel;
    }
    /**
     * @param identLevel The indentation level to set.
     */
    protected void setIdentLevel(int identLevel) throws StateBuilderException {
        if(identLevel < 0){
            throw new StateBuilderException("Indentation");
        }
        this.m_identLevel = identLevel;
        String space = " ";
        String ident = "";
        setIdent("");
        for(int i = 0; i < getIdentLength(); i++){
            ident += space;
        }        
        for(int i = 0; i < identLevel; i++){
            m_ident += ident;
        }
    }
    
    /**
     * push the ident level
     */
    protected void pushIdentLevel() throws StateBuilderException {
        setIdentLevel(getIdentLevel() + 1);
    }
    
    /**
     * pop the ident level
     *
     */
    protected void popIdentLevel() throws StateBuilderException {
        setIdentLevel(getIdentLevel() - 1);
    }    
    
    /**
     * @return Returns the indentation length.
     */
    protected int getIdentLength() {
        return m_identLength;
    }
    
    /**
     * @param identLength The indentation length to set.
     */
    protected void setIdentLength(int identLength) {
        this.m_identLength = identLength;
    }

   	public void setModel(StateMachineModel model) {
		this.model = model;
	}
   	
	public StateMachineModel getModel() {
		return model;
	}
	
    public String[] getCommandLine() {
        return commandLine;
    }

    public void setCommandLine(String[] commandLine) {
        this.commandLine = commandLine;
    }
}
