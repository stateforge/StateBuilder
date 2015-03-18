package com.stateforge.statemachine.examples.Led;

/**
 * Led
 */
public class Led
{
    private LedContext context;

    public Led(){
        this.context = new LedContext(this);
    }

    public void on(){
        this.context.on();
    }

    public void off(){
        this.context.off();
    }

    void doOn(){
        System.out.println("Led.doOn");
    }

    void doOff(){
        System.out.println("Led.doOff");
    }
    
    public static void main( String[] args )
    {
        System.out.println("Led");
    }
}
