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
        LightPink = 0xFFFFB6C1, // ǳ�ۺ� 
        Pink = 0xFFFFC0CB, // �ۺ� 
        Crimson = 0xFFDC143C, // ���/�ɺ� 
        LavenderBlush = 0xFFFFF0F5, // ���Ϻ� 
        PaleVioletRed = 0xFFDB7093, // ���������� 
        HotPink = 0xFFFF69B4, // ����ķۺ� 
        DeepPink = 0xFFFF1493, // ��ۺ� 
        MediumVioletRed = 0xFFC71585, // ���������� 
        Orchid = 0xFFDA70D6, // ����ɫ/������ 
        Thistle = 0xFFD8BFD8, // ��ɫ 
        Plum = 0xFFDDA0DD, // ����ɫ/������ 
        Violet = 0xFFEE82EE, // ������ 
        Magenta = 0xFFFF00FF, // ���/õ��� 
        Fuchsia = 0xFFFF00FF, // �Ϻ�/�������� 
        DarkMagenta = 0xFF8B008B, // ����� 
        Purple = 0xFF800080, // ��ɫ 
        MediumOrchid = 0xFFBA55D3, // �������� 
        DarkViolet = 0xFF9400D3, // �������� 
        DarkOrchid = 0xFF9932CC, // �������� 
        Indigo = 0xFF4B0082, // ����/����ɫ 
        BlueViolet = 0xFF8A2BE2, // �������� 
        MediumPurple = 0xFF9370DB, // ����ɫ 
        MediumSlateBlue = 0xFF7B68EE, // �а���ɫ/�а����� 
        SlateBlue = 0xFF6A5ACD, // ʯ��ɫ/������ 
        DarkSlateBlue = 0xFF483D8B, // ������ɫ/�������� 
        Lavender = 0xFFE6E6FA, // ����ɫ/Ѭ�²ݵ��� 
        GhostWhite = 0xFFF8F8FF, // ����� 
        Blue = 0xFF0000FF, // ���� 
        MediumBlue = 0xFF0000CD, // ����ɫ 
        MidnightBlue = 0xFF191970, // ��ҹ�� 
        DarkBlue = 0xFF00008B, // ����ɫ 
        Navy = 0xFF000080, // ������ 
        RoyalBlue = 0xFF4169E1, // �ʼ���/���� 
        CornflowerBlue = 0xFF6495ED, // ʸ������ 
        LightSteelBlue = 0xFFB0C4DE, // ������ 
        LightSlateGray = 0xFF778899, // ������/��ʯ��� 
        SlateGray = 0xFF708090, // ��ʯɫ/ʯ��� 
        DodgerBlue = 0xFF1E90FF, // ����ɫ/������ 
        AliceBlue = 0xFFF0F8FF, // ����˿�� 
        SteelBlue = 0xFF4682B4, // ����/���� 
        LightSkyBlue = 0xFF87CEFA, // ������ɫ 
        SkyBlue = 0xFF87CEEB, // ����ɫ 
        DeepSkyBlue = 0xFF00BFFF, // ������ 
        LightBlue = 0xFFADD8E6, // ���� 
        PowderBlue = 0xFFB0E0E6, // ����ɫ/��ҩ�� 
        CadetBlue = 0xFF5F9EA0, // ����ɫ/������ 
        Azure = 0xFFF0FFFF, // ε��ɫ 
        LightCyan = 0xFFE0FFFF, // ����ɫ 
        PaleTurquoise = 0xFFAFEEEE, // ���̱�ʯ 
        Cyan = 0xFF00FFFF, // ��ɫ 
        Aqua = 0xFF00FFFF, // ǳ��ɫ/ˮɫ 
        DarkTurquoise = 0xFF00CED1, // ���̱�ʯ 
        DarkSlateGray = 0xFF2F4F4F, // ���߻�ɫ/��ʯ��� 
        DarkCyan = 0xFF008B8B, // ����ɫ 
        Teal = 0xFF008080, // ˮѼɫ 
        MediumTurquoise = 0xFF48D1CC, // ���̱�ʯ 
        LightSeaGreen = 0xFF20B2AA, // ǳ������ 
        Turquoise = 0xFF40E0D0, // �̱�ʯ 
        Aquamarine = 0xFF7FFFD4, // ��ʯ���� 
        MediumAquamarine = 0xFF66CDAA, // �б�ʯ���� 
        MediumSpringGreen = 0xFF00FA9A, // �д���ɫ 
        MintCream = 0xFFF5FFFA, // �������� 
        SpringGreen = 0xFF00FF7F, // ����ɫ 
        MediumSeaGreen = 0xFF3CB371, // �к����� 
        SeaGreen = 0xFF2E8B57, // ������ 
        Honeydew = 0xFFF0FFF0, // ��ɫ/�۹�ɫ 
        LightGreen = 0xFF90EE90, // ����ɫ 
        PaleGreen = 0xFF98FB98, // ����ɫ 
        DarkSeaGreen = 0xFF8FBC8F, // �������� 
        LimeGreen = 0xFF32CD32, // �������� 
        Lime = 0xFF00FF00, // ������ 
        ForestGreen = 0xFF228B22, // ɭ���� 
        Green = 0xFF008000, // ���� 
        DarkGreen = 0xFF006400, // ����ɫ 
        Chartreuse = 0xFF7FFF00, // ����ɫ/���ؾ��� 
        LawnGreen = 0xFF7CFC00, // ����ɫ/��ƺ�� 
        GreenYellow = 0xFFADFF2F, // �̻�ɫ 
        DarkOliveGreen = 0xFF556B2F, // ������� 
        YellowGreen = 0xFF9ACD32, // ����ɫ 
        OliveDrab = 0xFF6B8E23, // ��魺�ɫ 
        Beige = 0xFFF5F5DC, // ��ɫ/����ɫ 
        LightGoldenrodYellow = 0xFFFAFAD2, // ���ջ� 
        Ivory = 0xFFFFFFF0, // ����ɫ 
        LightYellow = 0xFFFFFFE0, // ǳ��ɫ 
        Yellow = 0xFFFFFF00, // ���� 
        Olive = 0xFF808000, // ��� 
        DarkKhaki = 0xFFBDB76B, // ���ƺ�ɫ/�ߴ�� 
        LemonChiffon = 0xFFFFFACD, // ���ʳ� 
        PaleGoldenrod = 0xFFEEE8AA, // �Ҿջ�/������ɫ 
        Khaki = 0xFFF0E68C, // �ƺ�ɫ/��ߴ�� 
        Gold = 0xFFFFD700, // ��ɫ 
        Cornsilk = 0xFFFFF8DC, // ����˿ɫ 
        Goldenrod = 0xFFDAA520, // ��ջ� 
        DarkGoldenrod = 0xFFB8860B, // ����ջ� 
        FloralWhite = 0xFFFFFAF0, // ���İ�ɫ 
        OldLace = 0xFFFDF5E6, // �ϻ�ɫ/����˿ 
        Wheat = 0xFFF5DEB3, // ǳ��ɫ/С��ɫ 
        Moccasin = 0xFFFFE4B5, // ¹Ƥɫ/¹Ƥѥ 
        Orange = 0xFFFFA500, // ��ɫ 
        PapayaWhip = 0xFFFFEFD5, // ��ľɫ/��ľ�� 
        BlanchedAlmond = 0xFFFFEBCD, // ����ɫ 
        NavajoWhite = 0xFFFFDEAD, // ���߰�/������ 
        AntiqueWhite = 0xFFFAEBD7, // �Ŷ��� 
        Tan = 0xFFD2B48C, // ��ɫ 
        BurlyWood = 0xFFDEB887, // Ӳľɫ 
        Bisque = 0xFFFFE4C4, // ������ 
        DarkOrange = 0xFFFF8C00, // ���ɫ 
        Linen = 0xFFFAF0E6, // ���鲼 
        Peru = 0xFFCD853F, // ��³ɫ 
        PeachPuff = 0xFFFFDAB9, // ����ɫ 
        SandyBrown = 0xFFF4A460, // ɳ��ɫ 
        Chocolate = 0xFFD2691E, // �ɿ���ɫ 
        SaddleBrown = 0xFF8B4513, // �غ�ɫ/����ɫ 
        Seashell = 0xFFFFF5EE, // ������ 
        Sienna = 0xFFA0522D, // ������ɫ 
        LightSalmon = 0xFFFFA07A, // ǳ������ɫ 
        Coral = 0xFFFF7F50, // ɺ�� 
        OrangeRed = 0xFFFF4500, // �Ⱥ�ɫ 
        DarkSalmon = 0xFFE9967A, // ������/����ɫ 
        Tomato = 0xFFFF6347, // ���Ѻ� 
        MistyRose = 0xFFFFE4E1, // ǳõ��ɫ/����õ�� 
        Salmon = 0xFFFA8072, // ����/����ɫ 
        Snow = 0xFFFFFAFA, // ѩ��ɫ 
        LightCoral = 0xFFF08080, // ��ɺ��ɫ 
        RosyBrown = 0xFFBC8F8F, // õ����ɫ 
        IndianRed = 0xFFCD5C5C, // ӡ�Ⱥ� 
        Red = 0xFFFF0000, // ���� 
        Brown = 0xFFA52A2A, // ��ɫ 
        FireBrick = 0xFFB22222, // ��שɫ/�ͻ�ש 
        DarkRed = 0xFF8B0000, // ���ɫ 
        Maroon = 0xFF800000, // ��ɫ 
        White = 0xFFFFFFFF, // ���� 
        WhiteSmoke = 0xFFF5F5F5, // ���� 
        Gainsboro = 0xFFDCDCDC, // ����ɫ 
        LightGrey = 0xFFD3D3D3, // ǳ��ɫ 
        Silver = 0xFFC0C0C0, // ����ɫ 
        DarkGray = 0xFFA9A9A9, // ���ɫ 
        Gray = 0xFF808080, // ��ɫ 
        DimGray = 0xFF696969, // ������ 
        Black = 0xFF000000, // ���� 
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

