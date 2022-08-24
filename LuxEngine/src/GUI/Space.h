#pragma once
#pragma once

namespace Lux::GUI
{
    class GUISpace
    {
    public:

        static constexpr v3 ToRenderSpaceOrigin(float aspect_ratio)
        {
            return { -aspect_ratio, -1.0f, 0.0f };
        }

        static inline v3 ToRenderSpace(float aspect_ratio, v3 position)
        {
            position.x /= 1000.0f;
            position.y /= 1000.0f;
            return { (position.x * 2.0f * aspect_ratio) - aspect_ratio, (position.y * 2.0f - 1.0f), position.z * 0.5f };
        }

        static inline v3 ToRenderSpaceDelta(float aspect_ratio, v3 position)
        {
            position.x /= 1000.0f;
            position.y /= 1000.0f;
            return { position.x * 2.0f * aspect_ratio, position.y * 2.0f, position.z };
        }

        static inline float ToRenderSpaceX(float aspect_ratio, float x)
        {
            return (x * 2.0f * aspect_ratio);
        }

        static inline float ToRenderSpaceY(float y)
        {
            return (y * 2.0f);
        }

        static inline float FromRenderSpaceX(float aspect_ratio, float x)
        {
            return (x / (2.0f * aspect_ratio));
        }
        static inline float FromRenderSpaceY(float y)
        {
            return (y / 2.0f);
        }

    };

}