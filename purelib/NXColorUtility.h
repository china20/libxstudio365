//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#ifndef _INS_COLORUTILITY_H_
#define _INS_COLORUTILITY_H_
#include <cocos2d.h>

USING_NS_CC;

namespace color_utility
{

    // color values
    typedef enum {
        LightPink = 0xFFFFB6C1, // Ç³·Ûºì 
        Pink = 0xFFFFC0CB, // ·Ûºì 
        Crimson = 0xFFDC143C, // Éîºì/ÐÉºì 
        LavenderBlush = 0xFFFFF0F5, // µ­×Ïºì 
        PaleVioletRed = 0xFFDB7093, // Èõ×ÏÂÞÀ¼ºì 
        HotPink = 0xFFFF69B4, // ÈÈÇéµÄ·Ûºì 
        DeepPink = 0xFFFF1493, // Éî·Ûºì 
        MediumVioletRed = 0xFFC71585, // ÖÐ×ÏÂÞÀ¼ºì 
        Orchid = 0xFFDA70D6, // °µ×ÏÉ«/À¼»¨×Ï 
        Thistle = 0xFFD8BFD8, // ¼»É« 
        Plum = 0xFFDDA0DD, // ÑóÀîÉ«/Àî×Ó×Ï 
        Violet = 0xFFEE82EE, // ×ÏÂÞÀ¼ 
        Magenta = 0xFFFF00FF, // Ñóºì/Ãµ¹åºì 
        Fuchsia = 0xFFFF00FF, // ×Ïºì/µÆÁýº£ÌÄ 
        DarkMagenta = 0xFF8B008B, // ÉîÑóºì 
        Purple = 0xFF800080, // ×ÏÉ« 
        MediumOrchid = 0xFFBA55D3, // ÖÐÀ¼»¨×Ï 
        DarkViolet = 0xFF9400D3, // °µ×ÏÂÞÀ¼ 
        DarkOrchid = 0xFF9932CC, // °µÀ¼»¨×Ï 
        Indigo = 0xFF4B0082, // µåÇà/×ÏÀ¼É« 
        BlueViolet = 0xFF8A2BE2, // À¶×ÏÂÞÀ¼ 
        MediumPurple = 0xFF9370DB, // ÖÐ×ÏÉ« 
        MediumSlateBlue = 0xFF7B68EE, // ÖÐ°µÀ¶É«/ÖÐ°åÑÒÀ¶ 
        SlateBlue = 0xFF6A5ACD, // Ê¯À¶É«/°åÑÒÀ¶ 
        DarkSlateBlue = 0xFF483D8B, // °µ»ÒÀ¶É«/°µ°åÑÒÀ¶ 
        Lavender = 0xFFE6E6FA, // µ­×ÏÉ«/Ñ¬ÒÂ²Ýµ­×Ï 
        GhostWhite = 0xFFF8F8FF, // ÓÄÁé°× 
        Blue = 0xFF0000FF, // ´¿À¶ 
        MediumBlue = 0xFF0000CD, // ÖÐÀ¶É« 
        MidnightBlue = 0xFF191970, // ÎçÒ¹À¶ 
        DarkBlue = 0xFF00008B, // °µÀ¶É« 
        Navy = 0xFF000080, // º£¾üÀ¶ 
        RoyalBlue = 0xFF4169E1, // »Ê¼ÒÀ¶/±¦À¶ 
        CornflowerBlue = 0xFF6495ED, // Ê¸³µ¾ÕÀ¶ 
        LightSteelBlue = 0xFFB0C4DE, // ÁÁ¸ÖÀ¶ 
        LightSlateGray = 0xFF778899, // ÁÁÀ¶»Ò/ÁÁÊ¯°å»Ò 
        SlateGray = 0xFF708090, // »ÒÊ¯É«/Ê¯°å»Ò 
        DodgerBlue = 0xFF1E90FF, // ÉÁÀ¼É«/µÀÆæÀ¶ 
        AliceBlue = 0xFFF0F8FF, // °®ÀöË¿À¶ 
        SteelBlue = 0xFF4682B4, // ¸ÖÀ¶/ÌúÇà 
        LightSkyBlue = 0xFF87CEFA, // ÁÁÌìÀ¶É« 
        SkyBlue = 0xFF87CEEB, // ÌìÀ¶É« 
        DeepSkyBlue = 0xFF00BFFF, // ÉîÌìÀ¶ 
        LightBlue = 0xFFADD8E6, // ÁÁÀ¶ 
        PowderBlue = 0xFFB0E0E6, // ·ÛÀ¶É«/»ðÒ©Çà 
        CadetBlue = 0xFF5F9EA0, // ¾üÀ¼É«/¾ü·þÀ¶ 
        Azure = 0xFFF0FFFF, // ÎµÀ¶É« 
        LightCyan = 0xFFE0FFFF, // µ­ÇàÉ« 
        PaleTurquoise = 0xFFAFEEEE, // ÈõÂÌ±¦Ê¯ 
        Cyan = 0xFF00FFFF, // ÇàÉ« 
        Aqua = 0xFF00FFFF, // Ç³ÂÌÉ«/Ë®É« 
        DarkTurquoise = 0xFF00CED1, // °µÂÌ±¦Ê¯ 
        DarkSlateGray = 0xFF2F4F4F, // °µÍß»ÒÉ«/°µÊ¯°å»Ò 
        DarkCyan = 0xFF008B8B, // °µÇàÉ« 
        Teal = 0xFF008080, // Ë®Ñ¼É« 
        MediumTurquoise = 0xFF48D1CC, // ÖÐÂÌ±¦Ê¯ 
        LightSeaGreen = 0xFF20B2AA, // Ç³º£ÑóÂÌ 
        Turquoise = 0xFF40E0D0, // ÂÌ±¦Ê¯ 
        Aquamarine = 0xFF7FFFD4, // ±¦Ê¯±ÌÂÌ 
        MediumAquamarine = 0xFF66CDAA, // ÖÐ±¦Ê¯±ÌÂÌ 
        MediumSpringGreen = 0xFF00FA9A, // ÖÐ´ºÂÌÉ« 
        MintCream = 0xFFF5FFFA, // ±¡ºÉÄÌÓÍ 
        SpringGreen = 0xFF00FF7F, // ´ºÂÌÉ« 
        MediumSeaGreen = 0xFF3CB371, // ÖÐº£ÑóÂÌ 
        SeaGreen = 0xFF2E8B57, // º£ÑóÂÌ 
        Honeydew = 0xFFF0FFF0, // ÃÛÉ«/ÃÛ¹ÏÉ« 
        LightGreen = 0xFF90EE90, // µ­ÂÌÉ« 
        PaleGreen = 0xFF98FB98, // ÈõÂÌÉ« 
        DarkSeaGreen = 0xFF8FBC8F, // °µº£ÑóÂÌ 
        LimeGreen = 0xFF32CD32, // ÉÁ¹âÉîÂÌ 
        Lime = 0xFF00FF00, // ÉÁ¹âÂÌ 
        ForestGreen = 0xFF228B22, // É­ÁÖÂÌ 
        Green = 0xFF008000, // ´¿ÂÌ 
        DarkGreen = 0xFF006400, // °µÂÌÉ« 
        Chartreuse = 0xFF7FFF00, // »ÆÂÌÉ«/²éÌØ¾ÆÂÌ 
        LawnGreen = 0xFF7CFC00, // ²ÝÂÌÉ«/²ÝÆºÂÌ 
        GreenYellow = 0xFFADFF2F, // ÂÌ»ÆÉ« 
        DarkOliveGreen = 0xFF556B2F, // °µéÏé­ÂÌ 
        YellowGreen = 0xFF9ACD32, // »ÆÂÌÉ« 
        OliveDrab = 0xFF6B8E23, // éÏé­ºÖÉ« 
        Beige = 0xFFF5F5DC, // Ã×É«/»Ò×ØÉ« 
        LightGoldenrodYellow = 0xFFFAFAD2, // ÁÁ¾Õ»Æ 
        Ivory = 0xFFFFFFF0, // ÏóÑÀÉ« 
        LightYellow = 0xFFFFFFE0, // Ç³»ÆÉ« 
        Yellow = 0xFFFFFF00, // ´¿»Æ 
        Olive = 0xFF808000, // éÏé­ 
        DarkKhaki = 0xFFBDB76B, // °µ»ÆºÖÉ«/Éî¿¨ß´²¼ 
        LemonChiffon = 0xFFFFFACD, // ÄûÃÊ³ñ 
        PaleGoldenrod = 0xFFEEE8AA, // »Ò¾Õ»Æ/²Ô÷è÷ëÉ« 
        Khaki = 0xFFF0E68C, // »ÆºÖÉ«/¿¨ß´²¼ 
        Gold = 0xFFFFD700, // ½ðÉ« 
        Cornsilk = 0xFFFFF8DC, // ÓñÃ×Ë¿É« 
        Goldenrod = 0xFFDAA520, // ½ð¾Õ»Æ 
        DarkGoldenrod = 0xFFB8860B, // °µ½ð¾Õ»Æ 
        FloralWhite = 0xFFFFFAF0, // »¨µÄ°×É« 
        OldLace = 0xFFFDF5E6, // ÀÏ»¨É«/¾ÉÀÙË¿ 
        Wheat = 0xFFF5DEB3, // Ç³»ÆÉ«/Ð¡ÂóÉ« 
        Moccasin = 0xFFFFE4B5, // Â¹Æ¤É«/Â¹Æ¤Ñ¥ 
        Orange = 0xFFFFA500, // ³ÈÉ« 
        PapayaWhip = 0xFFFFEFD5, // ·¬Ä¾É«/·¬Ä¾¹Ï 
        BlanchedAlmond = 0xFFFFEBCD, // °×ÐÓÉ« 
        NavajoWhite = 0xFFFFDEAD, // ÄÉÍß°×/ÍÁÖø°× 
        AntiqueWhite = 0xFFFAEBD7, // ¹Å¶­°× 
        Tan = 0xFFD2B48C, // ²èÉ« 
        BurlyWood = 0xFFDEB887, // Ó²Ä¾É« 
        Bisque = 0xFFFFE4C4, // ÌÕÅ÷»Æ 
        DarkOrange = 0xFFFF8C00, // Éî³ÈÉ« 
        Linen = 0xFFFAF0E6, // ÑÇÂé²¼ 
        Peru = 0xFFCD853F, // ÃØÂ³É« 
        PeachPuff = 0xFFFFDAB9, // ÌÒÈâÉ« 
        SandyBrown = 0xFFF4A460, // É³×ØÉ« 
        Chocolate = 0xFFD2691E, // ÇÉ¿ËÁ¦É« 
        SaddleBrown = 0xFF8B4513, // ÖØºÖÉ«/Âí°°×ØÉ« 
        Seashell = 0xFFFFF5EE, // º£±´¿Ç 
        Sienna = 0xFFA0522D, // »ÆÍÁô÷É« 
        LightSalmon = 0xFFFFA07A, // Ç³öÙÓãÈâÉ« 
        Coral = 0xFFFF7F50, // Éºº÷ 
        OrangeRed = 0xFFFF4500, // ³ÈºìÉ« 
        DarkSalmon = 0xFFE9967A, // ÉîÏÊÈâ/öÙÓãÉ« 
        Tomato = 0xFFFF6347, // ·¬ÇÑºì 
        MistyRose = 0xFFFFE4E1, // Ç³Ãµ¹åÉ«/±¡ÎíÃµ¹å 
        Salmon = 0xFFFA8072, // ÏÊÈâ/öÙÓãÉ« 
        Snow = 0xFFFFFAFA, // Ñ©°×É« 
        LightCoral = 0xFFF08080, // µ­Éºº÷É« 
        RosyBrown = 0xFFBC8F8F, // Ãµ¹å×ØÉ« 
        IndianRed = 0xFFCD5C5C, // Ó¡¶Èºì 
        Red = 0xFFFF0000, // ´¿ºì 
        Brown = 0xFFA52A2A, // ×ØÉ« 
        FireBrick = 0xFFB22222, // »ð×©É«/ÄÍ»ð×© 
        DarkRed = 0xFF8B0000, // ÉîºìÉ« 
        Maroon = 0xFF800000, // ÀõÉ« 
        White = 0xFFFFFFFF, // ´¿°× 
        WhiteSmoke = 0xFFF5F5F5, // °×ÑÌ 
        Gainsboro = 0xFFDCDCDC, // µ­»ÒÉ« 
        LightGrey = 0xFFD3D3D3, // Ç³»ÒÉ« 
        Silver = 0xFFC0C0C0, // Òø»ÒÉ« 
        DarkGray = 0xFFA9A9A9, // Éî»ÒÉ« 
        Gray = 0xFF808080, // »ÒÉ« 
        DimGray = 0xFF696969, // °µµ­»Ò 
        Black = 0xFF000000, // ´¿ºÚ 
    } Color;

    static Color3B toccc3b(uint32_t value)
    {
        return Color3B((value >> 16) & 0xff, (value >> 8) & 0xff, (value) & 0xff);
    }

    static Color4B toccc4b(uint32_t value)
    {
        return Color4B((value >> 16) & 0xff, (value >> 8) & 0xff, (value) & 0xff, (value >> 24) & 0xff);
    }

    static uint32_t c3b2dw(const Color3B& value)
    {
        auto rvalue =  (uint32_t)0xff << 24 |
            (uint32_t)value.b << 16 |
            (uint32_t)value.g << 8 |
            (uint32_t)value.r;
        return rvalue;
    }

    static uint32_t c4b2dw(const Color4B& value)
    {
        auto rvalue =  (uint32_t)value.a << 24 |
            (uint32_t)value.b << 16 |
            (uint32_t)value.g << 8 |
            (uint32_t)value.r;
        return rvalue;
    }
};

#endif

