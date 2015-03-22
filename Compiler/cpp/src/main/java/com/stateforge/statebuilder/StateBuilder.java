package com.stateforge.statebuilder;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintStream;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.xml.parsers.ParserConfigurationException;

import com.stateforge.statebuilder.StateBuilderException;
import com.stateforge.statebuilder.StateBuilderInterface;
import com.stateforge.statebuilder.cpp.StateBuilderCpp;
import com.stateforge.statebuilder.cpp.StateBuilderHpp;
import com.stateforge.statebuilder.model.StateMachineModel;
import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.xml.sax.SAXException;

public class StateBuilder extends StateBuilderAppBase {

	// Options
	private final static String LOCK = "lock";
	private final static String LOCK_SHORT = "L";
	
	/**
	 * Default constructor
	 */
	public StateBuilder()  {
		super();
		setAppName("StateBuilderCpp");
		setPomProperties("META-INF/maven/com.stateforge.statebuilder/statebuilder-cpp/pom.properties");
		setSchemaName("StateMachineCpp-v1.xsd");
		setFileExtension("fsmcpp");
		m_logger = Logger.getLogger(getClass().getName());
		m_logger.setLevel(Level.OFF);
	}

	/**
	 * entry point of the program
	 * 
	 * @param args
	 *            command line parameters
	 */
	 public static void main(String[] args) {
		 StateBuilder stateBuilder = new StateBuilder();
		 ErrorCode error = stateBuilder.generate(args);
		 System.exit(error.value());
	 }

	/**
	 * generate the code from one input file
	 * 
	 * @param fsmcppInputFileName
	 *            input file
	 * @param cmd
	 *            the command line argument
	 * @throws StateBuilderException
	 * @throws SAXException
	 * @throws ParserConfigurationException
	 * @throws SAXException
	 *             parsing exception
	 */
	 @Override
	 public void generate(String fsmcppInputFileName, CommandLine cmd) 
	 	throws StateBuilderException, ParserConfigurationException, SAXException, IOException {
		 //System.out.println("fsmcppInputFileName " + fsmcppInputFileName);
		 //System.out.println("current dir " + System.getProperty("user.dir"));
		 StateMachineModel model = createModel(fsmcppInputFileName);

		 StateBuilderInterface fsmCompilerCpp = new StateBuilderCpp(model);
		 m_fsmCompilerList.add(fsmCompilerCpp);
		 StateBuilderInterface fsmCompilerHpp = new StateBuilderHpp(model);
		 m_fsmCompilerList.add(fsmCompilerHpp);

		 // Go through the list of compilers and generate the code for each
		 // of them
		 for (StateBuilderInterface compiler : m_fsmCompilerList) {
			 try {
				 //System.out.println("getFileExtension: " + getFileExtension());
				 String output = getAbsoluteStateMachinePath(
						 fsmcppInputFileName,
						 model.getStateMachine().getSettings().getName()) + compiler.getFileExtension();;
				 
				 // Create the file from the given name
				 File outputFile = new File(output);

				 // create the PrintStream
				 PrintStream stream = new PrintStream(outputFile);

				 // Add eventual copyright
				 generatePrependFile(stream, getPrependFile());

				 compiler.setStream(stream);
				 // Command line options
				 compiler.setVersion(getVersion());

				 // Set the library: qt, boost, std or libev
				 compiler.setLibrary(model.getStateMachine().getSettings().getLibrary().value());

				 // Generate the observer code ?
				 compiler.setLock(cmd.hasOption(LOCK_SHORT));

				 compiler.setCommandLine(this.cliArgs);

				 // Do the real work here
				 compiler.generate();

				 // save to file to the disk
				 stream.close();
			 } catch (FileNotFoundException fnfEx) {
				 throw new StateBuilderException(fnfEx);
			 }
		 }
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
		this.cliArgs = args;
		Options options = createOptions();
		
		options.addOption(new Option(LOCK_SHORT, LOCK, false,
				"Add code for multithreaded state machine"));
        return createCommandLine(args, options);
	}
}
