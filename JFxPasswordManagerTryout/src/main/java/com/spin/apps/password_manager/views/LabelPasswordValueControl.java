package com.spin.apps.password_manager.views;

import com.jfoenix.controls.JFXCheckBox;
import com.jfoenix.controls.JFXPasswordField;
import com.jfoenix.controls.JFXTextField;
import com.spin.apps.password_manager.views.prototypes.IViewEventsListener;
import com.spin.apps.password_manager.views.prototypes.SubViewController;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.control.Control;
import javafx.scene.control.Label;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.text.Font;

import javax.swing.*;

public class LabelPasswordValueControl extends SubViewController {

    IViewEventsListener listener;

    @FXML private Label nameLabel;

    @FXML private JFXPasswordField passwordPField;

    @FXML private JFXCheckBox viewPsswdCBox;

    @FXML private HBox passwdFldHBox;

    private JFXTextField passwordTField;

    @Override
    public void setListner(IViewEventsListener listner)
    {
        this.listener = listner;
    }

    @FXML
    public void initialize()
    {
        //Introduce new text field which we will push in when the flag is changed to visible
        passwordTField = new JFXTextField();
        passwordTField.setFont(new Font(passwordTField.getFont().getFamily(), passwordPField.getFont().getSize()));

        //Bind the text and password fields to always maintain the same data
        passwordPField.textProperty().bindBidirectional(passwordTField.textProperty());
        viewPsswdCBox.setOnAction(x -> checkboxStateChanged());

        //Notify the watchers on change in data
        passwordPField.setOnInputMethodTextChanged(x -> raiseDataChangedEvent());
    }

    private void checkboxStateChanged()
    {
        if(viewPsswdCBox.isSelected())
        {
            passwdFldHBox.getChildren().clear();
            passwdFldHBox.getChildren().addAll(passwordTField, viewPsswdCBox);
            HBox.setHgrow(passwordTField, Priority.ALWAYS);
        }
        else
        {
            passwdFldHBox.getChildren().clear();
            passwdFldHBox.getChildren().addAll(passwordPField, viewPsswdCBox);
            HBox.setHgrow(passwordPField, Priority.ALWAYS);
        }
    }

    public void setLabel(String str)
    {
        this.nameLabel.setText(str);
    }

    public void setValue(String value)
    {
        this.passwordPField.setText(value);
    }

    public String getValue()
    {
        return passwordTField.getText();
    }

    private void raiseDataChangedEvent()
    {
        if(listener != null)
        {
            listener.dataChaged(this, null);
        }
    }
}
