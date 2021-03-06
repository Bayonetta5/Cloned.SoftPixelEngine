/*
 * Physics joint header
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#ifndef __SP_PHYSICS_JOINT_H__
#define __SP_PHYSICS_JOINT_H__


#include "Base/spStandard.hpp"

#ifdef SP_COMPILE_WITH_PHYSICS


#include "Base/spDimensionVector3D.hpp"
#include "Base/spDimensionMatrix4.hpp"


namespace sp
{
namespace physics
{


class RigidBody;
class PhysicsBaseObject;

//! Rigid body joint types.
enum EPhysicsJoints
{
    JOINT_BALL,         //! Can be used for a rope.
    JOINT_HINGE,        //! Can be used for a door.
    JOINT_SLIDER,       //! Can be used for a sliding object.
    JOINT_CORKSCREW,    //! Can be used for a sliding and rotating object.
    JOINT_UNIVERSAL,    //! Can be used for a free rotating object.
};

struct SPhysicsJointConstruct
{
    SPhysicsJointConstruct()
    {
    }
    SPhysicsJointConstruct(
        const dim::vector3df &Pnt) :
        PointA(Pnt),
        PointB(Pnt)
    {
    }
    SPhysicsJointConstruct(
        const dim::vector3df &Pnt, const dim::vector3df &Dir) :
        PointA      (Pnt),
        PointB      (Pnt),
        DirectionA  (Dir),
        DirectionB  (Dir)
    {
        DirectionA.normalize();
        DirectionB.normalize();
    }
    SPhysicsJointConstruct(
        const dim::vector3df &PntA, const dim::vector3df &PntB,
        const dim::vector3df &DirA, const dim::vector3df &DirB) :
        PointA      (PntA),
        PointB      (PntB),
        DirectionA  (DirA),
        DirectionB  (DirB)
    {
        DirectionA.normalize();
        DirectionB.normalize();
    }
    SPhysicsJointConstruct(
        const dim::matrix4f TransA, const dim::matrix4f TransB) :
        TransformA(TransA), TransformB(TransB)
    {
    }
    ~SPhysicsJointConstruct()
    {
    }
    
    /* Members */
    dim::vector3df PointA, PointB;
    dim::vector3df DirectionA, DirectionB;
    dim::matrix4f TransformA, TransformB;
};


/**
This is the physics joint class. In some physics engines this is called a "constraint".
This class represents any kind of physics joints, i.e. ball, hinge, slider etc.
e.g. a hinge can be used for doors or wheels.
*/
class SP_EXPORT PhysicsJoint
{
    
    public:
        
        virtual ~PhysicsJoint();
        
        /* === Functions === */
        
        virtual void setPosition(const dim::vector3df &Position) = 0;
        virtual dim::vector3df getPosition() const = 0;
        
        virtual void translate(const dim::vector3df &Direction);
        
        // Enables or disables the limitations. By default disabled.
        virtual void setLimit(bool Enable) = 0;
        virtual bool getLimit() const = 0;
        
        //! Sets the limitation values. This can be either an angle or linear value.
        virtual void setLimit(f32 Min, f32 Max, bool Enable = true) = 0;
        virtual void getLimit(f32 &Min, f32 &Max) const = 0;
        
        /**
        Activates the motor interaction.
        \param Enable: Specifies whether the hinge motor is to be enabled or disabled. By default disabled.
        Once the motor is enabled it runs until it will be disabled.
        \param MotorPower: Specifies the power of the motor. This should be at least the mass of
        the heaviest rigid body which is connected to this joint.
        \see getMotor
        */
        virtual void setMotor(bool Enable, f32 MotorPower = 100.0f) = 0;
        virtual bool getMotor() const = 0;
        
        virtual void runMotor(f32 Velocity) = 0;
        
        //! Returns the joint linear value. This can also be an angle (for a hinge-joint).
        virtual f32 getLinearValue() const = 0;
        
        /* === Inline functions === */
        
        //! Returns the type of this joint.
        inline EPhysicsJoints getType() const
        {
            return Type_;
        }
        
        //! Returns a pointer to the first joint's physics object.
        inline PhysicsBaseObject* getFirstObject() const
        {
            return ObjectA_;
        }
        //! Returns a pointer to the second joint's physics object.
        inline PhysicsBaseObject* getSecondObject() const
        {
            return ObjectB_;
        }
        
    protected:
        
        PhysicsJoint(
            const EPhysicsJoints Type, PhysicsBaseObject* ObjectA, PhysicsBaseObject* ObjectB
        );
        
        /* === Members === */
        
        EPhysicsJoints Type_;
        
        PhysicsBaseObject* ObjectA_;
        PhysicsBaseObject* ObjectB_;
        
};


} // /namespace physics

} // /namespace sp


#endif

#endif



// ================================================================================
