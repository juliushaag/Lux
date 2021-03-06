#define USED_IN_FONTCPP
#include "Font.h"


#include "Core/IO.h"

#include "Utils/Assert.h"
#include "Utils/Logger.h"

#include <msdfgen.h>
#include <msdfgen-ext.h>
#include <msdf-atlas-gen/msdf-atlas-gen.h>


#define STBIWDEF inline
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

using namespace msdf_atlas;

namespace Lux
{

void* Font::s_FreetypeLibraryHandle = nullptr;

void Font::Init()
{
	Verify(s_FreetypeLibraryHandle == nullptr);
	s_FreetypeLibraryHandle = static_cast<void*>(msdfgen::initializeFreetype());

	Verify(s_FreetypeLibraryHandle);
}

void Font::Shutdown()
{
	Verify(s_FreetypeLibraryHandle != nullptr);
	msdfgen::deinitializeFreetype(static_cast<msdfgen::FreetypeHandle*>(s_FreetypeLibraryHandle));
}

Font::Font(const std::string& filepath)
: m_Bitmap(nullptr)
{
	Verify(s_FreetypeLibraryHandle);
	
	auto *font = msdfgen::loadFont(static_cast<msdfgen::FreetypeHandle*>(s_FreetypeLibraryHandle), filepath.c_str());

	Verify(font);	
	
	if (!font)
		ERROR("Failed to open font {}", filepath);

	std::vector<GlyphGeometry> glyphs;
	FontGeometry fontGeometry(&glyphs);

	fontGeometry.loadCharset(font, 1.0f, Charset::ASCII);

	const double maxCornerAngle = 3.0;
	for(GlyphGeometry& glyph : glyphs)
		glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);

	TightAtlasPacker packer;

	packer.setDimensionsConstraint(TightAtlasPacker::DimensionsConstraint::SQUARE);
	packer.setMinimumScale(32.0);

	packer.setPixelRange(2.0);
	packer.setMiterLimit(1.0);

	packer.pack(glyphs.data(), glyphs.size());

	int width, height;
	packer.getDimensions(width, height);

	ImmediateAtlasGenerator<
		float, 
		3, 
		msdfGenerator,
		BitmapAtlasStorage<msdf_atlas::byte, 3>
	> generator(width, height);

	GeneratorAttributes attributes;
	generator.setThreadCount(4);

	generator.generate(glyphs.data(), glyphs.size());

	auto& storage = generator.atlasStorage();
	
	msdfgen::BitmapConstRef<msdf_atlas::byte, 3> bitmap = storage;

	BitmapSpec spec;

	spec.unpack_aligned = true;
	spec.type = ImageType::RGB;
	spec.mag_filter = FilterMethod::LINEAR;
	spec.min_filter = FilterMethod::LINEAR;

	m_Bitmap = Bitmap::Create(spec, bitmap.width, bitmap.height);
	m_Bitmap->set_data(static_cast<const void*>(bitmap.pixels), bitmap.width * bitmap.height * 3);

	for (auto& glyph : glyphs)
	{
		/*
			(c,d)
			A
		(a,b) 

		*/


		double l, r, t, b;
		glyph.getQuadAtlasBounds(l, b, r, t);
		float glyph_left =	l / bitmap.width;
		float glyph_width = (r - l) / bitmap.width;

		float glyph_bottom = b  / bitmap.height;
		float glyph_height = (t - b) / bitmap.height;

		double advance = glyph.getAdvance();

		double x1, y1, x2, y2;
		glyph.getQuadPlaneBounds(x1, y1, x2, y2);

		float width = x2 - x1;
		float height = y2 - y1;

		float posx = x1;
		float posy = y1;

		auto cp =  glyph.getCodepoint();


		m_GlyphMetaData.insert({ 
			cp, 
			Glyph { 
				glm::translate(glm::mat4(1.0f), { posx, posy, 0.0f }) * glm::scale(glm::mat4(1.0f), { width, height, 1.0f }), 
				std::array<v2, 4> {
					v2 { glyph_left,	glyph_bottom + glyph_height},
					v2 { glyph_left + glyph_width,				glyph_bottom + glyph_height},
					v2 { glyph_left + glyph_width,				glyph_bottom},
					v2 { glyph_left,	glyph_bottom}
				}, 
				advance } 
			});
	}

	m_FontHandle = static_cast<void*>(font);
	
}
Font::~Font()
{
	msdfgen::destroyFont(static_cast<msdfgen::FontHandle*>(m_FontHandle));
}
;


};