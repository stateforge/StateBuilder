package com.stateforge.statebuilder;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import java.util.logging.Logger;


import javax.xml.parsers.ParserConfigurationException;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;
import org.apache.commons.cli.PosixParser;
import org.xml.sax.SAXException;

import com.stateforge.statebuilder.model.StateMachineModel;

public class StateBuilderAppBase {
	
	public enum ErrorCode {
		OK(0),
		OK_VERSION(1),
		OK_HELP(2),
		OK_LICENSE(3), 
		OK_UNLICENSE(4),
		// Cannot be -1 because of Launch4j
		KO(-2),
		KO_COMMAND_LINE(-3), 
		KO_SAX_PARSING(-4), 
		KO_PARSER(-5),  
		KO_LICENSE(-6),
		KO_LICENSE_NOTFOUND(-7),
		KO_IO(-8), 
		KO_INVALID_CONFIG_FILE(-9), 
		KO_UNKNOWN(-10);
		
		private int errorCode;
		
		private ErrorCode(int c) {
			errorCode = c;
	    } 
		public int value(){
			return errorCode;
		}
	}
	
	private ErrorCode errorCode;
	public final static String FSM_SUFFIX = "Fsm";
	protected final static String LICENSE = "license";
	protected final static String LICENSE_SHORT = "l";
	protected final static String LICENSE_REMOVE = "--UnLic";
	protected final static String DIRECTORY = "directory";
	protected final static String DIRECTORY_SHORT = "d";
	protected final static String PREPEND_FILE = "prepend-file";
	protected final static String PREPEND_FILE_SHORT = "p";
	protected final static String VERSION = "version";
	protected final static String VERSION_SHORT = "v";
	protected final static String HELP = "help";
	protected final static String HELP_SHORT = "h";
	
	//What can be find in the config file
	protected final static String OPTION_PREFIX = "generator.";
	protected final static String OPTION_PATH = "path";
	protected final static String OPTION_PREPEND = "prepend";
	protected final static String OPTION_OBSERVER = "observer";//TODO
	protected final static String OPTION_ON = "on";
	protected final static String OPTION_OFF = "off";
	
	protected String[] cliArgs;
	protected List<String> _fileList = new ArrayList<String>();
	protected CommandLine cmd;
	protected Options options;
	
	protected boolean m_bShowHelp = false;
	protected String m_outputDirectory; // where to store the result
	protected String m_prependFile;// File to prepend to the generated files
	protected List<StateBuilderInterface> m_fsmCompilerList; // List of generators
	protected Logger m_logger;
	protected String appName;
	protected String fileExtension;

	protected static String pomProperties;
    private String schemaName;
    
    protected LicenseController licenseController;
	/**
	 * Default constructor
	 * @param licensingInfo 
	 */
	public StateBuilderAppBase(LicenseableClass licensingInfo) {
		this.licenseController = new LicenseController(licensingInfo);
		m_fsmCompilerList = new ArrayList<StateBuilderInterface>();
	}
	
	protected void createLicenseController(LicenseableClass licensingInfo){
		this.licenseController = new LicenseController(licensingInfo);
		
	}
	public ErrorCode getErrorCode() {
		return errorCode;
	}

	public void setErrorCode(ErrorCode errorCode) {
		this.errorCode = errorCode;
	}
	
	public String getSchemaName() {
		return schemaName;
	}

	public void setSchemaName(String schemaName) {
		this.schemaName = schemaName;
	}
	
	public static String getPomProperties() {
		return StateBuilderAppBase.pomProperties;
	}

	public static void setPomProperties(String pomProperties) {
		StateBuilderAppBase.pomProperties = pomProperties;
	}

	public String getAppName() {
		return appName;
	}

	public void setAppName(String appName) {
		this.appName = appName;
	}

