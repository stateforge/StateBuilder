package com.stateforge.statemachine.examples.trafficlight;

import com.stateforge.statemachine.context.IContextEnd;
import com.stateforge.statemachine.listener.ObserverConsole;

/**
 * TrafficLight
 *
 */
public class TrafficLight
{
    private Light light;
    private TrafficLightContext context;
    
    public TrafficLight(){
        this.light = new Light();
        this.context = new TrafficLightContext(light);
        this.context.setObserver(ObserverConsole.getInstance());
        this.context.setEndHandler(new IContextEnd() {
            public void end(Throwable throwable) {
                System.out.println("context has ended");
            }
        });
    }
    
    public void start(){
        this.context.enterInitialState();
    }
    
    public static void main( String[] args )
    {
        System.out.println( "TrafficLight" );
        TrafficLight trafficLight = new TrafficLight();
        trafficLight.start();
        
        try {
            Thread.sleep(60000);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        System.out.println( "TrafficLight ends" );
    }    
}
