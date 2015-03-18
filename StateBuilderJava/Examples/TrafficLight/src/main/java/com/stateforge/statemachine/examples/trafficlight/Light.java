package com.stateforge.statemachine.examples.trafficlight;

public class Light {

    private long redDuration = 1000;
    private long yellowDuration = 1000;
    private long greenDuration = 1000;
    private long maxDuration = 3500;
    
    public void turnOnGreen()
    {
        System.out.println("TurnOnGreen");
    }

    public void turnOffGreen()
    {
        System.out.println("TurnOffGreen");
    }

    public void turnOnYellow()
    {
        System.out.println("TurnOnYellow");
    }

    public void turnOffYellow()
    {
        System.out.println("TurnOffYellow");
    }
    public void turnOnRed()
    {
        System.out.println("TurnOnRed");
    }

    public void turnOffRed()
    {
        System.out.println("TurnOffRed");
    }

    public void setRedDuration(long redDuration) {
        this.redDuration = redDuration;
    }

    public long getRedDuration() {
        return redDuration;
    }

    public void setYellowDuration(long yellowDuration) {
        this.yellowDuration = yellowDuration;
    }

    public long getYellowDuration() {
        return yellowDuration;
    }

    public void setGreenDuration(long greenDuration) {
        this.greenDuration = greenDuration;
    }

    public long getGreenDuration() {
        return greenDuration;
    }

    public void setMaxDuration(long maxDuration) {
        this.maxDuration = maxDuration;
    }

    public long getMaxDuration() {
        return maxDuration;
    }
    
}
