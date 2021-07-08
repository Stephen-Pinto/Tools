package com.spin.apps.password_manager.views;

import com.spin.apps.password_manager.views.prototypes.ViewController;
import com.spin.apps.password_manager.views.prototypes.IViewEventsListener;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.TextField;
import org.springframework.stereotype.Component;

@Component
public class NewLabelNameTB extends ViewController {

    private IViewEventsListener listener;

    @FXML TextField newLabelNameTF;

    @FXML public void onEnter(ActionEvent actionEvent)
    {
        this.listener.actionCompleted(this, newLabelNameTF.getText());
        this.getStage().close();
    }

    @Override
    public void setListner(IViewEventsListener listner)
    {
        this.listener = listener;
    }
}
