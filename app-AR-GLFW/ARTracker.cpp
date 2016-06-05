//#############################################################################
//  File:      ARTracker.cpp
//  Author:    Michael G�ttlicher
//  Date:      Spring 2016
//  Codestyle: https://github.com/cpvrlab/SLProject/wiki/Coding-Style-Guidelines
//  Copyright: Marcus Hudritsch, Michael G�ttlicher
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#include <stdafx.h>
#ifdef SL_MEMLEAKDETECT       // set in SL.h for debug config only
#include <debug_new.h>        // memory leak detector
#endif
#include <SLScene.h>
#include <ARTracker.h>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/video/tracking.hpp>
//#include <opencv2/aruco.hpp>

using namespace cv;

//-----------------------------------------------------------------------------
//! constructor
ARTracker::ARTracker(Mat intrinsics, Mat distoriton) :
    _intrinsics(intrinsics),
    _distortion(distoriton)
{
    //set up matrices for storage of translation and rotation vector
    _rVec = Mat(Size(3, 1), CV_64F);
    _tVec = Mat(Size(3, 1), CV_64F);
    //set up matrix for rotation matrix after rodrigues transformation
    _rMat = Mat(3,3,CV_64F);
}
//-----------------------------------------------------------------------------
//! destructor
ARTracker::~ARTracker()
{
}
//-----------------------------------------------------------------------------