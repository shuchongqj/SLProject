
#pragma once

/**

This file contains a few example implementations for the provided SLLeap listener interfaces.
Some of these should be provided as finished listeners should this be added to the core library.

*/


// first test, direct implementation of Leap::Listener
class SampleListener : public Leap::Listener {
  public:
    virtual void onInit(const Leap::Controller&);
    virtual void onConnect(const Leap::Controller&);
    virtual void onDisconnect(const Leap::Controller&);
    virtual void onExit(const Leap::Controller&);
    virtual void onFrame(const Leap::Controller&);
    virtual void onFocusGained(const Leap::Controller&);
    virtual void onFocusLost(const Leap::Controller&);
    virtual void onDeviceChange(const Leap::Controller&);
    virtual void onServiceConnect(const Leap::Controller&);
    virtual void onServiceDisconnect(const Leap::Controller&);
};

// Simple hand listener, generates and moves spheres, boxes and cylinders as a 'skeletal' hand model
class SampleHandListener : public SLLeapHandListener
{
public:
    void init(SLfloat scl = 1.0f)
    {
        SLNode* parent = new SLNode("fucking parent");
        parent->scale(scl);
        SLScene::current->root3D()->addChild(parent);

        
        SLMesh* jointMesh = new SLSphere(0.005f);
        SLMesh* boneMesh = new SLCylinder(0.0015f, 0.1f);
        /*SLMesh* jointMeshBig = new SLSphere(0.01f);
        SLMesh* palmMesh = new SLBox(-0.015f, -0.005f, -0.015f, 0.015f, 0.005f, 0.015f);
        
        leftHand = new SLNode(palmMesh, "fucking palm");
        rightHand = new SLNode(palmMesh);
        
        leftArm = new SLNode;
        rightArm = new SLNode;
        SLNode* tempCont = new SLNode(boneMesh);
        tempCont->position(0, 0, -0.14f);
        tempCont->scale(2.8f);
        leftArm->addChild(tempCont);
        tempCont = new SLNode(boneMesh);
        tempCont->position(0, 0, -0.14f);
        tempCont->scale(2.8f);
        rightArm->addChild(tempCont);

        leftWrist = new SLNode(jointMeshBig);
        rightWrist = new SLNode(jointMeshBig);
        leftElbow = new SLNode(jointMeshBig);
        rightElbow = new SLNode(jointMeshBig);
        
        parent->addChild(leftHand);
        parent->addChild(rightHand);
        
        parent->addChild(leftArm);
        parent->addChild(rightArm);
        parent->addChild(leftElbow);
        parent->addChild(rightElbow);
        parent->addChild(leftWrist);
        parent->addChild(rightWrist);
        */

        // generate joints and bones for fingers
        SLfloat boneScales[5][4] = { 
            { 0.01f, 0.36f, 0.3f, 0.15f}, // thumb
            { 0.6f, 0.36f, 0.3f, 0.12f},  // index
            { 0.6f, 0.36f, 0.33f, 0.12f}, // middle
            { 0.5f, 0.36f, 0.3f, 0.12f},  // ring
            { 0.5f, 0.3f, 0.25f, 0.12f}   // pinky
        };

        for (SLint i = 0; i < 5; ++i) {
            // joints
            for (SLint j = 0; j < 5; ++j) {
                leftJoints[i][j] = new SLNode(jointMesh);
                rightJoints[i][j] = new SLNode(jointMesh);

                parent->addChild(leftJoints[i][j]);
                parent->addChild(rightJoints[i][j]);
            }
            // bones
            for (SLint j = 0; j < 4; ++j) {
                SLNode* meshCont = new SLNode(boneMesh);
                meshCont->translation(0, 0, -0.5 * boneScales[i][j] * 0.1f);
                meshCont->scale(1, 1, boneScales[i][j]);
                
                leftBones[i][j] = new SLNode;
                leftBones[i][j]->addChild(meshCont);
                parent->addChild(leftBones[i][j]);

                
                meshCont = new SLNode(boneMesh);
                meshCont->translation(0, 0, -0.5 * boneScales[i][j] * 0.1f);
                meshCont->scale(1, 1, boneScales[i][j]);
                
                rightBones[i][j] = new SLNode;
                rightBones[i][j]->addChild(meshCont);
                parent->addChild(rightBones[i][j]);
            }
        }


        parent->needUpdate();
    }

