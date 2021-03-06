/*
 * Scene node header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_SCENE_SCENENODE_H__
#define __SP_SCENE_SCENENODE_H__


#include "Base/spStandard.hpp"
#include "Base/spDimension.hpp"
#include "Base/spInputOutputLog.hpp"
#include "Base/spMaterialStates.hpp"
#include "Base/spGeometryStructures.hpp"
#include "Base/spNode.hpp"
#include "Base/spMath.hpp"
#include "Base/spTransformation3D.hpp"
#include "SceneGraph/spBoundingVolume.hpp"
#include "RenderSystem/spShaderProgram.hpp"

#include <list>


namespace sp
{
namespace scene
{


class Animation;

/*
 * Global members
 */

//! \todo Refactor this part. Don't use global matrices anymore!
extern dim::matrix4f spProjectionMatrix;
extern dim::matrix4f spViewMatrix;
extern dim::matrix4f spViewInvMatrix;
extern dim::matrix4f spWorldMatrix;
extern dim::matrix4f spTextureMatrix[MAX_COUNT_OF_TEXTURES];


/*
 * Enumerations
 */

//! Scene node types.
enum ENodeTypes
{
    NODE_BASICNODE,     //!< Basic scene node.
    NODE_CUSTOM,        //!< Custom scene node.
    NODE_SCENEGRAPH,    //!< Scene graph tree node.
    NODE_CAMERA,        //!< View camera.
    NODE_LIGHT,         //!< Light source.
    NODE_MESH,          //!< 3D mesh object.
    NODE_BILLBOARD,     //!< Billboard/ particle/ sprite.
    NODE_TERRAIN,       //!< Terrain object.
};


/**
Nodes are the root of each object. This is the parent class of Entity, Camera, Light, Sprite and Terrain objects.
Here you can find all the functions for locating the object in position, rotation and scaling. Some basic
information about visibility, user data (void*) etc. are implemented in this class, too.
*/
class SP_EXPORT SceneNode : public Node
{
    
    public:
        
        /* === Header === */
        
        SceneNode(const ENodeTypes Type = NODE_BASICNODE);
        virtual ~SceneNode();
        
        /* === Functions === */
        
        SceneNode* copy() const;
        
        /**
        Sets the object's position matrix.
        \param Position: Matrix which is to be used for the position transformation.
        \param isGlobal: Only useful when the object is a child of an other.
        If so and "isGlobal" is true the transformation will be processed global.
        */
        void setPositionMatrix(const dim::matrix4f &Position, bool isGlobal);
        dim::matrix4f getPositionMatrix(bool isGlobal) const;
        
        void setRotationMatrix(const dim::matrix4f &Rotation, bool isGlobal);
        dim::matrix4f getRotationMatrix(bool isGlobal) const;
        
        void setScaleMatrix(const dim::matrix4f &Scale, bool isGlobal);
        dim::matrix4f getScaleMatrix(bool isGlobal) const;
        
        void setPosition(const dim::vector3df &Position, bool isGlobal = false);
        dim::vector3df getPosition(bool isGlobal = false) const;
        
        /**
        Sets the object's rotation. A typically rotation in the engine is performed by
        the follwoing combination: Y, X, Z. This is the order in which the rotation axles are
        processed. If you want to perform an individual rotation use "setRotationMatrix"
        with your own matrix transformation.
        */
        void setRotation(const dim::vector3df &Rotation, bool isGlobal = false);
        dim::vector3df getRotation(bool isGlobal = false) const;
        
        void setScale(const dim::vector3df &Scale, bool isGlobal = false);
        dim::vector3df getScale(bool isGlobal = false) const;
        
        virtual void lookAt(const dim::vector3df &Position, bool isGlobal = false);
        
        /**
        Adds the specified animation to this scene node or rather connects the animation
        with this scene node. Normally "Animation::addSceneNode" is used but this function
        has the same effect.
        */
        virtual void addAnimation(Animation* Anim);
        
