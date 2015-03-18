package com.stateforge.statebuilder;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.List;

import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.ErrorHandler;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;

import com.stateforge.statebuilder.model.*;
import com.stateforge.utils.LocationRecordingHandler;
import com.stateforge.utils.XmlUtils;

public class StateMachineXmlModel implements ErrorHandler {
	
	public final String TAG_SETTINGS = "settings";
	public final String TAG_ASYNCHRONOUS = "asynchronous";
	public final String TAG_SERIALIZE = "serialize";
	public final String TAG_LIBRARY = "library";
	public final String TAG_OBSERVER = "observer";
	public final String TAG_NAMESPACE = "namespace";
	public final String TAG_DESCRIPTION = "description";
	public final String TAG_CONTEXT = "context";
	public final String TAG_INSTANCE = "instance";
	public final String TAG_CLASS = "class";
	public final String TAG_INCLUDE = "include";
	public final String TAG_OBJECT = "object";
    public final String TAG_NAME = "name";
    public final String TAG_EVENT_ID = "id";
    public final String TAG_PARAMETER = "parameter";
    public final String TAG_TYPE = "type";
    public final String TAG_PASSEDBY = "passedBy";
    public final String TAG_CONST = "const";
    public final String TAG_FINAL = "final";
    public final String TAG_POINTER = "pointer";
    public final String TAG_REFERENCE = "reference";
    public final String TAG_STATE = "state";
    public final String TAG_ONENTRY = "onEntry";
    public final String TAG_ONEXIT = "onExit";
    public final String TAG_STATE_NAME = "name";
    public final String TAG_NEXT_STATE = "nextState";
    public final String TAG_EVENTS = "events";
    public final String TAG_EVENT_SOURCE = "eventSource";
    public final String TAG_EVENT_INTERFACE = "interface";
    public final String TAG_EVENT_FILE = "file";
    public final String TAG_EVENT = "event";
    public final String TAG_TRANSITION = "transition";
    public final String TAG_CONDITION = "condition";
    public final String TAG_KIND = "kind";
    public final String TAG_TIMER = "timer";
    public final String TAG_ID = "id";
    public final String TAG_TIMER_START = "timerStart";
    public final String TAG_TIMER_STOP = "timerStop";
    public final String TAG_DURATION = "duration";
    public final String TAG_ACTION = "action";
    public final String TAG_PARALLEL = "parallel";
    public final String TAG_FEEDER = "feeder";

    public final String ERROR_TRANSITION_ORDER = "S1001";
    public final String ERROR_TRANSITION_DUPLICATED = "S1002";
    public final String ERROR_EVENT_DOES_NOT_EXIST = "S1003";
    public final String ERROR_NEXT_STATE_DOES_NOT_EXIST = "S1004";
    public final String ERROR_COMPOSITE_AND_FINAL = "S1005";
    public final String ERROR_XSD_VALIDATION = "S1006";
    public final String ERROR_CONDITION_DUPLICATED = "S1007";
    public final String ERROR_SYNCHRONOUS_CANNOT_HAVE_TIMERS = "S1008";
    public final String ERROR_TIMER_DOES_NOT_EXIST = "S1009";
    public final String ERROR_STATE_DUPLICATED = "S1010";
    public final String ERROR_INTERNAL = "S1011";
    public final String ERROR_FILE_NOT_FOUND = "S1012";
    public final String ERROR_EVENT_INTERFACE_NOT_FOUND_IN_FILE = "S1013";
    public final String ERROR_EVENT_INTERFACE_INVALID = "S1014";
    public final String ERROR_EVENT_DUPLICATED = "S1015";
    public final String ERROR_TIMER_DUPLICATED = "S1016";
    public final String ERROR_SERIALIZED_AND_TIMERS = "S1017";
    public final String ERROR_SERIALIZED_AND_EVENT_WITH_PARAMETER = "S1018";
    
	//private String inputFilename;
	private File inputFile;
	private String name;
	private String schemaName;
	HashMap<String, String> stateMap = new HashMap<String, String>();
	
