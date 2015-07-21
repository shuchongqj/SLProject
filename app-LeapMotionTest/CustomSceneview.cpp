

#include <random>

#include "SLRectangle.h"
#include "SLBox.h"
#include "SLSphere.h"
#include "SLAnimation.h"
#include "SLLightSphere.h"
#include "SLText.h"
#include "SLTexFont.h"
#include "SLAssImpImporter.h"

#include "SLInputManager.h"
#include "CustomSceneView.h"

using namespace Leap; // dont use using!







// builds a custom scene with a grid where every node is animated
void SLScene::onLoad(SLSceneView* sv, SLCmd cmd)
{
    init();

    name("Virtual Reality test scene");
    info(sv, "Test scene for virtual reality size perception.");
        

    SLAssimpImporter importer;
    SLNode* scene = new SLNode;
    scene->scale(1);
        
    // scene floor
    SLMaterial* matFloor = new SLMaterial("floor", new SLGLTexture("tron_floor2.png"
                                        ,SL_ANISOTROPY_MAX
                                        ,GL_LINEAR),
                                        NULL, NULL, NULL,
                                        _programs[TextureOnly]);
    SLNode* floor = new SLNode(
                        new SLRectangle(SLVec2f(-1000, -1000), SLVec2f(1000,1000),
                                        SLVec2f(-1000, -1000), SLVec2f(1000,1000),
                                        1, 1, "rectF", matFloor), "rectFNode"); 
    floor->rotate(-90, 1,0,0);
    scene->addChild(floor);

    // scene skybox
    // load hand mesh
    SLNode* riggedHands = importer.load("DAE/Hands/rigged_hands.dae");
    riggedHands->scale(1.5f);// the default hands are ~13cm this scaling change makes them 20cm long
    scene->addChild(riggedHands);

    // table
    SLNode* table = importer.load("DAE/Table/table.dae");
    table->translate(0, 0, -1);
    scene->addChild(table);

    // create crates of various sizes
    SLNode* crate = importer.load("DAE/Crate/crate.dae");
    SLMesh* crateMesh = importer.meshes()[7];
        
        
    crate->rotate(20, 0, 1, 0);
    crate->translate(2, 0, -1, TS_World);
    scene->addChild(crate);
        
    crate = new SLNode;
    crate->addMesh(crateMesh);
    crate->rotate(20, 0, 1, 0);
    crate->translate(3.1, 0, -1, TS_World);
    scene->addChild(crate);
        
    crate = new SLNode(crateMesh);
    crate->rotate(-10, 0, 1, 0);
    crate->translate(2.5, 1, -1, TS_World);
    scene->addChild(crate);

        
    crate = new SLNode(crateMesh);
    crate->rotate(60, 0, 1, 0);
    crate->translate(-4, 0, 1, TS_World);
    crate->scale(2);
    scene->addChild(crate);
        
    crate = new SLNode(crateMesh);
    crate->rotate(30, 0, 1, 0);
    crate->translate(-5, 0, -8, TS_World);
    crate->scale(4);
    scene->addChild(crate);

    SLCamera* cam1 = new SLCamera();
    cam1->eyeHeight(1.67f); // set vr eye height
    cam1->translation(0, 0.0f, 0);    // eye height for 180cm high male
    cam1->lookAt(0, 0.0f, -1.0f);
    cam1->focalDist(22);
    cam1->setInitialState();
    cam1->camAnim(walkingYUp);
    //cam1->camAnim(flyingYUp);
    scene->addChild(cam1);

    // big astroboy
    // Start animation
    SLNode* astroboyBig = importer.load("DAE/AstroBoy/AstroBoy.dae");
    SLAnimPlayback* charAnim = importer.skeleton()->getAnimPlayback("unnamed_anim_0");
    charAnim->playForward();
    charAnim->playbackRate(0.8f);

    astroboyBig->translate(-1.5f, 0.0f, -1.0f);

    scene->addChild(astroboyBig);

    // small astroboy on table
    SLNode* astroboySmall = importer.load("DAE/AstroBoy/AstroBoy.dae");
    charAnim = importer.skeleton()->getAnimPlayback("unnamed_anim_0");
    charAnim->playForward();
    charAnim->playbackRate(2.0f);

    astroboySmall->translate(0.0f, 1.1f, -1.0f);
    astroboySmall->scale(0.1f);
    scene->addChild(astroboySmall);

    sv->camera(cam1);
        
    SLLightSphere* light1 = new SLLightSphere(5, 20, 5, 0.5f, 1.0f, 1.0f, 2.0f);
    light1->ambient(SLCol4f(0.1f, 0.1f, 0.1f));
    light1->diffuse(SLCol4f(1.0f, 0.7f, 0.3f));
    light1->specular(SLCol4f(0.5f, 0.3f, 0.1f));
    light1->attenuation(1,0,0);
                
    SLLightSphere* light2 = new SLLightSphere(-10.0f, -15.0, 10.0f, 0.2f, 1.0f, 1.0f, 0.0f);
    light2->ambient(SLCol4f(0.0f, 0.0f, 0.0f));
    light2->diffuse(SLCol4f(0.0f, 4.0f, 10.0f));
    light2->specular(SLCol4f(0.0f, 0.0f, 0.0f));
    light2->attenuation(1,0.5f,0);
        
    SLLightSphere* light3 = new SLLightSphere(-10.0f, -15.0, -10.0f, 0.2f, 1.0f, 1.0f, 0.0f);
    light3->ambient(SLCol4f(0.0f, 0.0f, 0.0f));
    light3->diffuse(SLCol4f(0.0f, 4.0f, 10.0f));
    light3->specular(SLCol4f(0.0f, 0.0f, 0.0f));
    light3->attenuation(1,0.5f,0);
        
    SLLightSphere* light4 = new SLLightSphere(10.0f, -15.0, -10.0f, 0.2f, 1.0f, 1.0f, 0.0f);
    light4->ambient(SLCol4f(0.0f, 0.0f, 0.0f));
    light4->diffuse(SLCol4f(0.0f, 4.0f, 10.0f));
    light4->specular(SLCol4f(0.0f, 0.0f, 0.0f));
    light4->attenuation(1,0.5f,0);
        
    SLLightSphere* light5 = new SLLightSphere(10.0f, -15.0, 10.0f, 0.2f, 1.0f, 1.0f, 0.0f);
    light5->ambient(SLCol4f(0.0f, 0.0f, 0.0f));
    light5->diffuse(SLCol4f(0.0f, 4.0f, 10.0f));
    light5->specular(SLCol4f(0.0f, 0.0f, 0.0f));
    light5->attenuation(1,0.5f,0);

    SLAnimation* anim = SLAnimation::create("anim_light2_updown", 10.0f, true, EC_inOutSine, AL_pingPongLoop);
    anim->createSimpleTranslationNodeTrack(light2, SLVec3f(0.0f, 1.0f, 0.0f));
    anim->createSimpleTranslationNodeTrack(light3, SLVec3f(0.0f, 2.0f, 0.0f));
    anim->createSimpleTranslationNodeTrack(light4, SLVec3f(0.0f, 1.0f, 0.0f));
    anim->createSimpleTranslationNodeTrack(light5, SLVec3f(0.0f, 2.0f, 0.0f));



    // 
    SLMaterial* whiteMat = new SLMaterial("mat", SLCol4f::WHITE, SLCol4f::WHITE, 1.0f, 1.0, 0.0f, 0.0f);
    whiteMat->emission(SLCol4f::WHITE);
    SLRectangle* plane0 = new SLRectangle(SLVec2f(-0.01f, 0.0f), SLVec2f(0.01f, 1.0f), 1, 1, "sizeIndicator0", whiteMat);
    SLRectangle* plane1 = new SLRectangle(SLVec2f(0.005f, 0.0f), SLVec2f(-0.005f, 1.0f), 1, 1, "sizeIndicator1", whiteMat);

    struct indicatorData {
        indicatorData(SLfloat px, SLfloat py, SLfloat pz, SLfloat r, SLfloat s, const SLstring& t)
            : pos(px, py, pz), yRot(r), yScale(s), text(t)
        { }

        SLVec3f pos;
        SLfloat yRot;
        SLfloat yScale;
        SLstring text;
    };
    indicatorData indicators[] = {
        // pos                             y rot    y scale text
        indicatorData(3.0f, 0.0f, -0.2f,    -20.0f,    1.0f,   "1m"),
        indicatorData(0.7f, 0.0f, -0.8f,    0.0f,    1.1f,   "1.10m"),
        indicatorData(0.05f, 1.1f, -1.0f,    0.0f,    0.18f,   "18cm"),
        indicatorData(-1.2f, 0.0f, -1.0f,    0.0f,    1.8f,   "1.80m"),
        indicatorData(-2.8f, 0.0f, 0.2f,    60.0f,    2.0f,   "2m"),
        indicatorData(-2.0f, 0.0f, -7.0f,   20.0f,   4.0f,   "4m")
    };        

    for (SLint i = 0; i < 6; i++)
    {
        SLNode* sizeIndicator = new SLNode;
        sizeIndicator->addMesh(plane0);
        sizeIndicator->addMesh(plane1);
        //sizeIndicator->scale();
        SLVec3f pos = indicators[i].pos;
        sizeIndicator->translate(pos, TS_World);
        sizeIndicator->scale(1, indicators[i].yScale, 1);
        sizeIndicator->rotate(indicators[i].yRot, 0, 1, 0, TS_World);
        
        SLText* sizeText1M = new SLText(indicators[i].text, SLTexFont::font22);
        sizeText1M->translate(pos.x + 0.05f, pos.y + 0.5f * indicators[i].yScale, pos.z);
        sizeText1M->rotate(indicators[i].yRot, 0, 1, 0, TS_World);
        sizeText1M->scale(0.005f);


        scene->addChild(sizeText1M);
        scene->addChild(sizeIndicator);
    }
        
    _backColor.set(0.0f,0.0f,0.0f);
    scene->addChild(light1);
    scene->addChild(light2);
    scene->addChild(light3);
    scene->addChild(light4);
    scene->addChild(light5);

    _root3D = scene;

    /**/

    sv->showMenu(true);
    sv->waitEvents(false);
    sv->onInitialize();
}

