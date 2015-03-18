package com.stateforge.statemachine.examples.microwave;

import com.stateforge.statemachine.listener.ObserverConsole;

/**
 * Microwave
 *
 */
public class Microwave
{
	private MicrowaveAction action;
	private MicrowaveContext context;
	
	public Microwave(){
		this.action = new MicrowaveAction();
		this.context = new MicrowaveContext(this.action);
		this.context.setObserver(ObserverConsole.getInstance());
	}

	public void evTurnOn() {
        this.context.evTurnOn();
    }
    
    public void evTurnOff() {
        this.context.evTurnOff();
    }

    public void evCookingDone() {
        this.context.evCookingDone();
    }

    public void evDoorClosed() {
        this.context.evDoorClosed();
    }

    public void evDoorOpened() {
        this.context.evDoorOpened();
    }

    public void evStart() {
        this.context.evStart();
    }
    
    public static void main( String[] args )
    {
        System.out.println("Microwave");
        Microwave microwave = new Microwave();
        microwave.evStart();
        microwave.evDoorOpened();
        microwave.evTurnOn();
        microwave.evDoorClosed();
        microwave.evDoorClosed();
        microwave.evCookingDone();
        microwave.evDoorOpened();
        microwave.evTurnOff();
        microwave.evDoorOpened();
        microwave.evTurnOn();
        microwave.evDoorClosed();        
    }
}
