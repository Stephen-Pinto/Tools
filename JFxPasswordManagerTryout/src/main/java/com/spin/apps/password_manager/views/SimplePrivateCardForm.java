package com.spin.apps.password_manager.views;

import com.spin.apps.password_manager.views.prototypes.IViewEventsListener;
import com.spin.apps.password_manager.views.prototypes.ViewController;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.VBox;
import org.springframework.stereotype.Component;

import java.io.IOException;

@Component
public class SimplePrivateCardForm extends ViewController implements IViewEventsListener {

    IViewEventsListener listener;
    final int PADDING_VAL = 5;
    final int MARGIN_VAL = 5;

    @FXML private VBox contentVB;

    @FXML
    public void Initialize() throws IOException
    {
        initializeControl();
    }

    public void initializeControl() throws IOException {
        //For later use
        //Stage stage = (Stage)((Node)event.getSource().getScene().getWindow());

        FXMLLoader loader = null;

        loader = new FXMLLoader(getClass().getResource("/views/LabelTextValueControl.fxml"));
        LabelTextValueControl labelTextValueController1 = new LabelTextValueControl();
        loader.setController(labelTextValueController1);
        AnchorPane labelValuePane1 = loader.load();
        labelTextValueController1.setLabel("Name");
        labelTextValueController1.setValue("Stephen");
        labelTextValueController1.setListner(this);

        loader = new FXMLLoader(getClass().getResource("/views/LabelTextValueControl.fxml"));
        LabelTextValueControl labelTextValueController2 = new LabelTextValueControl();
        loader.setController(labelTextValueController2);
        AnchorPane labelValuePane2 = loader.load();
        labelTextValueController2.setLabel("Age");
        labelTextValueController2.setValue("27");
        labelTextValueController2.setListner(this);

        loader = new FXMLLoader(getClass().getResource("/views/LabelDateValueControl.fxml"));
        AnchorPane labelValuePane3 = loader.load();

        loader = new FXMLLoader(getClass().getResource("/views/LabelPasswordValueControl.fxml"));
        LabelPasswordValueControl labelPasswordValueControl = new LabelPasswordValueControl();
        loader.setController(labelPasswordValueControl);
        AnchorPane labelValuePane4 = loader.load();
        labelPasswordValueControl.setLabel("Passsword");

        contentVB.getChildren().addAll(labelValuePane1, labelValuePane2, labelValuePane3, labelValuePane4);
    }

    @Override
    public void actionCompleted(Object sender, Object data) {

    }

    @Override
    public void dataChaged(Object sender, Object data) {
        System.out.println();
    }

    @Override
    public void setListner(IViewEventsListener listner) {
        this.listener = listner;
    }

}

