package com.stateforge.statebuilder.java;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.xml.parsers.ParserConfigurationException;

import com.stateforge.statebuilder.StateBuilderAppBase;
import com.stateforge.statebuilder.StateBuilderException;
import com.stateforge.statebuilder.model.StateMachineModel;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.xml.sax.SAXException;

public class StateBuilderJava extends StateBuilderAppBase {
	
    /**
	 * Default constructor
	 */
	public StateBuilderJava() {
		super(new StateBuilderJavaLicensingInfo());
		setAppName("StateBuilderJava");
		
		m_logger = Logger.getLogger(getClass().getName());
		m_logger.setLevel(Level.OFF);
		setFileExtension("fsmjava");
		setPomProperties("META-INF/maven/com.stateforge.statebuilder/statebuilder-java/pom.properties");
		setSchemaName("StateMachineJava-v1.xsd");
	}

	/**
	 * entry point of the program
	 * 
	 * @param args
	 *            command line parameters
	 */
	 public static int run(String[] args) {
		 StateBuilderJava stateBuilder = new StateBuilderJava();
		 ErrorCode error = stateBuilder.generate(args);
		 return error.value();
	 }
	/**
	 * generate the code from one input file
	 * 
	 * @param inputFileName
	 *            input file
	 * @param cmd
	 *            the command line argument
	 * @throws StateBuilderException
	 * @throws SAXException
	 *             parsing exception
	 * @throws IOException 
	 * @throws Exception 
	 */
	 
	public void generate(
			String inputFileName, 
			CommandLine cmd) 
	throws StateBuilderException, ParserConfigurationException, SAXException, IOException {

		StateMachineModel model = createModel(inputFileName);
		CoderStateMachine stateBuilderJava = new CoderStateMachine(model);

		// Command line options
		stateBuilderJava.setVersion(getVersion());

		stateBuilderJava.setCommandLine(this.cliArgs);
		// Output directory
		String ouputDir = getOutputDirectory();
		if(ouputDir == null){
			ouputDir = computeDefaultOutputDirectory(inputFileName);
		}
		stateBuilderJava.setOutputDirectory(ouputDir);

		// Do the real work here
		stateBuilderJava.generate();
	}

	/**
	 * Parse the command line and exit on error
	 * 
	 * @param args
	 * @return CommandLine
	 * @throws ParseException 
	 */
	@Override
	public CommandLine commandLineParse(String[] args)
			throws ParseException {
		Options options = createOptions();
		
		//Add eventual option for java here
		
        return createCommandLine(args, options);
	}
}
