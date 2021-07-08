package com.spin.apps.password_manager;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.spin.apps.password_manager.crypto.PasswordStrengthener;
import com.spin.apps.password_manager.crypto.RandomDataBuilder;
import com.spin.apps.password_manager.crypto.bciphers.BlockCipherHelper;
import com.spin.apps.password_manager.crypto.bciphers.BlockCipherHelperFactory;
import com.spin.apps.password_manager.crypto.cipher_chain.BlockCipherChain;
import com.spin.apps.password_manager.data.DataPadder;
import com.spin.apps.password_manager.types.BlockCipherEngine;
import com.spin.apps.password_manager.types.CustomField;
import com.spin.apps.password_manager.types.PrivateCard;
import com.spin.apps.password_manager.util.StrConvrHelper;
import org.bouncycastle.crypto.params.ParametersWithIV;
import org.springframework.beans.factory.annotation.Autowired;
import java.util.Calendar;
import java.util.Date;

public class GeneralTests {

    @Autowired
    BlockCipherHelperFactory blockCipherHelperFactory;

    @Autowired
    public StrConvrHelper strConvrHelper;

    @Autowired
    public BlockCipherChain cipherChain;

    @Autowired
    public PasswordStrengthener strengthener;

    public void testImpl()
    {
        byte[] salt = strengthener.generateSalt();

        System.out.println(String.format("Salt : %s \nWith length : %d", strConvrHelper.convertToString(salt), salt.length));
        System.out.print("\n");

        byte[] hash1 = strengthener.generate_Key(("stephen").toCharArray(), salt);
        byte[] hash2 = strengthener.generate_Key(("stephen").toCharArray(), salt);

        System.out.println(String.format("Hash1 : %s \nWith length : %d", strConvrHelper.convertToString(hash1), hash1.length));
        System.out.print("\n");
        System.out.println(String.format("Hash2 : %s \nWith length : %d", strConvrHelper.convertToString(hash2), hash2.length));
    }

    public void testCipher()
    {
        try {

            byte[] salt = strengthener.generateSalt();
            BlockCipherHelper helper = null;
            ParametersWithIV key = strengthener.generate_ParametersWithIV(("stephen").toCharArray(), salt, 2000);
            //helper = blockCipherHelperFactory.getTwofishHelper(true);
            helper = blockCipherHelperFactory.getRijndaelHelper(false);
            //helper = new SerpentHelper(key);
            //helper = new AESHelper(key);

            byte[] plainText = "This is my hello crypto world".getBytes();
            byte[] cipherText = helper.encrypt(key, plainText);
            System.out.println("Cipher text : \n" + strConvrHelper.convertToString(cipherText));

            System.out.println("\n");

            byte[] newPlainText = helper.decrypt(key, cipherText);
            System.out.println("De-Cipher text : \n" + new String(newPlainText));
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    public void testCipherChain()
    {
        try {

            byte[] salt = strengthener.generateSalt();
            ParametersWithIV key = strengthener.generate_ParametersWithIV(("stephen").toCharArray(), salt, 2000);
            cipherChain.put(BlockCipherEngine.TWOFISH, key);
            cipherChain.put(BlockCipherEngine.RIJNDAEL, key);
            cipherChain.put(BlockCipherEngine.SERPENT, key);

            System.out.println();
            byte[] plainText = "This is my hello crypto world".getBytes();
            System.out.println(String.format("Plain text:\n%s\nLength: %d", new String(plainText), plainText.length));

            byte[] cipherText = cipherChain.encrypt(plainText);
            System.out.println(String.format("\nCipher text:\n%s\nLength: %d", strConvrHelper.convertToString(cipherText), cipherText.length));

            byte[] newPlainText = cipherChain.decrypt(cipherText);
            System.out.println(String.format("\nDe-Cipher text:\n%s\nLength: %d", new String(newPlainText), newPlainText.length));
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    public void testPadding()
    {
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(new Date());

        PrivateCard card = new PrivateCard();
        card.setTitle("my private card");
        card.setDateCreated(calendar.getTime());

        calendar.add(Calendar.DATE, 15);
        card.setDateExpiry(calendar.getTime());

        card.getFields().add(new CustomField("somefield", "somevalue"));
        card.getFields().add(new CustomField("someotherfield", "someothervalue"));
        //Gson gson = new GsonBuilder().setDateFormat("dd.MM.yyyy HH:mm").setPrettyPrinting().create();
        Gson gson = new GsonBuilder().setDateFormat("dd.MM.yyyy HH:mm").create();
        String json = gson.toJson(card);

        System.out.println(String.format("Private card string:\n%s\nLength: %d", json, json.length()));
        DataPadder padder = new DataPadder();
        String paddedData = padder.pad(json, 400);
        String unpaddedData = padder.unpad(paddedData);
        System.out.println(String.format("Padded private card string:\n%s\nLength: %d", paddedData, paddedData.length()));
        System.out.println(String.format("Unpadded private card string:\n%s\nLength: %d", unpaddedData, unpaddedData.length()));
    }

    public void RunTests()
    {
        //testCipher();
        testCipherChain();
        //testPadding();
        //System.out.println("Hello there !");
    }
}
