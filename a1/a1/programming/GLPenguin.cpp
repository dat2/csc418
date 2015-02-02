#include "GLPenguin.h"

#define RGB_TO_OPENGL(x,y,z) x/255.0, y/255.0, z/255.0

const float LEG_LENGTH = 100.0f;
const float LEG_WIDTH = 20.0f;

const float BODY_WIDTH = 80.0f;
const float BODY_LENGTH = 100.0f;

const float JOINT_RADIUS = 3.0f;

void GLPenguin::draw(void)
{
    // Note that successive transformations are applied *before* the previous
    // ones.

    // Push the current transformation matrix on the stack
    transformStack().pushMatrix();

        drawBody();

        // Draw the 'arm'

        // Move the arm to the joint hinge
        transformStack().translate(0.0, -BODY_LENGTH/3 + LEG_WIDTH);

        // Rotate along the hinge
        transformStack().rotateInDegrees(m_arm_angle);

        // Scale the size of the arm
        transformStack().scale(LEG_WIDTH, LEG_LENGTH);

        // Move to center location of arm, under previous rotation
        transformStack().translate(0.0, -0.5);

        // Draw the square for the arm
        m_gl_state.setColor(RGB_TO_OPENGL(255, 0, 0));
        m_unit_square.draw();

    // Retrieve the previous state of the transformation stack
    transformStack().popMatrix();
}
// draw the simple hexagonal body
void GLPenguin::drawBody(void)
{
    transformStack().pushMatrix();
        transformStack().scale(BODY_WIDTH, BODY_LENGTH);

        m_gl_state.setColor(RGB_TO_OPENGL(0,0,0));
        m_unit_hexagon.draw();
    transformStack().popMatrix();
}
}
