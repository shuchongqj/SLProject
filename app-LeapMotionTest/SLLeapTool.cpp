//#############################################################################
//  File:      SLLeapTool.cpp
//  Author:    Marc Wacker
//  Date:      January 2015
//  Codestyle: https://github.com/cpvrlab/SLProject/wiki/Coding-Style-Guidelines
//  Copyright: 2002-2014 Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#include <stdafx.h>
#include "SLLeapTool.h"
#include <SLLeapDefines.h>


SLQuat4f SLLeapTool::toolRotation() const
{
    Leap::Vector dir = _tool.direction();
    SLVec3f slDir(dir.x, dir.y, dir.z);
    return SLQuat4f(-SLVec3f::AXISZ, slDir);
}
SLVec3f SLLeapTool::toolTipPosition() const
{
    Leap::Vector pos = _tool.tipPosition();
    return SLVec3f(pos.x, pos.y, pos.z) * LM_TO_SL_UNITS;
}
SLVec3f SLLeapTool::toolTipVelocity() const
{
    Leap::Vector vel = _tool.tipVelocity();
    return SLVec3f(vel.x, vel.y, vel.z) * LM_TO_SL_UNITS; // return our scaled units/s instead of millimeters/s
}
