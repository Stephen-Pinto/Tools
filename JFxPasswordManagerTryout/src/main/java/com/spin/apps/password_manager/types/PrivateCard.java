package com.spin.apps.password_manager.types;

import com.spin.apps.password_manager.types.CustomField;
import lombok.Data;

import java.util.ArrayList;
import java.util.Date;

@Data
public class PrivateCard
{
    private int id;
    private String title;

    private Date dateCreated;
    private Date dateExpiry;

    private ArrayList<CustomField> fields = new ArrayList<>();

    public boolean hasExpired()
    {
        Date today = new Date();
        if(dateExpiry.compareTo(dateExpiry) > 0)
            return true;
        else
            return false;
    }
}
