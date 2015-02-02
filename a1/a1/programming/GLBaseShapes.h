/*************************************************************************
    CSC418/2504, Winter 20l5
    Assignment 1

    Instructions:
        See main.cpp for more instructions.

        This file contains the UnitSquare and the UnitCircle classes.
**************************************************************************/
#ifndef __GL_BASE_SHAPES
#define __GL_BASE_SHAPES

#include "GLShape.h"
#include <cmath>

// Before transformed, this class displays a unit square centered at the
// origin.
class UnitSquare : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location)
    {
    // Use two triangles to create the square.
        GLfloat square_vertices[][2] =
        {
            { -0.5, -0.5 },  // Triangle 1
            {  0.5, -0.5 },
            {  0.5,  0.5 },
            { -0.5, -0.5 },  // Triangle 2
            {  0.5,  0.5 },
            { -0.5,  0.5 },
        };

        initialize(
        shader_input_location,
            reinterpret_cast<const GLfloat *>(square_vertices),
            /*num_vertices=*/6,
        GL_TRIANGLES); // Each group of three coordinates is a triangle
    }
};

// Before transformed, this class displays a unit circle centered at the
// origin.
class UnitCircle : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location, int num_circle_segments)
    {
        // We will draw a circle as a triangle fan.  We are careful to send
    // the second coordinate twice to properly close the circle.
        //        3     2     1
        //         +----+----+
        //        / \   |   /
        //       /   \  |  /
        //      /     \ | /
        //     /       \|/
        //   4+---------+ 0
        //        ...
        std::vector<GLfloat> circle_vertices;
        circle_vertices.push_back(0.0);
        circle_vertices.push_back(0.0);
        for (int i=0; i<=num_circle_segments; ++i)
        {
            double angle = (2 * M_PI * i) / num_circle_segments;
            circle_vertices.push_back(cos(angle));
            circle_vertices.push_back(sin(angle));
        }

        initialize(
        shader_input_location,
            &circle_vertices[0],
            num_circle_segments + 1,
        GL_TRIANGLE_FAN);
    }
};

class UnitHexagon : public GLShape
{
public:
    using GLShape::initialize;

    void initialize(int shader_input_location)
    {

        GLfloat hex_vertices[][2] =
        {
            {  0.0,  0.0 },
            {  0.5,  1.0 },
            { -0.5,  1.0 }, // top slice
            { -1.0, -0.5 }, // left top slice
            { -0.5, -1.0 }, // left bottom slice
            {  0.5, -1.0 }, // bottom slice
            {  1.0, -0.5 }, // right bottom slice
            {  0.5,  1.0 }, // top right slice
        };

        initialize(
        shader_input_location,
            reinterpret_cast<const GLfloat *>(hex_vertices),
            /*num_vertices=*/8,
        GL_TRIANGLE_FAN);
    }

};

#endif
