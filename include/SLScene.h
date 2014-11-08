//#############################################################################
//  File:      SLScene.h
//  Author:    Marcus Hudritsch
//  Date:      July 2014
//  Codestyle: https://github.com/cpvrlab/SLProject/wiki/Coding-Style-Guidelines
//  Copyright: 2002-2014 Marcus Hudritsch
//             This software is provide under the GNU General Public License
//             Please visit: http://opensource.org/licenses/GPL-3.0
//#############################################################################

#ifndef SLSCENE_H
#define SLSCENE_H

#include <stdafx.h>
#include <SLMaterial.h>
#include <SLEventHandler.h>
#include <SLLight.h>
#include <SLNode.h>
#include <SLGLOculus.h>

class SLSceneView;
class SLButton;
class SLText;

//-----------------------------------------------------------------------------
typedef std::vector<SLSceneView*> SLVSceneView; //!< Vector of SceneView pointers
//-----------------------------------------------------------------------------
//! The SLScene class represents the top level instance holding the scene structure
/*!      
The SLScene class holds everything that is common for all scene views such as 
the root pointer (_root3D) to the scene, the background color, an array of
lights as well as the global resources (_meshes (SLMesh), _materials (SLMaterial), 
_textures (SLGLTexture) and _shaderProgs (SLGLShaderProg)).
All these resources and the scene with all nodes to whitch _root3D pointer points
get deleted in the method unInit. A scene could have multiple scene views. 
A pointer of each is stored in the vector _sceneViews. 
The onLoad method can build a of several built in test and demo scenes.
You can access the current scene from everywhere with the static pointer _current.
*/
class SLScene: public SLObject    
{  
    friend class SLNode;
    friend class SLSceneView;
   
    public:           
                            SLScene         (SLstring name="");
                           ~SLScene         ();
            // Setters
            void            root3D          (SLNode* root3D){_root3D = root3D;}
            void            menu2D          (SLButton* menu2D){_menu2D = menu2D;}
            void            backColor       (SLCol4f backColor){_backColor=backColor;}
            void            globalAmbiLight (SLCol4f gloAmbi){_globalAmbiLight=gloAmbi;}
            void            info            (SLSceneView* sv, SLstring infoText, 
                                             SLCol4f color=SLCol4f::WHITE);
                           
            // Getters
     inline SLSceneView*    sv              (SLuint index) {return _sceneViews[index];}
     inline SLNode*         root3D          () {return _root3D;}
            SLint           currentID       () {return _currentID;}
            SLfloat         timeSec         () {return (SLfloat)_timer.getElapsedTimeInSec();}
            SLfloat         timeMilliSec    () {return (SLfloat)_timer.getElapsedTimeInMilliSec();}
            SLButton*       menu2D          () {return _menu2D;}
            SLButton*       menuGL          () {return _menuGL;}
            SLGLTexture*    texCursor       () {return _texCursor;}
            SLCol4f         globalAmbiLight () {return _globalAmbiLight;}
            SLCol4f         backColor       () {return _backColor;}
            SLCol4f*        backColorV      () {return &_backColor;}
            SLVLight&       lights          () {return _lights;}
            SLVEventHandler& eventHandlers  () {return _eventHandlers;}
            SLVMaterial&    materials       () {return _materials;}
            SLVMesh&        meshes          () {return _meshes;}
            SLVGLTexture&   textures        () {return _textures;}
            SLVGLShaderProg& shaderProgs    () {return _shaderProgs;}
            SLGLShaderProg* shaderProgs     (SLStdShaderProg i) {return _shaderProgs[i];}
            SLText*         info            (SLSceneView* sv);
            SLstring        infoAbout_en    () {return _infoAbout_en;}
            SLstring        infoCredits_en  () {return _infoCredits_en;}
            SLstring        infoHelp_en     () {return _infoHelp_en;}
            SLNode*         selectedNode    () {return _selectedNode;}
            SLMesh*         selectedMesh    () {return _selectedMesh;}
            SLGLOculus*     oculus          () {return &_oculus;}   
            
            // Misc.
   virtual  void            onLoad          (SLSceneView* sv, SLCmd sceneName);
            void            init            ();
            void            unInit          ();
            void            selectNode      (SLNode* nodeToSelect);
            void            selectNodeMesh  (SLNode* nodeToSelect,
                                             SLMesh* meshToSelect);
            SLbool          onCommandAllSV  (const SLCmd cmd);

     static SLScene*        current;          //!< global static scene pointer

   protected:
            SLVSceneView    _sceneViews;      //!< Vector of all sceneview pointers

            SLNode*         _root3D;          //!< Root node for 3D scene

            SLTimer         _timer;           //!< high precision timer
            SLCol4f         _backColor;       //!< Background color 
            SLCol4f         _globalAmbiLight; //!< global ambient light intensity
            SLint           _currentID;       //!< Identifier of current scene
            SLbool          _rootInitialized; //!< Flag if scene is intialized
            SLNode*         _selectedNode;    //!< Pointer to the selected node
            SLMesh*         _selectedMesh;    //!< Pointer to the selected mesh
            
            SLText*         _info;            //!< Text node for scene info
            SLText*         _infoGL;          //!< Root text node for 2D GL stats infos
            SLText*         _infoRT;          //!< Root text node for 2D RT stats infos
            SLText*         _infoLoading;     //!< Root text node for 2D loading text
            SLstring        _infoAbout_en;    //!< About info text
            SLstring        _infoCredits_en;  //!< Credits info text
            SLstring        _infoHelp_en;     //!< Help info text

            SLButton*       _menu2D;          //!< Root button node for 2D GUI
            SLButton*       _menuGL;          //!< Root button node for OpenGL menu
            SLButton*       _menuRT;          //!< Root button node for RT menu
            SLButton*       _menuPT;          //!< Root button node for PT menu
            SLButton*       _btnAbout;        //!< About button
            SLButton*       _btnHelp;         //!< Help button
            SLButton*       _btnCredits;      //!< Credits button
            SLGLTexture*    _texCursor;       //!< Texture for the virtual cursor
            
            SLGLOculus      _oculus;          //!< Oculus Rift interface            
            
            SLVLight        _lights;          //!< Vector of all lights
            SLVEventHandler _eventHandlers;   //!< Vector of all event handler

            SLVMesh         _meshes;          //!< Vector of all meshes
            SLVMaterial     _materials;       //!< Vector of all materials pointers
            SLVGLTexture    _textures;        //!< Vector of all texture pointers
            SLVGLShaderProg _shaderProgs;     //!< Vector of all shaderProg pointers
            SLint           _numProgsPreload; //!< No. of preloaded shaderProgs
};
//-----------------------------------------------------------------------------
#endif
