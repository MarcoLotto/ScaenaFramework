/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <vector>
#include <glm/glm.hpp> 
using namespace std;

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

/*
Plane.h
Written by Matthew Fisher

A standard 3D plane (space plane;) i. e. the surface defined by a*x + b*y + c*z + d = 0
*/

class Plane
{
private:
	 float a, b, c, d;        //the (a, b, c, d) in a*x + b*y + c*z + d = 0.

public:
    //
    // Initalization
    //
    Plane();
    Plane(Plane &P);
    Plane(float _a, float _b, float _c, float _d);
    Plane(vec3 &NormalizedNormal, float _d);

    //
    // Static constructors
    //
    void constructFromPointNormal(vec3 Pt, vec3 Normal);                //loads the plane from a point on the surface and a normal vector
    void constructFromPoints(vec3 &V1, vec3 &V2, vec3 &V3);        //loads the plane from 3 points on the surface

    //
    // Math functions
    //
    float unsignedDistance(vec3 &Pt) ;
    float signedDistance(vec3 &Pt) ;
    bool fitToPoints(vector<vec3> &Points, float &ResidualError);
    bool fitToPoints(vector<vec4> &Points, vec3 &Basis1, vec3 &Basis2, float &NormalEigenvalue, float &ResidualError);
    vec3 closestPoint(vec3 &Point);

    //
    // Line intersection
    //
    vec3 intersectLine(vec3 &V1, vec3 &V2);         //determines the intersect of the line defined by the points V1 and V2 with the plane.
                                                            //Returns the point of intersection.  Origin is returned if no intersection exists.
    vec3 intersectLine(vec3 &V1, vec3 &V2, bool &Hit) ;    //determines the intersect of the line defined by the points V1 and V2 with the plane.
                                                                    //If there is no intersection, Hit will be false.
    float intersectLineRatio(vec3 &V1, vec3 &V2);    //Paramaterize the line with the variable t such that t = 0 is V1 and t = 1 is V2.
                                                                //returns the t for this line that lies on this plane.
    float dot(vec4 &V);            //dot product of a plane and a 4D vector
    float dotCoord(vec3 &V);        //dot product of a plane and a 3D coordinate
    float dotNormal(vec3 &V);    //dot product of a plane and a 3D normal

    //
    // Normalization
    //
    void normalize();

	 //
    // Accessors
    //
    vec3 normal() 
    {
        return vec3(a, b, c);
    }

    Plane Flip()
    {
        Plane Result;
        Result.a = -a;
        Result.b = -b;
        Result.c = -c;
        Result.d = -d;
        return Result;
    }   
};
