package com.stateforge.statebuilder.HelloWorld;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.stateforge.statemachine.examples.HelloWorld.HelloWorld;

/**
 * Unit test for HelloWorld.
 */
public class HelloWorldTest   
{
    private HelloWorld helloWorld;

    @Before
    public void setUp() throws Exception {
        this.helloWorld = new HelloWorld();
    }

    @After
    public void tearDown() throws Exception {

    } 

    @Test
    public void testStart()
    {
        this.helloWorld.start();
    }

    @Test
    public void testStop()
    {
        this.helloWorld.stop();
    }

    @Test
    public void testStartStop()
    {
        this.helloWorld.start();
        this.helloWorld.stop();
    }
}
