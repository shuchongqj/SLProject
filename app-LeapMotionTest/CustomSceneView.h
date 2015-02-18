
#include <SLSceneView.h>
#include "Leap.h"
#include "SLLeapController.h"
#include <SLBox.h>
#include <SLSphere.h>
#include <SLCylinder.h>

#include "SampleListeners.h"

#pragma warning(disable:4996)


class CustomSceneView : public SLSceneView
{
public:   
    CustomSceneView()
    { }
    ~CustomSceneView();

    void                postSceneLoad();
    
private:
    SLLeapController            _leapController;            
    SampleHandListener          _slHandListener;
    SLRiggedLeapHandListener    _riggedListener;
    SampleToolListener          _slToolListener;
    SampleGestureListener       _gestureListener;
    SampleObjectMover           _objectMover;
    vector<SLNode*>             _movableBoxes;
    SLNode*                     _currentGrabbedObject[2];
    SLQuat4f                    _prevRotation[2];
    SLVec3f                     _prevPosition[2];

    void grabCallback(SLVec3f&, SLQuat4f&, bool);
    void moveCallback(SLVec3f&, SLQuat4f&, bool);
    void releaseCallback(bool);
};
