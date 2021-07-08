package com.spin.apps.password_manager.crypto;

import com.spin.apps.password_manager.AppConfiguration;
import org.bouncycastle.crypto.PBEParametersGenerator;
import org.bouncycastle.crypto.generators.PKCS5S2ParametersGenerator;
import org.bouncycastle.crypto.params.KeyParameter;
import org.bouncycastle.crypto.params.ParametersWithIV;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Configuration;
import org.springframework.stereotype.Component;

import javax.crypto.spec.SecretKeySpec;

@Component
@Configuration
public class PasswordStrengthener {

    @Autowired
    AppConfiguration configuration;

    @Autowired
    RandomDataBuilder randomDataBuilder;

    //Generator for PBKDF2 according to PKCS5-S2 specification
    final PBEParametersGenerator generator = new PKCS5S2ParametersGenerator();

    public byte[] generateSalt()
    {
        return randomDataBuilder.generateSalt(configuration.getLengthInBytes());
    }

    public byte[] generateKey()
    {
        return randomDataBuilder.generateRandomKey(configuration.getLengthInBytes());
    }

    public byte[] generate_Key(char[] password, byte[] salt)
    {
        return generate_Key(password, salt, 2000);
    }

    public byte[] generate_Key(char[] password, byte[] salt, int iterationCount)
    {
        generator.init(PBEParametersGenerator.PKCS5PasswordToUTF8Bytes(password), salt, iterationCount);
        KeyParameter parameter = (KeyParameter) generator.generateDerivedParameters(configuration.getLengthInBits());
        return parameter.getKey();
    }

    public ParametersWithIV generate_ParametersWithIV(char[] password, byte[] salt, int iterationCount)
    {
        generator.init(PBEParametersGenerator.PKCS5PasswordToUTF8Bytes(password), salt, iterationCount);
        return (ParametersWithIV) generator.generateDerivedParameters(configuration.getLengthInBits(), configuration.getLengthInBits());
    }
}
