package com.stateforge.statebuilder.model;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import com.stateforge.statebuilder.StateBuilderException;


public class StateMachineModel {
    private HashMap<String , State> stateMap = new HashMap<String, State>();
    private HashMap<String , Event> eventMap = new HashMap<String, Event>();
    private HashMap<String , Timer> timerMap = new HashMap<String, Timer>();
    private Map<String , Integer> transitionMap = new LinkedHashMap<String, Integer>();
    
    public Map<String, Integer> getTransitionMap() {
		return transitionMap;
	}

	private ArrayList<Event> events = new ArrayList<Event>();
    private ArrayList<Timer> timers = new ArrayList<Timer>();
    
    private StateMachine stateMachine = new StateMachine();
    
	public StateMachine getStateMachine() {
		return stateMachine;
	}
	
    public boolean addEvent(Event event)
    {
    	if(this.eventMap.containsKey(event.getId()) == true){
    		return false;
    	} else {
        	this.eventMap.put(event.getId(), event);
            this.events.add(event);	
    		return true;
    	}
    }
    
    public boolean addTimer(Timer timer)
    {
    	if(addEvent(timer) == false){
    		return false;
    	} else {
        	this.timerMap.put(timer.getName(), timer);
            this.timers.add(timer);	
    		return true;
    	}
    }    
    
	public Event getEventFromId(String eventId) {
		if(eventId == null){
			return null;
		}		
		
		if(this.eventMap.containsKey(eventId) == true){
			return this.eventMap.get(eventId);
		} else {
			return null;
		}	
	}
	
    public Timer getTimerFromName(String timerName)
    {
        Timer timer = null;
        if (this.timerMap.containsKey(timerName) == true) {
            timer = this.timerMap.get(timerName);
        }
        return timer;
    }
    
    public List<Event> getEventsForState(State state)
    {
        if ((state.isTop()) || 
            (state.kind == StateKind.FINAL) ||
            (state.getParallel() != null)){
            return getEventsAll();
        }

        List<Event> eventList = new ArrayList<Event>();

        for(Transition transition : state.getTransition()){
        	Event event = transition.getEvent();
        	if(eventList.contains(event) == false){
        		eventList.add(event);
        	}
        }
        
        return eventList;
    }

	public List<Event> getEventsAll() {
		return this.events;
	}
	
	public List<Timer> getTimersAll() {
		return this.timers;
    }
	
	public String getEventMethodNameSync(Event event){
	    return event.getId() + "Sync";
	}
	
    public boolean addState(State state)
    {
    	if(this.stateMap.containsKey(state.getName()) == true){
    		return false;
    	} else {
    		this.stateMap.put(state.getName(), state);
    		return true;
    	}
    }

	public Map<String, State> getStateMap() {
		return this.stateMap;
	}
    
	public State getState(String stateName) {
		if(stateName == null){
			return null;
		}
		
		if(this.stateMap.containsKey(stateName) == true){
			return this.stateMap.get(stateName);
		} else {
			return null;
		}		
	}

    public State getStateLeaf(State state) {
        if(state == null){
            return null;
        }

        if(state.getState().size() == 0){
        	return state;
        } else {
            return getStateLeaf(state.getState().get(0));
        }
    }
    
    public State getStateTop(State state) {
        if(state.getParent() == null){
            return state;
        } else {
            return getStateTop(state.getParent());
        }
    }

    public boolean isAllTransitionGuarded(State state,
                                          String stateClassName,
                                          List<Transition> transitionList) throws StateBuilderException {
        
        boolean bGuard = true;
        for(int i = 0; i < transitionList.size(); i++){
            Transition transition = transitionList.get(i);
            if(getCondition(transition).isEmpty() == true){
                return false;
            }
        }
        return bGuard;
    }
    
    /***
     * Does the state or its parent state handle this event ?
     * @param state
     * @param event
     * @return true if the state handle the event
     * @throws StateBuilderException
     */
    public boolean isEventHandledByState(State state, Event event) throws StateBuilderException {
        List<Event> eventList = getEventsAll();
        String eventId = event.getId();
        
        for(Event eventIterator : eventList){
            if(eventIterator.getId().compareTo(eventId) == 0){
                return true;
            }
        }
        
        return false;
    }
    
