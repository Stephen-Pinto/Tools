package com.spin.apps.password_manager.crypto.cipher_chain;

import com.spin.apps.password_manager.crypto.bciphers.BlockCipherHelper;
import com.spin.apps.password_manager.crypto.bciphers.BlockCipherHelperFactory;
import com.spin.apps.password_manager.types.BlockCipherEngine;
import org.apache.commons.lang3.ArrayUtils;
import org.bouncycastle.crypto.InvalidCipherTextException;
import org.bouncycastle.crypto.params.ParametersWithIV;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import java.util.*;

@Component
public class BlockCipherChain extends ArrayList<AbstractMap.SimpleImmutableEntry<BlockCipherEngine, ParametersWithIV>> {

    @Autowired
    private BlockCipherHelperFactory helperFactory;

    ArrayList<BlockCipherHelper> blockCipherHelperList = null;

    public BlockCipherChain()
    {
    }

    protected void init()
    {
        //We prepare or clear list of all helpers
        if(blockCipherHelperList == null)
            blockCipherHelperList = new ArrayList<>();
        else
            blockCipherHelperList.clear();

        for (int i = 0; i < this.size(); i++)
        {
            BlockCipherHelper blockCipherHelper;

            //Since we want padding for the last item we check if its last and allow padding else normal
            if(i == (this.size() - 1))
                blockCipherHelper = helperFactory.getHelper(get(i).getKey(), true);
            else
                blockCipherHelper = helperFactory.getHelper(get(i).getKey(), false);

            //Add to list
            blockCipherHelperList.add(blockCipherHelper);
        }
    }

    public void put(BlockCipherEngine engine, ParametersWithIV key)
    {
        this.add(new AbstractMap.SimpleImmutableEntry<>(engine, key));
    }

    public byte[] encrypt(byte[] plainText) throws InvalidCipherTextException
    {
        //If cipher list is not prepared prepare it first
        if(blockCipherHelperList == null || blockCipherHelperList.size() != this.size())
            init();

        byte[] output = prependBlockSize(plainText);

        for (int i = 0; i < this.size(); i++)
        {
            BlockCipherHelper helper = blockCipherHelperList.get(i);
            output = helper.encrypt(this.get(i).getValue(), output);
        }

        return output;
    }

    public byte[] decrypt(byte[] cipherText) throws InvalidCipherTextException
    {
        //If cipher list is not prepared prepare it first
        if(blockCipherHelperList == null || blockCipherHelperList.size() != this.size())
            init();

        byte[] output = cipherText;

        for (int j = this.size() - 1; j >= 0 ; j--)
        {
            BlockCipherHelper helper = blockCipherHelperList.get(j);
            output = helper.decrypt(this.get(j).getValue(), output);
        }

        return stripBlockSize(output);
    }

    private byte[] prependBlockSize(byte[] originalData)
    {
        byte[] dataLenTag = ("[" + originalData.length + "]").getBytes();
        byte[] newData = ArrayUtils.addAll(dataLenTag, originalData);
        System.out.println("New data : " + new String(newData));
        return newData;
    }

    private byte[] stripBlockSize(byte[] data)
    {
        String numStr = "";

        if((char)data[0] != '[')
            return null;

        int i = 1;
        for (; i < data.length; i++)
        {
            if((char)data[i] == ']')
                break;

            numStr += (char)data[i];
        }

        if(i == data.length)
            return null;

        Integer size = Integer.parseInt(numStr);
        int index = i + 1;
        byte[] originalData = Arrays.copyOfRange(data, index, size + index);
        return originalData;
    }
}
