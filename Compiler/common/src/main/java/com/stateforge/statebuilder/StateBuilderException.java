package com.stateforge.statebuilder;

public class StateBuilderException extends Exception {

    static final long serialVersionUID = -2090113613490464460L;

    public final String TRANSITION_ORDER = "S1001";
    public final String INDENTATION = "I1001";
    
    String errorCode;
    
    public StateBuilderException() {
        super();
    }

    public StateBuilderException(String message) {
        super(message);
    }
    
    public StateBuilderException(String errorCode, String message) {
        super(message);
        this.errorCode = errorCode;
    }

    public StateBuilderException(String message, Throwable cause) {
        super(message, cause);
    }

    public StateBuilderException(Throwable cause) {
        super(cause);
    }

}