	private boolean m_bValidationError;
	private InputStream xmlInputStream;

    public StateMachineXmlModel(InputStream xmlInputStream, String inputFileName, String name, String schemaName)
    {
    	this.xmlInputStream = xmlInputStream;
    	//this.inputFilename = inputFileName;
    	this.inputFile = new File(inputFileName);
    	this.name = name;
    	this.schemaName = schemaName;
    }
    
    public StateMachineModel build() throws ParserConfigurationException, SAXException, IOException, StateBuilderException
    {
    	Document fsmDom = createDom(this.xmlInputStream);
    	
    	StateMachineModel model = createStateMachineModel(fsmDom);
    	validateModel(model, fsmDom);
    	
    	return model;
    }

	/// Validate the XML input file according to its XSD.
    /// Errors are reported through SchemaValidationEventHandler
    protected Document createDom(InputStream xmlInputStream) throws ParserConfigurationException, SAXException, IOException
    {
		InputStream schemaInputStream = getClass().getClassLoader().getResourceAsStream(schemaName);
		
		// Create the DOM document from the XML instance
		Document fsmDom = XmlUtils.documentCreate(xmlInputStream, schemaInputStream, this);
		
		if (m_bValidationError == true) {
			throw new ParserConfigurationException();
		}
		return fsmDom;
    }
    
    /// Create and fill the State Machine object from its Document Object Model.
    private StateMachineModel createStateMachineModel(Document fsmDom) throws StateBuilderException
    {
    	StateMachineModel model = new StateMachineModel();
    	
    	Element stateMachineElement = fsmDom.getDocumentElement();
    	
		Node nodeSibling = stateMachineElement.getFirstChild();
		while(nodeSibling != null){ 
			if(Node.ELEMENT_NODE == nodeSibling.getNodeType()){
				if(nodeSibling.getNodeName().compareTo(TAG_SETTINGS) == 0){
			    	createSettings(model, (Element)nodeSibling);
    			} else if(nodeSibling.getNodeName().compareTo(TAG_EVENTS) == 0){
    				createEvents(model, (Element)nodeSibling);
    			} else if(nodeSibling.getNodeName().compareTo(TAG_STATE) == 0){
    				createState(model, null, null, (Element)nodeSibling);
    			}		
			}	
			
			nodeSibling = nodeSibling.getNextSibling();
		}
    
    	return model;
    }

    /// Create and fill the settings object from its element.
    private void createSettings(StateMachineModel model, Element settingsElement)
    {
    	Settings settings = new Settings();
    	model.getStateMachine().setSettings(settings);
    	
    	String name = settingsElement.getAttribute(TAG_NAME);
    	if(name.isEmpty() == true){
    		settings.setName(this.getName());
    	} else {
    		settings.setName(name);	
    	}
    	
    	settings.setAsynchronous(Boolean.parseBoolean(settingsElement.getAttribute(TAG_ASYNCHRONOUS)));
    	
    	settings.setSerialize(Boolean.parseBoolean(settingsElement.getAttribute(TAG_SERIALIZE)));
    	
    	try {
    	    settings.setLibrary(Library.fromValue(settingsElement.getAttribute(TAG_LIBRARY)));
    	} catch (IllegalArgumentException e){
    		
    	}
    	
    	try {
    	    settings.setObserver(Observer.fromValue(settingsElement.getAttribute(TAG_OBSERVER)));
    	} catch (IllegalArgumentException e){
    		settings.setObserver(Observer.STRING);
    	}
    	
    	settings.setNamespace(settingsElement.getAttribute(TAG_NAMESPACE));
    	settings.setDescription(settingsElement.getAttribute(TAG_DESCRIPTION));
    	Node nodeSibling = settingsElement.getFirstChild();
    	while(nodeSibling != null){ 
    		if(Node.ELEMENT_NODE == nodeSibling.getNodeType()){
    			if(nodeSibling.getNodeName().compareTo(TAG_CONTEXT) == 0){
    				createContext(settings, (Element)nodeSibling);
    			} else if(nodeSibling.getNodeName().compareTo(TAG_OBJECT) == 0){
    				createObject(settings, (Element)nodeSibling);
    			} else if(nodeSibling.getNodeName().compareTo(TAG_INCLUDE) == 0){
    				settings.getInclude().add(nodeSibling.getTextContent());
    			}	
    		}
    		
    		nodeSibling = nodeSibling.getNextSibling();
    	}

    	
    }

