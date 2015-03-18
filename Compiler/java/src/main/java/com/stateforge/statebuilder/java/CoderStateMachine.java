package com.stateforge.statebuilder.java;

import com.stateforge.statebuilder.StateBuilderException;
import com.stateforge.statebuilder.StateBuilderInterface;
import com.stateforge.statebuilder.model.StateMachineModel;
import com.sun.codemodel.JCodeModel;
import java.io.File;
import java.io.IOException;

public class CoderStateMachine extends StateBuilderInterface {

    private static final String FILE_EXTENSION = ".java";

    public CoderStateMachine(StateMachineModel model) {
        super(model);      
    }

    public void generate() throws StateBuilderException {

        StateMachineModel model = getModel();
        writeCompilerInfo();

        try {
            JCodeModel code = new JCodeModel();

            //Context
            CoderContext coderContext = new CoderContext(model, code);
            coderContext.code();

            //State
            CoderState coderState = new CoderState(model, code);
            coderState.code();

            //Parallel
            CoderParallel coderParallel = new CoderParallel(model, code);
            coderParallel.code();

            //Write java files
            code.build(new File(getOutputDirectory()));
        } catch (IOException e) {
            throw new StateBuilderException(e);
        }
    }

    private void writeCompilerInfo() {
        // TODO HEEFRE

    }

    @Override
    public String getFileExtension() {
        return FILE_EXTENSION;
    }
}
