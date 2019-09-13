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
#include <iostream>

#include <gf/Clock.h>
#include <gf/Collision.h>
#include <gf/Color.h>
#include <gf/Curves.h>
#include <gf/Entity.h>
#include <gf/Event.h>
#include <gf/Models.h>
#include <gf/PhysicsModel.h>
#include <gf/PhysicsBody.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include <gf/Log.h>

static constexpr float Radius = 20.0f;
static constexpr float SideLength = 2 * Radius;

static constexpr float Impulse = 20000.0f;

class PhysicsEntity : public gf::Entity {
public:
  virtual gf::PhysicsBody& getBody() = 0;
};

class Block : public PhysicsEntity {
public:
  Block(gf::Vector2f position)
  : m_geometry(gf::Vector2f(SideLength, SideLength))
  , m_body(m_geometry, gf::PhysicsBody::Static)
  {
    m_body.setPosition(position);
  }

  gf::PhysicsBody& getBody() override {
    return m_body;
  }

  void render(gf::RenderTarget &target, const gf::RenderStates& states) override {
    gf::RectangleShape shape({ SideLength, SideLength });
    shape.setColor(gf::Color::Azure);
    shape.setPosition(m_body.getPosition());
    shape.setAnchor(gf::Anchor::Center);
    target.draw(shape, states);

//     m_body.render(target);
  }

private:
  gf::PolygonGeometry m_geometry;
  gf::PhysicsBody m_body;
};

class Ball : public PhysicsEntity {
public:
  Ball(gf::Vector2f position, gf::Vector2f impulse)
  : m_geometry(Radius)
  , m_body(m_geometry, gf::PhysicsBody::Dynamic)
  {
    m_body.setDensity(0.1f);
    m_body.setPosition(position);
    m_body.applyLinearImpulse(impulse);
  }

  gf::PhysicsBody& getBody() override {
    return m_body;
  }

  void render(gf::RenderTarget &target, const gf::RenderStates& states) override {
    gf::CircleShape shape(Radius);
    shape.setColor(gf::Color::Orange);
    shape.setPosition(m_body.getPosition());
    shape.setAnchor(gf::Anchor::Center);
    target.draw(shape, states);

//     m_body.render(target);
  }

private:
  gf::CircleGeometry m_geometry;
  gf::PhysicsBody m_body;
};


class PhysicsWorld {
public:
  PhysicsWorld()
  : m_fixed(m_physics)
  {
  }

  void addEntity(std::unique_ptr<PhysicsEntity> entity) {
    m_physics.addBody(entity->getBody());
    m_entities.push_back(std::move(entity));
  }

  void clear() {
    m_physics.clear();
    m_entities.clear();
  }

  void update(gf::Time time) {
    m_fixed.update(time);

    for (auto& entity : m_entities) {
      entity->update(time);
    }
  }

  void render(gf::RenderTarget& target, const gf::RenderStates& states = gf::RenderStates()) {
    for (auto& entity : m_entities) {
      entity->render(target, states);
    }
  }

