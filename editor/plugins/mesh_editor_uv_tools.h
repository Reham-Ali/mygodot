/**************************************************************************/
/*  mesh_editor_uv_tools.h                                                */
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

#ifndef MESH_EDITOR_UV_TOOLS_H
#define MESH_EDITOR_UV_TOOLS_H

#include "editor/plugins/editor_plugin.h"
#include "scene/3d/mesh_instance_3d.h"
#include "scene/gui/option_button.h"

class AcceptDialog;
class AspectRatioContainer;
class ConfirmationDialog;
class MenuButton;
class SpinBox;

class MeshEditorUvTools : public Control {
	GDCLASS(MeshEditorUvTools, Control);

	AcceptDialog *err_dialog = nullptr;
	AcceptDialog *debug_uv_dialog = nullptr;
	AspectRatioContainer *debug_uv_arc = nullptr;
	Control *debug_uv = nullptr;
	Vector<Vector2> uv_lines;

public:
	void debug_uv_draw();
	void create_uv_lines(Ref<Mesh> mesh, int p_layer);
	void create_uv2(Node3D *node, Ref<Mesh> mesh);

	MeshEditorUvTools();
};

#endif // MESH_EDITOR_UV_TOOLS_H
