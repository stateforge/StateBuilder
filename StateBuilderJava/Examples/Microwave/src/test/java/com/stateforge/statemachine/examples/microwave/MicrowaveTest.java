package com.stateforge.statemachine.examples.microwave;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

/**
 * Unit test for simple HelloWorld.
 */
public class MicrowaveTest   
{
	private Microwave microwave;

    @Before
    public void setUp() throws Exception {
        this.microwave = new Microwave();
    }

    @After
    public void tearDown() throws Exception {
        
    } 
    
    @Test
    public void testAll()
    {
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
