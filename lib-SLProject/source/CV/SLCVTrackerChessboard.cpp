//#############################################################################
//  File:      SLCVTrackerChessboard.cpp
//  Author:    Michael G�ttlicher, Marcus Hudritsch
//  Date:      Spring 2016
//  Codestyle: https://github.com/cpvrlab/SLProject/wiki/Coding-Style-Guidelines
//  Copyright: Marcus Hudritsch, Michael G�ttlicher
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#include <stdafx.h>         // precompiled headers
#include <SLCVTrackerChessboard.h>

using namespace cv;
//-----------------------------------------------------------------------------
SLCVTrackerChessboard::SLCVTrackerChessboard(SLNode* node) : SLCVTracker(node)
{
    SLCVCalibration& calib = SLScene::current->calibration();

    //generate vectors for the points on the chessboard
    for (int w = 0; w < calib.boardSize().width; w++)
        for (int h = 0; h < calib.boardSize().height; h++)
            _boardPoints.push_back(Point3d(double(w * calib.boardSquareM()), 
                                           double(h * calib.boardSquareM()), 
                                           0.0));
}
//-----------------------------------------------------------------------------
//! Tracks the chessboard image in the given image for the first sceneview
bool SLCVTrackerChessboard::track(cv::Mat image, 
                                  SLCVCalibration& calib,
                                  SLSceneView* sv)
{
    assert(!image.empty() && "Image is empty");
    assert(!calib.intrinsics().empty() && "Calibration is empty");
    assert(_node && "Node pointer is null");
    assert(sv && "No sceneview pointer passed");
    assert(sv->camera() && "No active camera in sceneview");
    
    SLCVCalibration& c= SLScene::current->calibration();

    //detect chessboard corners
    SLint flags = CALIB_CB_ADAPTIVE_THRESH | 
                  CALIB_CB_NORMALIZE_IMAGE | 
                  CALIB_CB_FAST_CHECK;

    vector<cv::Point2f> corners;

    _isVisible = cv::findChessboardCorners(image, c.boardSize(), corners, flags);

    if(_isVisible)
    {
        cv::Mat rVec, tVec;

        //find the camera extrinsic parameters
        bool solved = solvePnP(SLCVMat(_boardPoints), 
                               SLCVMat(corners), 
                               calib.intrinsics(), 
                               calib.distortion(), 
                               rVec, 
                               tVec, 
                               false, 
                               cv::SOLVEPNP_ITERATIVE);
        if (solved)
        {   _viewMat = createGLMatrix(tVec, rVec);

            if (_node == sv->camera())
                _node->om(_viewMat.inverse());
            else
            {   _node->om(calcObjectMatrix(sv->camera()->om(),_viewMat));
                _node->setDrawBitsRec(SL_DB_HIDDEN, false);
            }
            return true;
        }
    }
    
    // Hide tracked node if not visible
    if (_node != sv->camera())
        _node->setDrawBitsRec(SL_DB_HIDDEN, true);

    return false;
}
//------------------------------------------------------------------------------