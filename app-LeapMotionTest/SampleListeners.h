
#pragma once

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

    SLVec3f posLeft;
    SLVec3f posRight;

    SLVec3f positionsLeft[26];
    SLVec3f positionsRight[26];
};

// Simple hand listener, generates and moves spheres, boxes and cylinders as a 'skeletal' hand model
class SampleHandListener : public SLLeapHandListener
{
public:
    void init()
    {
        SLNode* parent = new SLNode("fucking parent");
        parent->scale(10.0f);
        SLScene::current->root3D()->addChild(parent);

        
        SLMesh* palmMesh = new SLBox(-0.015f, -0.005f, -0.015f, 0.015f, 0.005f, 0.015f);
        SLMesh* jointMesh = new SLSphere(0.005f);
        SLMesh* jointMeshBig = new SLSphere(0.01f);
        SLMesh* boneMesh = new SLCylinder(0.0015f, 0.1f);
        
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
        
        // generate joints and bones for fingers
        SLfloat boneScales[5][4] = { 
            { 0.01f, 0.36f, 0.3f, 0.15f},  // thumb
            { 0.6f, 0.36f, 0.3f, 0.12f},  // index
            { 0.6f, 0.36f, 0.33f, 0.12f},  // middle
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
                meshCont->position(0, 0, -0.5 * boneScales[i][j] * 0.1f);
                meshCont->scale(1, 1, boneScales[i][j]);
                
                leftBones[i][j] = new SLNode;
                leftBones[i][j]->addChild(meshCont);
                parent->addChild(leftBones[i][j]);

                
                meshCont = new SLNode(boneMesh);
                meshCont->position(0, 0, -0.5 * boneScales[i][j] * 0.1f);
                meshCont->scale(1, 1, boneScales[i][j]);
                
                rightBones[i][j] = new SLNode;
                rightBones[i][j]->addChild(meshCont);
                parent->addChild(rightBones[i][j]);
            }
        }


        
        parent->addChild(leftHand);
        parent->addChild(rightHand);
        
        parent->addChild(leftArm);
        parent->addChild(rightArm);
        parent->addChild(leftElbow);
        parent->addChild(rightElbow);
        parent->addChild(leftWrist);
        parent->addChild(rightWrist);
        parent->needUpdate();
    }

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

    virtual void onLeapHandChange(const vector<SLLeapHand>& hands)
    {
        for (SLint i = 0; i < hands.size(); ++i)
        {
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
            arm->rotation(hands[i].armRotation(), TS_Parent);


            for (SLint j = 0; j < hands[i].fingers().size(); ++j)
            {
                // set joint positions
                for (SLint k = 0; k < 5; ++k) {
                    SLNode* joint = (hands[i].isLeft()) ? leftJoints[j][k] : rightJoints[j][k];
                    joint->position(hands[i].fingers()[j].jointPosition(k));
                }
                
                // set bone positions
                for (SLint k = 0; k < 4; ++k) {                    
                    SLNode* bone = (hands[i].isLeft()) ? leftBones[j][k] : rightBones[j][k];
                    bone->position(hands[i].fingers()[j].boneCenter(k));
                    bone->rotation(hands[i].fingers()[j].boneRotation(k), TS_Parent);
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

        _toolNode->position(tool.toolTipPosition());
        _toolNode->rotation(tool.toolRotation());
    }
};

// simple gesture listener, outputs the type of default gestures it receives
class SampleGestureListener : public SLLeapGestureListener
{
protected:
    virtual void onLeapGesture(const SLLeapGesture& gesture)
    {/*
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
        : _modelScale(1.0f)
    {
        for (SLint i = 0; i < 5; ++i) {
            for (SLint j = 0; j < 4; ++j) {
                _leftFingers[i][j] = _rightFingers[i][j] = NULL;
            }
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

    void setModelScale(SLfloat s) { _modelScale = s; }

    // @todo provide enums for finger type and bone type
    void setLFingerJoint(SLint fingerType, SLint boneType, const SLstring& name)
    {
        if (!_skeleton) return;

        _leftFingers[fingerType][boneType] = _skeleton->getJoint(name);
    }
    void setRFingerJoint(SLint fingerType, SLint boneType, const SLstring& name)
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

    virtual void onLeapHandChange(const vector<SLLeapHand>& hands)
    {
        for (SLint i = 0; i < hands.size(); ++i)
        {
            SLQuat4f rot = hands[i].palmRotation();
            SLJoint* jnt = (hands[i].isLeft()) ? _leftWrist : _rightWrist;

            jnt->rotation(rot, TS_World);
            jnt->position(hands[i].palmPosition() * _modelScale, TS_World); // note the correction for the models scaling
            
            for (SLint j = 0; j < hands[i].fingers().size(); ++j)
            {                
                for (SLint k = 0; k < 4; ++k) {                    
                    SLJoint* bone = (hands[i].isLeft()) ? _leftFingers[j][k] : _rightFingers[j][k];
                    if (bone == NULL)
                        continue;

                    bone->rotation(hands[i].fingers()[j].boneRotation(k), TS_World);
                }
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

protected:
    SLfloat grabThreshold;
    SLbool grabbing[2];

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
            grabPosition *= 10.0f;
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
