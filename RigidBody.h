﻿#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "CbMath.h"

struct Shape;

struct RigidBody
{
	Vec2 position;        // Position
	Vec2 velocity;        // Linear Velocity
	Vec2 force;           // Linear Force 
	real orientation;     // Orientation or Angle
	real angularVelocity; // Angular Velocity	
	real torque;          // Angular Force or Torque
	real m;							  // Mass
	real invm;            // Inverse Mass
	real I;               // Inertia
	real invI;            // Inverse Inertia
	real friction;        // Friction
	real restitution;     // Restitution
	real linearDamping;   // Linear  Damping
	real angularDamping;  // Angular Damping
	real gravityScale;    // Gravity Scale

	Shape* shape; // Circle, OBB
	
	//Constructor RigidBody
	RigidBody(Shape&, const Vec2&, const real&);

	//Set Dynamic Object 
	void Dynamic(const real& Density);

	//Set Static Object.
	void Static(void);
};
#endif