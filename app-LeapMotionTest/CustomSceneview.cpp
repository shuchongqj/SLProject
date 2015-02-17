

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

void drawXZGrid(const SLMat4f& mat)
{
    // for now we don't want to update the mesh implementation
    // or the buffer implementation, so we don't have vertex color support
    static bool         initialized = false;
    static SLGLBuffer   grid;
    static SLint        indexX;
    static SLint        indexZ;
    static SLint        indexGrid;
    static SLint        numXVerts;
    static SLint        numZVerts;
    static SLint        numGridVerts;

    if (!initialized)
    {
        vector<SLVec3f>  gridVert;

        SLint gridLineNum = 21;
        gridLineNum += gridLineNum%2 - 1; // make sure grid is odd
        SLint gridHalf = gridLineNum / 2;
        SLfloat gridSize = 1;
        SLfloat gridMax = (SLfloat)gridHalf/(gridLineNum-1) * gridSize;
        SLfloat gridMin = -gridMax;

        
        // x
        gridVert.push_back(SLVec3f(gridMin, 0, 0));
        gridVert.push_back(SLVec3f(gridMax, 0, 0));
        // z
        gridVert.push_back(SLVec3f(0, 0, gridMin));
        gridVert.push_back(SLVec3f(0, 0, gridMax));

        indexX = 0;
        indexZ = 2;
        indexGrid = 4;
        numXVerts = 2;
        numZVerts = 2;
        numGridVerts = (gridLineNum-1)*4;

        for (int i = 0; i < gridLineNum; ++i) 
        {
            SLfloat offset = (SLfloat)(i - gridHalf);
            offset /= (SLfloat)(gridLineNum-1);
            offset *= gridSize;
            
            // we're at the center
            if (offset != 0) 
            {
                // horizontal lines
                gridVert.push_back(SLVec3f(gridMin, 0, offset));
                gridVert.push_back(SLVec3f(gridMax, 0, offset));
                // vertical lines
                gridVert.push_back(SLVec3f(offset, 0, gridMin));
                gridVert.push_back(SLVec3f(offset, 0, gridMax));
            }
        }

        grid.generate(&gridVert[0], gridVert.size(), 3);

        initialized = true;
    }

    
    SLGLState* state = SLGLState::getInstance();
    state->pushModelViewMatrix();
    state->modelViewMatrix = mat;

    grid.drawArrayAsConstantColorLines(SLCol3f::RED,   1.0f, indexX, numXVerts);
    grid.drawArrayAsConstantColorLines(SLCol3f::BLUE, 1.0f, indexZ, numZVerts);
    grid.drawArrayAsConstantColorLines(SLCol3f(0.45f, 0.45f, 0.45f),  0.8f, indexGrid, numGridVerts);
    
    state->popModelViewMatrix();
}

















void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
    // Get the most recent frame and report some basic information
    const Frame frame = controller.frame();


    HandList hands = frame.hands();
    for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
        // Get the first hand
        const Hand hand = *hl;
        SLint index = 0;
        std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
        Vector pos = hand.palmPosition();
        SLVec3f slPos(pos.x, pos.y, pos.z);
        if (hand.isLeft())
            positionsLeft[index++] = slPos;
        else
            positionsRight[index++] = slPos;
        

        
    
        FingerList fingers = hand.fingers();
        for(Leap::FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); fl++) {
            Bone bone;
            Bone::Type boneType;
            for (int b = 0; b < 4; ++b) {
                boneType = static_cast<Bone::Type>(b);
                bone = (*fl).bone(boneType);

                SLVec3f prevPos(bone.prevJoint().x, bone.prevJoint().y, bone.prevJoint().z);
                SLVec3f nextPos(bone.nextJoint().x, bone.nextJoint().y, bone.nextJoint().z);

                if (hand.isLeft()) {
                    positionsLeft[index++] = prevPos;
                    if (boneType == Bone::Type::TYPE_DISTAL)
                        positionsLeft[index++] = nextPos;
                }
                else {
                    positionsRight[index++] = prevPos;
                    if (boneType == Bone::Type::TYPE_DISTAL)
                        positionsRight[index++] = nextPos;
                }
            }
        }
    }
}

void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void SampleListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}


















