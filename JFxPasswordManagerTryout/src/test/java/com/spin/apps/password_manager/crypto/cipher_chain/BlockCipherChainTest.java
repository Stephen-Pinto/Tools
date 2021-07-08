package com.spin.apps.password_manager.crypto.cipher_chain;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.spin.apps.password_manager.crypto.PasswordStrengthener;
import com.spin.apps.password_manager.crypto.RandomDataBuilder;
import com.spin.apps.password_manager.types.BlockCipherEngine;
import com.spin.apps.password_manager.types.CustomField;
import com.spin.apps.password_manager.types.PrivateCard;
import com.spin.apps.password_manager.util.StrConvrHelper;
import org.bouncycastle.crypto.InvalidCipherTextException;
import org.bouncycastle.crypto.params.ParametersWithIV;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import java.util.Date;

//@TestPropertySource(value = "/../../../main/resources/application.properties")
//@ContextConfiguration(classes = {MainClass.class}, loader = AnnotationConfigContextLoader.class)
//@EnableAutoConfiguration(exclude={DataSourceAutoConfiguration.class})
//@SpringBootApplication

@RunWith( SpringJUnit4ClassRunner.class )
@SpringBootTest
public class BlockCipherChainTest
{
    @Autowired
    BlockCipherChain chainCipher;
    @Autowired
    PasswordStrengthener strengthener;
    @Autowired
    StrConvrHelper strConvrHelper;

    @Test
    public void test()
    {
        //strengthener = new PasswordStrengthener();
        //strHelper = new StrHelper();
        //chainCipher = new BlockCipherChain();

        byte[] salt = strengthener.generateSalt();
        ParametersWithIV key = strengthener.generate_ParametersWithIV(("stephen").toCharArray(), salt, 2000);

        PrivateCard card = new PrivateCard();
        card.setTitle("Test card");
        card.setDateCreated(new Date());
        card.setId(10);
        card.getFields().add(new CustomField("Field 1", "some value"));
        card.getFields().add(new CustomField("Field 2", "some other value"));
        Gson gson = new GsonBuilder().setDateFormat("dd.MM.yyyy HH:mm").create();

        chainCipher.put(BlockCipherEngine.TWOFISH, key);
        chainCipher.put(BlockCipherEngine.SERPENT, key);

        System.out.println();

        String jsonStr = gson.toJson(card);
        System.out.println(String.format("JSon string:\n%s", jsonStr));
        byte[] encryptedData = null, decryptedData = null;
        try
        {
            encryptedData = chainCipher.encrypt(jsonStr.getBytes());

            System.out.println(String.format("\nEncrypted str: \n%s", strConvrHelper.convertToString(encryptedData)));

            decryptedData = chainCipher.decrypt(encryptedData);

            String newJsonStr = new String(decryptedData);
            System.out.println(String.format("\nDecrypted str: \n%s", newJsonStr));

            PrivateCard newCardObj = gson.fromJson(newJsonStr, PrivateCard.class);
            Assert.assertEquals(card.getTitle(), newCardObj.getTitle());

        } catch (InvalidCipherTextException e) {
            e.printStackTrace();
            Assert.assertTrue(false);
        }
    }
}