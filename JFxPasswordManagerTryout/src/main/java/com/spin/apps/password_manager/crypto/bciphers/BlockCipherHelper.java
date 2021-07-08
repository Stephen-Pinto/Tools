package com.spin.apps.password_manager.crypto.bciphers;

import com.spin.apps.password_manager.types.BlockCipherEngine;
import org.bouncycastle.crypto.BufferedBlockCipher;
import org.bouncycastle.crypto.InvalidCipherTextException;
import org.bouncycastle.crypto.params.ParametersWithIV;

public class BlockCipherHelper
{
    protected BufferedBlockCipher bufferedBlockCipher;
    protected BlockCipherEngine engine;

    public BlockCipherEngine getEngine() {
        return engine;
    }

    public BlockCipherHelper(BufferedBlockCipher blockCipher, BlockCipherEngine engine)
    {
        this.engine = engine;
        this.bufferedBlockCipher = blockCipher;
    }

    public byte[] encrypt(ParametersWithIV key, byte[] data) throws InvalidCipherTextException
    {
        bufferedBlockCipher.init(true, key);
        byte[] cipherText = process(data);
        return cipherText;
    }

    public byte[] decrypt(ParametersWithIV key, byte[] data) throws InvalidCipherTextException
    {
        bufferedBlockCipher.init(false, key);
        byte[] plainText = process(data);
        return plainText;
    }

    protected byte[] process(byte[] input) throws InvalidCipherTextException
    {
        int bytesProcessed = 0;

        //Prepare output buffer
        byte[] output = new byte[bufferedBlockCipher.getOutputSize(input.length)];

        //Process these bytes
        bytesProcessed = bufferedBlockCipher.processBytes(
                input,
                0,
                input.length,
                output,
                0
        );

        //Finalize
        bufferedBlockCipher.doFinal(output, bytesProcessed);
        return output;
    }
}
