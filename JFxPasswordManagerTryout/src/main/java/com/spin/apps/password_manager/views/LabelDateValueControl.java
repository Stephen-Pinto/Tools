package com.spin.apps.password_manager.views;

import com.jfoenix.controls.JFXDatePicker;
import com.spin.apps.password_manager.views.prototypes.IViewEventsListener;
import com.spin.apps.password_manager.views.prototypes.SubViewController;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import java.time.LocalDate;
import java.time.ZoneId;
import java.util.Date;

public class LabelDateValueControl extends SubViewController {

    IViewEventsListener listener;

    @FXML private Label nameLabel;

    @FXML private JFXDatePicker valueDtPicker;

    @FXML
    public void initalize()
    {
        valueDtPicker.setOnInputMethodTextChanged(x -> raiseDataChangedEvent());
    }

    @Override
    public void setListner(IViewEventsListener listner)
    {
        this.listener = listner;
    }

    public void setLabel(String str)
    {
        this.nameLabel.setText(str);
    }

    public void setValue(Date value)
    {
        //Convert from zone independent value to local zone dependent value and set
        LocalDate localDate = value.toInstant().atZone(ZoneId.systemDefault()).toLocalDate();
        this.valueDtPicker.setValue(localDate);
    }

    public Date getValue()
    {
        //Convert to zone independent value from local zone depenedent value
        return Date.from(valueDtPicker.getValue().atStartOfDay(ZoneId.systemDefault()).toInstant());
    }

    private void raiseDataChangedEvent()
    {
        if(listener != null)
        {
            listener.dataChaged(this, null);
        }
    }
}
