package com.stateforge.statebuilder.ant;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.BuildException;
import java.io.File;
import java.io.FileFilter;
import java.util.ArrayList;
import java.util.List;
import com.stateforge.statebuilder.java.StateBuilderJavaApp;

/**
 * StateBuilder Ant Task 
 *
 */
public class StateBuilderAntTask extends Task
{
    String input;
    
    public void setInput(String input) {
        this.input = input;
    }

    String output;
    
    public void setOutput(String output) {
        this.output = output;
    }

    /**
     * Location of the file.
     * @parameter default-value="${project.build.directory}/generated-sources/statebuilder"
     */
    private File outputDirectory;

    public StateBuilderAntTask() {
        //();
    }

    public File getOutputDirectory() {
        return outputDirectory;
    }

    public void setOutputDirectory(File outputDirectory) {
        this.outputDirectory = outputDirectory;
    }

    /**
     * Location of the input directory.
     * @parameter default-value="${basedir}/src/main/statemachine"
     */
    private File inputDirectory;

    public File getInputDirectory() {
        return inputDirectory;
    }


    public void setInputDirectory(File inputDirectory) {
        this.inputDirectory = inputDirectory;
    }

    public void execute()
    {
        this.outputDirectory = new File(this.output);

        if (!this.outputDirectory.exists()) {
            this.outputDirectory.mkdirs();
        }

        this.inputDirectory = new File(this.input);

        if(this.inputDirectory.exists() == false){
            throw new BuildException("Input directory does not exist: " + this.inputDirectory);
        }

        List<File> stateMachineFiles = getStateMachineFiles();

        for(File stateMachineFile : stateMachineFiles){
            generate(stateMachineFile);        	
        }
    }

    private void generate(File stateMachineFile)  {
    	log(stateMachineFile.getPath());
        String args[] = new String[3];
        args[0] = stateMachineFile.getPath();
        args[1] = "-d";
        args[2] = getOutputDirectory().getAbsolutePath();
        
        int rv = StateBuilderJavaApp.run(args);
        if(rv < 0){
        	throw new BuildException("StateBuilderJava returns " + rv);
        }
    }

    public final List<File> getStateMachineFiles()
    {
        List<File> stateMachineFiles = new ArrayList<File>();
        File[] files = this.inputDirectory.listFiles( new StateMachineFileFilter( ) );
        if ( files != null ){
            for ( int i = 0; i < files.length; i++ ) {
                stateMachineFiles.add( files[i] );
            }
        }
        //getLog().info(stateMachineFiles.size() + " state machine(s) to generate");
        return stateMachineFiles;
    }

    /**
     * A class used to look up .fsmjava documents from a given directory.
     */
    final class StateMachineFileFilter
    implements FileFilter
    {

        public StateMachineFileFilter(  )
        {
        }

        /**
         * Returns true if the file ends with an fsmjava extension.
         * 
         * @param file
         *            The filed being reviewed by the filter.
         * @return true if an fsmjava file.
         */
        public boolean accept( final java.io.File file )
        {
            boolean accept = file.isFile() && file.getName().endsWith( ".fsmjava" );
            return accept;
        }
    }
}
