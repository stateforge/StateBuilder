package com.stateforge.statemachine.examples.trafficlight;



import org.junit.After;
import org.junit.Before;
import org.junit.Test;

/**
 * Unit test for TrafficLight
 */
public class TrafficLightTest 
{
    private TrafficLight trafficLight;

    @Before
    public void setUp() throws Exception {
        this.trafficLight = new TrafficLight();
    }

    @After
    public void tearDown() throws Exception {

    } 

    @Test
    public void testStart()
    {
        this.trafficLight.start();
        try {
            Thread.sleep(10000);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}
