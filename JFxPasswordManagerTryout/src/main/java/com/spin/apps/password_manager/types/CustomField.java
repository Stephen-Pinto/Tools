package com.spin.apps.password_manager.types;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.RequiredArgsConstructor;

@Data
@AllArgsConstructor
@RequiredArgsConstructor
public class CustomField
{
    private final String name;
    private final String value;
    private String type;
}
