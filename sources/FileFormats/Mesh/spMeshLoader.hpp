/*
 * Mesh loader header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_MESHLOADER_H__
#define __SP_MESHLOADER_H__


#include "Base/spStandard.hpp"
#include "Base/spInputOutput.hpp"
#include "Base/spBaseFileFormatHandler.hpp"
#include "SceneGraph/spSceneMesh.hpp"
#include "SceneGraph/Animation/spNodeAnimation.hpp"
#include "SceneGraph/Animation/spMorphTargetAnimation.hpp"
#include "SceneGraph/Animation/spSkeletalAnimation.hpp"
#include "RenderSystem/spTextureBase.hpp"


namespace sp
{
namespace scene
{


//! Mesh loader flags. Used in the SceneManager::loadMesh function.
enum EMeshLoaderFlags
{
    MESHFLAG_SINGLE_MODEL = 0x0001, //!< Only a single 3D model is to be created. Disallows model fragmentation.
};


/*
 * ======= MeshLoader class =======
 */

class SP_EXPORT MeshLoader : public io::BaseFileFormatHandler
{
    
    public:
        
        virtual ~MeshLoader()
        {
        }
        
        virtual Mesh* loadMesh(const io::stringc &Filename, const io::stringc &TexturePath) = 0;
        
        inline Mesh* loadMesh(const io::stringc &Filename, const io::stringc &TexturePath, const s32 Flags)
        {
            LoadingFlags_ = Flags;
            return loadMesh(Filename, TexturePath);
        }
        
    protected:
        
        /* === Functions === */
        
        MeshLoader() :
            io::BaseFileFormatHandler   (   ),
            Mesh_                       (0  ),
            Surface_                    (0  ),
            Anim_                       (0  ),
            LoadingFlags_               (0  )
        {
        }
        
        inline bool openLoadFile(const io::stringc &Filename, const io::stringc &TexturePath)
        {
            Filename_       = Filename;
            TexturePath_    = TexturePath;
            Mesh_           = MemoryManager::createMemory<Mesh>("scene::Mesh");
            
            return ( File_ = FileSys_.readResourceFile(Filename) ) != 0;
        }
        
        inline bool hasFlag(s32 Flag) const
        {
            return (LoadingFlags_ & Flag) != 0;
        }
        
        /* === Members === */
        
        Mesh* Mesh_;
        video::MeshBuffer* Surface_;
        Animation* Anim_;
        s32 LoadingFlags_;
        
        io::stringc TexturePath_;
        
};


/*
 * ======= SceneLoaderRAW class =======
 */

class SceneGraph;

class SP_EXPORT SceneLoader : public io::BaseFileFormatHandler
{
    
    public:
        
        virtual ~SceneLoader()
        {
        }
        
        virtual Mesh* loadScene(
            const io::stringc &Filename, const io::stringc &TexturePath, const s32 Flags
        ) = 0;
        
    protected:
        
        /* Protected fucntions */
        
        SceneLoader() :
            io::BaseFileFormatHandler   (   ),
            Mesh_                       (0  ),
            MeshBase_                   (0  ),
            MeshTrans_                  (0  ),
            Surface_                    (0  ),
            Flags_                      (0  )
        {
        }
        
        /* Members */
        
        Mesh* Mesh_;
        Mesh* MeshBase_;
        Mesh* MeshTrans_;
        
        video::MeshBuffer* Surface_;
        
        io::stringc TexturePath_;
        
        s32 Flags_;
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
