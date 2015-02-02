#include "GLPenguin.h"

void GLPenguin::drawBody(void)
{
    const float BODY_WIDTH = 60.0f;
    const float BODY_LENGTH = 100.0f;

    // Draw the 'body'
    transformStack.pushMatrix();
        // Scale square to size of body
        transformStack.scale(BODY_WIDTH, BODY_LENGTH);

        // Set the colour to green
        m_gl_state.setColor(0.0, 1.0, 0.0);

        // Draw the square for the body
        m_unit_square.draw();
    transformStack.popMatrix();
}