        //! Removes the specified animation.
        virtual void removeAnimation(Animation* Anim);
        
        //! Clears the animation list.
        virtual void clearAnimations();
        
        //! Returns a pointer to the animation with the specified name or null if there is no animation with that name.
        Animation* findAnimation(const io::stringc &Name) const;
        
        /* === Parents === */
        
        /**
        \param[in] isGlobal If true the visibility uses the parent hierarchy.
        \return true if this scene node and all scene nodes in its parent hierarchy are visible. Otherwise false.
        */
        virtual bool getVisible(bool isGlobal = false) const;
        
        /**
        Sets the parent object. With parents object transformations become much more easier. If the parent object
        will be moved, rotated or scaled this child object will be transformed in dependency of its parent.
        e.g. a car has four children even their tires. When the car moves around the scene you don't need to handle
        the tire transformation. But don't forget to disable parents when the parent object has been deleted but the
        children further exist!
        \param ParentNode: Pointer to the new parent Node object. If you want to disable the parent set it to 0.
        \param isGlobal: If true the current object transformation looks the same. The transformation will be performed
        in global space. Otherwise in object space.
        */
        void setParent(SceneNode* Parent, bool isGlobal);
        
        /* === Children === */
        
        //! Adds the specified child.
        virtual void addChild(SceneNode* Child);
        
        //! Adds the specified children.
        virtual void addChildren(const std::list<SceneNode*> &Children);
        
        //! Removes the specified child and returns true if the child could be removed.
        virtual bool removeChild(SceneNode* Child);
        
        //! Removes the first child and returns true if a child could be removed.
        virtual bool removeChild();
        
        //! Removes the specified children and returns the count of removed children.
        virtual u32 removeChildren(const std::list<SceneNode*> &Children);
        
        //! Removes all children.
        virtual void removeChildren();
        
        /* === Parent system === */
        
        //! Updates the objects transformation.
        virtual void updateTransformation();
        virtual void updateTransformationBase(const dim::matrix4f &BaseMatrix);
        
        Transformation getTransformation(bool isGlobal) const;
        
        //! Loads the transformation into the render system which has been updated previously.
        virtual void loadTransformation();
        
        /* === Inline functions === */
        
        /**
        Returns the scene node type.
        \see ENodeTypes
        */
        inline ENodeTypes getType() const
        {
            return Type_;
        }
        
        /**
        Sets the new bounding volume.
        \see BoundingVolume
        */
        inline void setBoundingVolume(const BoundingVolume &BoundVolume)
        {
            BoundVolume_ = BoundVolume;
        }
        
        /**
        Returns a reference to the bounding volume.
        \see BoundingVolume
        */
        inline BoundingVolume& getBoundingVolume()
        {
            return BoundVolume_;
        }
        /**
        Returns a constant reference to the bounding volume.
        \see BoundingVolume
        */
        inline const BoundingVolume& getBoundingVolume() const
        {
            return BoundVolume_;
        }
        
        #if 1 // !!!DEPRECATED!!!

        //! \deprecated Use setPosition instead.
        inline void setPositionMatrix(const dim::matrix4f &Position)
        {
            Transform_.setPosition(Position.getPosition());
        }
        //! \deprecated Use getPosition instead.
        inline dim::matrix4f getPositionMatrix() const
        {
            return dim::getPositionMatrix(Transform_.getPosition());
        }
        
        //! \deprecated Use setRotation instead.
        inline void setRotationMatrix(const dim::matrix4f &Rotation)
        {
            Transform_.setRotation(Rotation);
        }
        //! \deprecated Use getRotation instead.
        inline dim::matrix4f getRotationMatrix() const
        {
            return Transform_.getRotationMatrix();
        }
        
        //! \deprecated Use setScale instead.
        inline void setScaleMatrix(const dim::matrix4f &Scale)
        {
            Transform_.setScale(Scale.getScale());
        }
        //! \deprecated Use getScale instead.
        inline dim::matrix4f getScaleMatrix() const
        {
            return dim::getScaleMatrix(Transform_.getScale());
        }

