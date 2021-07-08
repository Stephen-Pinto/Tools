package com.spin.apps.password_manager.data;

import com.spin.apps.password_manager.crypto.RandomDataBuilder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class DataPadder
{
    private String padIdentifier = "#$101$#";

    @Autowired
    private RandomDataBuilder helper;

    public String pad(String data, int newSize)
    {
        //Check if there is enough space for padding
        if((data.length() + padIdentifier.length()) < newSize)
        {
            //Before adding random string add pad indentifier for unpadding simplicity
            data += padIdentifier;

            //If there is still room for more padding add random bytes
            if(data.length() < newSize)
            {
                data += helper.generateRandomString(newSize - data.length());
            }
        }

        return data;
    }

    public String unpad(String data)
    {
        //If padded then search for our pad identifier else return
        if(data.contains(padIdentifier))
            return data.substring(0, data.indexOf(padIdentifier));
        else
            return data;
    }
}