    void setOrigin(SLVec3f& orig) { _position = orig; }
    void setOrientation(SLQuat4f& orient) { _orientation = orient; }

protected:
    SLNode* leftHand;
    SLNode* rightHand;
    SLint leftId;
    SLint rightId;
    
    SLNode* leftJoints[5][5];
    SLNode* leftBones[5][4];
    SLNode* rightJoints[5][5];
    SLNode* rightBones[5][4];
    
    SLNode* leftArm;
    SLNode* rightArm;
    SLNode* leftElbow;
    SLNode* rightElbow;
    SLNode* leftWrist;
    SLNode* rightWrist;

    SLVec3f _position;
    SLQuat4f _orientation;

    virtual void onLeapHandChange(const vector<SLLeapHand>& hands)
    {
        for (SLint i = 0; i < hands.size(); ++i)
        {/*
            SLNode* hand = (hands[i].isLeft()) ? leftHand : rightHand;
            SLNode* elbow = (hands[i].isLeft()) ? leftElbow : rightElbow;
            SLNode* wrist = (hands[i].isLeft()) ? leftWrist : rightWrist;
            SLNode* arm = (hands[i].isLeft()) ? leftArm : rightArm;
            
            hand->position(hands[i].palmPosition());            
            hand->rotation(hands[i].palmRotation(), TS_Parent);
            
            SLQuat4f test = hands[i].palmRotation();
            hand->rotation(hands[i].palmRotation(), TS_Parent);

            elbow->position(hands[i].elbowPosition());
            wrist->position(hands[i].wristPosition());
            
            arm->position(hands[i].armCenter());
            arm->rotation(hands[i].armRotation(), TS_Parent);*/

            // @note    the above code is commented out to not mess with the viewer in VR for now.

            for (SLint j = 0; j < hands[i].fingers().size(); ++j)
            {
                // set joint positions
                
                for (SLint k = 0; k < 5; ++k) {
                    SLNode* joint = (hands[i].isLeft()) ? leftJoints[j][k] : rightJoints[j][k];
                    joint->translation(hands[i].fingers()[j].jointPosition(k) + _position * 0.04f);
                }
                
                // set bone positions
                for (SLint k = 0; k < 4; ++k) {                    
                    SLFingerBone fb = (SLFingerBone)k;
                    SLNode* bone = (hands[i].isLeft()) ? leftBones[j][k] : rightBones[j][k];
                    bone->translation(hands[i].fingers()[j].boneCenter(fb) + _position* 0.04f);
                    bone->rotation(hands[i].fingers()[j].boneRotation(fb) * _orientation, TS_World);
                }
            }
        }

    }
};

// simple tool listener, visualizes a single tool as a cylinder and a sphere
class SampleToolListener : public SLLeapToolListener
{
public:
    void init()
    {
        SLScene* s = SLScene::current;
        
        SLMesh* toolTipMesh = new SLSphere(0.03f);
        SLMesh* toolMesh = new SLCylinder(0.015f, 2.0f);
        
        _toolNode = new SLNode;
        _toolNode->addMesh(toolTipMesh);
        _toolNode->addMesh(toolMesh);

        s->root3D()->addChild(_toolNode);
    }

protected:
    // only one tool allowed currently
    SLNode* _toolNode;

    virtual void onLeapToolChange(const vector<SLLeapTool>& tools)
    {
        if (tools.size() == 0)
            return;

        const SLLeapTool& tool = tools[0];

        _toolNode->translation(tool.toolTipPosition());
        _toolNode->rotation(tool.toolRotation());
    }
};

// simple gesture listener, outputs the type of default gestures it receives
class SampleGestureListener : public SLLeapGestureListener
{
protected:
    virtual void onLeapGesture(const SLLeapGesture& gesture)
    {/* Just a small example for now, gestures aren't fully implemented tough.
        switch (gesture.type())
        {
        case SLLeapGesture::Swipe: SL_LOG("SWIPE\n") break;
        case SLLeapGesture::KeyTap: SL_LOG("KEY TAP\n") break;
        case SLLeapGesture::ScreenTap: SL_LOG("SCREEN TAP\n") break;
        case SLLeapGesture::Circle: SL_LOG("CIRCLE\n") break;
        }*/
    }
};

