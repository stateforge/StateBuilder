package com.stateforge.statemachine.examples.Unitest01;

public class Action {

    public void doPrint(String message) {
        System.out.println(message);
        throw new NullPointerException("something went wrong");
    }
}
