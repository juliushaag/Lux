
#include "Bitmap.h"

#include "System/OpenGL/Bitmap.h"

namespace Lux
{


Bitmap* Bitmap::create(u32 width, u32 height, ImageType type)
{
    return new OpenGL::Bitmap(width, height, type);
}

}