	/**
	 * 
	 * @param inputFilename src/MyStateMachine.fsm
	 * @param baseName MyStateMachine
	 * @return
	 */
	public String getAbsoluteStateMachinePath(
			String inputFilename,
			String baseName){
		//System.out.println("input filename " + inputFilename);
		//File file = new File(inputFilename);
		//System.out.println(file.getName());
		String ouputDir;
		
		if (getOutputDirectory() == null) {
			ouputDir = computeDefaultOutputDirectory(inputFilename);
		} else {  
			ouputDir = new File(getOutputDirectory()).getAbsolutePath();
		}
		
		//System.out.println("output dir " + ouputDir);
		String outputFile  = new File(ouputDir + File.separator + baseName + FSM_SUFFIX).getAbsolutePath();
		//System.out.println("output filename " + outputFile);
		return outputFile;
	}
	
	public String computeDefaultOutputDirectory(String inputFilename)
	{
		String ouputDir;
		File file = new File(inputFilename);
		if(file.getParentFile() != null){
			ouputDir = file.getParentFile().getAbsolutePath();
		} else {
			ouputDir = new File(".").getAbsolutePath();
		}
		return ouputDir;
	}
	/**
	 * @return Returns the output directory.
	 */
	public String getOutputDirectory() {
		return m_outputDirectory;
	}

	/**
	 * Set the output directory.
	 * 
	 * @param outputDirectory
	 *            The output directory to set.
	 */
	public void setOutputDirectory(String outputDirectory) {
		this.m_outputDirectory = outputDirectory;
	}

	public String getPrependFile() {
		return m_prependFile;
	}

	public void setPrependFile(String prependFile) {
		this.m_prependFile = prependFile;
	}
	
	public CommandLine getCmd() {
		return cmd;
	}

	public void setCmd(CommandLine cmd) {
		this.cmd = cmd;
	}
	
	protected StateMachineModel createModel(String inputFileName) throws StateBuilderException, ParserConfigurationException, SAXException, IOException
	{
		File inputFile = new File(inputFileName);
		InputStream fsmcppInputStream = new FileInputStream(inputFile);
		
		// Check if file exists
		if (inputFile.exists() == false) {
			throw new StateBuilderException("The input file "
					+ inputFileName + " does not exist");
		}

		String name = inputFile.getName();

		// Remove the file extension suffix
		int indexOfDot = name.indexOf('.');
		if (indexOfDot > 0) {
			name = name.substring(0, indexOfDot);
		}

		StateMachineXmlModel stateMachineXmlModel = new StateMachineXmlModel(fsmcppInputStream, inputFileName, name, getSchemaName());
		
		StateMachineModel model = stateMachineXmlModel.build();
		return model;
	}
	
	/**
	 * Prepend a file to the generated file, useful to include copyright
	 * 
	 * @param prependFileName
	 *            the file to prepend
	 * @throws StateBuilderException
	 */
	protected void generatePrependFile(PrintStream printStream,
			String prependFileName) throws StateBuilderException {
		if (prependFileName == null) {
			return;
		}
		try {
			FileReader fileReader = new FileReader(prependFileName);
			BufferedReader bufferReader = new BufferedReader(fileReader);
			String line;
			while ((line = bufferReader.readLine()) != null) {
				printStream.println(line);
			}
			bufferReader.close();
		} catch (IOException ioEx) {
			throw new StateBuilderException(ioEx);
		}
		printStream.flush();
	}
	
	/**
	 * Fetch the version from pom.properties
	 **/
	public String getVersion() {
		try {
			InputStream in = StateBuilderAppBase.class
					.getClassLoader()
					.getResourceAsStream(StateBuilderAppBase.getPomProperties());
			if (in == null) {
				return "x.y.z";
			}
			Properties properties = new Properties();
			properties.load(in);
			String version = (String) properties.get("version");
			return version;
		} catch (IOException ioEx) {
			return ioEx.getMessage();
		}
	}
	
