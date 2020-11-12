#include "renderer.hpp"

unsigned long render::fonts::watermark_font;
unsigned long render::fonts::menufont;
unsigned long render::fonts::menucontent;
unsigned long render::fonts::Sweaponicons;
unsigned long render::fonts::espfont;
unsigned long render::fonts::tabicons;
unsigned long render::fonts::headerfont;
unsigned long render::fonts::bodyfont;

void render::initialize() {
	render::fonts::watermark_font = interfaces::surface->font_create();
	render::fonts::menucontent = interfaces::surface->font_create();
	render::fonts::Sweaponicons = interfaces::surface->font_create();
	render::fonts::espfont = interfaces::surface->font_create();
	render::fonts::tabicons = interfaces::surface->font_create();
	render::fonts::headerfont = interfaces::surface->font_create();
	render::fonts::bodyfont = interfaces::surface->font_create();

	
	interfaces::surface->set_font_glyph(render::fonts::watermark_font, "Segoe UI", 16, 500, 0, 0, font_flags::fontflag_outline);

	interfaces::surface->set_font_glyph(render::fonts::menucontent, "Segoe UI", 18, 1000, 0, 0, font_flags::fontflag_antialias);

	interfaces::surface->set_font_glyph(render::fonts::Sweaponicons, "AstriumWep", 35, 1200, 0, 0, font_flags::fontflag_none);

	interfaces::surface->set_font_glyph(render::fonts::espfont, "Tahoma", 13, 1200, 0, 0, font_flags::fontflag_outline);
	
	interfaces::surface->set_font_glyph(render::fonts::tabicons, "untitled-font-1", 20, 0, 0, 0, font_flags::fontflag_outline);

	interfaces::surface->set_font_glyph(render::fonts::headerfont, "Roboto Mono", 22, 1000, 0, 0, font_flags::fontflag_antialias);

	interfaces::surface->set_font_glyph(render::fonts::bodyfont, "Roboto Mono", 21, 0, 0, 0, font_flags::fontflag_antialias);
	
	console::log("[setup] render initialized!\n");
}

void render::draw_line(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_line(x1, y1, x2, y2);
}

void render::draw_text_string(std::int32_t x, std::int32_t y, unsigned long font, std::string string, bool text_centered, color colour) {
	const auto converted_text = std::wstring(string.begin(), string.end());

	int width, height;
	interfaces::surface->get_text_size(font, converted_text.c_str(), width, height);

	interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_text_font(font);
	if (text_centered)
		interfaces::surface->draw_text_pos(x - (width / 2), y);
	else
		interfaces::surface->draw_text_pos(x, y);
	interfaces::surface->draw_render_text(converted_text.c_str(), wcslen(converted_text.c_str()));
}

void render::draw_text_wchar(std::int32_t x, std::int32_t y, unsigned long font, const wchar_t* string, color colour) {
	interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_text_font(font);
	interfaces::surface->draw_text_pos(x, y);
	interfaces::surface->draw_render_text(string, wcslen(string));
}

void render::text(std::int32_t x, std::int32_t y, unsigned long font, const wchar_t *text, bool centered, color color) {
	interfaces::surface->draw_text_font(font);
	int text_width, text_height;

	if (centered) {
		interfaces::surface->get_text_size(font, text, text_width, text_height);
		interfaces::surface->draw_text_pos(x - text_width / 2, y);
	}
	else
		interfaces::surface->draw_text_pos(x, y );

	interfaces::surface->set_text_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_render_text(text, wcslen(text));
}

void render::text(std::int32_t x, std::int32_t y, unsigned long font, std::string text, bool centered, color color) {
	wchar_t temp[128];
	int text_width, text_height;
	if (MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, temp, 128) > 0) {
		interfaces::surface->draw_text_font(font);
		if (centered) {
			interfaces::surface->get_text_size(font, temp, text_width, text_height);
			interfaces::surface->draw_text_pos(x - text_width / 2, y);
		}
		else
			interfaces::surface->draw_text_pos(x, y);
		interfaces::surface->set_text_color(color.r, color.g, color.b, color.a);
		interfaces::surface->draw_render_text(temp, wcslen(temp));
	}
}

void render::draw_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, width, height);
}

void render::draw_filled_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_filled_rectangle(x, y, width, height);
}

void render::draw_outline(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, width, height);
}

void render::draw_textured_polygon(std::int32_t vertices_count, vertex_t* vertices, color color) {
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	unsigned int texture_id{};
	if (!texture_id) {
		texture_id = interfaces::surface->create_new_texture_id(true);
		interfaces::surface->set_texture_rgba(texture_id, buf, 1, 1);
	}
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->set_texture(texture_id);
	interfaces::surface->draw_polygon(vertices_count, vertices);
}

void render::draw_circle(std::int32_t x, std::int32_t y, std::int32_t radius, std::int32_t segments, color color) {
	float step = M_PI * 2.0 / segments;
	for (float a = 0; a < (M_PI * 2.0); a += step) {
		float x1 = radius * cos(a) + x;
		float y1 = radius * sin(a) + y;
		float x2 = radius * cos(a + step) + x;
		float y2 = radius * sin(a + step) + y;
		interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
		interfaces::surface->draw_line(x1, y1, x2, y2);
	}
}

vec2_t render::get_text_size(unsigned long font, std::string text) {
	std::wstring a(text.begin(), text.end());
	const wchar_t* wstr = a.c_str();
	static int w, h;

	interfaces::surface->get_text_size(font, wstr, w, h);
	return { static_cast<float>(w), static_cast<float>(h) };
}

void render::draw_xhair(int x, int y, bool outline, const color& c) {
	if (outline) {
		interfaces::surface->set_drawing_color(0, 0, 0, c.a);
		interfaces::surface->draw_filled_rectangle(x - 3, y - 1, 7, 3);
		interfaces::surface->draw_filled_rectangle(x - 1, y - 3, 3, 7);
	}

	interfaces::surface->set_drawing_color(c.r, c.g, c.b, c.a);
	interfaces::surface->draw_filled_rectangle(x - 2, y, 5, 1);
	interfaces::surface->draw_filled_rectangle(x, y - 2, 1, 5);
}