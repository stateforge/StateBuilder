package com.stateforge.statemachine.examples.HelloWorld;

//import com.stateforge.statemachine.examples.HelloWorld.HelloWorldConte;

/**
 * Hello world!
 *
 */
public class HelloWorld
{
	private HelloWorldAction action;
	private HelloWorldContext context;
	
	public HelloWorld(){
		this.action = new HelloWorldAction();
		this.context = new HelloWorldContext(this.action);
	}
	
    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
    }
     
    public void start(){
    	this.context.start();
    }
    
    public void stop(){
        this.context.stop();
    }
    
}