CustomSceneView::~CustomSceneView()
{
}

void CustomSceneView::postSceneLoad()
{
    // register our custom listeners with the leap input controller
    _leapController.registerHandListener(&_slHandListener);
    _leapController.registerHandListener(&_riggedListener);
    _leapController.registerToolListener(&_slToolListener);
    _leapController.registerGestureListener(&_gestureListener);
    _leapController.registerHandListener(&_objectMover);
    
    // init the tool and hand listener (aka build the meshes and add them to the scene root)
    _slHandListener.init(25.0f);
    _slHandListener.setOrigin(SLVec3f(0.0f, 1.0f, -4.0f));
    _objectMover.setOrigin(SLVec3f(0.0f, 1.0f, -4.0f));
    _objectMover.setScaling(25.0f); // same as slhandlistener to move our boxes
    _slToolListener.init();

    // set the skeleton that this rigged listener should control
    // @todo allow for an easier way to search and find the desired skeleton via name strings
    _riggedListener.setSkeleton(SLScene::current->animManager().skeletons()[0]);
    _riggedListener.setModelScale(66.6666666f); // the model has a scale factor applied to it. To get world units we need to know the reverse factor for it
    _riggedListener.setOrigin(SLVec3f(0, 1.1f, -0.6f)); // origin of the model in world units 
    
    // set the wrist joints
    _riggedListener.setLWrist("L_Wrist");
    _riggedListener.setRWrist("R_Wrist");
    
    // bind the thumb joints to the correct leap bones
        // right
        _riggedListener.setRFingerJoint(FT_Thumb, FB_Proximal, "R_ThumbA");
        _riggedListener.setRFingerJoint(FT_Thumb, FB_Intermediate, "R_ThumbB");
        _riggedListener.setRFingerJoint(FT_Thumb, FB_Distal, "R_ThumbC");
        
        //left
        _riggedListener.setLFingerJoint(FT_Thumb, FB_Proximal, "L_ThumbA");
        _riggedListener.setLFingerJoint(FT_Thumb, FB_Intermediate, "L_ThumbB");
        _riggedListener.setLFingerJoint(FT_Thumb, FB_Distal, "L_ThumbC");
    
    // bind the index finger joints to the correct leap bones
        // right
        _riggedListener.setRFingerJoint(FT_Index, FB_Metacarpal, "R_IndexA");
        _riggedListener.setRFingerJoint(FT_Index, FB_Proximal, "R_IndexB");
        _riggedListener.setRFingerJoint(FT_Index, FB_Intermediate, "R_IndexC");
        _riggedListener.setRFingerJoint(FT_Index, FB_Distal, "R_IndexD");
    
        // left
        _riggedListener.setLFingerJoint(FT_Index, FB_Metacarpal, "L_IndexA");
        _riggedListener.setLFingerJoint(FT_Index, FB_Proximal, "L_IndexB");
        _riggedListener.setLFingerJoint(FT_Index, FB_Intermediate, "L_IndexC");
        _riggedListener.setLFingerJoint(FT_Index, FB_Distal, "L_IndexD");

    // bind the middle finger joints to the correct leap bones
        // right
        _riggedListener.setRFingerJoint(FT_Middle, FB_Metacarpal, "R_MiddleA");
        _riggedListener.setRFingerJoint(FT_Middle, FB_Proximal, "R_MiddleB");
        _riggedListener.setRFingerJoint(FT_Middle, FB_Intermediate, "R_MiddleC");
        _riggedListener.setRFingerJoint(FT_Middle, FB_Distal, "R_MiddleD");

        // left
        _riggedListener.setLFingerJoint(FT_Middle, FB_Metacarpal, "L_MiddleA");
        _riggedListener.setLFingerJoint(FT_Middle, FB_Proximal, "L_MiddleB");
        _riggedListener.setLFingerJoint(FT_Middle, FB_Intermediate, "L_MiddleC");
        _riggedListener.setLFingerJoint(FT_Middle, FB_Distal, "L_MiddleD");
    
    // bind the ring finger joints to the correct leap bones
        // right
        _riggedListener.setRFingerJoint(FT_Ring, FB_Metacarpal, "R_RingA");
        _riggedListener.setRFingerJoint(FT_Ring, FB_Proximal, "R_RingB");
        _riggedListener.setRFingerJoint(FT_Ring, FB_Intermediate, "R_RingC");
        _riggedListener.setRFingerJoint(FT_Ring, FB_Distal, "R_RingD");

        // left
        _riggedListener.setLFingerJoint(FT_Ring, FB_Metacarpal, "L_RingA");
        _riggedListener.setLFingerJoint(FT_Ring, FB_Proximal, "L_RingB");
        _riggedListener.setLFingerJoint(FT_Ring, FB_Intermediate, "L_RingC");
        _riggedListener.setLFingerJoint(FT_Ring, FB_Distal, "L_RingD");
    
    // bind the pinky joints to the correct leap bones
        // right
        _riggedListener.setRFingerJoint(FT_Pinky, FB_Metacarpal, "R_PinkyA");
        _riggedListener.setRFingerJoint(FT_Pinky, FB_Proximal, "R_PinkyB");
        _riggedListener.setRFingerJoint(FT_Pinky, FB_Intermediate, "R_PinkyC");
        _riggedListener.setRFingerJoint(FT_Pinky, FB_Distal, "R_PinkyD");

        // left
        _riggedListener.setLFingerJoint(FT_Pinky, FB_Metacarpal, "L_PinkyA");
        _riggedListener.setLFingerJoint(FT_Pinky, FB_Proximal, "L_PinkyB");
        _riggedListener.setLFingerJoint(FT_Pinky, FB_Intermediate, "L_PinkyC");
        _riggedListener.setLFingerJoint(FT_Pinky, FB_Distal, "L_PinkyD");
    
    
    // set bones for the pinch detection on the skeleton
    // @note    the R_IndexEND and L_IndexEND bones seem to be messed up
    //          that is why we use the last index bone and add an offset of
    //          1.5f to get the meshes finger tip for that finger.
    _riggedListener.setIndexPinchOffset(SLVec3f(0, 0, 1.5f)); 

    _riggedListener.setRIndexPinchBone("R_IndexD");
    _riggedListener.setRThumbPinchBone("R_ThumbEND");
    _riggedListener.setLIndexPinchBone("L_IndexD");
    _riggedListener.setLThumbPinchBone("L_ThumbEND");


    // Build interactive cube scenes for the big hands to move
    _currentGrabbedObject[0] = NULL;
    _currentGrabbedObject[1] = NULL;

    SLfloat colSpacing = 1.1f;
    SLfloat rowSpacing = 1.1f;
    SLint cols = 10;
    SLint rows = 5;
    SLint index = 0;

    SLScene::current->meshes()[7];

    for (SLint i = 0; i < cols; ++i) {
        for(SLint j = 0; j < rows; ++j) {

            SLNode* node = new SLNode;
            SLNode* childNode = new SLNode;
            childNode->addMesh(SLScene::current->meshes()[8]);
            childNode->translate(0, -0.5f, 0); // make sure local origin is in the middle of the visible mesh for 'node'
            node->addChild(childNode);
            SLScene::current->root3D()->addChild(node);
            node->translation((float)i * colSpacing - 0.5f * (float)cols * colSpacing, 2.0f + rowSpacing * (float)j + 1.5f, -5.0f, TS_World);

            _movableBoxes.push_back(node);
        }
    }

    // connect the make shift object mover callbacks
    _objectMover.setGrabCallback(std::bind(&CustomSceneView::grabCallback,this,placeholders::_1,placeholders::_2,placeholders::_3));
    _objectMover.setReleaseCallback(std::bind(&CustomSceneView::releaseCallback,this,placeholders::_1));
    _objectMover.setMoveCallback(std::bind(&CustomSceneView::moveCallback,this,placeholders::_1,placeholders::_2,placeholders::_3));
}