    private void createObject(Settings settings, Element objectElement)
    {
    	ObjectType object = new ObjectType();
    	object.setInstance(objectElement.getAttribute(TAG_INSTANCE));
    	object.setClazz(objectElement.getAttribute(TAG_CLASS));
    	object.setInclude(objectElement.getAttribute(TAG_INCLUDE));
    	//Java specific
    	if(objectElement.getAttribute(TAG_NAMESPACE).isEmpty() == false){
    	    object.setInclude(objectElement.getAttribute(TAG_NAMESPACE));
    	}
    	settings.getObject().add(object);
    }

    private void createContext(Settings settings, Element contextElement)
    {
    	Context context = new Context();
    	context.setInstance(contextElement.getAttribute(TAG_INSTANCE));
    	context.setClazz(contextElement.getAttribute(TAG_CLASS));
    	settings.setContext(context);
    }
    
    private void createEvents(StateMachineModel model, Element eventsElement) throws StateBuilderException {
		Events events = new Events();
		NodeList nodeList = eventsElement.getElementsByTagName(TAG_EVENT_SOURCE);
		
		for(int i = 0; i < nodeList.getLength(); i++){
			createEventSource(model, events, (Element)nodeList.item(i));
		}
		
		model.getStateMachine().setEvents(events);
	}
   
    private void createEventSource(
    		StateMachineModel model,
    		Events events,
    		Element eventSourceElement) throws StateBuilderException {
    	EventSource eventSource = new EventSource();
    	eventSource.setName(eventSourceElement.getAttribute(TAG_NAME));
    	eventSource.setDescription(eventSourceElement.getAttribute(TAG_DESCRIPTION));
    	
    	Node nodeSibling = eventSourceElement.getFirstChild();
    	while(nodeSibling != null){ 
    		if(Node.ELEMENT_NODE == nodeSibling.getNodeType()){
    			if(nodeSibling.getNodeName().compareTo(TAG_EVENT) == 0){
    				createEvent(model, eventSource, (Element)nodeSibling);
    			} else if(nodeSibling.getNodeName().compareTo(TAG_TIMER) == 0){
    				if(model.getStateMachine().getSettings().isSerialize() == true){
    					reportError((Element)nodeSibling, ERROR_SERIALIZED_AND_TIMERS, "serialize is not compatible with timer");
    				}
    				createTimer(model, eventSource, (Element)nodeSibling);
    			}	
    		}
    		nodeSibling = nodeSibling.getNextSibling();
    	}
    	
    	events.getEventSource().add(eventSource);
	}    
    
	private void createEvent(
			StateMachineModel model, 
			EventSource eventSource,
			Element eventElement) throws StateBuilderException {
		Event event = Event.create();
		event.setId(eventElement.getAttribute(TAG_ID));
		event.setName(eventElement.getAttribute(TAG_NAME));
		event.setDescription(eventElement.getAttribute(TAG_DESCRIPTION));

		if(model.addEvent(event) == true){
			eventSource.getEvent().add(event);
		} else {
			reportError(
		            eventElement,
					ERROR_EVENT_DUPLICATED,
					"Duplicated event id " + event.getId());
		}
		
    	Node nodeSibling = eventElement.getFirstChild();
    	
    	while(nodeSibling != null){ 
    		if(Node.ELEMENT_NODE == nodeSibling.getNodeType()){
    			if(nodeSibling.getNodeName().compareTo(TAG_PARAMETER) == 0){
    				//only parameterless event has numberId because it can be serialized, 
    				if(model.getStateMachine().getSettings().isSerialize() == true){
    					reportError((Element)nodeSibling,
    							ERROR_SERIALIZED_AND_EVENT_WITH_PARAMETER, 
    							"serialize is not compatible with event with parameter");
    				}
    				event.setNumberId(0);
    				createParameter(model, event, (Element)nodeSibling);
    			}
    		}
    		
    		nodeSibling = nodeSibling.getNextSibling();
    	}	
	}

