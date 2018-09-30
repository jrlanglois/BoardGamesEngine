/*
    ==============================================================================

    This file is part of the Board Games Engine library.
    Copyright (c) 2014 to present - Joel R. Langlois

    Permission is granted to use this software under the terms of GPL v2 (or any later version).

    Details of these licenses can be found at: www.gnu.org/licenses

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU General Public License for more details.

    ------------------------------------------------------------------------------

    To release a closed-source product which uses the Board Games Engine,
    please contact Joel at joel.r.langlois@gmail.com for additional details.

    ==============================================================================
*/
#ifndef BGE_SYSTEM_TRAY_ICON_COMPONENT_H
#define BGE_SYSTEM_TRAY_ICON_COMPONENT_H

class BGESystemTrayIconComponent : public SystemTrayIconComponent
{
public:
    BGESystemTrayIconComponent();

    //==============================================================================
    void updateTooltipFromBoardGame (BoardGame& boardGame);

    //==============================================================================
    /** @internal */
    void mouseDown (const MouseEvent& e) override;

private:
    //==============================================================================
    void bringWindowToFocus();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BGESystemTrayIconComponent)
};

#endif //BGE_SYSTEM_TRAY_ICON_COMPONENT_H