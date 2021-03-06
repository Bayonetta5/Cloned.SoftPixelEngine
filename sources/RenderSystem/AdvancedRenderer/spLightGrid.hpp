/*
 * Light grid header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_LIGHT_GRID_H__
#define __SP_LIGHT_GRID_H__


#include "Base/spStandard.hpp"

#if defined(SP_COMPILE_WITH_ADVANCEDRENDERER)


#include "RenderSystem/spTextureBase.hpp"


namespace sp
{

namespace scene
{
    class Light;
    class Camera;
    class SceneGraph;
}

namespace video
{


class ShaderResource;
class ShaderClass;
class Shader;


//!!!
//#define _DEB_DEPTH_EXTENT_
#ifdef _DEB_DEPTH_EXTENT_
extern video::ShaderResource* _debDepthExt_;
#endif

/**
The light grid is used by the deferred renderer for tiled shading.
\since Version 3.3
*/
class SP_EXPORT LightGrid
{
    
    public:
        
        //! Light grid size (always 32 x 32).
        static const dim::size2di GRID_SIZE;

        LightGrid();
        ~LightGrid();
        
        /* === Functions === */
        
        /**
        Creates the light grid.
        \param[in] Resolution Specifies the resolution. This should be the same as specified for the engine's graphics device.
        \param[in] MaxNumLights Specifies the maximal number of lights.
        \return True if the grid could be created successful.
        */
        bool createGrid(const dim::size2di &Resolution, u32 MaxNumLights);
        void deleteGrid();
        
        /**
        Updates the data for the light raw models (position, radius etc. but no color).
        \param[in] PointLights Specifies the list of all point lights. Each element is a 4D vector
        containing the position (XYZ) and radius (W).
        \param[in] NumLights Specifies how many lights from the list are to be used.
        */
        void updateLights(const std::vector<dim::vector4df> &PointLights, u32 NumLights);

        /**
        Builds the light grid. For Direct3D 11 this function uses a compute shader.
        Otherwise the grid will be computed on the CPU.
        */
        void build(scene::SceneGraph* Graph, scene::Camera* ActiveCamera, video::Texture* DepthTexture);

        //! Binds the TLI texture.
        s32 bind(s32 TexLayerBase);
        //! Unbinds the TLI texture.
        s32 unbind(s32 TexLayerBase);
        
        //! Sets the new resolution (or rather resizes the current resolution).
        void setResolution(const dim::size2di &Resolution);
        
        /* === Static functions === */

        static dim::size2di computeNumTiles(const dim::size2di &Resolution);

        /* === Inline functions === */
        
        //! Returns the TLI (Tile Light Index List) texture object. This is an signed integer texture buffer.
        inline Texture* getTLITexture() const
        {
            return TLITexture_;
        }

        /**
         * Returns the LG (Light Grid) shader resource object. This is an 'uint shader buffer'.
         * \code
         * // HLSL Example:
         * Buffer<uint> MyLGBuffer : register(t0);
         * 
         * // GLSL Example:
         * layout(std430, binding = 0) buffer MyLGBuffer
         * {
         *     uint Offset[];
         * };
         * \endcode
         */
        inline ShaderResource* getLGShaderResource() const
        {
            return LGShaderResource_;
        }
        
        /**
         * Returns the TLI (Tile Light Index List) shader resource object. This is a 'structured shader buffer'.
         * \code
         * // For HLSL and GLSL:
         * struct SLightNode
         * {
         *     uint LightID;
         *     uint Next;
         * };
         * 
         * // HLSL Example:
         * StructuredBuffer<SLightNode> LightList : register(t2);
         * 
         * // GLSL Example:
         * layout(std430, binding = 2) buffer BufferLightList
         * {
         *     SLightNode LightList[];
         * };
         * \endcode
         */
        inline ShaderResource* getTLIShaderResource() const
        {
            return TLIShaderResource_;
        }
        
        /**
        Returns the number of tiles on X and Y axes. This can only be set on light-grid creation time.
        \see createGrid
        */
        inline const dim::size2di& getNumTiles() const
        {
            return NumTiles_;
        }

        /**
        Returns true if the light grid building process is hardware accelerated.
        \deprecated Only GPU is to be used for generating the light grid!
        */
        inline bool useGPU() const
        {
            return ShdClass_ != 0 && ShdClassInit_ != 0;
        }
        
    private:
        
        /* === Functions === */
        
        bool createTLITexture();

        bool createShaderResources();
        bool setupShaderResources();
        bool createComputeShaders(const dim::size2di &Resolution);
        
        void setupMainConstBuffer(
            Shader* CompShd, Shader* CompShdInit, const dim::size2di &Resolution
        );
        
        void buildOnGPU(scene::SceneGraph* Graph, scene::Camera* Cam, video::Texture* DepthTexture);
        void buildOnCPU(scene::SceneGraph* Graph, scene::Camera* Cam, video::Texture* DepthTexture);

        /* === Members === */
        
        //! Shader class for building the tile-light-index list buffer.
        ShaderClass* ShdClass_;
        //! Shader class for initializing the light-grid buffer.
        ShaderClass* ShdClassInit_;
        
        //! This is a texture buffer storing the light indicies. Currently used for OpenGL. \deprecated
        Texture* TLITexture_;
        
        ShaderResource* LGShaderResource_;
        
        //! This is the shader resource filled by the compute shader. This is private only.
        ShaderResource* TLIShaderResource_;
        
        ShaderResource* SRGlobalCounter_;
        ShaderResource* SRPointLights_;
        
        dim::size2di NumTiles_;
        u32 NumLights_;
        u32 MaxNumLights_;
        
};


} // /namespace video

} // /namespace sp


#endif

#endif



// ================================================================================
