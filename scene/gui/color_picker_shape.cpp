/**************************************************************************/
/*  color_picker_shape.cpp                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "color_picker_shape.h"

#include "scene/gui/margin_container.h"

void ColorPickerShape::apply_picker_color() {
	color_picker->_copy_hsv_to_color();
	color_picker->last_color = color_picker->color;
	color_picker->set_pick_color(color_picker->color);
}

void ColorPickerShape::emit_color_changed() {
	color_picker->emit_signal(SNAME("color_changed"), color_picker->color);
}

void ColorPickerShape::emit_color_changed_if_not_deferred() {
	if (!color_picker->deferred_mode_enabled) {
		emit_color_changed();
	}
}

void ColorPickerShape::add_recent_preset() {
	color_picker->add_recent_preset(color_picker->color);
}

void ColorPickerShape::draw_sv_square(Control *p_control, const Rect2 &p_square) {
	const ColorPicker::ThemeCache &theme_cache = color_picker->theme_cache;
	const Vector2 end = p_square.get_end();
	const float h = color_picker->h;
	const float s = color_picker->s;
	const float v = color_picker->v;

	PackedVector2Array points = {
		p_square.position,
		Vector2(end.x, p_square.position.y),
		end,
		Vector2(p_square.position.x, end.y),
	};

	Color color1 = color_picker->color;
	color1.set_hsv(h, 1, 1);
	Color color2 = color1;
	color2.set_hsv(h, 1, 0);

	PackedColorArray colors = {
		Color(1, 1, 1, 1),
		Color(1, 1, 1, 1),
		Color(0, 0, 0, 1),
		Color(0, 0, 0, 1)
	};
	p_control->draw_polygon(points, colors);

	colors = {
		Color(color1, 0),
		Color(color1, 1),
		Color(color2, 1),
		Color(color2, 0)
	};
	p_control->draw_polygon(points, colors);

	const Vector2 half_cursor_size = theme_cache.picker_cursor->get_size() * 0.5;

	Vector2 cursor_pos;
	cursor_pos.x = CLAMP(p_square.position.x + p_square.size.x * s, p_square.position.x, end.x);
	cursor_pos.y = CLAMP(p_square.position.y + p_square.size.y * (1.0 - v), p_square.position.y, end.y);

	p_control->draw_texture(theme_cache.picker_cursor, cursor_pos - half_cursor_size);
}

Vector2 ColorPickerShape::get_square_uv(const Rect2 &p_square, const Vector2 &p_pos) const {
	const Vector2 pos = (p_pos - p_square.position).clamp(Vector2(), p_square.size);
	return pos / p_square.size;
}

void ColorPickerShape::draw_circle(Control *p_control) {
	const float v = color_picker->v;

	Ref<ShaderMaterial> material = p_control->get_material();
	material->set_shader_parameter(SNAME("v"), v);
	p_control->draw_rect(Rect2(Point2(), p_control->get_size()), Color(1, 1, 1));
}

void ColorPickerShape::draw_circle_cursor(Control *p_control) {
	const ColorPicker::ThemeCache &theme_cache = color_picker->theme_cache;
	const float h = color_picker->h;
	const float s = color_picker->s;

	const Vector2 center = p_control->get_size() * 0.5;
	const Vector2 cursor_pos(
			center.x + (center.x * Math::cos(h * Math_TAU) * s),
			center.y + (center.y * Math::sin(h * Math_TAU) * s));
	p_control->draw_texture(theme_cache.picker_cursor, cursor_pos - theme_cache.picker_cursor->get_size() * 0.5);
}

ColorPickerShape::ColorPickerShape(ColorPicker *p_color_picker) {
	color_picker = p_color_picker;
}

void ColorPickerShapeRectangle::_sv_square_input(const Ref<InputEvent> &p_event, int64_t p_shape) {
	ColorPickerShapeRectangle *shape = reinterpret_cast<ColorPickerShapeRectangle *>(p_shape);
	ColorPicker *color_picker = shape->color_picker;

	Ref<InputEventMouseButton> bev = p_event;
	if (bev.is_valid() && bev->get_button_index() == MouseButton::LEFT) {
		if (bev->is_pressed()) {
			Vector2 uv = shape->get_square_uv(Rect2(Vector2(), shape->sv_square->get_size()), bev->get_position());
			color_picker->s = uv.x;
			color_picker->v = 1.0 - uv.y;
			shape->changing_color = true;

			shape->apply_picker_color();
			shape->emit_color_changed_if_not_deferred();
		} else {
			shape->emit_color_changed();
			shape->add_recent_preset();
			shape->changing_color = false;
		}
	}

	Ref<InputEventMouseMotion> mev = p_event;
	if (mev.is_valid() && shape->changing_color) {
		Vector2 uv = shape->get_square_uv(Rect2(Vector2(), shape->sv_square->get_size()), mev->get_position());
		color_picker->s = uv.x;
		color_picker->v = 1.0 - uv.y;

		shape->apply_picker_color();
		shape->emit_color_changed_if_not_deferred();
	}
}

void ColorPickerShapeRectangle::_hue_slider_input(const Ref<InputEvent> &p_event, int64_t p_shape) {
	ColorPickerShapeRectangle *shape = reinterpret_cast<ColorPickerShapeRectangle *>(p_shape);
	ColorPicker *color_picker = shape->color_picker;
	const float h = color_picker->h;
	Control *hue_slider = shape->hue_slider;

	Ref<InputEventMouseButton> bev = p_event;
	if (bev.is_valid() && bev->get_button_index() == MouseButton::LEFT) {
		if (bev->is_pressed()) {
			shape->changing_color = true;
			float y = bev->get_position().y;
			float height = hue_slider->get_size().y;
			y = CLAMP(y, 0, height);
			color_picker->h = y / height;
		} else {
			shape->changing_color = false;
		}
		shape->apply_picker_color();

		if (!bev->is_pressed()) {
			shape->add_recent_preset();
			shape->emit_color_changed();
		} else {
			shape->emit_color_changed_if_not_deferred();
		}
	}

	Ref<InputEventMouseMotion> mev = p_event;
	if (mev.is_valid() && shape->changing_color) {
		float y = mev->get_position().y;
		float height = hue_slider->get_size().y;
		y = CLAMP(y, 0, height);
		color_picker->h = y / height;

		shape->apply_picker_color();
		shape->emit_color_changed_if_not_deferred();
	}
}

void ColorPickerShapeRectangle::_sv_square_draw(int64_t p_shape) {
	ColorPickerShapeRectangle *shape = reinterpret_cast<ColorPickerShapeRectangle *>(p_shape);
	Control *sv_square = shape->sv_square;
	shape->draw_sv_square(sv_square, Rect2(Vector2(), sv_square->get_size()));
}

void ColorPickerShapeRectangle::_hue_slider_draw(int64_t p_shape) {
	ColorPickerShapeRectangle *shape = reinterpret_cast<ColorPickerShapeRectangle *>(p_shape);
	Control *hue_slider = shape->hue_slider;

	const ColorPicker::ThemeCache &theme_cache = shape->color_picker->theme_cache;
	const float h = shape->color_picker->h;

	const Vector2 size = hue_slider->get_size();
	hue_slider->draw_texture_rect(theme_cache.color_hue, Rect2(0, 0, -size.y, size.x), false, Color(1, 1, 1), true);

	int y = size.y * h;
	Color color = Color::from_hsv(h, 1, 1);
	hue_slider->draw_line(Vector2(0, y), Vector2(size.x, y), color.inverted());
}

void ColorPickerShapeRectangle::update_theme() {
	const ColorPicker::ThemeCache &theme_cache = color_picker->theme_cache;
	sv_square->set_custom_minimum_size(Size2(theme_cache.sv_width, theme_cache.sv_height));
	hue_slider->set_custom_minimum_size(Size2(theme_cache.h_width, 0));
}

ColorPickerShapeRectangle::ColorPickerShapeRectangle(ColorPicker *p_color_picker) :
		ColorPickerShape(p_color_picker) {
	int64_t self = reinterpret_cast<int64_t>(this); // Hack to allow connecting non-Object methods.

	sv_square = memnew(Control);
	p_color_picker->shape_container->add_child(sv_square);
	sv_square->connect(SceneStringName(gui_input), callable_mp_static(ColorPickerShapeRectangle::_sv_square_input).bind(self));
	sv_square->connect(SceneStringName(draw), callable_mp_static(ColorPickerShapeRectangle::_sv_square_draw).bind(self));

	hue_slider = memnew(Control);
	p_color_picker->shape_container->add_child(hue_slider);
	hue_slider->connect(SceneStringName(gui_input), callable_mp_static(ColorPickerShapeRectangle::_hue_slider_input).bind(self));
	hue_slider->connect(SceneStringName(draw), callable_mp_static(ColorPickerShapeRectangle::_hue_slider_draw).bind(self));

	controls.append(sv_square);
	controls.append(hue_slider);
}

void ColorPickerShapeWheel::_wheel_draw(int64_t p_shape) {
	ColorPickerShapeWheel *shape = reinterpret_cast<ColorPickerShapeWheel *>(p_shape);
	Control *wheel = shape->wheel;
	wheel->draw_rect(Rect2(Point2(), wheel->get_size()), Color(1, 1, 1));
}

void ColorPickerShapeWheel::_wheel_uv_draw(int64_t p_shape) {
	ColorPickerShapeWheel *shape = reinterpret_cast<ColorPickerShapeWheel *>(p_shape);
	Control *wheel_uv = shape->wheel_uv;

	const float h = shape->color_picker->h;

	const Vector2 uv_size = wheel_uv->get_size();
	const Vector2 ring_radius = uv_size * Math_SQRT12 * 0.42;
	const Vector2 center = uv_size * 0.5;

	const Rect2 uv_rect(center - ring_radius, ring_radius * 2.0);
	shape->draw_sv_square(wheel_uv, uv_rect);

	// Wheel cursor.
	double h1 = h - (0.5 / 360);
	double h2 = h + (0.5 / 360);

	Color color;
	color.set_hsv(h, 1, 1);

	PackedVector2Array points = {
		Vector2(center.x + (center.x * Math::cos(h1 * Math_TAU)), center.y + (center.y * Math::sin(h1 * Math_TAU))),
		Vector2(center.x + (center.x * Math::cos(h1 * Math_TAU) * 0.84), center.y + (center.y * Math::sin(h1 * Math_TAU) * 0.84)),
		Vector2(center.x + (center.x * Math::cos(h2 * Math_TAU)), center.y + (center.y * Math::sin(h2 * Math_TAU))),
		Vector2(center.x + (center.x * Math::cos(h2 * Math_TAU) * 0.84), center.y + (center.y * Math::sin(h2 * Math_TAU) * 0.84))
	};
	wheel_uv->draw_multiline(points, color.inverted());
}

void ColorPickerShapeWheel::update_theme() {
	const ColorPicker::ThemeCache &theme_cache = color_picker->theme_cache;
	wheel_margin->set_custom_minimum_size(Size2(theme_cache.sv_width, theme_cache.sv_height));
	wheel_margin->add_theme_constant_override(SNAME("margin_bottom"), 8 * theme_cache.base_scale);
}

ColorPickerShapeWheel::ColorPickerShapeWheel(ColorPicker *p_color_picker) :
		ColorPickerShape(p_color_picker) {
	int64_t self = reinterpret_cast<int64_t>(this);

	wheel_margin = memnew(MarginContainer);
	p_color_picker->shape_container->add_child(wheel_margin);

	Ref<ShaderMaterial> material;
	material.instantiate();
	material->set_shader(ColorPicker::wheel_shader);

	wheel = memnew(Control);
	wheel->set_material(material);
	wheel_margin->add_child(wheel);
	wheel->connect(SceneStringName(draw), callable_mp_static(ColorPickerShapeWheel::_wheel_draw).bind(self));

	wheel_uv = memnew(Control);
	wheel_margin->add_child(wheel_uv);
	// wheel_uv->connect(SceneStringName(gui_input), callable_mp(this, &ColorPicker::_uv_input).bind(wheel_uv));
	wheel_uv->connect(SceneStringName(draw), callable_mp_static(ColorPickerShapeWheel::_wheel_uv_draw).bind(self));

	controls.append(wheel_margin);
	controls.append(wheel);
	controls.append(wheel_uv);
}

void ColorPickerShapeVHSCircle::_circle_draw(int64_t p_shape) {
	ColorPickerShapeVHSCircle *shape = reinterpret_cast<ColorPickerShapeVHSCircle *>(p_shape);
	shape->draw_circle(shape->circle);
}

void ColorPickerShapeVHSCircle::_circle_overlay_draw(int64_t p_shape) {
	ColorPickerShapeVHSCircle *shape = reinterpret_cast<ColorPickerShapeVHSCircle *>(p_shape);
	shape->draw_circle_cursor(shape->circle_overlay);
}

void ColorPickerShapeVHSCircle::_value_slider_draw(int64_t p_shape) {
	ColorPickerShapeVHSCircle *shape = reinterpret_cast<ColorPickerShapeVHSCircle *>(p_shape);
	Control *value_slider = shape->value_slider;

	const float h = shape->color_picker->h;
	const float s = shape->color_picker->s;
	const float v = shape->color_picker->v;

	const Vector2 size = value_slider->get_size();
	PackedVector2Array points{
		Vector2(),
		Vector2(size.x, 0),
		size,
		Vector2(0, size.y)
	};

	Color color = Color::from_hsv(h, s, 1);
	PackedColorArray colors = {
		color,
		color,
		Color(),
		Color()
	};

	value_slider->draw_polygon(points, colors);

	int y = size.y * (1 - CLAMP(v, 0, 1));
	color.set_hsv(h, 1, v);
	value_slider->draw_line(Vector2(0, y), Vector2(size.x, y), color.inverted());
}

void ColorPickerShapeVHSCircle::update_theme() {
	const ColorPicker::ThemeCache &theme_cache = color_picker->theme_cache;
	circle_margin->set_custom_minimum_size(Size2(theme_cache.sv_width, theme_cache.sv_height));
	circle_margin->add_theme_constant_override(SNAME("margin_bottom"), 8 * theme_cache.base_scale);
	value_slider->set_custom_minimum_size(Size2(theme_cache.h_width, 0));
}

ColorPickerShapeVHSCircle::ColorPickerShapeVHSCircle(ColorPicker *p_color_picker) :
		ColorPickerShape(p_color_picker) {
	int64_t self = reinterpret_cast<int64_t>(this);

	circle_margin = memnew(MarginContainer);
	p_color_picker->shape_container->add_child(circle_margin);

	Ref<ShaderMaterial> material;
	material.instantiate();
	material->set_shader(ColorPicker::circle_shader);

	circle = memnew(Control);
	circle->set_material(material);
	circle_margin->add_child(circle);
	circle->connect(SceneStringName(draw), callable_mp_static(ColorPickerShapeVHSCircle::_circle_draw).bind(self));

	circle_overlay = memnew(Control);
	circle_overlay->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	circle->add_child(circle_overlay);
	circle_overlay->connect(SceneStringName(draw), callable_mp_static(ColorPickerShapeVHSCircle::_circle_overlay_draw).bind(self));

	value_slider = memnew(Control);
	p_color_picker->shape_container->add_child(value_slider);
	// hue_slider->connect(SceneStringName(gui_input), callable_mp_static(ColorPickerShapeRectangle::_hue_slider_input).bind(self));
	value_slider->connect(SceneStringName(draw), callable_mp_static(ColorPickerShapeVHSCircle::_value_slider_draw).bind(self));

	controls.append(circle_margin);
	controls.append(circle);
	controls.append(circle_overlay);
	controls.append(value_slider);
}

void ColorPickerShapeOKHSLCircle::_circle_draw(int64_t p_shape) {
	ColorPickerShapeOKHSLCircle *shape = reinterpret_cast<ColorPickerShapeOKHSLCircle *>(p_shape);
	shape->draw_circle(shape->circle);
}

void ColorPickerShapeOKHSLCircle::_circle_overlay_draw(int64_t p_shape) {
	ColorPickerShapeOKHSLCircle *shape = reinterpret_cast<ColorPickerShapeOKHSLCircle *>(p_shape);
	shape->draw_circle_cursor(shape->circle_overlay);
}

void ColorPickerShapeOKHSLCircle::_value_slider_draw(int64_t p_shape) {
	ColorPickerShapeOKHSLCircle *shape = reinterpret_cast<ColorPickerShapeOKHSLCircle *>(p_shape);
	Control *value_slider = shape->value_slider;

	const float h = shape->color_picker->h;
	const float s = shape->color_picker->s;
	const float v = shape->color_picker->v;

	const Vector2 size = value_slider->get_size();
	PackedVector2Array points{
		Vector2(size.x, 0),
		Vector2(size.x, size.y * 0.5),
		size,
		Vector2(0, size.y),
		Vector2(0, size.y * 0.5),
		Vector2()
	};

	Color color1 = Color::from_ok_hsl(h, s, 1);
	Color color2 = Color::from_ok_hsl(h, s, 0.5);
	Color color3 = Color::from_ok_hsl(h, s, 0);
	PackedColorArray colors = {
		color1,
		color2,
		color3,
		color3,
		color2,
		color1,
	};

	value_slider->draw_polygon(points, colors);

	int y = size.y * (1 - CLAMP(v, 0, 1));
	value_slider->draw_line(Vector2(0, y), Vector2(size.x, y), Color::from_hsv(h, 1, v).inverted());
}

void ColorPickerShapeOKHSLCircle::update_theme() {
	const ColorPicker::ThemeCache &theme_cache = color_picker->theme_cache;
	circle_margin->set_custom_minimum_size(Size2(theme_cache.sv_width, theme_cache.sv_height));
	circle_margin->add_theme_constant_override(SNAME("margin_bottom"), 8 * theme_cache.base_scale);
	value_slider->set_custom_minimum_size(Size2(theme_cache.h_width, 0));
}

ColorPickerShapeOKHSLCircle::ColorPickerShapeOKHSLCircle(ColorPicker *p_color_picker) :
		ColorPickerShape(p_color_picker) {
	int64_t self = reinterpret_cast<int64_t>(this);

	circle_margin = memnew(MarginContainer);
	p_color_picker->shape_container->add_child(circle_margin);

	Ref<ShaderMaterial> material;
	material.instantiate();
	material->set_shader(ColorPicker::circle_ok_color_shader);

	circle = memnew(Control);
	circle->set_material(material);
	circle_margin->add_child(circle);
	circle->connect(SceneStringName(draw), callable_mp_static(ColorPickerShapeOKHSLCircle::_circle_draw).bind(self));

	circle_overlay = memnew(Control);
	circle_overlay->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
	circle->add_child(circle_overlay);
	circle_overlay->connect(SceneStringName(draw), callable_mp_static(ColorPickerShapeOKHSLCircle::_circle_overlay_draw).bind(self));

	value_slider = memnew(Control);
	p_color_picker->shape_container->add_child(value_slider);
	// hue_slider->connect(SceneStringName(gui_input), callable_mp_static(ColorPickerShapeRectangle::_hue_slider_input).bind(self));
	value_slider->connect(SceneStringName(draw), callable_mp_static(ColorPickerShapeOKHSLCircle::_value_slider_draw).bind(self));

	controls.append(circle_margin);
	controls.append(circle);
	controls.append(circle_overlay);
	controls.append(value_slider);
}
