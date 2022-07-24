#pragma once

#include "Panel.h"

class EditorCamera;

class EditViewport final : public Panel
{
private:

	EditorCamera* _camera;

public:

	~EditViewport() override = default;

	void Init() override;

	void Update() override;
};
