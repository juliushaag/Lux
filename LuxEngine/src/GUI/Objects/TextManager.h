#pragma once

#include "GUIObject.h"

#include "GlyphObject.h"

namespace Lux::GUI
{

class TextManager final : public GUIObject 
{
public:

	TextManager(const std::string& text_str, const Ref<Font>& font, const v3& position, const v4& color)
		:	GUIObject(TypeComponent::TEXT)
	{
		auto& text = attach_component<TextComponent>();

		text.text = text_str;
		text.font = font;
		text.position = position;
		text.color = color;

		recalculate();
	}

	void set_text(std::string_view comp_text)
	{
		auto& text = get<TextComponent>();
		
		if(comp_text == text.text)
			return;

		text.text = comp_text;
		
		recalculate();
	}

	void set_position(const v3& position)
	{
		auto& text = get<TextComponent>();

		text.position = position;

		recalculate();
	}

	void set_font(const Ref<Font>& font)
	{
		auto& text = get<TextComponent>();
		
		if(font == text.font)
			return;

		text.font = font;
		
		recalculate();
	}
	
	void set_color(const v4& color)
	{
		auto& text = get<TextComponent>();

		text.color = color;
		for(auto id : text.glyphs)
			Get<GlyphComponent>(id).color = text.color;
	}

	void fit_to_bbox(float width, float height)
	{
		auto& text = get<TextComponent>();
		if(!text.font)
			return;

		float scale = height / text.font->lineheight();
		if(text.linelength)
		{
			float width_scale = width / (text.linelength / text.scale);


			scale = std::min(width_scale, scale);
		}

		INFO("Setting text scale to {}", scale);

		text.scale = scale;
		recalculate();
	}

	virtual void refresh(v2 position, float width, float height, float depth) override
	{
		auto& text = get<TextComponent>();
		auto& transform = get<TransformComponent>();


		transform.position = text.position = { position.x, position.y, depth };
		transform.scale = { width, height, 1.0f};

		transform.position.z -= 0.1f;

		fit_to_bbox(width, height);
	}


	void recalculate()
	{
		auto& text = get<TextComponent>();

		text.glyphs.reserve(text.text.size());

		text.linelength = 0.0;

		float lineheight = text.font->lineheight() * text.scale;
		v3 position = text.position;

		position.y += text.font->descender() * text.scale;

		float scale = text.scale;

		u32 index = 0;
		for (auto character : text.text)
		{
			auto& glyph = text.font->getGlyph(character);

			if (character != ' ')
			{
				UUID glyph_id = 0;
				if(text.glyphs.size() == index)
				{
					text.glyphs.emplace_back(CreateObject<GlyphObject>());

				}
				glyph_id = text.glyphs[index++];


				GlyphObject& glyph_obj = GetObject<GlyphObject>(glyph_id);
				
				GlyphComponent& glyph_component = glyph_obj.get_glyph_component();



				glyph_component.color = text.color;
				glyph_component.font = text.font;
				glyph_component.tex_coords = glyph.text_coords;
				glyph_component.glyph = character;
				glyph_component.char_transform = glyph.transform;

				TransformComponent& transform = glyph_obj.get_transform_component();
				
				transform.position = position;
				transform.scale = { scale, scale, 1.0f };
				
				index++;
			}

			text.linelength += (float)glyph.advance * scale;
			position.x += (float)glyph.advance * scale;
		
		}

		while (text.glyphs.size() + 1 > index)
		{
			delete &GetObject(text.glyphs.back());
			text.glyphs.pop_back();
		}


	}

	void destroy(UUID id)
	{
		auto& text = get<TextComponent>();

		for (auto& glyph : text.glyphs)
			DestroyPlain(glyph);
	}
	
};

}