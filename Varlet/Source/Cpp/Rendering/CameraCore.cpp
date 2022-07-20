#include "CameraCore.h"
#include "RendererAPI.h"

namespace Varlet
{
    CameraCore::CameraCore()
    {
        _fov = 45.f;
        _projection = glm::perspective(glm::radians(_fov), 960.f / 540.f, 0.1f, 250.f);
        _framebuffer = RendererAPI::CreateFrameBuffer(960, 540);
    }

    CameraCore::~CameraCore()
    {
        delete _framebuffer;
    }

    void CameraCore::Bind() const
    {
        _framebuffer->Bind();
    }

    void CameraCore::UnBind() const
    {
        _framebuffer->UnBind();
    }

    const glm::mat4& CameraCore::GetView() const
    {
        return _view;
    }

    const glm::mat4& CameraCore::GetProjection() const
    {
        return _projection;
    }

    const glm::mat4 CameraCore::GetViewProjection() const
    {
        return _projection * _view;
    }

    const Texture* CameraCore::GetRendereTexture() const
    {
        return _framebuffer->GetTexture();
    }

    void CameraCore::ResizeView(const int32_t& width, const int32_t& height)
    {
        delete _framebuffer;
        _framebuffer = RendererAPI::CreateFrameBuffer(width, height);
        _projection = glm::perspective(glm::radians(_fov), static_cast<float>(width) / static_cast<float>(height), 0.1f, 250.f);
        _resolution.x = width;
        _resolution.y = height;
    }

    void CameraCore::GetResolution(int32_t& width, int32_t& height) const
    {
        width = _resolution.x;
        height = _resolution.y;
    }
}
