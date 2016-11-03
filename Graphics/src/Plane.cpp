/**********************************
* SCAENA FRAMEWORK
**********************************/
/*
Plane.cpp
Written by Matthew Fisher

A standard 3D plane (space plane.)  Essentially just the surface defined by a*x + b*y + c*z + d = 0
See Plane.h for a description of these functions.
*/

#include "Plane.h"

Plane::Plane()
{
}

Plane::Plane(Plane &P)
{
    this->a = P.a;
    this->b = P.b;
    this->c = P.c;
    this->d = P.d;
}

Plane::Plane(float _a, float _b, float _c, float _d)
{
    this->a = _a;
    this->b = _b;
    this->c = _c;
    this->d = _d;
}

Plane::Plane(vec3 &NormalizedNormal, float _d)
{
    this->a = NormalizedNormal.x;
    this->b = NormalizedNormal.y;
    this->c = NormalizedNormal.z;
    this->d = _d;
}

void Plane::constructFromPointNormal(vec3 Pt, vec3 Normal)
{    
    vec3 NormalizedNormal = glm::normalize(Normal);
    this->a = NormalizedNormal.x;
    this->b = NormalizedNormal.y;
    this->c = NormalizedNormal.z;
    this->d = -glm::dot(Pt, NormalizedNormal);
}

void Plane::normalize()
{   
    float Distance = sqrtf(a * a + b * b + c * c);
    this->a /= Distance;
    this->b /= Distance;
    this->c /= Distance;
    this->d /= Distance;
}

void Plane::constructFromPoints(vec3 &V0, vec3 &V1, vec3 &V2){
    vec3 Normal = glm::normalize(glm::cross(V1 - V0, V2 - V0));
   this->constructFromPointNormal(V0, Normal);
}

vec3 Plane::intersectLine(vec3 &V1, vec3 &V2) 
{
    vec3 Diff = V1 - V2;
    float Denominator = a * Diff.x + b * Diff.y + c * Diff.z;
    if(Denominator == 0.0f)
    {
        return (V1 + V2) * 0.5f;
    }
    float u = (a * V1.x + b * V1.y + c * V1.z + d) / Denominator;

    return (V1 + u * (V2 - V1));
}

vec3 Plane::intersectLine(vec3 &V1, vec3 &V2, bool &Hit) 
{
    Hit = true;
    vec3 Diff = V2 - V1;
    float denominator = a * Diff.x + b * Diff.y + c * Diff.z;
    if(denominator == 0) {Hit = false; return V1;}
    float u = (a * V1.x + b * V1.y + c * V1.z + d) / denominator;

    return (V1 + u * (V2 - V1));
}

float Plane::intersectLineRatio(vec3 &V1, vec3 &V2)
{
    vec3 Diff = V2 - V1;
    float Denominator = a * Diff.x + b * Diff.y + c * Diff.z;
    if(Denominator == 0.0f)
    {
        return 0.0f;
    }
    return (a * V1.x + b * V1.y + c * V1.z + d) / -Denominator;
}

float Plane::signedDistance(vec3 &Pt) 
{
    return (a * Pt.x + b * Pt.y + c * Pt.z + d);
}

float Plane::unsignedDistance( vec3 &Pt) 
{
    return glm::abs(a * Pt.x + b * Pt.y + c * Pt.z + d);
}

vec3 Plane::closestPoint(vec3 &Point)
{
    return (Point - normal() * signedDistance(Point));
}

float Plane::dot(vec4 &V)
{
    return a * V.x + b * V.y + c * V.z + d * V.w;
}

float Plane::dotCoord(vec3 &V)
{
    return a * V.x + b * V.y + c * V.z + d;
}

float Plane::dotNormal(vec3 &V)
{
    return a * V.x + b * V.y + c * V.z;
}
