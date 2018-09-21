/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2018 Julien Bernard
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
#include "Activities.cc"
#include "Activity.cc"
#include "AnimatedSprite.cc"
#include "Animation.cc"
#include "AssetManager.cc"
#include "BasicSprite.cc"
#include "BasicText.cc"
#include "Blend.cc"
#include "BufferedGeometry.cc"
#include "Circ.cc"
#include "Clipboard.cc"
#include "Clock.cc"
#include "Collision.cc"
#include "Color.cc"
#include "ColorRamp.cc"
#include "Console.cc"
#include "ConsoleFont.cc"
#include "Control.cc"
#include "Controls.cc"
#include "Coordinates.cc"
#include "Cursor.cc"
#include "Curve.cc"
#include "Curves.cc"
#include "Direction.cc"
#include "Drawable.cc"
#include "Easings.cc"
#include "Effects.cc"
#include "Entity.cc"
#include "EntityContainer.cc"
#include "Flags.cc"
#include "Font.cc"
#include "Gamepad.cc"
#include "Geometry.cc"
#include "GraphicsInfo.cc"
#include "Grid.cc"
#include "Heightmap.cc"
#include "Image.cc"
#include "Keyboard.cc"
#include "Library.cc"
#include "Log.cc"
#include "Logo.cc"
#include "Map.cc"
#include "Math.cc"
#include "Matrix.cc"
#include "MessageManager.cc"
#include "Model.cc"
#include "ModelContainer.cc"
#include "Models.cc"
#include "Monitor.cc"
#include "Move.cc"
#include "NinePatch.cc"
#include "Noise.cc"
#include "Noises.cc"
#include "Orientation.cc"
#include "Particles.cc"
#include "Path.cc"
#include "Paths.cc"
#include "PhysicsBody.cc"
#include "PhysicsGeometry.cc"
#include "PhysicsModel.cc"
#include "Polygon.cc"
#include "Polyline.cc"
#include "PostProcessing.cc"
#include "RandomBinaryTree.cc"
#include "Random.cc"
#include "Range.cc"
#include "Rect.cc"
#include "RenderPipeline.cc"
#include "RenderTarget.cc"
#include "RenderTexture.cc"
#include "RenderWindow.cc"
#include "ResourceManager.cc"
#include "Serialization.cc"
#include "SerializationOps.cc"
#include "Shader.cc"
#include "Shape.cc"
#include "Shapes.cc"
#include "Sleep.cc"
#include "Spatial.cc"
#include "SpriteBatch.cc"
#include "Sprite.cc"
#include "Stream.cc"
#include "Streams.cc"
#include "StringUtils.cc"
#include "SystemInfo.cc"
#include "Text.cc"
#include "TextureAtlas.cc"
#include "Texture.cc"
#include "TileLayer.cc"
#include "Time.cc"
#include "Tmx.cc"
#include "Transformable.cc"
#include "Transform.cc"
#include "Triangulation.cc"
#include "UI.cc"
#include "Vector.cc"
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

#include "vendor/pugixml/src/pugixml.cpp"

#ifndef __APPLE__
#ifdef GF_OPENGL3
#include "vendor/glad/gl33/src/glad.cc"
#else
#include "vendor/glad/gles20/src/glad.cc"
#endif
#endif
