package com.spin.apps.password_manager.views.prototypes;

import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public abstract class ViewController implements IViewEventsGenerator {

    protected Stage baseStage;

    public Stage getStage()
    {
        return baseStage;
    }

    public void setStage(Stage stage)
    {
        this.baseStage = stage;
    }

    public void close()
    {
        baseStage.close();
    }

//    public void setStage(Stage stage) throws IOException
//    {
//        setStage(stage);
//        FXMLLoader fxmlLoader = getLoader();
//        Parent parent = fxmlLoader.load();
//        stage.setScene(new Scene(parent));
//    }


//    public abstract FXMLLoader getLoader();
//    {
//        FXMLLoader loader = new FXMLLoader(getClass().getResource(fxmlPath));
//        //loader.setController(this);
//        return loader;
//    }
}