    public String getContextClassName(State state) throws StateBuilderException {
        Context context = getStateMachine().getSettings().getContext();
        State stateTop = getStateTop(state);
        if(stateTop.isRoot() == true){
            if((context != null) && 
            		(context.getClazz() != null) &&
            		(context.getClazz().isEmpty() == false)){
                return context.getClazz();
            } else {
                return getStateMachine().getSettings().getName() + "Context";
            }
        } else {
            return getStateMachine().getSettings().getName() + stateTop.getName() +  "Context";
        }    
    }
    
    public Transition getTransition(State state, String eventId, String condition) {
    	for(Transition transition : state.getTransition()){
    		if(transition.getEvent().getId().equals(eventId)){
    			if ((condition == null) && (getCondition(transition) == null)){
    				return transition;
    			}

    			if ((condition != null) && (condition.equals(getCondition(transition)))){
    				return transition;
    			}
    		}
    	}

    	return null;
    }

    public void addTransitionNameToMap(Transition transition){
    	String transitionName = getTransitionName(transition);
    	transitionMap.put(transitionName, transitionMap.size());
    }
    
    public String getTransitionName(Transition transition)
    {
        String condition = getCondition(transition);
        if (condition.isEmpty() == false){
            return transition.getEvent().getId() + "[" + condition + "]";
        } else {
            return transition.getEvent().getId();
        }
    }
    
    public boolean hasTransition(State state, Transition transitionToMatch){
    	for(Transition transition : state.getTransition()){
    		if(transition.getEvent() == transitionToMatch.getEvent()){
    			if(getCondition(transition).equals(getCondition(transitionToMatch))){
    				return true;	
    			}
    		}
    	}
    	return false;
    }
    
    public String getCondition(Transition transition) {
    	if ((transition.getConditionElement() != null) && (transition.getConditionElement().isEmpty() == false)) {
    		return transition.getConditionElement();
    	}
    	else if ((transition.getCondition() != null) && (transition.getCondition().isEmpty() == false)) {
    		return transition.getCondition();
    	}
    	else {
    		return "";
    	}
    }

    /*
     * Depth = 0: context
     * Depth = 1: context.ContextParent
     * Depth = 2: context.ContextParent.ContextParent 
     */
    public int contextDepth(State state, State stateNext) throws StateBuilderException
    {
        int depth = 0;
        return contextDepth(state, stateNext, depth);
    }
    
    /*
     * Compute the context depth between the current state and the next state. 
     */
    private int contextDepth(State state, State stateNext, int depth) throws StateBuilderException 
    {
        if ((state == null) || (stateNext == null)){
            return 0;
        }
        
        if (state.getStateParallel() == stateNext.getStateParallel())
        {
            return depth;
        }
        else
        {
            depth++;
            return contextDepth(state.getStateParallel(), stateNext, depth);
        }
    }
    
    //TODO move to some c++ dependent
    /*
     * Depth = 0: context
     * Depth = 1: context.ContextParent
     * Depth = 2: context.ContextParent.ContextParent 
     */
    public String getContextParentString(State state, State stateNext) throws StateBuilderException {
    	StringBuilder sb = new StringBuilder(getContextInstance());
        
        int contextDepth = contextDepth(state, stateNext);
        
        for (int i = 1; i <= contextDepth; i++)
        {
        	sb.append(".GetParentContext()");
        }
        return sb.toString();
    }
    
    
    public String getContextParentClassName(State state) throws StateBuilderException {
    	
    	if(state.getStateParallel() == null){
    		return getContextClassName(state);
    	} else {
    		return getContextClassName(state.getStateParallel());
    	}
    }
    
    public List<Transition> getTransitionList(State state, Event eventToMatch) {
    	List<Transition> transitions = new ArrayList<Transition>();
    	for (Transition transition : state.getTransition())
    	{
    		Event event = transition.getEvent();
    		if(event.getId().equals(eventToMatch.getId())){
    			transitions.add(transition);	
    		}
    	}

        return transitions;
    }
    
    /**
     * Get the context instance name, context by default
     * It can be also taken from the state machine description file at //StateMachine/settings/context/@instance
     */
    public String getContextInstance(){
        Context context = getStateMachine().getSettings().getContext();
        if((context != null) && (context.getInstance() != null) && (context.getInstance().isEmpty() == false)){
            return context.getInstance();
        } else {
            return "context";
        }
    }


}
