package com.stateforge.statemachine.examples.ping;

public interface  IEchoClientListener {
    void messageReceived();
    void error(String message);
}
