package com.spin.apps.password_manager.views;

import com.spin.apps.password_manager.views.prototypes.IViewEventsListener;
import com.spin.apps.password_manager.views.prototypes.SubViewController;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;

public class LabelTextValueControl extends SubViewController {

    IViewEventsListener listener;

    @FXML private Label nameLabel;

    @FXML private TextField valueTField;

    public LabelTextValueControl()
    {
        //Everytime data is changed we will notify the parent
        //this.valueTField.setOnAction(x -> raiseDataChangedEvent());
    }

    @FXML
    public void initialize()
    {
        valueTField.setOnInputMethodTextChanged(x -> raiseDataChangedEvent());
    }

    @Override
    public void setListner(IViewEventsListener listner) {
        this.listener = listner;
    }

    public void setLabel(String str)
    {
        this.nameLabel.setText(str);
    }

    public void setValue(String value)
    {
        this.valueTField.setText(value);
    }

    public String getValue()
    {
        return valueTField.getText();
    }

    private void raiseDataChangedEvent()
    {
        if(listener != null)
        {
            listener.dataChaged(this, null);
        }
    }
}