// rigged hand listener, has the reference of a specific SLSkeleton and binds the LM bones to SLjoints in the skeleton
class SLRiggedLeapHandListener : public SLLeapHandListener
{
public:
    SLRiggedLeapHandListener()
        : _modelScale(1.0f),
        _thumbPinchOffset(0, 0, 0, 1),
        _indexPinchOffset(0, 0, 0, 1)
    {
        for (SLint i = 0; i < 5; ++i) {
            for (SLint j = 0; j < 4; ++j) {
                _leftFingers[i][j] = _rightFingers[i][j] = NULL;
            }
        }

        for (SLint i = 0; i < 2; ++i) {
            _leftPinchPair[i] = NULL;
            _rightPinchPair[i] = NULL;
        }
    }

    void setSkeleton(SLSkeleton* skel){
        _skeleton = skel;
    }
    
    void setLWrist(const SLstring& name)
    {
        _leftWrist = _skeleton->getJoint(name);
    }
    void setRWrist(const SLstring& name)
    {
        _rightWrist = _skeleton->getJoint(name);
    }
    void setLArm(const SLstring& name)
    {

    }
    void setRArm(const SLstring& name)
    {

    }

    void setThumbPinchOffset(const SLVec3f& o) { _thumbPinchOffset.set(o.x, o.y, o.z, 1); }
    void setIndexPinchOffset(const SLVec3f& o) { _indexPinchOffset.set(o.x, o.y, o.z, 1); }

    // set a special bone to be used for pinch detection of the left thumb
    void setLThumbPinchBone(const SLstring& name)
    {
        _leftPinchPair[0] = _skeleton->getJoint(name);
    }
    void setRThumbPinchBone(const SLstring& name)
    {
        _rightPinchPair[0] = _skeleton->getJoint(name);
    }
    void setLIndexPinchBone(const SLstring& name)
    {
        _leftPinchPair[1] = _skeleton->getJoint(name);
    }
    void setRIndexPinchBone(const SLstring& name)
    {
        _rightPinchPair[1] = _skeleton->getJoint(name);
    }


    void setModelScale(SLfloat s) { _modelScale = s; }
    void setOrigin(SLVec3f& orig) { _position = orig; }
    void setOrientation(SLQuat4f& orient) { _orientation = orient; }

    // @todo provide enums for finger type and bone type
    void setLFingerJoint(SLFingerType fingerType, SLFingerBone boneType, const SLstring& name)
    {
        if (!_skeleton) return;

        _leftFingers[fingerType][boneType] = _skeleton->getJoint(name);
    }
    void setRFingerJoint(SLFingerType fingerType, SLFingerBone boneType, const SLstring& name)
    {
        if (!_skeleton) return;

        if (_skeleton->getJoint(name))
            _skeleton->getJoint(name)->setInitialState();
        _rightFingers[fingerType][boneType] = _skeleton->getJoint(name);
    }

protected:
    
    SLSkeleton* _skeleton;
    SLJoint* _leftFingers[5][4];
    SLJoint* _rightFingers[5][4];
    SLJoint* _leftWrist;
    SLJoint* _rightWrist;
    SLfloat _modelScale;

    SLVec3f _position;  //!< origin position for the hands
    SLQuat4f _orientation; //!< origin orientation for the hands

    // additional support for grab callbacks
    SLVec4f _thumbPinchOffset;
    SLVec4f _indexPinchOffset;
    SLJoint* _leftPinchPair[2];
    SLJoint* _rightPinchPair[2];

    virtual void onLeapHandChange(const vector<SLLeapHand>& hands)
    {
        for (SLint i = 0; i < hands.size(); ++i)
        {
            SLQuat4f rot = hands[i].palmRotation() * _orientation;
            SLJoint* jnt = (hands[i].isLeft()) ? _leftWrist : _rightWrist;

            jnt->rotation(rot, TS_World);
            jnt->translation((hands[i].palmPosition() + _position) * _modelScale, TS_World); // note the correction for the models scaling
            
            for (SLint j = 0; j < hands[i].fingers().size(); ++j)
            {                
                for (SLint k = 0; k < 4; ++k) {                    
                    SLJoint* bone = (hands[i].isLeft()) ? _leftFingers[j][k] : _rightFingers[j][k];
                    if (bone == NULL)
                        continue;

                    bone->rotation(hands[i].fingers()[j].boneRotation((SLFingerBone)k) * _orientation, TS_World);
                }
            }

            detectPinch(hands[i]);
        }
    }