	private void createParameter(StateMachineModel model, Event event, Element parameterElement) {
		Parameter parameter = new Parameter();
		parameter.setName(parameterElement.getAttribute(TAG_NAME));
		parameter.setType(parameterElement.getAttribute(TAG_TYPE));
		String passedBy = parameterElement.getAttribute(TAG_PASSEDBY);
		if(passedBy.isEmpty() == false){
		    parameter.setPassedBy(PassedBy.fromValue(passedBy));
		}
	
		//C++
		parameter.setConst(Boolean.parseBoolean(parameterElement.getAttribute(TAG_CONST)));
		//Java
		parameter.setFinal(Boolean.parseBoolean(parameterElement.getAttribute(TAG_FINAL)));
		
		event.getParameter().add(parameter);
	}

	private void createTimer(StateMachineModel model, EventSource eventSource, Element timerElement) throws StateBuilderException {
		if(model.getStateMachine().getSettings().isAsynchronous() == false){
			reportError(
					timerElement,
					ERROR_SYNCHRONOUS_CANNOT_HAVE_TIMERS,
					"Synchronous state machine cannot have timers");
		}
		
		Timer timer = new Timer();
		timer.setId(timerElement.getAttribute(TAG_ID));
		timer.setName(timerElement.getAttribute(TAG_NAME));
		timer.setDescription(timerElement.getAttribute(TAG_DESCRIPTION));

		if(model.addTimer(timer) == true){
			eventSource.getTimer().add(timer);
		} else {
			reportError(
					timerElement,
					ERROR_EVENT_DUPLICATED,
					"Duplicated timer id " + timer.getId());
		}
	}

	private void createState(StateMachineModel model, State stateParent, State stateParallel, Element stateElement) throws StateBuilderException {
    	State state = new State();
    	
    	state.setParent(stateParent);
    	state.setStateParallel(stateParallel);
    	
    	// Attributes
    	state.setName(stateElement.getAttribute(TAG_NAME));
    	state.setDescription(stateElement.getAttribute(TAG_DESCRIPTION));
    	
    	if(stateParent == null){
    		state.setTop(true);
    		if(stateParallel == null){
    			model.getStateMachine().setState(state);
    			state.setRoot(true);
    		}
    	} else {
    		stateParent.getState().add(state);
    	}
    	
	   	if((stateParent == null) && (stateParallel != null) && (stateParallel.getParallel() != null)){
	   		stateParallel.getParallel().getState().add(state);
    	}

    	
    	String kind = stateElement.getAttribute(TAG_KIND);
    	if(kind.isEmpty() == false){
    		state.setKind(StateKind.fromValue(kind));
    		
    		if(state.getKind() == StateKind.HISTORY){
    			stateParent.setHasHistory(true);
    		}  
    	}

		if(model.addState(state) == false){
			reportError(
					stateElement,
					ERROR_STATE_DUPLICATED,
					"Duplicated state " + state.getName());
		}
		
		// Elements
		Node nodeSibling = stateElement.getFirstChild();
		while(nodeSibling != null){ 
			if(Node.ELEMENT_NODE == nodeSibling.getNodeType()){
				if(nodeSibling.getNodeName().compareTo(TAG_STATE) == 0){
					createState(model, state, stateParallel, (Element)nodeSibling);
    			} else if(nodeSibling.getNodeName().compareTo(TAG_PARALLEL) == 0){
    				createParallel(model, state, (Element)nodeSibling);
    			} else if(nodeSibling.getNodeName().compareTo(TAG_ONENTRY) == 0){
    				createOnEntryExit(model, state, (Element)nodeSibling, TAG_ONENTRY);
    			} else if(nodeSibling.getNodeName().compareTo(TAG_ONEXIT) == 0){
    				createOnEntryExit(model, state, (Element)nodeSibling, TAG_ONEXIT);
    			} else if(nodeSibling.getNodeName().compareTo(TAG_TRANSITION) == 0){
    				createTransition(model, state, (Element)nodeSibling);
    			}
			}
			
			nodeSibling = nodeSibling.getNextSibling();
		}
		
		if(state.getState().size() == 0){
			state.setLeaf(true);
		} 
		
		
	} 
    

	
	private void createParallel(StateMachineModel model, State state, Element parallelElement) throws StateBuilderException{
		Parallel parallel = new Parallel();
		
		state.setParallel(parallel);
		
		String nextStateName = parallelElement.getAttribute(TAG_NEXT_STATE);
         
		parallel.setNextState(nextStateName);
		
		State stateTop = model.getStateTop(state);
        stateTop.getParallelList().add(state);
        
		// Elements
		Node nodeSibling = parallelElement.getFirstChild();
		while(nodeSibling != null){ 
			if(Node.ELEMENT_NODE == nodeSibling.getNodeType()){
				if(nodeSibling.getNodeName().compareTo(TAG_STATE) == 0){
					createState(model, null, state, (Element)nodeSibling);
    			} 
			}
			
			nodeSibling = nodeSibling.getNextSibling();
		}
	}
	
