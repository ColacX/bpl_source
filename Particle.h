#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif


class Particle
{
    public:
        Particle(glm::vec2 pos, glm::vec2 velocity, float lifespan, bool rotation, glm::vec3 rotationAxis, float rotationAngle):
            m_pos(pos), m_velocity(velocity), m_lifespan(lifespan), m_life(lifespan), m_rotation(rotation), m_rotationAxis(rotationAxis), m_deltaAngle(rotationAngle), m_rotationAngle(0)
        {};
        virtual ~Particle();

        virtual void draw(glm::mat4 &projection, GLuint progID) = 0;

        virtual void update(float deltaTime) = 0;
        float getLifespan() { return m_lifespan; };

    protected:
        glm::vec2 m_pos;
        glm::vec2 m_velocity;
        float m_lifespan;
		float m_life;
        bool m_rotation;
		glm::vec3 m_rotationAxis;
		float m_deltaAngle;
        float m_rotationAngle;
};

class ParticleOne : public Particle
{
    public:
		ParticleOne(glm::vec2 pos, glm::vec2 velocity, float lifespan, bool rotation, glm::vec3 rotationAxis, float rotationAngle, float size);

        virtual ~ParticleOne();

        void draw(glm::mat4 &projection, GLuint progID);
        void update(float deltaTime);

    private:
        float m_vertices[198];
        float m_color[264];
};

class ParticleZero : public Particle
{
    public:
		ParticleZero(glm::vec2 pos, glm::vec2 velocity, float lifespan, bool rotation, glm::vec3 rotationAxis, float rotationAngle, float size);

        virtual ~ParticleZero();

        void draw(glm::mat4 &projection, GLuint progID);
        void update(float deltaTime);

    private:
        float m_vertices[432];
        float m_color[576];
};

#endif // PARTICLE_H