    // just a first test, that's why we have statics here
    void detectPinch(const SLLeapHand& hand) 
    {
        static SLfloat grabThreshold = 0.8f;
        static SLbool grabbing[2] = {false, false};
        static bool initialized = false;
        static SLNode* pinchVisualization;
        if (!initialized) {
            initialized = true;

            pinchVisualization = new SLNode(new SLSphere(0.02f));
            pinchVisualization->drawBits()->on(SL_DB_HIDDEN);
            SLScene::current->root3D()->addChild(pinchVisualization);
        }
       
        SLJoint* thumb = hand.isLeft() ? _leftPinchPair[0] : _rightPinchPair[0];
        SLJoint* indexFinger = hand.isLeft() ? _leftPinchPair[1] : _rightPinchPair[1];

        // can't detect a pinchbased on the model we could use 
        // the tip position from the leap, but for now we just return...
        if (!thumb ||!indexFinger)
            return;

        SLVec3f grabPosition = hand.fingers()[0].tipPosition() + 
                                hand.fingers()[1].tipPosition();

        
        SLVec4f temp = thumb->updateAndGetWM() * _thumbPinchOffset;
        SLVec3f thumbPos = SLVec3f(temp.x, temp.y, temp.z);
        temp = indexFinger->updateAndGetWM() * _indexPinchOffset;
        SLVec3f indexPos = SLVec3f(temp.x, temp.y, temp.z);

        grabPosition = thumbPos + indexPos;
        grabPosition *= 0.5f;
        grabPosition /= _modelScale;


        int asd= 0;

        //grabPosition /= _modelScale;
        
        
        SLint index = hand.isLeft() ? 0 : 1;
        SL_LOG("pinch: %f\n", hand.pinchStrength());
        if (hand.pinchStrength() > grabThreshold) {
            // display visualization of grab position
            pinchVisualization->translation(grabPosition, TS_World);

            if (!grabbing[index]) {
                grabbing[index]= true;
                pinchVisualization->drawBits()->off(SL_DB_HIDDEN);
            }
        }
        else {
            if (grabbing[index]) {
                SL_LOG("released pinch\n");
                grabbing[index] = false;
                pinchVisualization->drawBits()->on(SL_DB_HIDDEN);
            }
        }

    }
};

// special hand listener with c++11 callback objects for grab and move events
class SampleObjectMover : public SLLeapHandListener
{
public:
    SampleObjectMover()
    : grabThreshold(0.9f)
    { 
        grabbing[0] = false;
        grabbing[1] = false;
    }

    void setGrabThreshold(SLfloat t) { grabThreshold = t; }
    void setGrabCallback(std::function<void(SLVec3f&, SLQuat4f&, bool)> cb) { grabCallback = cb; }
    void setReleaseCallback(std::function<void(bool)> cb) { releaseCallback = cb; }
    void setMoveCallback(std::function<void(SLVec3f&, SLQuat4f&, bool)> cb) { moveCallback = cb; }
    
    void setOrigin(SLVec3f& orig) { _position = orig; }
    void setScaling(SLfloat scl) { _scaling = scl; }
protected:
    SLfloat grabThreshold;
    SLbool grabbing[2];

    SLVec3f _position;
    SLfloat _scaling;

    std::function<void(SLVec3f&, SLQuat4f&, bool)> grabCallback;
    std::function<void(bool)> releaseCallback;
    std::function<void(SLVec3f&, SLQuat4f&, bool)> moveCallback;

    virtual void onLeapHandChange(const vector<SLLeapHand>& hands)
    {
        for (SLint i = 0; i < hands.size(); ++i)
        {
            SLLeapHand hand = hands[i];

            // just use the right hand for a first test
            SLbool left = hand.isLeft();
            SLint index = (left) ? 0 : 1;

            // For now just the intermediate position between thumb and index finger
            // @note  a pinch can also be between any other finger and the ttumb, so this
            //        currently only works for index and thumb pinches
            SLVec3f grabPosition = hand.fingers()[0].tipPosition() + 
                                   hand.fingers()[1].tipPosition();
            grabPosition *= 0.5f;
            grabPosition *= _scaling;
            grabPosition += _position;

            SLQuat4f palmRotation = hand.palmRotation();

            if (hand.pinchStrength() > grabThreshold) {
                if (grabbing[index]) {
                    moveCallback(grabPosition, palmRotation, left);
                }
                else {
                    grabCallback(grabPosition, palmRotation, left);
                    grabbing[index] = true;
                }
            }
            else {
                if (grabbing[index]) {
                    releaseCallback(left);
                    grabbing[index] = false;
                }
            }

        }
    }
};