	protected Options createOptions(){
		Options options = new Options();
		options.addOption(new Option(DIRECTORY_SHORT, DIRECTORY, true,
				"Output directory"));
		
		options.addOption(new Option(PREPEND_FILE_SHORT, PREPEND_FILE, true,
				"Prepend file, e.g Copyright"));
	
		options.addOption(new Option(VERSION_SHORT, VERSION, false,
				"Show version"));
		
		options.addOption(new Option(HELP_SHORT, HELP, false, "Show help"));
		
		options.addOption(new Option(LICENSE_SHORT, LICENSE, true, "install license"));
		setOptions(options);
		
		return options;
	}
	
	public Options getOptions() {
		return options;
	}

	public void setOptions(Options options) {
		this.options = options;
	}

	protected CommandLine createCommandLine(String[] args, Options options) throws ParseException {
		CommandLineParser cmdParser = new PosixParser();
		CommandLine cmd = cmdParser.parse(options, args);
		return cmd;
	}
	
	protected boolean processCliCommon(CommandLine cmd){

		if (cmd.hasOption(VERSION)) {
			System.out.println(getAppName() + " version: " + getVersion());
			setErrorCode(ErrorCode.OK_VERSION);
		} else if (cmd.hasOption(LICENSE)) {
			licenseInstall(cmd.getOptionValue(LICENSE));
		} else if (cmd.hasOption(HELP_SHORT) || (cmd.getArgs().length == 0)) {
			HelpFormatter hf = new HelpFormatter();
			hf.printHelp(getAppName() + " [option] myStateMachine." + getFileExtension(),
					getOptions());
			setErrorCode(ErrorCode.OK_HELP);
		}  else {
			// file to prepend
			String prependFile = cmd.getOptionValue(PREPEND_FILE_SHORT);
			setPrependFile(prependFile);

			// Output directory
			setOutputDirectory(cmd.getOptionValue(DIRECTORY_SHORT));

			return false;
		}
		
		return true;
	}

	void readConfigFile(String configFileName) throws StateBuilderException{
		try {
			FileInputStream fstream = new FileInputStream(configFileName);
			DataInputStream in = new DataInputStream(fstream);
			BufferedReader br = new BufferedReader(new InputStreamReader(in));
			String line;
			while ((line = br.readLine()) != null)   {
				readConfigLine(line);
			}
			in.close();
		} catch (FileNotFoundException e) {
			throw new StateBuilderException(e);
		} catch (IOException e) {
			throw new StateBuilderException(e);
		}
		
	}
	
	protected void readConfigLine(String line){
		System.out.println(line);
		if(line.startsWith("#")){
			return;
		}
		
		if(line.startsWith(OPTION_PREFIX)){
			String[] args = line.split("=");
			if(args.length >= 2){
				String optionName = args[0];
				String optionValue = args[1];
				System.out.println("option " + optionName + " = " + optionValue);
				if(optionName.contains(OPTION_PATH)){
					System.out.println("OutputDirectory: " + optionValue);
					setOutputDirectory(optionValue);
				} else if(optionName.contains(OPTION_PREPEND)){
					setPrependFile(optionValue);
				} else if(optionName.contains(OPTION_OBSERVER)){
					if(optionValue.contains(OPTION_OFF)){
					//TODO
					}
				}
			}
		} else {
			System.out.println("file: " + line);
			_fileList.add(line);
		}
	}
	/**
	 * 
	 * @param cmd
	 * @return ture if the command line has one argument which doesn't end with the .fsmcpp or .fsmjava
	 */
	protected boolean hasConfigFile(CommandLine cmd){
		if(cmd.getArgs().length != 1){
			return false;
		}
		
		String fileName = cmd.getArgs()[0];
		if(fileName.endsWith(getFileExtension())){
			return false;
		}
		
		return true;
	}
	
	public String getFileExtension() {
		return fileExtension;
	}

	public void setFileExtension(String fileExtension) {
		this.fileExtension = fileExtension;
	}
	
    public boolean verifyLicense() {
        return this.licenseController.verifyLicense();
    }
      
