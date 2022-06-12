#pragma once 

#include "Utils/Types.h"

#include "Core/Event.h"
#include "Core/Input.h"

#include "Graphics/Renderer2D.h"

#include "GUI.h"
#include "GUIStyle.h"



#include <string>

namespace Lux::GUI
{

enum class ResizingPoint
{
    None,
    Left,
    Right,
    Top,
    Bottom,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};

}