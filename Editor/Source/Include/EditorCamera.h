#pragma once

#include "VarletFramework.h"

class EditorCamera final : public Component
{
private:

	Camera* _camera;
	Transform* _transform;

public:

	void InternalStart();

	void InternalUpdate();

	const uint32_t& GetRendereTexture() const;
};

