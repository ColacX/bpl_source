#include "Particle.h"

#include <iostream>

using namespace glm;


Particle::~Particle()
{
    //dtor
}

// #################################################
//          PARTICLE ONE
// #################################################


ParticleOne::ParticleOne(glm::vec2 pos, glm::vec2 velocity, float lifespan, bool rotation, glm::vec3 rotationAxis, float rotationAngle, float size) :
    Particle(pos, velocity, lifespan, rotation, rotationAxis, rotationAngle)
{
    float verticesTmp[] = {0, 0, 0,   size, 0, 0,     size, 6*size, 0,          // FRONT
                            0, 0, 0,    size, 6*size, 0,    0, 6*size, 0,
                            0, 4*size, 0,   0, 6*size, 0,   -1.5*size, 4.5*size, 0,
                            0, 4*size, 0,   -1.5*size, 4.5*size, 0,    -1.5*size, 3.5*size, 0,

                            0, 0, -size,   size, 0, -size,     size, 6*size, -size,          // BACK
                            0, 0, -size,    size, 6*size, -size,    0, 6*size, -size,
                            0, 4*size, -size,   0, 6*size, -size,   -1.5*size, 4.5*size, -size,
                            0, 4*size, -size,   -1.5*size, 4.5*size, -size,    -1.5*size, 3.5*size, -size,

                            size, 0, 0,     size, 0, -size,     size, 6*size, -size,    // RIGHT
                            size, 0, 0,     size, 6*size, -size,    size, 6*size, 0,

                            0, 6*size, 0,   size, 6*size, 0,    size, 6*size, -size,    // TOP
                            0, 6*size, 0,   size, 6*size, -size,  0, 6*size, -size,

                            0, 0, 0,   size, 0, 0,    size, 0, -size,    // BOTTOM BOTTOM
                            0, 0, 0,   size, 0, -size,  0, 0, -size,

                            -1.5*size, 3.5*size, 0,     0, 4*size, 0,   0, 4*size, -size,   // BOTTOM 2
                            -1.5*size, 3.5*size, 0,     0, 4*size, -size,   -1.5*size, 3.5*size, -size,

                            0, 0, 0,    0, 4*size, 0,   0, 4*size, -size,   // LEFT 1
                            0, 0, 0,    0, 4*size, -size,   0, 0, -size,

                            -1.5*size, 3.5*size, 0,     -1.5*size, 4.5*size, 0,     -1.5*size, 4.5*size, -size,  // LEFT 2
                            -1.5*size, 3.5*size, 0,     -1.5*size, 4.5*size, -size, -1.5*size, 3.5*size, -size,

                            -1.5*size, 4.5*size, 0,     0, 6*size, 0,   0, 6*size, -size,      // LEFT 3
                            -1.5*size, 4.5*size, 0,     0, 6*size, -size,  -1.5*size, 4.5*size, -size};

    for(int i(0); i<198; i++)
    {
        m_vertices[i] = verticesTmp[i];
    }
    for(int i(0); i<96; i += 4)
    {
        m_color[i] = 0.1;
        m_color[i+1] = 0.4;
        m_color[i+2] = 0.6;
        m_color[i+3] = 1.0;
    }
    for(int i(96); i<264; i+=4)
    {
        m_color[i] = 0;
        m_color[i+1] = 0;
        m_color[i+2] = 0.3;
        m_color[i+3] = 1.0;
    }

}

ParticleOne::~ParticleOne()
{

}

void ParticleOne::draw(glm::mat4 &projection, GLuint progID)
{
    glUseProgram(progID);

    mat4 modelview;

        modelview = mat4(1.0);
        modelview = translate(modelview, vec3(m_pos.x, m_pos.y, -1));
        if(m_rotation) {
            modelview = rotate(modelview, m_rotationAngle, m_rotationAxis);
        }

        // vertices
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
        glEnableVertexAttribArray(0);

        // color
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, m_color);
        glEnableVertexAttribArray(1);

        // matrix
        glUniformMatrix4fv(glGetUniformLocation(progID, "projection"), 1, GL_FALSE, value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(progID, "modelview"), 1, GL_FALSE, value_ptr(modelview));

        glDrawArrays(GL_TRIANGLES, 0, 66);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);


    glUseProgram(0);


}

