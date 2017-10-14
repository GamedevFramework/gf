/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef GF_CONSOLE_CHAR_H
#define GF_CONSOLE_CHAR_H

#include "Portability.h"

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  /**
   * @ingroup game
   * @brief Named console characters
   *
   * The name of the characters is the name defined in Unicode.
   *
   * @sa gf::Console
   */
  struct GF_API ConsoleChar final {
    /**
     * @brief Deleted copy constructor
     */
    ConsoleChar() = delete;

    /**
     * @brief U+263A White Smiling Face
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t WhiteSmilingFace = u'☺';

    /**
     * @brief U+263B Black Smiling Face
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BlackSmilingFace = u'☻';

    /**
     * @brief U+2665 Black Heart Suit
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BlackHeartSuit = u'♥';

    /**
     * @brief U+2666 Black Diamond Suit
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BlackDiamondSuit = u'♦';


    /**
     * @brief U+2663 Black Club Suit
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BlackClubSuit = u'♣';

    /**
     * @brief U+2660 Black Spade Suit
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BlackSpadeSuit = u'♠';

    /**
     * @brief U+2022 Bullet
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t Bullet = u'•';

    /**
     * @brief U+25D8 Inverse Bullet
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t InverseBullet = u'◘';

    /**
     * @brief U+25CB White Circle
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t WhiteCircle = u'○';

    /**
     * @brief U+25D9 Inverse White Circle
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t InverseWhiteCircle = u'◙';

    /**
     * @brief U+2642 Male Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t MaleSign = u'♂';

    /**
     * @brief U+2640 Female Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t FemaleSign = u'♀';

    /**
     * @brief U+266A Eighth Note
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t EighthNote = u'♪';

    /**
     * @brief U+266B Beamed Eighth Notes
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BeamedEighthNotes = u'♫';

    /**
     * @brief U+263C White Sun With Rays
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t WhiteSunWithRays = u'☼';

    /**
     * @brief U+25BA Black Right-Pointing Pointer
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BlackRightPointingPointer = u'►';

    /**
     * @brief U+25C4 Black Left-Pointing Pointer
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BlackLeftPointingPointer = u'◄';

    /**
     * @brief U+2195 Up Down Arrow
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t UpDownArrow = u'↕';

    /**
     * @brief U+203C Double Exclamation Mark
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t DoubleExclamationMark = u'‼';

    /**
     * @brief U+00B6 Pilcrow Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t PilcrowSign = u'¶';

    /**
     * @brief U+00A7 Section Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t SectionSign = u'§';

    /**
     * @brief U+25AC Black Rectangle
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BlackRectangle = u'▬';

    /**
     * @brief U+21A8 Up Down Arrow With Base
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t UpDownArrowWithBase = u'↨';

    /**
     * @brief U+2191 Upwards Arrow
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t UpwardsArrow = u'↑';

    /**
     * @brief U+2193 Downwards Arrow
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t DownwardsArrow = u'↓';

    /**
     * @brief U+2192 Rightwards Arrow
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t RightwardsArrow = u'→';

    /**
     * @brief U+2190 Leftwards Arrow
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t LeftwardsArrow = u'←';

    /**
     * @brief U+221F Right Angle
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t RightAngle = u'∟';

    /**
     * @brief U+2194 Left Right Arrow
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t LeftRightArrow = u'↔';

    /**
     * @brief U+25B2 Black Up-Pointing Triangle
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BlackUpPointingTriangle = u'▲';

    /**
     * @brief U+25BC Black Down-Pointing Triangle
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BlackDownPointingTriangle = u'▼';

    /**
     * @brief U+2302 House
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t House = u'⌂';

    /**
     * @brief U+00A2 Cent Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t CentSign = u'¢';

    /**
     * @brief U+00A3 Pound Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t PoundSign = u'£';

    /**
     * @brief U+00A5 Yen Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t YenSign = u'¥';

    /**
     * @brief U+20A7 Peseta Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t PesetaSign = u'₧';

    /**
     * @brief U+0192 Latin Small Letter F With Hook
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t LatinSmallLetterFWithHook = u'ƒ';

    /**
     * @brief U+00AA Feminine Ordinal Indicator
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t FeminineOrdinalIndicator = u'ª';

    /**
     * @brief U+00BA Masculine Ordinal Indicator
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t MasculineOrdinalIndicator = u'º';

    /**
     * @brief U+00BF Inverted Question Mark
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t InvertedQuestionMark = u'¿';

    /**
     * @brief U+2310 Reversed Not Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t ReversedNotSign = u'⌐';

    /**
     * @brief U+00AC Not Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t NotSign = u'¬';

    /**
     * @brief U+00BD Vulgar Fraction One Half
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t VulgarFractionOneHalf = u'½';

    /**
     * @brief U+00BC Vulgar Fraction One Quarter
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t VulgarFractionOneQuarter = u'¼';

    /**
     * @brief U+00A1 Inverted Exclamation Mark
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t InvertedExclamationMark = u'¡';

    /**
     * @brief U+00AB Left-Pointing Double Angle Quotation Mark
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t LeftPointingDoubleAngleQuotationMark = u'«';

    /**
     * @brief U+00BB Right-Pointing Double Angle Quotation Mark
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t RightPointingDoubleAngleQuotationMark = u'»';

    /**
     * @brief U+2591 Light Shade
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t LightShade = u'░';

    /**
     * @brief U+2592 Medium Shade
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t MediumShade = u'▒';

    /**
     * @brief U+2593 Dark Shade
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t DarkShade = u'▓';

    /**
     * @brief U+2502 Box Drawings Light Vertical
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsLightVertical = u'│';

    /**
     * @brief U+2524 Box Drawings Light Vertical And Left
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsLightVerticalAndLeft = u'┤';

    /**
     * @brief U+2561 Box Drawings Vertical Single And Left Double
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsVerticalSingleAndLeftDouble = u'╡';

    /**
     * @brief U+2562 Box Drawings Vertical Double And Left Single
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsVerticalDoubleAndLeftSingle = u'╢';

    /**
     * @brief U+2556 Box Drawings Down Double And Left Single
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDownDoubleAndLeftSingle = u'╖';

    /**
     * @brief U+2555 Box Drawings Down Single And Left Double
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDownSingleAndLeftDouble = u'╕';

    /**
     * @brief U+2563 Box Drawings Double Vertical And Left
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDoubleVerticalAndLeft = u'╣';

    /**
     * @brief U+2551 Box Drawings Double Vertical
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDoubleVertical = u'║';

    /**
     * @brief U+2557 Box Drawings Double Down And Left
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDoubleDownAndLeft = u'╗';

    /**
     * @brief U+255D Box Drawings Double Up And Left
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDoubleUpAndLeft = u'╝';

    /**
     * @brief U+255C Box Drawings Up Double And Left Single
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsUpDoubleAndLeftSingle = u'╜';

    /**
     * @brief U+255B Box Drawings Up Single And Left Double
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsUpSingleAndLeftDouble = u'╛';

    /**
     * @brief U+2510 Box Drawings Light Down And Left
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsLightDownAndLeft = u'┐';

    /**
     * @brief U+2514 Box Drawings Light Up And Right
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsLightUpAndRight = u'└';

    /**
     * @brief U+2534 Box Drawings Light Up And Horizontal
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsLightUpAndHorizontal = u'┴';

    /**
     * @brief U+252C Box Drawings Light Down And Horizontal
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsLightDownAndHorizontal = u'┬';

    /**
     * @brief U+251C Box Drawings Light Vertical And Right
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsLightVerticalAndRight = u'├';

    /**
     * @brief U+2500 Box Drawings Light Horizontal
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsLightHorizontal = u'─';

    /**
     * @brief U+253C Box Drawings Light Vertical And Horizontal
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsLightVerticalAndHorizontal = u'┼';

    /**
     * @brief U+255E Box Drawings Vertical Single And Right Double
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsVerticalSingleAndRightDouble = u'╞';

    /**
     * @brief U+255F Box Drawings Vertical Double And Right Single
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsVerticalDoubleAndRightSingle = u'╟';

    /**
     * @brief U+255A Box Drawings Double Up And Right
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDoubleUpAndRight = u'╚';

    /**
     * @brief U+2554 Box Drawings Double Down And Right
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDoubleDownAndRight = u'╔';

    /**
     * @brief U+2569 Box Drawings Double Up And Horizontal
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDoubleUpAndHorizontal = u'╩';

    /**
     * @brief U+2566 Box Drawings Double Down And Horizontal
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDoubleDownAndHorizontal = u'╦';

    /**
     * @brief U+2560 Box Drawings Double Vertical And Right
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDoubleVerticalAndRight = u'╠';

    /**
     * @brief U+2550 Box Drawings Double Horizontal
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDoubleHorizontal = u'═';

    /**
     * @brief U+256C Box Drawings Double Vertical And Horizontal
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDoubleVerticalAndHorizontal = u'╬';

    /**
     * @brief U+2567 Box Drawings Up Single And Horizontal Double
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsUpSingleAndHorizontalDouble = u'╧';

    /**
     * @brief U+2568 Box Drawings Up Double And Horizontal Single
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsUpDoubleAndHorizontalSingle = u'╨';

    /**
     * @brief U+2564 Box Drawings Down Single And Horizontal Double
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDownSingleAndHorizontalDouble = u'╤';

    /**
     * @brief U+2565 Box Drawings Down Double And Horizontal Single
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDownDoubleAndHorizontalSingle = u'╥';

    /**
     * @brief U+2559 Box Drawings Up Double And Right Single
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsUpDoubleAndRightSingle = u'╙';

    /**
     * @brief U+2558 Box Drawings Up Single And Right Double
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsUpSingleAndRightDouble = u'╘';

    /**
     * @brief U+2552 Box Drawings Down Single And Right Double
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDownSingleAndRightDouble = u'╒';

    /**
     * @brief U+2553 Box Drawings Down Double And Right Single
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsDownDoubleAndRightSingle = u'╓';

    /**
     * @brief U+256B Box Drawings Vertical Double And Horizontal Single
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsVerticalDoubleAndHorizontalSingle = u'╫';

    /**
     * @brief U+256A Box Drawings Vertical Single And Horizontal Double
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsVerticalSingleAndHorizontalDouble = u'╪';

    /**
     * @brief U+2518 Box Drawings Light Up And Left
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsLightUpAndLeft = u'┘';

    /**
     * @brief U+250C Box Drawings Light Down And Right
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BoxDrawingsLightDownAndRight = u'┌';

    /**
     * @brief U+2588 Full Block
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t FullBlock = u'█';

    /**
     * @brief U+2584 Lower Half Block
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t LowerHalfBlock = u'▄';

    /**
     * @brief U+258C Left Half Block
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t LeftHalfBlock = u'▌';

    /**
     * @brief U+2590 Right Half Block
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t RightHalfBlock = u'▐';

    /**
     * @brief U+2580 Upper Half Block
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t UpperHalfBlock = u'▀';

    /**
     * @brief U+03B1 Greek Small Letter Alpha
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreekSmallLetterAlpha = u'α';

    /**
     * @brief U+00DF Latin Small Letter Sharp S
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t LatinSmallLetterSharpS = u'ß';

    /**
     * @brief U+0393 Greek Capital Letter Gamma
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreekCapitalLetterGamma = u'Γ';

    /**
     * @brief U+03C0 Greek Small Letter Pi
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreekSmallLetterPi = u'π';

    /**
     * @brief U+03A3 Greek Capital Letter Sigma
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreekCapitalLetterSigma = u'Σ';

    /**
     * @brief U+03C3 Greek Small Letter Sigma
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreekSmallLetterSigma = u'σ';

    /**
     * @brief U+00B5 Micro Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t MicroSign = u'µ';

    /**
     * @brief U+03C4 Greek Small Letter Tau
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreekSmallLetterTau = u'τ';

    /**
     * @brief U+03A6 Greek Capital Letter Phi
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreekCapitalLetterPhi = u'Φ';

    /**
     * @brief U+0398 Greek Capital Letter Theta
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreekCapitalLetterTheta = u'Θ';

    /**
     * @brief U+03A9 Greek Capital Letter Omega
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreekCapitalLetterOmega = u'Ω';

    /**
     * @brief U+03B4 Greek Small Letter Delta
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreekSmallLetterDelta = u'δ';

    /**
     * @brief U+221E Infinity
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t Infinity = u'∞';

    /**
     * @brief U+03C6 Greek Small Letter Phi
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreekSmallLetterPhi = u'φ';

    /**
     * @brief U+03B5 Greek Small Letter Epsilon
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreekSmallLetterEpsilon = u'ε';

    /**
     * @brief U+2229 Intersection
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t Intersection = u'∩';

    /**
     * @brief U+2261 Identical To
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t IdenticalTo = u'≡';

    /**
     * @brief U+00B1 Plus-Minus Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t PlusMinusSign = u'±';

    /**
     * @brief U+2265 Greater-Than Or Equal To
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t GreaterThanOrEqualTo = u'≥';

    /**
     * @brief U+2264 Less-Than Or Equal To
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t LessThanOrEqualTo = u'≤';

    /**
     * @brief U+2320 Top Half Integral
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t TopHalfIntegral = u'⌠';

    /**
     * @brief U+2321 Bottom Half Integral
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BottomHalfIntegral = u'⌡';

    /**
     * @brief U+00F7 Division Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t DivisionSign = u'÷';

    /**
     * @brief U+2248 Almost Equal To
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t AlmostEqualTo = u'≈';

    /**
     * @brief U+00B0 Degree Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t DegreeSign = u'°';

    /**
     * @brief U+2219 Bullet Operator
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BulletOperator = u'∙';

    /**
     * @brief U+00B7 Middle Dot
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t MiddleDot = u'·';

    /**
     * @brief U+221A Square Root
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t SquareRoot = u'√';

    /**
     * @brief U+207F Superscript Latin Small Letter N
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t SuperscriptLatinSmallLetterN = u'ⁿ';

    /**
     * @brief U+00B2 Superscript Two
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t SuperscriptTwo = u'²';

    /**
     * @brief U+25A0 Black Square
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t BlackSquare = u'■';

    /**
     * @brief U+00A0 No-Break Space
     *
     * Appears in gf::ConsoleFontFormat::CodePage437
     */
    static constexpr char16_t NoBreakSpace = u'\u00A0';


    /**
     * @brief U+03B2 Greek Small Letter Beta
     *
     * Appears in gf::ConsoleFontFormat::CodePage437 (as alternative)
     */
    static constexpr char16_t GreekSmallLetterBeta = u'β';


    /**
     * @brief U+03A0 Greek Capital Letter Pi
     *
     * Appears in gf::ConsoleFontFormat::CodePage437 (as alternative)
     */
    static constexpr char16_t GreekCapitalLetterPi = u'Π';

    /**
     * @brief U+220F N-Ary Product
     *
     * Appears in gf::ConsoleFontFormat::CodePage437 (as alternative)
     */
    static constexpr char16_t NAryProduct = u'∏';

    /**
     * @brief U+2211 N-Ary Summation
     *
     * Appears in gf::ConsoleFontFormat::CodePage437 (as alternative)
     */
    static constexpr char16_t NArySummation = u'∑';

    /**
     * @brief U+03BC Greek Small Letter Mu
     *
     * Appears in gf::ConsoleFontFormat::CodePage437 (as alternative)
     */
    static constexpr char16_t GreekSmallLetterMu = u'μ';

    /**
     * @brief U+2126 Ohm Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437 (as alternative)
     */
    static constexpr char16_t OhmSign = u'Ω';

    /**
     * @brief U+03D5 Greek Phi Symbol
     *
     * Appears in gf::ConsoleFontFormat::CodePage437 (as alternative)
     */
    static constexpr char16_t GreekPhiSymbol = u'ϕ';

    /**
     * @brief U+2205 Empty Set
     *
     * Appears in gf::ConsoleFontFormat::CodePage437 (as alternative)
     */
    static constexpr char16_t EmptySet = u'∅';

    /**
     * @brief U+2300 Diameter Sign
     *
     * Appears in gf::ConsoleFontFormat::CodePage437 (as alternative)
     */
    static constexpr char16_t DiameterSign = u'⌀';

    /**
     * @brief U+00D8 Latin Capital Letter O With Stroke
     *
     * Appears in gf::ConsoleFontFormat::CodePage437 (as alternative)
     */
    static constexpr char16_t LatinCapitalLetterOWithStroke = u'Ø';

    /**
     * @brief U+00F8 Latin Small Letter O With Stroke
     *
     * Appears in gf::ConsoleFontFormat::CodePage437 (as alternative)
     */
    static constexpr char16_t LatinSmallLetterOWithStroke = u'ø';

    /**
     * @brief U+2208 Element Of
     *
     * Appears in gf::ConsoleFontFormat::CodePage437 (as alternative)
     */
    static constexpr char16_t ElementOf = u'∈';

    /**
     * @brief U+2598 Quadrant Upper Left
     *
     * Appears in gf::ConsoleFontFormat::ModifiedCodePage437
     */
    static constexpr char16_t QuadrantUpperLeft = u'▘';

    /**
     * @brief U+259D Quadrant Upper Right
     *
     * Appears in gf::ConsoleFontFormat::ModifiedCodePage437
     */
    static constexpr char16_t QuadrantUpperRight = u'▝';

    /**
     * @brief U+2597 Quadrant Lower Right
     *
     * Appears in gf::ConsoleFontFormat::ModifiedCodePage437
     */
    static constexpr char16_t QuadrantLowerRight = u'▗';

    /**
     * @brief U+259A Quadrant Upper Left And Lower Right
     *
     * Appears in gf::ConsoleFontFormat::ModifiedCodePage437
     */
    static constexpr char16_t QuadrantUpperLeftAndLowerRight = u'▚';

    /**
     * @brief U+2596 Quadrant Lower Left
     *
     * Appears in gf::ConsoleFontFormat::ModifiedCodePage437
     */
    static constexpr char16_t QuadrantLowerLeft = u'▖';

  };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}

#endif // GF_CONSOLE_CHAR_H