	private void createTransition(StateMachineModel model, State state, Element transitionElement) throws StateBuilderException {
			Transition transition = new Transition();
			String eventId = transitionElement.getAttribute(TAG_EVENT);
			Event event = model.getEventFromId(eventId);
			if(event == null){
				reportError(
						transitionElement,
						ERROR_EVENT_DOES_NOT_EXIST,
						"Event id " + eventId + " does not exist");
			}
			
			transition.setEvent(event);
			transition.setAction(transitionElement.getAttribute(TAG_ACTION));
			transition.setNextState(transitionElement.getAttribute(TAG_NEXT_STATE));
			transition.setConditionElement(transitionElement.getAttribute(TAG_CONDITION));
			transition.setCondition("");
			
			Node nodeSibling = transitionElement.getFirstChild();
			while(nodeSibling != null){ 
				if(Node.ELEMENT_NODE == nodeSibling.getNodeType()){
					if(nodeSibling.getNodeName().compareTo(TAG_ACTION) == 0){
						processAction(transition.getActionGroup(), (Element)nodeSibling);
					} else if(nodeSibling.getNodeName().compareTo(TAG_TIMER_START) == 0){
						parserTimerStart(model, transition.getActionGroup(), (Element)nodeSibling);
					} else if(nodeSibling.getNodeName().compareTo(TAG_TIMER_STOP) == 0){
						parserTimerStop(model, transition.getActionGroup(), (Element)nodeSibling);
					} else if(nodeSibling.getNodeName().compareTo(TAG_CONDITION) == 0){
						transition.setCondition(nodeSibling.getTextContent());
					} 	
				}
				
				nodeSibling = nodeSibling.getNextSibling();
			}			
			
			model.addTransitionNameToMap(transition);
			// Check for duplicated condition
			if((transition.getConditionElement().isEmpty() == false) && 
		       (transition.getCondition().isEmpty() == false)){
				reportError(
						transitionElement,
						ERROR_CONDITION_DUPLICATED,
						"condition must be either as attribute or element, not both");	
			}
			
			// Add transition to state
			if(model.hasTransition(state, transition) == true){
				reportError(
						transitionElement,
						ERROR_TRANSITION_DUPLICATED,
						"Duplicated transition");				
			} else {
				state.getTransition().add(transition);				
			}
			
	}