void ParticleOne::update(float deltaTime)
{
    m_lifespan -= deltaTime;
    m_pos += m_velocity * deltaTime * 0.5f;
    if(m_rotation){
        m_rotationAngle += m_deltaAngle;
        if(m_rotationAngle >= 360.0f) { m_rotationAngle -= 360.0f; }
    }
    for(int i(3); i<264; i+=4)
    {
		m_color[i] = (float)m_lifespan / m_life;
    }
}



// #################################################
//          PARTICLE ZERO
// #################################################


ParticleZero::~ParticleZero()
{

}

ParticleZero::ParticleZero(glm::vec2 pos, glm::vec2 velocity, float lifespan, bool rotation, glm::vec3 rotationAxis, float rotationAngle, float size) :
    Particle(pos, velocity, lifespan, rotation, rotationAxis, rotationAngle)
{
    float verticesTmp[] = { -0.25*size, 0, 0,                1.25*size, 0, 0,             0.75*size, size, 0,      // FRONT
                            -0.25*size, 0, 0,                0.75*size, size, 0,          0.25*size, size, 0,
                            1.25*size, 0, 0,             2.5*size, 2*size, 0,    0.75*size, size, 0,
                            0.75*size, size, 0,          2.5*size, 2.*size, 0,   1.5*size, 2*size, 0,
                            2.5*size, 2*size, 0,    2.5*size, 4*size, 0,    1.5*size, 2*size, 0,
                            1.5*size, 2*size, 0,    2.5*size, 4*size, 0,    1.5*size, 4*size, 0,
                            2.5*size, 4*size, 0,    1.25*size, 6*size, 0,        1.5*size, 4*size, 0,
                            1.5*size, 4*size, 0,    1.25*size, 6*size, 0,        0.75*size, 5*size, 0,
                            1.25*size, 6*size, 0,        -0.25*size, 6*size, 0,           0.75*size, 5*size, 0,
                            0.75*size, 5*size, 0,        -0.25*size, 6*size, 0,           0.25*size, 5*size, 0,
                            -0.5*size, 4*size, 0,   0.25*size, 5*size, 0,           -1.5*size, 4*size, 0,
                            -1.5*size, 4*size, 0,   0.25*size, 5*size, 0,           -0.25*size, 6*size, 0,
                            -0.5*size, 2*size, 0,   -0.5*size, 4*size, 0,   -1.5*size, 2*size, 0,
                            -1.5*size, 2*size, 0,   -0.5*size, 4*size, 0,   -1.5*size, 4*size, 0,
                            -0.25*size, 0, 0,                0.25*size, size, 0,             -0.5*size, 2*size, 0,
                            -0.25*size, 0, 0,                -0.5*size, 2*size, 0,   -1.5*size, 2*size, 0, // 16 triangles

                            -0.25*size, 0, -size,                1.25*size, 0, -size,             0.75*size, size, -size,      // BACK
                            -0.25*size, 0, -size,                0.75*size, size, -size,          0.25*size, size, -size,
                            1.25*size, 0, -size,             2.5*size, 2*size, -size,    0.75*size, size, -size,
                            0.75*size, size, -size,          2.5*size, 2.*size, 0,   1.5*size, 2*size, -size,
                            2.5*size, 2*size, -size,    2.5*size, 4*size, -size,    1.5*size, 2*size, -size,
                            1.5*size, 2*size, -size,    2.5*size, 4*size, -size,    1.5*size, 4*size, -size,
                            2.5*size, 4*size, -size,    1.25*size, 6*size, -size,        1.5*size, 4*size, -size,
                            1.5*size, 4*size, -size,    1.25*size, 6*size, -size,        0.75*size, 5*size, -size,
                            1.25*size, 6*size, -size,        -0.25*size, 6*size, -size,           0.75*size, 5*size, -size,
                            0.75*size, 5*size, -size,        -0.25*size, 6*size, -size,           0.25*size, 5*size, -size,
                            -0.5*size, 4*size, -size,   0.25*size, 5*size, -size,           -1.5*size, 4*size, -size,
                            -1.5*size, 4*size, -size,   0.25*size, 5*size, -size,           -0.25*size, 6*size, -size,
                            -0.5*size, 2*size, -size,   -0.5*size, 4*size, -size,   -1.5*size, 2*size, -size,
                            -1.5*size, 2*size, -size,   -0.5*size, 4*size, -size,   -1.5*size, 4*size, -size,
                            -0.25*size, 0, -size,                0.25*size, size, -size,             -0.5*size, 2*size, -size,
                            -0.25*size, 0, -size,                -0.5*size, 2*size, -size,   -1.5*size, 2*size, -size,

                            -0.25*size, 0, 0,       1.25*size, 0, 0,        1.25*size, 0, -size,    // EXT EDGE
                            -0.25*size, 0, 0,       1.25*size, 0, -size,    -0.25*size, 0, -size,
                            1.25*size, 0, 0,        2.5*size, 2*size, 0,    2.5*size, 2*size, -size,
                               1.25*size, 0, 0,     2.5*size, 2*size, -size, 1.25*size, 0, -size,
                               2.5*size, 2*size, 0,    2.5*size, 4*size, 0,   2.5*size, 4*size, -size,
                               2.5*size, 2*size, 0,    2.5*size, 4*size, -size,   2.5*size, 2*size, -size,
                            2.5*size, 4*size, 0,    1.25*size, 6*size, 0,   1.25*size, 6*size, -size,
                            2.5*size, 4*size, 0,    1.25*size, 6*size, -size,   2.5*size, 4*size, -size,
                            1.25*size, 6*size, 0,   -0.25*size, 6*size, 0,      -0.25*size, 6*size, -size,
                            1.25*size, 6*size, 0,   -0.25*size, 6*size, -size,  1.25*size, 6*size, -size,
                            -1.5*size, 4*size, 0,   -0.25*size, 6*size, 0,      -0.25*size, 6*size, -size,
                            -1.5*size, 4*size, 0,   -0.25*size, 6*size, -size,  -1.5*size, 4*size, -size,
                            -1.5*size, 2*size, 0,   -1.5*size, 4*size, 0,       -1.5*size, 4*size, -size,
                            -1.5*size, 2*size, 0,   -1.5*size, 4*size, -size,   -1.5*size, 2*size, -size,
                            -0.25*size, 0, 0,       -1.5*size, 2*size, 0,       -1.5*size, 2*size, -size,
                            -0.25*size, 0, 0,       -1.5*size, 2*size, -size,   -0.25*size, 0, -size };

    for(int i(0); i<432; i++)
    {
        m_vertices[i] = verticesTmp[i];
    }
    for(int i(0); i<384; i+=4)
    {
        m_color[i] = 1.0;
        m_color[i+1] = 1.0;
        m_color[i+2] = 1.0;
        m_color[i+3] = 1.0;
    }
    for(int i(384); i<576; i+=4)
    {
        m_color[i] = 0.1;
        m_color[i+1] = 0.3;
        m_color[i+2] = 0.1;
        m_color[i+3] = 1.0;
    }

}

