package com.spin.apps.password_manager.types;

import lombok.Data;
import java.util.Date;

@Data
public class PrivateNote
{
    private int id;
    private String title;

    private Date dateCreated;
    private Date dateExpiry;

    private String content;
}
