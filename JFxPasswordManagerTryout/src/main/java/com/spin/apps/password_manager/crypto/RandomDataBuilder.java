package com.spin.apps.password_manager.crypto;

import com.spin.apps.password_manager.AppConfiguration;
import org.apache.commons.text.CharacterPredicates;
import org.apache.commons.text.RandomStringGenerator;
import org.bouncycastle.crypto.digests.SHA3Digest;
import org.bouncycastle.crypto.prng.DigestRandomGenerator;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import javax.crypto.spec.SecretKeySpec;
import java.security.SecureRandom;

@Component
public class RandomDataBuilder {

    @Autowired
    AppConfiguration configuration;

    //Random generator
    DigestRandomGenerator generator;

    public byte[] generateSalt(int size)
    {
        //If null create instance
        if(generator == null)
            generator = new DigestRandomGenerator(new SHA3Digest(256));

        //Generate salt by the specified size
        byte[] salt = new byte[size];
        generator.nextBytes(salt);
        return salt;
    }

    public byte[] generateRandomKey(int length)
    {
        SecureRandom secureRandom = new SecureRandom();
        byte[] dataBytes = new byte[length];
        SecretKeySpec newKey = new SecretKeySpec(dataBytes, "AES");
        return newKey.getEncoded();
    }

    public String generateRandomString(int length)
    {
        RandomStringGenerator rndmGnrtr =
                new RandomStringGenerator.Builder()
                        .withinRange('0', 'z')
                        .filteredBy(CharacterPredicates.LETTERS, CharacterPredicates.DIGITS)
                        .build();

        return rndmGnrtr.generate(length);
    }
}