// builds a custom scene with a grid where every node is animated
void SLScene::onLoad(SLSceneView* sv, SLCmd cmd)
{
    init();

    /*
    name("LeapMotion Test Example");
    info(sv, "LeapMotion Test Example");

    
    _backColor.set(0.1f,0.1f,0.1f);
    
    SLLightSphere* light1 = new SLLightSphere(0.1f);
    light1->position(2, -4, 3);

    SLLightSphere* light2 = new SLLightSphere(0.1f);
    light2->ambient(SLCol4f(0.2f, 0.1f, 0.0f, 1.0f));
    light2->diffuse(SLCol4f(2.0f, 0.9f, 0.5f, 1.0f));
    light2->position(2, 1, -3);
    
    SLLightSphere* light3 = new SLLightSphere(0.1f);
    light3->position(-5, 15, 10);

    SLMaterial* mat = new SLMaterial("floorMat", SLCol4f::GRAY, SLCol4f::GRAY);

    SLNode* scene = new SLNode;
    scene->addChild(light1);
    scene->addChild(light2);
    scene->addChild(light3);
    
    
    SLAssimpImporter importer("Importer.log");
    //SLNode* meshDAE = importer.load("DAE/AstroBoy/AstroBoy.dae");
    //scene->addChild(meshDAE);
    importer.logFileVerbosity(LV_Detailed);
    SLNode* meshDAE2 = importer.load("DAE/Hands/rigged_hands.dae");
    scene->addChild(meshDAE2);
    //meshDAE2->scale(12.5);
    //meshDAE2->translate(0, 0, 5);

    
    for (SLint i = 0; i < importer.meshes().size(); ++i) {
        //importer.meshes()[i]->skinningMethod(SM_HardwareSkinning);
    }
    


    SLCamera* cam1 = new SLCamera;
    cam1->position(-4, 3, 3);
    cam1->lookAt(0, 0, 1);
    cam1->focalDist(6);

    scene->addChild(cam1);
    
    _root3D = scene;

    sv->camera(cam1);

    

    /*/
    
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
    cam1->position(0, 0.0f, 0);    // eye height for 180cm high male
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
    _riggedListener.setModelScale(66.6666666f); // this particular model is scaled down by 0.01 to fit real life units (meters) 
    _riggedListener.setOrigin(SLVec3f(0, 1.1f, -0.6f));

    //_riggedListener.setScaleCorrection(SLVec3f(10, 10, 10));

    // set the wrist joints
    _riggedListener.setLWrist("L_Wrist");
    _riggedListener.setRWrist("R_Wrist");
    
    // bind the thumb joints to the correct leap bones
    _riggedListener.setRFingerJoint(0, 1, "R_ThumbA");
    _riggedListener.setRFingerJoint(0, 2, "R_ThumbB");
    _riggedListener.setRFingerJoint(0, 3, "R_ThumbC");
    
    // bind the index finger joints to the correct leap bones
    _riggedListener.setRFingerJoint(1, 0, "R_IndexA");
    _riggedListener.setRFingerJoint(1, 1, "R_IndexB");
    _riggedListener.setRFingerJoint(1, 2, "R_IndexC");
    _riggedListener.setRFingerJoint(1, 3, "R_IndexD");
    
    // bind the middle finger joints to the correct leap bones
    _riggedListener.setRFingerJoint(2, 0, "R_MiddleA");
    _riggedListener.setRFingerJoint(2, 1, "R_MiddleB");
    _riggedListener.setRFingerJoint(2, 2, "R_MiddleC");
    _riggedListener.setRFingerJoint(2, 3, "R_MiddleD");
    
    // bind the ring finger joints to the correct leap bones
    _riggedListener.setRFingerJoint(3, 0, "R_RingA");
    _riggedListener.setRFingerJoint(3, 1, "R_RingB");
    _riggedListener.setRFingerJoint(3, 2, "R_RingC");
    _riggedListener.setRFingerJoint(3, 3, "R_RingD");
    
    // bind the pinky joints to the correct leap bones
    _riggedListener.setRFingerJoint(4, 0, "R_PinkyA");
    _riggedListener.setRFingerJoint(4, 1, "R_PinkyB");
    _riggedListener.setRFingerJoint(4, 2, "R_PinkyC");
    _riggedListener.setRFingerJoint(4, 3, "R_PinkyD");
    
    // same as above for the LEFT hand
    _riggedListener.setLFingerJoint(0, 1, "L_ThumbA");
    _riggedListener.setLFingerJoint(0, 2, "L_ThumbB");
    _riggedListener.setLFingerJoint(0, 3, "L_ThumbC");
    
    _riggedListener.setLFingerJoint(1, 0, "L_IndexA");
    _riggedListener.setLFingerJoint(1, 1, "L_IndexB");
    _riggedListener.setLFingerJoint(1, 2, "L_IndexC");
    _riggedListener.setLFingerJoint(1, 3, "L_IndexD");
    
    _riggedListener.setLFingerJoint(2, 0, "L_MiddleA");
    _riggedListener.setLFingerJoint(2, 1, "L_MiddleB");
    _riggedListener.setLFingerJoint(2, 2, "L_MiddleC");
    _riggedListener.setLFingerJoint(2, 3, "L_MiddleD");
    
    _riggedListener.setLFingerJoint(3, 0, "L_RingA");
    _riggedListener.setLFingerJoint(3, 1, "L_RingB");
    _riggedListener.setLFingerJoint(3, 2, "L_RingC");
    _riggedListener.setLFingerJoint(3, 3, "L_RingD");
    
    _riggedListener.setLFingerJoint(4, 0, "L_PinkyA");
    _riggedListener.setLFingerJoint(4, 1, "L_PinkyB");
    _riggedListener.setLFingerJoint(4, 2, "L_PinkyC");
    _riggedListener.setLFingerJoint(4, 3, "L_PinkyD");

    /* OLD ASTROBOY BIND, NEEDS TO BE REDONE
    _riggedListener.setLWrist("L_wrist");
    _riggedListener.setRWrist("R_wrist");
    // thumb
    //_riggedListener.setLFingerJoint(0, 1, "L_thumbOrient");
    _riggedListener.setLFingerJoint(0, 1, "L_thumb_01");
    _riggedListener.setLFingerJoint(0, 3, "L_thumb_02");
    _riggedListener.setRFingerJoint(0, 1, "R_thumb_01");
    _riggedListener.setRFingerJoint(0, 3, "R_thumb_02");
    // index
    _riggedListener.setLFingerJoint(1, 1, "L_index_01");
    _riggedListener.setLFingerJoint(1, 2, "L_index_02");
    _riggedListener.setRFingerJoint(1, 1, "R_index_01");
    _riggedListener.setRFingerJoint(1, 2, "R_index_02");
    // middle
    _riggedListener.setLFingerJoint(2, 1, "L_middle_01");
    _riggedListener.setLFingerJoint(2, 2, "L_middle_02");
    _riggedListener.setRFingerJoint(2, 1, "R_middle_01");
    _riggedListener.setRFingerJoint(2, 2, "R_middle_01");
    // pinky
    _riggedListener.setLFingerJoint(4, 1, "L_pinky_01");
    _riggedListener.setLFingerJoint(4, 2, "L_pinky_02");
    _riggedListener.setRFingerJoint(4, 1, "R_pinky_01");
    _riggedListener.setRFingerJoint(4, 2, "R_pinky_02");
    */


    // cubes to grab and interact with
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
            node->position((float)i * colSpacing - 0.5f * (float)cols * colSpacing, 2.0f + rowSpacing * (float)j + 1.5f, -5.0f, TS_World);

            _movableBoxes.push_back(node);
        }
    }

    
    _objectMover.setGrabCallback(std::bind(&CustomSceneView::grabCallback,this,placeholders::_1,placeholders::_2,placeholders::_3));
    _objectMover.setReleaseCallback(std::bind(&CustomSceneView::releaseCallback,this,placeholders::_1));
    _objectMover.setMoveCallback(std::bind(&CustomSceneView::moveCallback,this,placeholders::_1,placeholders::_2,placeholders::_3));
}

void CustomSceneView::preDraw()
{
    
}


void CustomSceneView::postDraw()
{/*
    // hacked in world grid with x, z axes marked by color
    SLMat4f orig(0.0f, 1.11f, -1.0f); // move origin on the table
    drawXZGrid(_camera->updateAndGetVM() * orig);*/
}


// some basic manipulation for now
SLbool CustomSceneView::onKeyPress(const SLKey key, const SLKey mod)
{
    return SLSceneView::onKeyPress(key, mod);
}

SLbool CustomSceneView::onKeyRelease(const SLKey key, const SLKey mod)
{
    return SLSceneView::onKeyRelease(key, mod);
}



void CustomSceneView::grabCallback(SLVec3f& pos, SLQuat4f& rot, bool isLeft)
{
    SLint index = (isLeft) ? 0 : 1;

    _prevRotation[index] = rot;
    _prevPosition[index] = pos;
    
    SLfloat radius = 0.6f;
    for (auto cube : _movableBoxes) {
        if (cube->position().x - radius < pos.x &&
            cube->position().y - radius < pos.y &&
            cube->position().z - radius < pos.z &&
            cube->position().x + radius > pos.x &&
            cube->position().y + radius > pos.y &&
            cube->position().z + radius > pos.z)
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
