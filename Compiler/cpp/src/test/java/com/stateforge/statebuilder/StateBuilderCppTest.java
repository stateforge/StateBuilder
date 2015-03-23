package com.stateforge.statebuilder;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import com.stateforge.statebuilder.StateBuilder;
import com.stateforge.statebuilder.StateBuilderAppBase.ErrorCode;

public class StateBuilderCppTest {
	
	private StateBuilder stateBuilder;
	private String config = "src/test/resources/statemachine.cfg";
	private String configEmpty = "src/test/resources/statemachineEmpty.cfg";
	private String configOutputDirNotExist = "src/test/resources/statemachineOutputPathNotExist.cfg";
	private String configCopyrightNotFound = "src/test/resources/statemachinePrependFileNotFound.cfg";
	private String fsmFile = "src/test/resources/Minimal.fsmcpp";
	private String fsmFilePersistenceHasTimer = "src/test/resources/PersistenceHasTimer.fsmcpp";
	private String fsmFilePersistenceEventWithParameter = "src/test/resources/PersistenceEventWithParameter.fsmcpp";
	
    @Before
    public void setUp() throws Exception {
        this.stateBuilder = new StateBuilder();
    }

    @After
    public void tearDown() throws Exception {
    } 

    @Test
    public void testConfig()
    {
    	String args[] = new String[] {config};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.OK);
    }
    
    @Test
    public void testConfigOutputDirNotExist()
    {
    	String args[] = new String[] {configOutputDirNotExist};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.KO);
    }
    @Test
    public void testConfigEmpty()
    {
    	String args[] = new String[] {configEmpty};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.KO);
    }
    
    @Test
    public void testConfigCopyrightNotFound()
    {
        String args[] = new String[] {configCopyrightNotFound};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.KO);
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
    
    @Test
    public void testGenerationFailed()
    {
    	String args[] = new String[] {"../../StateBuilderCpp/examples/unitest/DuplicatedCondition.fsmcpp"};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.KO);
    }   
    
    @Test
    public void testFsmFound()
    {
    	String args[] = new String[] {"notexist.fsmcpp"};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.KO_IO);
    }
    
    @Test
    public void testPersistenceHasTimer()
    {
    	String args[] = new String[] {fsmFilePersistenceHasTimer};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.KO);
    }
    
    @Test
    public void testPersistenceEventWithParameter()
    {
    	String args[] = new String[] {fsmFilePersistenceEventWithParameter};
    	ErrorCode error = stateBuilder.generate(args);
        assertTrue(error == ErrorCode.KO);
    }
}
