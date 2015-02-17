//#############################################################################
//  File:      SLLeapController.h
//  Author:    Marc Wacker
//  Date:      January 2015
//  Codestyle: https://github.com/cpvrlab/SLProject/wiki/Coding-Style-Guidelines
//  Copyright: 2002-2015 Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#ifndef SLLEAPFINGER_H
#define SLLEAPFINGER_H

#include <stdafx.h>
#include <Leap.h>
#include <SLLeapDefines.h>

class SLLeapHand;



class SLLeapFinger
{
public:
                SLLeapFinger    (SLFingerType type);
    
    void        leapHand        (const Leap::Hand& hand);
    
    /// @todo define number of bones and joints as constants somewhere
    SLint       numBones        () const { return 4; }
    SLint       numJoints       () const { return 5; }

    SLVec3f     tipPosition     () const;
    SLVec3f     jointPosition   (SLint joint) const;
    SLVec3f     boneCenter      (SLFingerBone boneType) const;
    SLVec3f     boneDirection   (SLFingerBone boneType) const;
    SLQuat4f    boneRotation    (SLFingerBone boneType) const;

    SLFingerType fingerType     () const;

protected:
    Leap::Hand          _hand;          //!< leap hand object
    Leap::Finger        _finger;        //!< leap finger object
    SLFingerType        _fingerType;    //!< leap finger type 
};

typedef vector<SLLeapFinger> SLVLeapFinger;

#endif