void ParticleZero::update(float deltaTime)
{
    m_lifespan -= deltaTime;
    m_pos += m_velocity * deltaTime * 0.5f;
    if(m_rotation){
        m_rotationAngle += m_deltaAngle;
        if(m_rotationAngle >= 360.0f) { m_rotationAngle -= 360.0f; }
		else if (m_rotationAngle <= -360.0f) { m_rotationAngle += 360.0f; }
    }
    for(int i(3); i<576; i+=4)
    {
		m_color[i] = (float)m_lifespan / m_life;
    }
}

void ParticleZero::draw(glm::mat4 &projection, GLuint progID)
{
    glUseProgram(progID);

    mat4 modelview;

        modelview = mat4(1.0);
        modelview = translate(modelview, vec3(m_pos.x, m_pos.y, -1));
        if(m_rotation) {
            modelview = rotate(modelview, m_rotationAngle, m_rotationAxis);
        }

        // vertices
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
        glEnableVertexAttribArray(0);

        // color
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, m_color);
        glEnableVertexAttribArray(1);

        // matrix
        glUniformMatrix4fv(glGetUniformLocation(progID, "projection"), 1, GL_FALSE, value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(progID, "modelview"), 1, GL_FALSE, value_ptr(modelview));

        glDrawArrays(GL_TRIANGLES, 0, 144);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);


    glUseProgram(0);


}
