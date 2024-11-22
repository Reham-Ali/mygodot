/**************************************************************************/
/*  color_picker_shape.h                                                  */
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

#ifndef COLOR_PICKER_SHAPE_H
#define COLOR_PICKER_SHAPE_H

#include "scene/gui/color_picker.h"

class ColorPickerShape {
public:
	ColorPicker *color_picker = nullptr;
    Vector<Control *> controls;

	virtual String get_name() const = 0;
	virtual Ref<Texture2D> get_icon() const = 0;
    virtual void update_theme() = 0;

    void apply_picker_color();
    void emit_color_changed();
    void emit_color_changed_if_not_deferred();
    void add_recent_preset();

    void draw_sv_square(Control *p_control, const Rect2 &p_square);
    Vector2 get_square_uv(const Rect2 &p_square, const Vector2 &p_pos) const;

    void draw_circle(Control *p_control);
    void draw_circle_cursor(Control *p_control);

	ColorPickerShape(ColorPicker *p_color_picker);
};

class ColorPickerShapeRectangle : public ColorPickerShape {
    Control *sv_square = nullptr;
    Control *hue_slider = nullptr;
    bool changing_color = false;

    static void _sv_square_input(const Ref<InputEvent> &p_event, int64_t p_shape);
    static void _hue_slider_input(const Ref<InputEvent> &p_event, int64_t p_shape);

    static void _sv_square_draw(int64_t p_shape);
    static void _hue_slider_draw(int64_t p_shape);

public:
	virtual String get_name() const override { return ETR("HSV Rectangle"); }
	virtual Ref<Texture2D> get_icon() const override { return color_picker->theme_cache.shape_rect; }
    virtual void update_theme() override;

	ColorPickerShapeRectangle(ColorPicker *p_color_picker);
};

class ColorPickerShapeWheel : public ColorPickerShape {
    MarginContainer *wheel_margin = nullptr;
    Control *wheel = nullptr;
    Control *wheel_uv = nullptr;

    static void _wheel_draw(int64_t p_shape);
    static void _wheel_uv_draw(int64_t p_shape);

public:
	virtual String get_name() const override { return ETR("HSV Wheel"); }
	virtual Ref<Texture2D> get_icon() const override { return color_picker->theme_cache.shape_rect_wheel; }
    virtual void update_theme() override;

	ColorPickerShapeWheel(ColorPicker *p_color_picker);
};

class ColorPickerShapeVHSCircle : public ColorPickerShape {
    MarginContainer *circle_margin = nullptr;
    Control *circle = nullptr;
    Control *circle_overlay = nullptr;
    Control *value_slider = nullptr;

    static void _circle_draw(int64_t p_shape);
    static void _circle_overlay_draw(int64_t p_shape);
    static void _value_slider_draw(int64_t p_shape);

public:
	virtual String get_name() const override { return ETR("VHS Circle"); }
	virtual Ref<Texture2D> get_icon() const override { return color_picker->theme_cache.shape_circle; }
    virtual void update_theme() override;

	ColorPickerShapeVHSCircle(ColorPicker *p_color_picker);
};

class ColorPickerShapeOKHSLCircle : public ColorPickerShape {
    MarginContainer *circle_margin = nullptr;
    Control *circle = nullptr;
    Control *circle_overlay = nullptr;
    Control *value_slider = nullptr;

    static void _circle_draw(int64_t p_shape);
    static void _circle_overlay_draw(int64_t p_shape);
    static void _value_slider_draw(int64_t p_shape);

public:
	virtual String get_name() const override { return ETR("OKHSL Circle"); }
	virtual Ref<Texture2D> get_icon() const override { return color_picker->theme_cache.shape_circle; }
    virtual void update_theme() override;

	ColorPickerShapeOKHSLCircle(ColorPicker *p_color_picker);
};

#endif // COLOR_PICKER_SHAPE_H
