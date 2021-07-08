package com.spin.apps.password_manager.util;

import org.springframework.stereotype.Component;

import java.util.Base64;

@Component
public class StrConvrHelper {

    public String convertToString(byte[] bytes)
    {
        return Base64.getEncoder().encodeToString(bytes);
    }
}