void CustomSceneView::grabCallback(SLVec3f& pos, SLQuat4f& rot, bool isLeft)
{
    SLint index = (isLeft) ? 0 : 1;

    _prevRotation[index] = rot;
    _prevPosition[index] = pos;
    
    SLfloat radius = 0.6f;
    for (auto cube : _movableBoxes) {
        if (cube->translation().x - radius < pos.x &&
            cube->translation().y - radius < pos.y &&
            cube->translation().z - radius < pos.z &&
            cube->translation().x + radius > pos.x &&
            cube->translation().y + radius > pos.y &&
            cube->translation().z + radius > pos.z)
        {
            _currentGrabbedObject[index] = cube;
            return;
        }
    }
}
void CustomSceneView::moveCallback(SLVec3f& pos, SLQuat4f& rot, bool isLeft)
{
    SLint index = (isLeft) ? 0 : 1;

    if (!_currentGrabbedObject[index]) 
        return;
    

    SLQuat4f relRot = _prevRotation[index] * rot.inverted();
    relRot.invert();
    _prevRotation[index] = rot;

    _currentGrabbedObject[index]->translate(pos - _prevPosition[index], TS_World);
    _currentGrabbedObject[index]->rotate(relRot, TS_World);

    _prevPosition[index] = pos;
}
void CustomSceneView::releaseCallback(bool isLeft)
{
    SLint index = (isLeft) ? 0 : 1;

    _currentGrabbedObject[index] = NULL;
    SL_LOG("RELEASED\n");
}
