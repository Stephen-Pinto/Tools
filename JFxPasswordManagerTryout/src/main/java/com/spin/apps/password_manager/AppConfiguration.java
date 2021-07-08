package com.spin.apps.password_manager;

import lombok.Data;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.PropertySource;

/*
* This class is the main configuration file for the application.
* Here we can change key size and other such properties which we will refer
* throughout the application. So one change here should change the behaviour of the
* application.
*/

@Data
@Configuration
@PropertySource(value = "classpath:passwordman.properties")
@ConfigurationProperties(prefix = "appconf")
public class AppConfiguration {

    private int keySize;

    public int getLengthInBits()
    {
        return keySize;
    }

    public int getLengthInBytes()
    {
        return (keySize / 8);
    }
}