        #endif // /!!!
        
        //! Sets the final world matrix used in the render system.
        inline void setupTransformation(bool isGlobal)
        {
            FinalWorldMatrix_ = getTransformation(isGlobal).getMatrix();
        }
        //! Sets the final world matrix. Use this immediately before the object will be renderd.
        inline void setupWorldMatrix(const dim::matrix4f &WorldMatrix)
        {
            FinalWorldMatrix_ = WorldMatrix;
        }
        
        /**
        Returns a constant reference to the transformation.
        \see Transformation
        */
        inline const Transformation& getTransformation() const
        {
            return Transform_;
        }
        /**
        Returns a reference to the transformation.
        \see Transformation
        */
        inline Transformation& getTransformation()
        {
            return Transform_;
        }
        
        inline dim::matrix4f getTransformMatrix(bool isGlobal = false) const
        {
            return getTransformation(isGlobal).getMatrix();
        }
        
        inline void setTransformation(const Transformation &Transform)
        {
            Transform_ = Transform;
        }
        
        //! Returns the children list.
        inline const std::vector<SceneNode*> &getSceneChildren() const
        {
            return SceneChildren_;
        }
        inline std::vector<SceneNode*> &getSceneChildren()
        {
            return SceneChildren_;
        }
        
        //! Sets the parent object only in object space (this is a little bit faster).
        inline void setParent(SceneNode* Parent)
        {
            SceneParent_ = Parent;
        }
        inline SceneNode* getParent() const
        {
            return SceneParent_;
        }
        
        //! Returns the whole animation list.
        inline const std::vector<Animation*>& getAnimationList() const
        {
            return AnimationList_;
        }
        
        /**
        Returns the count of animations which are linked to this scene node.
        \todo Rename to "getNumAnimations"
        */
        inline size_t getAnimationCount() const
        {
            return AnimationList_.size();
        }
        
        //! Moves the object in the specified direction. This is dependent on the current rotation transformation.
        inline void move(const dim::vector3df &Direction)
        {
            Transform_.move(Direction);
        }
        //! Turns the object with the specified relative euler rotation.
        inline void turn(const dim::vector3df &RelativeRotation)
        {
            Transform_.turn(RelativeRotation);
        }
        //! Turns the object into the specified relative rotation with a none-centered origin.
        inline void turn(const dim::quaternion &RelativeRotation, const dim::vector3df &GlobalRotationOrigin)
        {
            Transform_.turn(RelativeRotation, GlobalRotationOrigin);
        }
        //! Moves the object in the specified direction. This is independent on the current rotation transformation.
        inline void translate(const dim::vector3df &Direction)
        {
            Transform_.translate(Direction);
        }
        //! Transforms the object with the specified size.
        inline void transform(const dim::vector3df &Size)
        {
            Transform_.transform(Size);
        }
        
        //! Returns a pointer to the specified animation object or null if that animation does not exist or the type is wrong.
        template <typename T> inline T* getAnimation(size_t Index = 0) const
        {
            return Index < AnimationList_.size() ? dynamic_cast<T*>(AnimationList_[Index]) : 0;
        }
        //! Returns a pointer to the specified animation object or null if that animation does not exist.
        inline Animation* getAnimation(size_t Index = 0) const
        {
            return Index < AnimationList_.size() ? AnimationList_[Index] : 0;
        }

    protected:
        
        friend class Animation;
        
        /* === Functions === */
        
        void copyRoot(SceneNode* NewNode) const;
        
        /* === Members === */
        
        BoundingVolume BoundVolume_;
        Transformation Transform_;
        
        dim::matrix4f FinalWorldMatrix_;
        
        SceneNode* SceneParent_;
        
        std::vector<SceneNode*> SceneChildren_;
        
        std::vector<Animation*> AnimationList_;
        
    private:
        
        /* === Members === */
        
        ENodeTypes Type_;
        
};


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