	private void processAction(List<Object> actionGroup,  Element actionElement) {
		String actionContent = actionElement.getTextContent();
		if(actionContent.isEmpty() == false){
		    actionGroup.add(actionElement.getTextContent());
		}
	}

	private void createOnEntryExit(StateMachineModel model, State state, Element onEntryExitElement, String tag) throws StateBuilderException{
		Actions action = new Actions();
		action.setAction(onEntryExitElement.getAttribute(TAG_ACTION));

		Node nodeSibling = onEntryExitElement.getFirstChild();
		while(nodeSibling != null){ 
			if(Node.ELEMENT_NODE == nodeSibling.getNodeType()){
				if(nodeSibling.getNodeName().compareTo(TAG_ACTION) == 0){
					processAction(action.getActionGroup(), (Element)nodeSibling);
				} else if(nodeSibling.getNodeName().compareTo(TAG_TIMER_START) == 0){
					parserTimerStart(model, action.getActionGroup(), (Element)nodeSibling);
				} else if(nodeSibling.getNodeName().compareTo(TAG_TIMER_STOP) == 0){
					parserTimerStop(model, action.getActionGroup(), (Element)nodeSibling);
				} 	
			}
			
			nodeSibling = nodeSibling.getNextSibling();
		}

		if(tag.equals(TAG_ONENTRY)){
			state.setOnEntry(action);
		} else if(tag.equals(TAG_ONEXIT)){
			state.setOnExit(action); 	
		} 
	}

	private void parserTimerStart(StateMachineModel model, List<Object> actionList, Element element) throws StateBuilderException {
		TimerStartAction timerStartAction = new TimerStartAction();
		timerStartAction.setTimer(element.getAttribute(TAG_TIMER));
		timerStartAction.setDuration(element.getAttribute(TAG_DURATION));
		actionList.add(timerStartAction);
		
		Timer timer = model.getTimerFromName(timerStartAction.getTimer());
		if(timer == null){
			reportError(
					element,
					ERROR_TIMER_DOES_NOT_EXIST,
					"Timer " + timerStartAction.getTimer() + " does not exist");			
		}
	}

	private void parserTimerStop(StateMachineModel model, List<Object> actionList, Element element) throws StateBuilderException {
		TimerStopAction timerStopAction = new TimerStopAction();
		timerStopAction.setTimer(element.getAttribute(TAG_TIMER));
		actionList.add(timerStopAction);
		
		Timer timer = model.getTimerFromName(timerStopAction.getTimer());
		if(timer == null){
			reportError(
					element,
					ERROR_TIMER_DOES_NOT_EXIST,
					"Timer " + timerStopAction.getTimer() + " does not exist");			
		}		
	}
	
    private void validateModel(StateMachineModel model, Document fsmDom) throws StateBuilderException {
    	Element stateMachineElement = fsmDom.getDocumentElement();
    	
		Node nodeSibling = stateMachineElement.getFirstChild();
		while(nodeSibling != null){ 
			if(Node.ELEMENT_NODE == nodeSibling.getNodeType()){
				if(nodeSibling.getNodeName().compareTo(TAG_STATE) == 0){
    				validateState(model, null, null, (Element)nodeSibling);
    			}		
			}	
			
			nodeSibling = nodeSibling.getNextSibling();
		}
	}
    
	private void validateState(StateMachineModel model, State stateParent, State stateParallel, Element stateElement) throws StateBuilderException
	{
		State state = model.getState(stateElement.getAttribute(TAG_NAME));
		if((state.getState().size() > 0) && (state.getKind() == StateKind.FINAL)){
			reportError(
					stateElement,
					ERROR_COMPOSITE_AND_FINAL,
					"Final state " + state.getName() + " cannot have child state");	
		}
		
		Node nodeSibling = stateElement.getFirstChild();
		while(nodeSibling != null){ 
			if(Node.ELEMENT_NODE == nodeSibling.getNodeType()){
				if(nodeSibling.getNodeName().compareTo(TAG_STATE) == 0){
					validateState(model, state, stateParallel, (Element)nodeSibling);
    			} else if(nodeSibling.getNodeName().compareTo(TAG_TRANSITION) == 0){
    				validateTransition(model, state, (Element)nodeSibling);
    			} else if(nodeSibling.getNodeName().compareTo(TAG_PARALLEL) == 0){
    				validateParallel(model, state, (Element)nodeSibling);
    			}
			}
			
			nodeSibling = nodeSibling.getNextSibling();
		}
	}

