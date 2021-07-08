package com.spin.apps.password_manager.crypto.bciphers;

import com.spin.apps.password_manager.types.BlockCipherEngine;
import org.bouncycastle.crypto.BlockCipher;
import org.bouncycastle.crypto.BufferedBlockCipher;
import org.bouncycastle.crypto.engines.AESEngine;
import org.bouncycastle.crypto.engines.RijndaelEngine;
import org.bouncycastle.crypto.engines.SerpentEngine;
import org.bouncycastle.crypto.engines.TwofishEngine;
import org.bouncycastle.crypto.modes.SICBlockCipher;
import org.bouncycastle.crypto.paddings.PKCS7Padding;
import org.bouncycastle.crypto.paddings.PaddedBufferedBlockCipher;
import org.springframework.stereotype.Component;
import java.security.InvalidParameterException;

@Component
public class BlockCipherHelperFactory
{
    public BlockCipherHelper getTwofishHelper(boolean withPadding)
    {
        SICBlockCipher sicBlockCipher = new SICBlockCipher(new TwofishEngine());
        BufferedBlockCipher bufferedBlockCipher = null;

        if(withPadding)
            bufferedBlockCipher = new PaddedBufferedBlockCipher(sicBlockCipher, new PKCS7Padding());
        else
            bufferedBlockCipher = new BufferedBlockCipher(sicBlockCipher);

        return new BlockCipherHelper(bufferedBlockCipher, BlockCipherEngine.TWOFISH);
    }

    public BlockCipherHelper getRijndaelHelper(boolean withPadding)
    {
        SICBlockCipher sicBlockCipher = new SICBlockCipher(new RijndaelEngine());
        BufferedBlockCipher bufferedBlockCipher = null;

        if(withPadding)
            bufferedBlockCipher = new PaddedBufferedBlockCipher(sicBlockCipher, new PKCS7Padding());
        else
            bufferedBlockCipher = new BufferedBlockCipher(sicBlockCipher);

        return new BlockCipherHelper(bufferedBlockCipher, BlockCipherEngine.RIJNDAEL);
    }

    public BlockCipherHelper getSerpentHelper(boolean withPadding)
    {
        SICBlockCipher sicBlockCipher = new SICBlockCipher(new SerpentEngine());
        BufferedBlockCipher bufferedBlockCipher = null;

        if(withPadding)
            bufferedBlockCipher = new PaddedBufferedBlockCipher(sicBlockCipher, new PKCS7Padding());
        else
            bufferedBlockCipher = new BufferedBlockCipher(sicBlockCipher);

        return new BlockCipherHelper(bufferedBlockCipher, BlockCipherEngine.SERPENT);
    }

    public BlockCipherHelper getAESHelper(boolean withPadding)
    {
        SICBlockCipher sicBlockCipher = new SICBlockCipher(new AESEngine());
        BufferedBlockCipher bufferedBlockCipher = null;

        if(withPadding)
            bufferedBlockCipher = new PaddedBufferedBlockCipher(sicBlockCipher, new PKCS7Padding());
        else
            bufferedBlockCipher = new BufferedBlockCipher(sicBlockCipher);

        return new BlockCipherHelper(bufferedBlockCipher, BlockCipherEngine.AES);
    }

    public BlockCipherHelper getHelper(BlockCipherEngine engines, boolean withPadding)
    {
        switch (engines)
        {
            case AES:
                return getAESHelper(withPadding);
            case SERPENT:
                return getSerpentHelper(withPadding);
            case TWOFISH:
                return getSerpentHelper(withPadding);
            case RIJNDAEL:
                return getRijndaelHelper(withPadding);
            default:
                throw new InvalidParameterException("Engine not supported");
        }
    }

    public BlockCipherHelper getHelper(BlockCipher blockCipher, boolean withPadding)
    {
        SICBlockCipher sicBlockCipher = new SICBlockCipher(blockCipher);
        BufferedBlockCipher bufferedBlockCipher = null;

        if(withPadding)
            bufferedBlockCipher = new PaddedBufferedBlockCipher(sicBlockCipher, new PKCS7Padding());
        else
            bufferedBlockCipher = new BufferedBlockCipher(sicBlockCipher);

        return new BlockCipherHelper(bufferedBlockCipher, BlockCipherEngine.OTHER);
    }
}
