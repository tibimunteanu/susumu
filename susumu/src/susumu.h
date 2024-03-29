#pragma once

//include this header in susumu apps

#include "supch.h"

#include "engine/core/base.h"
#include "engine/core/app.h"
#include "engine/core/log.h"
#include "engine/core/assert.h"
#include "engine/core/layer.h"
#include "engine/core/timestep.h"
#include "engine/core/input.h"
#include "engine/core/keycodes.h"
#include "engine/core/mousecodes.h"
#include "engine/debug/instrumentor.h"
#include "engine/imgui/imgui_layer.h"
#include "engine/math/math.h"
#include "engine/scene/scene.h"
#include "engine/scene/entity.h"
#include "engine/scene/scriptable_entity.h"
#include "engine/scene/components.h"

#include "engine/renderer/renderer.h"
#include "engine/renderer/renderer_2d.h"
#include "engine/renderer/render_command.h"
#include "engine/renderer/shader.h"
#include "engine/renderer/framebuffer.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/buffer.h"
#include "engine/renderer/vertex_array.h"
#include "engine/renderer/camera.h"
#include "engine/renderer/orthographic_camera_controller.h"
