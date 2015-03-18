package com.stateforge.statebuilder.Led;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.stateforge.statemachine.examples.Led.Led;

/**
 * Unit test for HelloWorld.
 */
public class LedTest   
{
    private Led helloWorld;

    @Before
    public void setUp() throws Exception {
        this.helloWorld = new Led();
    }

    @After
    public void tearDown() throws Exception {

    } 

    @Test
    public void testOn()
    {
        this.helloWorld.on();
    }

    @Test
    public void testOff()
    {
        this.helloWorld.off();
    }

    @Test
    public void testOnOff()
    {
        this.helloWorld.on();
        this.helloWorld.off();
    }
}