    public void licenseInstall(String licenseFileName){
    	 try {
    		 File licenceFile = new File(licenseFileName);
    		 if(licenceFile.exists() == true){
    			 if(this.licenseController.installLicense(licenseFileName) == true){
    				 System.out.println("License installed successfully");
    				 setErrorCode(ErrorCode.OK_LICENSE);
    			 } else {
    				 System.err.println("License file \"" + licenseFileName + "\" was **NOT** installed successfully");
    				 setErrorCode(ErrorCode.KO_LICENSE);
    			 }
    		 } else {
    			 System.err.println("Cannot find the license " + licenceFile.getCanonicalPath());
    			 setErrorCode(ErrorCode.KO_LICENSE_NOTFOUND);
    		 }
         }
         catch (Exception failure) {
        	 failure.printStackTrace();
        	 System.err.println("Exception while installing license: " + failure.getLocalizedMessage());
        	 setErrorCode(ErrorCode.KO_LICENSE);
         }	
    }
    
    public boolean removeLicense() {
        try {
            this.licenseController.removeLicense();
            System.out.println("License removed successfully");
            setErrorCode(ErrorCode.OK_UNLICENSE);
        } catch (Exception ex) {
            setErrorCode(ErrorCode.KO_LICENSE_NOTFOUND);
            return false;
        }
        return true;
    }
    
    protected void generate(
    		String fsmcppInputFileName, 
    		CommandLine cmd) 
    	throws StateBuilderException, ParserConfigurationException, SAXException, IOException {
    	
    }
    
    protected void generateAll(
    		CommandLine cmd) throws StateBuilderException, ParserConfigurationException, SAXException, IOException {
    	if(_fileList.isEmpty()){
    		throw new StateBuilderException("no state machine to generate");
    	}
    	
    	for(String file : _fileList){
    		generate(file, cmd);
    	}
    }
    
	protected CommandLine commandLineParse(String[] args)
			throws ParseException {
		return null;
	}
	
    public ErrorCode generate(String[] args) {
		System.out.println(getAppName() + " "+ getVersion() + " : All rights reserved, 2011-2013");
		setErrorCode(ErrorCode.KO_UNKNOWN);
				
		// Unlicense is hidden
		for(String argument : args){
			if(argument.compareTo(LICENSE_REMOVE) == 0){
				removeLicense();
				return getErrorCode();
			}
		}
		
		try {
			CommandLine cmd = commandLineParse(args);
			// Generate from all fsm files given on the command line
			if(processCliCommon(cmd) == false){
				if(verifyLicense() == false){
					setErrorCode(ErrorCode.KO_LICENSE);
				} else {
					if(hasConfigFile(cmd)){
						readConfigFile(cmd.getArgs()[0]);
					} else {
						for (int i = 0; i < cmd.getArgs().length; i++) {
							//System.out.print(cmd.getArgs()[i]);
							_fileList.add(cmd.getArgs()[i]);
						}
					}
					
					generateAll(cmd);
					setErrorCode(ErrorCode.OK);
				}
			}
		} catch (ParseException e){
			System.err.println(getAppName() + ": Invalid command line argument, "
					+ e.getLocalizedMessage());
			setErrorCode(ErrorCode.KO_COMMAND_LINE);
		} catch (StateBuilderException e) {
			// e.printStackTrace();
			System.err.println(getAppName() + ": An error has occured: "
					+ e.getLocalizedMessage());
			setErrorCode(ErrorCode.KO);
		} catch (SAXException saxException) {
			// State machine is not a well formed xml file
			System.err
					.println(getAppName() + ": An error has occured during parsing : "
							+ saxException.getLocalizedMessage());
			setErrorCode(ErrorCode.KO_SAX_PARSING);
		} catch (ParserConfigurationException pcEx) {
			// State machine is not a valid a xml file according to is schema
			// file
			setErrorCode(ErrorCode.KO_PARSER);
		} catch (IOException e) {
			System.err.println(getAppName() + ":Input/Output error:  "
					+ e.getLocalizedMessage());
			setErrorCode(ErrorCode.KO_IO);
		}
		
		//System.out.println(getAppName() + " ended with " + getErrorCode().value());
		return getErrorCode();
	}


}