  void setupDemo(int demo) {
    clear();
    std::cout << "Demo " << demo << ": ";

    switch (demo) {
      case 1: {
        std::cout << "One ball and one block with no restitution\n";
        std::unique_ptr<PhysicsEntity> block1(new Block({ 400.0f, 250.0f }));
        addEntity(std::move(block1));
        std::unique_ptr<PhysicsEntity> ball1(new Ball({ 100.0f, 250.0f }, { Impulse, 0.0f }));
        addEntity(std::move(ball1));
        break;
      }

      case 2: {
        std::cout << "One ball and one block with some restitution\n";
        std::unique_ptr<PhysicsEntity> block1(new Block({ 400.0f, 250.0f }));
        addEntity(std::move(block1));
        std::unique_ptr<PhysicsEntity> ball1(new Ball({ 100.0f, 250.0f }, { Impulse, 0.0f }));
        ball1->getBody().setRestitution(0.5f);
        addEntity(std::move(ball1));
        break;
      }

      case 3: {
        std::cout << "Multiple balls colliding in line\n";

        for (int i = 0; i < 4; ++i) {
          std::unique_ptr<PhysicsEntity> ball0(new Ball({ 250.0f - (3 - 2 * i) * Radius, 250.0f }, { 0.0f, 0.0f }));
          ball0->getBody().setRestitution(1.0f);
          addEntity(std::move(ball0));
        }

        std::unique_ptr<PhysicsEntity> ball1(new Ball({ 50.0f, 250.0f }, { Impulse, 0.0f }));
        ball1->getBody().setRestitution(1.0f);
        addEntity(std::move(ball1));
        break;
      }

      case 4: {
        std::cout << "One ball and one block colliding in diagonal\n";
        std::unique_ptr<PhysicsEntity> block1(new Block({ 350.0f, 250.0f }));
        addEntity(std::move(block1));
        std::unique_ptr<PhysicsEntity> ball1(new Ball({ 150.0f - 2 * Radius, 50.0f }, { Impulse / gf::Sqrt2, Impulse / gf::Sqrt2 }));
        ball1->getBody().setRestitution(1.0f);
        addEntity(std::move(ball1));
        break;
      }

      case 5: {
        std::cout << "Two balls colliding in diagonal\n";
        std::unique_ptr<PhysicsEntity> ball1(new Ball({ 250.0f, 250.0f }, { 0.0f, 0.0f }));
        ball1->getBody().setRestitution(1.0f);
        addEntity(std::move(ball1));
        std::unique_ptr<PhysicsEntity> ball2(new Ball({ 50.0f - 2 * Radius, 50.0f }, { Impulse / gf::Sqrt2, Impulse / gf::Sqrt2 }));
        ball2->getBody().setRestitution(1.0f);
        addEntity(std::move(ball2));
        break;
      }

      case 6: {
        std::cout << "Two balls colliding face to face\n";
        std::unique_ptr<PhysicsEntity> ball1(new Ball({ 250.0f, 250.0f }, { Impulse * 0.5f, 0.0f }));
        ball1->getBody().setRestitution(1.0f);
        addEntity(std::move(ball1));
        std::unique_ptr<PhysicsEntity> ball2(new Ball({ 450.0f, 250.0f }, { -Impulse * 1.5f, 0.0f }));
        ball2->getBody().setRestitution(1.0f);
        addEntity(std::move(ball2));
        break;
      }

      case 7: {
        std::cout << "Pool!\n";
        for (int i = 0; i < 5; ++i) {
          gf::Vector2f position;
          position.x = 300.0f + i * gf::Sqrt3 * Radius;

          for (int j = 0; j <= i; ++j) {
            position.y = 250.0f + (2 * j - i) * Radius;

            std::unique_ptr<PhysicsEntity> ball0(new Ball(position, { 0.0f, 0.0f }));
            ball0->getBody().setRestitution(1.0f);
            ball0->getBody().setLinearDamping(0.4f);
            addEntity(std::move(ball0));
          }
        }

        std::unique_ptr<PhysicsEntity> ball1(new Ball({ 0.0f, 250.0f }, { 3 * Impulse, 0.0f }));
        ball1->getBody().setRestitution(1.0f);
        ball1->getBody().setLinearDamping(0.4f);
        addEntity(std::move(ball1));
      }

      default:
        break;
    }
  }

private:
  gf::PhysicsModel m_physics;
  gf::FixedTimestepModel m_fixed;
  std::vector<std::unique_ptr<PhysicsEntity>> m_entities;
};


int main() {
  static constexpr gf::Vector2i ScreenSize(500, 500);
  static constexpr gf::RectF World = gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 500.0f, 500.0f });

  gf::Window window("42_physics", ScreenSize, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::ExtendView view(World);
  view.setInitialScreenSize(ScreenSize);

  renderer.setView(view);

  PhysicsWorld world;

  std::cout << "Gamedev Framework (gf) example #42: Physics\n";
  std::cout << "This example shows the physics model in various situations.\n";
  std::cout << "How to use:\n";
  std::cout << "\t1-7: show one of the demos\n";

  gf::Clock clock;
  renderer.clear(gf::Color::White);

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          switch (event.key.scancode) {
            case gf::Scancode::Num1:
              world.setupDemo(1);
              break;

            case gf::Scancode::Num2:
              world.setupDemo(2);
              break;

            case gf::Scancode::Num3:
              world.setupDemo(3);
              break;

            case gf::Scancode::Num4:
              world.setupDemo(4);
              break;

            case gf::Scancode::Num5:
              world.setupDemo(5);
              break;

            case gf::Scancode::Num6:
              world.setupDemo(6);
              break;

            case gf::Scancode::Num7:
              world.setupDemo(7);
              break;

            case gf::Scancode::Escape:
              window.close();
              break;

            default:
              break;
          }
          break;

        default:
          break;
      }
    }

    gf::Time time = clock.restart();
    world.update(time);

    renderer.clear();
    world.render(renderer);
    renderer.display();
  }

  return 0;
}
