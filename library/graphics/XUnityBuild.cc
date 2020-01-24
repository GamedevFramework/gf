/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2019 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

/*
 * This file is meant for single compilation unit (a.k.a. unity build)
 * See: https://en.wikipedia.org/wiki/Single_Compilation_Unit
 */

#include "data/generated.cc"

#include "Action.cc"
#include "AnimatedSprite.cc"
#include "Animation.cc"
#include "BasicSprite.cc"
#include "BasicText.cc"
#include "Blend.cc"
#include "BufferedGeometry.cc"
#include "Clipboard.cc"
#include "Console.cc"
#include "ConsoleFont.cc"
#include "Control.cc"
#include "Controls.cc"
#include "Coordinates.cc"
#include "Cursor.cc"
#include "Curve.cc"
#include "Curves.cc"
#include "Drawable.cc"
#include "Effects.cc"
#include "Entity.cc"
#include "EntityContainer.cc"
#include "Font.cc"
#include "Gamepad.cc"
#include "GraphicsHandle.cc"
#include "GraphicsInfo.cc"
#include "Grid.cc"
#include "Hexagon.cc"
#include "Keyboard.cc"
#include "Library.cc"
#include "Logo.cc"
#include "Monitor.cc"
#include "NinePatch.cc"
#include "Particles.cc"
#include "Paths.cc"
#include "PhysicsBody.cc"
#include "PhysicsGeometry.cc"
#include "PhysicsModel.cc"
#include "PostProcessing.cc"
#include "RenderPipeline.cc"
#include "RenderTarget.cc"
#include "RenderTexture.cc"
#include "RenderWindow.cc"
#include "ResourceManager.cc"
#include "Scene.cc"
#include "SceneManager.cc"
#include "Shader.cc"
#include "Shape.cc"
#include "Shapes.cc"
#include "SpriteBatch.cc"
#include "Sprite.cc"
#include "Stagger.cc"
#include "SystemInfo.cc"
#include "Text.cc"
#include "TextureAtlas.cc"
#include "Texture.cc"
#include "TileLayer.cc"
#include "Tmx.cc"
#include "Transformable.cc"
#include "UI.cc"
#include "VertexArray.cc"
#include "VertexBuffer.cc"
#include "Vertex.cc"
#include "View.cc"
#include "ViewContainer.cc"
#include "Views.cc"
#include "Widget.cc"
#include "WidgetContainer.cc"
#include "Widgets.cc"
#include "Window.cc"

#include "priv/Debug.cc"

#include <pugixml.cpp>

#ifndef __APPLE__
#ifdef GF_OPENGL3
#include "vendor/glad/gl33/src/glad.cc"
#else
#include "vendor/glad/gles20/src/glad.cc"
#endif
#endif
