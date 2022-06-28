#include "Transform.h"

glm::vec3 Transform::GetPosition() const
{
    return _position;
}

glm::vec3 Transform::GetRotation() const
{
    return _rotation;
}

glm::vec3 Transform::GetScale() const
{
    return _scale;
}