	private void validateParallel(StateMachineModel model, State state, Element parallelElement) throws StateBuilderException 
	{	
		String nextStateName = parallelElement.getAttribute(TAG_NEXT_STATE);
        
		if((nextStateName.isEmpty() == false) && (model.getState(nextStateName) == null)){
			reportError(
					parallelElement,
					ERROR_NEXT_STATE_DOES_NOT_EXIST,
			"Next state " + nextStateName + " does not exist");
		}
		
		Node nodeSibling = parallelElement.getFirstChild();
		while(nodeSibling != null){ 
			if(Node.ELEMENT_NODE == nodeSibling.getNodeType()){
				if(nodeSibling.getNodeName().compareTo(TAG_STATE) == 0){
					validateState(model, null, state, (Element)nodeSibling);
    			} 
			}
			
			nodeSibling = nodeSibling.getNextSibling();
		}
	}

	private void validateTransition(StateMachineModel model, State state, Element transitionElement) throws StateBuilderException
	{
		String nextStateName = transitionElement.getAttribute(TAG_NEXT_STATE);
        
		if((nextStateName.isEmpty() == false) && (model.getState(nextStateName) == null)){
			reportError(
					transitionElement,
					ERROR_NEXT_STATE_DOES_NOT_EXIST,
			"Next state " + nextStateName + " does not exist");
		}
		
		int transitionNumber = 0;
		 
		List<Transition> transitions = model.getTransitionList(state, model.getEventFromId(transitionElement.getAttribute(TAG_EVENT)));
		int numTransition = transitions.size();
		if(numTransition >= 2){
			for(Transition transition : transitions){
				transitionNumber++;
				if((model.getCondition(transition).isEmpty() == true) && (transitionNumber < numTransition)){
					reportError(
							transitionElement,
							ERROR_TRANSITION_ORDER,
							"transition without condition must be the last one");	
				}
			}
		}
	}
	
	public void warning(SAXParseException exception) throws SAXException {
		System.out.println(this.inputFile.getAbsoluteFile() + "("
				+ exception.getLineNumber() + "," + exception.getColumnNumber()
				+ ") : " + " warning S1000: " + exception.getMessage());
	}

	/**
	 * Callback when the file is not valid according to its schema file
	 */
	public void error(SAXParseException exception) throws SAXException {
		System.out.println(this.inputFile.getAbsoluteFile() + "("
				+ exception.getLineNumber() + "," + exception.getColumnNumber()
				+ ") : " + " error S1001: " + exception.getMessage());
		m_bValidationError = true;
	}

	/**
	 * Callback when the file is not well formed
	 */
	public void fatalError(SAXParseException exception) throws SAXException {
		System.out.println(this.inputFile.getAbsoluteFile() + "("
				+ exception.getLineNumber() + "," + exception.getColumnNumber()
				+ ") : " + " error S1002: " + exception.getMessage());
	}

    private void reportError(Element element, String errorCode, String error) throws StateBuilderException
    {	 
        System.out.println(this.inputFile.getAbsoluteFile() + "("
				+ element.getUserData(LocationRecordingHandler.KEY_LINE_NO).toString() + "," 
				+ element.getUserData(LocationRecordingHandler.KEY_COLUMN_NO).toString()
				+ ") : " + " error " + errorCode + ": " + error);
        
        throw new StateBuilderException(errorCode, error);
    }
    
    
	public void setName(String name) {
		this.name = name;
	}

	public String getName() {
		return name;
	}

}
