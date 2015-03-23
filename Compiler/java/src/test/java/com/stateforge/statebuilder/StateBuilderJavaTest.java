package com.stateforge.statebuilder;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.stateforge.statebuilder.java.StateBuilderJava;
import com.stateforge.statebuilder.StateBuilderAppBase.ErrorCode;

public class StateBuilderJavaTest {
	
	private StateBuilderJava stateBuilder;
	private String fsmFile = "src/test/resources/Ping.fsmjava";
	
    @Before
    public void setUp() throws Exception {
        this.stateBuilder = new StateBuilderJava();
    }

    @After
    public void tearDown() throws Exception {
    } 

    @Test
    public void testHelpLong()
    {
    	String args[] = new String[] {"--help"};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.OK_HELP);
    }
    
    @Test
    public void testHelpShort()
    {
    	String args[] = new String[] {"-h"};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.OK_HELP);
    }
 
    @Test
    public void testVersionLong()
    {
    	String args[] = new String[] {"--version"};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.OK_VERSION);
    }
    
    @Test
    public void testVersionShort()
    {
    	String args[] = new String[] {"-v"};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.OK_VERSION);
    }
    
    @Test
    public void testNoOption()
    {
    	String args[] = new String[] {};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.OK_HELP);
    }
    
    @Test
    public void testOptionDoesNotExist()
    {
    	String args[] = new String[] {"--optionDoesNotExist"};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.KO_COMMAND_LINE);
    }
    
    @Test
    public void testSequence()
    {
    	testGenerateFsm();
    	testGenerateFsm();
    	testGenerateFsm();
    	testGenerateFsm();
    }
    
        
    public void testGenerateFsm()
    {
    	String args[] = new String[] {fsmFile};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.OK);
    }
    
}
