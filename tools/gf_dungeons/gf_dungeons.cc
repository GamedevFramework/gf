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
#include <cassert>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <tuple>

#include <gf/Array2D.h>
#include <gf/Color.h>
#include <gf/Direction.h>
#include <gf/Event.h>
#include <gf/Font.h>
#include <gf/Log.h>
#include <gf/Math.h>
#include <gf/Random.h>
#include <gf/RenderWindow.h>
#include <gf/StringUtils.h>
#include <gf/UI.h>
#include <gf/Unused.h>
#include <gf/VectorOps.h>
#include <gf/Views.h>
#include <gf/ViewContainer.h>
#include <gf/VertexArray.h>
#include <gf/Window.h>

#include "config.h"


namespace {

  enum class State : uint8_t {
    Wall,
    Void,
  };

  static int number(State state) {
    switch (state) {
      case State::Wall:
        return 0;
      case State::Void:
        return 1;
    }

    assert(false);
    return 0;
  }

  using Dungeon = gf::Array2D<State>;

  class DungeonGenerator {
  public:
    enum class Phase {
      Start,
      Iterate,
      Finish,
    };

    DungeonGenerator()
    : m_phase(Phase::Start)
    {

    }

    virtual ~DungeonGenerator() {

    }

    Phase getPhase() const {
      return m_phase;
    }

    void setPhase(Phase phase) {
      m_phase = phase;
    }

    virtual Dungeon generate(gf::Vector2u size, gf::Random& random) = 0;

  private:
    Phase m_phase;
  };


  /*
   * Cellular automaton
   */

  class CellularAutomaton : public DungeonGenerator {
  public:
    enum class Mode : int {
      Diamond4    = 0,
      Square8     = 1,
      Diamond12   = 2,
      Square24    = 3,
    };

    static int modeMax(int mode) {
      switch (mode) {
        case 0:
          return 4;
        case 1:
          return 8;
        case 2:
          return 12;
        case 3:
          return 24;
        default:
          assert(false);
          break;
      }

      assert(false);
      return 0;
    }

    // public parameters

    float threshold;
    Mode mode;
    int survivalThreshold;
    int birthThreshold;
    int iterations;


    virtual Dungeon generate(gf::Vector2u size, gf::Random& random) override {
      switch (getPhase()) {
        case Phase::Start:
          m_base = generateBase(size, random);
          // fallthrough
        case Phase::Iterate:
          m_dungeon = computeFirst(m_base, threshold);
          computeIterations();
          // fallthrough
        case Phase::Finish:
          break;
      }

      setPhase(Phase::Finish);
      return m_dungeon;
    }

  private:
    static gf::Array2D<float> generateBase(gf::Vector2u size, gf::Random& random) {
      gf::Array2D<float> ret(size);

      for (auto& value : ret) {
        value = random.computeUniformFloat(0.0f, 1.0f);
      }

      return ret;
    }

    static Dungeon computeFirst(const gf::Array2D<float>& array, float threshold) {
      Dungeon ret(array.getSize());

      for (auto pos : array.getPositionRange()) {
        if (array(pos) > threshold) {
          ret(pos) = State::Void;
        } else {
          ret(pos) = State::Wall;
        }
      }

      return ret;
    }

    void computeIterations() {
      Dungeon result(m_dungeon.getSize());

      for (int i = 0; i < iterations; ++i) {
        for (auto row : m_dungeon.getRowRange()) {
          for (auto col : m_dungeon.getColRange()) {
            gf::Vector2u pos(col, row);
            int count = 0;

            switch (mode) {
              case Mode::Diamond4: {
                for (auto neighbor : m_dungeon.get4NeighborsRange(pos)) {
                  count += number(m_dungeon(neighbor));
                }
                break;
              }
              case Mode::Square8:
                for (auto neighbor : m_dungeon.get8NeighborsRange(pos)) {
                  count += number(m_dungeon(neighbor));
                }
                break;
              case Mode::Diamond12:
                for (auto neighbor : m_dungeon.get12NeighborsRange(pos)) {
                  count += number(m_dungeon(neighbor));
                }
                break;
              case Mode::Square24:
                for (auto neighbor : m_dungeon.get24NeighborsRange(pos)) {
                  count += number(m_dungeon(neighbor));
                }
                break;
            }

            if (m_dungeon(pos) == State::Void) {
              if (count >= survivalThreshold) {
                result(pos) = State::Void;
              } else {
                result(pos) = State::Wall;
              }
            } else {
              if (count >= birthThreshold) {
                result(pos) = State::Void;
              } else {
                result(pos) = State::Wall;
              }
            }
          }
        }

        m_dungeon.swap(result);
      }
    }

  private:
    gf::Array2D<float> m_base;
    Dungeon m_dungeon;
  };


  /*
   * Tunneling
   */

  class Tunneling : public DungeonGenerator {
  public:

    // public parameters

    int maxRooms;
    int roomSizeMinimum;
    int roomSizeMaximum;

    virtual Dungeon generate(gf::Vector2u size, gf::Random& random) override {
      switch (getPhase()) {
        case Phase::Start:
          m_savedRandom = random;
          // fallthrough
        case Phase::Iterate:
          m_random = m_savedRandom;
          generateRooms(size);
          // fallthrough
        case Phase::Finish:
          random = m_random;
          break;
      }

      setPhase(Phase::Finish);
      return m_dungeon;
    }

  private:
    void generateRooms(gf::Vector2u size) {
      m_rooms.clear();
      m_dungeon = Dungeon(size, State::Wall);

      for (int i = 0; i < maxRooms; ++i) {
        gf::Vector2u roomPos, roomSize;
        roomSize.width = m_random.computeUniformInteger(roomSizeMinimum, roomSizeMaximum);
        roomSize.height = m_random.computeUniformInteger(roomSizeMinimum, roomSizeMaximum);
        roomPos.x = m_random.computeUniformInteger(0u, size.width - roomSize.width - 1);
        roomPos.y = m_random.computeUniformInteger(0u, size.height - roomSize.height - 1);

        gf::RectU room = gf::RectU::fromPositionSize(roomPos, roomSize);

        if (m_rooms.empty()) {
          createRoom(room);
          m_rooms.push_back(room);
        } else {
          if (std::any_of(m_rooms.begin(), m_rooms.end(), [&room](const gf::RectU& other) { return room.intersects(other); })) {
            continue;
          }

          createRoom(room);

          auto center = room.getCenter();
          auto previousCenter = m_rooms.back().getCenter();

          if (m_random.computeBernoulli(0.5)) {
            createHorizontalTunnel(previousCenter.x, center.x, previousCenter.y);
            createVerticalTunnel(center.x, center.y, previousCenter.y);
          } else {
            createVerticalTunnel(previousCenter.x, center.y, previousCenter.y);
            createHorizontalTunnel(previousCenter.x, center.x, center.y);
          }

          m_rooms.push_back(room);
        }
      }
    }

    void createRoom(const gf::RectU& room) {
      for (unsigned x = room.min.x + 1; x < room.max.x; ++x) {
        for (unsigned y = room.min.y + 1; y < room.max.y; ++y) {
          m_dungeon({ x, y }) = State::Void;
        }
      }
    }

    void createHorizontalTunnel(unsigned x1, unsigned x2, unsigned y) {
      if (x2 < x1) {
        std::swap(x1, x2);
      }

      for (unsigned x = x1; x <= x2; ++x) {
        m_dungeon({ x, y }) = State::Void;
      }
    }

    void createVerticalTunnel(unsigned x, unsigned y1, unsigned y2) {
      if (y2 < y1) {
        std::swap(y1, y2);
      }

      for (unsigned y = y1; y <= y2; ++y) {
        m_dungeon({ x, y }) = State::Void;
      }
    }

  private:
    gf::Random m_savedRandom;
    gf::Random m_random;
    std::vector<gf::RectU> m_rooms;
    Dungeon m_dungeon;
  };

  /*
   * BSP Tree
   */

  struct Tree {
    std::unique_ptr<Tree> left;
    std::unique_ptr<Tree> right;

    gf::RectU space;
    gf::RectU room;

    Tree(gf::RectU initialSpace)
    : left(nullptr)
    , right(nullptr)
    , space(initialSpace)
    {

    }

    bool split(gf::Random& random, unsigned leafSizeMinimum) {
      if (left || right) {
        return false;
      }

      bool splitHorizontally = random.computeBernoulli(0.5);

      if (space.getWidth() >= 1.25 * space.getHeight()) {
        splitHorizontally = false;
      } else if (space.getHeight() >= 1.25 * space.getWidth()) {
        splitHorizontally = true;
      }

      unsigned max = splitHorizontally ? space.getHeight() : space.getWidth();

      if (max <= 2 * leafSizeMinimum) {
        return false;
      }

      unsigned split = random.computeUniformInteger(leafSizeMinimum, max - leafSizeMinimum);

      if (splitHorizontally) {
        left = std::unique_ptr<Tree>(new Tree(gf::RectU::fromPositionSize(space.min, { space.getWidth(), split })));
        right = std::unique_ptr<Tree>(new Tree(gf::RectU::fromPositionSize({ space.min.x, space.min.y + split }, { space.getWidth(), space.getHeight() - split })));
      } else {
        left = std::unique_ptr<Tree>(new Tree(gf::RectU::fromPositionSize(space.min, { split, space.getHeight() })));
        right = std::unique_ptr<Tree>(new Tree(gf::RectU::fromPositionSize({ space.min.x + split, space.min.y }, { space.getWidth() - split, space.getHeight() })));
      }

      return true;
    }

    void recursiveSplit(gf::Random& random, unsigned leafSizeMinimum, unsigned leafSizeMaximum) {
      assert(!left && !right);

      if (space.getWidth() > leafSizeMaximum || space.getHeight() > leafSizeMaximum || random.computeBernoulli(0.2)) {
        if (split(random, leafSizeMinimum)) {
          assert(left);
          left->recursiveSplit(random, leafSizeMinimum, leafSizeMaximum);
          assert(right);
          right->recursiveSplit(random, leafSizeMinimum, leafSizeMaximum);
        }
      }
    }

    void createRooms(gf::Random& random, unsigned roomSizeMinimum, unsigned roomSizeMaximum) {
      if (left || right) {
        assert(left && right);

        left->createRooms(random, roomSizeMinimum, roomSizeMaximum);
        right->createRooms(random, roomSizeMinimum, roomSizeMaximum);

        if (random.computeBernoulli(0.5)) {
          room = left->room;
        } else {
          room = right->room;
        }
      } else {
        gf::Vector2u position, size;
        size.width = random.computeUniformInteger(roomSizeMinimum, std::min(roomSizeMaximum, space.getWidth() - 1));
        size.height = random.computeUniformInteger(roomSizeMinimum, std::min(roomSizeMaximum, space.getHeight() - 1));
        position.x = random.computeUniformInteger(0u, space.getWidth() - size.width - 1);
        position.y = random.computeUniformInteger(0u, space.getHeight() - size.height - 1);
        position += space.getPosition();

        room = gf::RectU::fromPositionSize(position, size);
      }
    }

  };

  class BinarySpacePartioningTree : public DungeonGenerator {
  public:
    BinarySpacePartioningTree()
    : m_root(gf::RectU::fromPositionSize({ 0u, 0u }, { 1u, 1u }))
    {

    }

    // public parameters

    int leafSizeMinimum = 0;
    int leafSizeMaximum = 0;
    int roomSizeMinimum = 0;
    int roomSizeMaximum = 0;

    virtual Dungeon generate(gf::Vector2u size, gf::Random& random) override {
      switch (getPhase()) {
        case Phase::Start:
          m_savedRandom = random;
          // fallthrough
        case Phase::Iterate:
          m_random = m_savedRandom;
          generateRooms(size);
          // fallthrough
        case Phase::Finish:
          random = m_random;
          break;
      }

      setPhase(Phase::Finish);
      return m_dungeon;
    }

  private:
    void generateRooms(gf::Vector2u size) {
      m_dungeon = Dungeon(size, State::Wall);

      m_root.space = gf::RectU::fromPositionSize({ 0u, 0u }, size);
      m_root.left = nullptr;
      m_root.right = nullptr;

      m_root.recursiveSplit(m_random, leafSizeMinimum, leafSizeMaximum);
      m_root.createRooms(m_random, roomSizeMinimum, roomSizeMaximum);
      walkTree(m_root);
    }

    void walkTree(const Tree& tree) {
      if (tree.left || tree.right) {
        assert(tree.left && tree.right);
        walkTree(*tree.left);
        walkTree(*tree.right);

        auto leftRoom = tree.left->room.getCenter();
        auto rightRoom = tree.right->room.getCenter();

        if (m_random.computeBernoulli(0.5)) {
          createHorizontalTunnel(rightRoom.x, leftRoom.x, rightRoom.y);
          createVerticalTunnel(leftRoom.x, leftRoom.y, rightRoom.y);
        } else {
          createVerticalTunnel(rightRoom.x, leftRoom.y, rightRoom.y);
          createHorizontalTunnel(rightRoom.x, leftRoom.x, leftRoom.y);
        }
      } else {
        createRoom(tree.room);
      }
    }

    void createRoom(const gf::RectU& room) {
      for (unsigned x = room.min.x + 1; x < room.max.x; ++x) {
        for (unsigned y = room.min.y + 1; y < room.max.y; ++y) {
          m_dungeon({ x, y }) = State::Void;
        }
      }
    }

    void createHorizontalTunnel(unsigned x1, unsigned x2, unsigned y) {
      if (x2 < x1) {
        std::swap(x1, x2);
      }

      for (unsigned x = x1; x <= x2; ++x) {
        m_dungeon({ x, y }) = State::Void;
      }
    }

    void createVerticalTunnel(unsigned x, unsigned y1, unsigned y2) {
      if (y2 < y1) {
        std::swap(y1, y2);
      }

      for (unsigned y = y1; y <= y2; ++y) {
        m_dungeon({ x, y }) = State::Void;
      }
    }
  private:
    gf::Random m_savedRandom;
    gf::Random m_random;
    Tree m_root;
    Dungeon m_dungeon;
  };


  /*
   * Drunkard's March
   */
  class DrunkardMarch : public DungeonGenerator {
  public:
    // public parameters

    float percentGoal;
    float weightForCenter;
    float weightForPreviousDirection;

    virtual Dungeon generate(gf::Vector2u size, gf::Random& random) override {
      switch (getPhase()) {
        case Phase::Start:
          // fallthrough
        case Phase::Iterate:
          generateDungeon(size, random);
          // fallthrough
        case Phase::Finish:
          break;
      }

      setPhase(Phase::Finish);
      return m_dungeon;
    }

  private:
    void generateDungeon(gf::Vector2u size, gf::Random& random) {
      m_dungeon = Dungeon(size, State::Wall);

      m_filled = 0;
      m_currentDirection = gf::Direction::Center;

      m_currentPosition.x = random.computeUniformInteger(2u, size.width - 2);
      m_currentPosition.y = random.computeUniformInteger(2u, size.height - 2);

      unsigned filledGoal = size.width * size.height * percentGoal;
      unsigned maxIterations = size.width * size.height * 10;

      for (unsigned i = 0; i < maxIterations; ++i) {
        walk(size, random);

        if (m_filled >= filledGoal) {
          break;
        }
      }

    }

    void walk(gf::Vector2u size, gf::Random& random) {
      static constexpr gf::Direction Directions[4] = {
        gf::Direction::Up,
        gf::Direction::Right,
        gf::Direction::Down,
        gf::Direction::Left
      };

      static constexpr float EdgePercent = 0.25f;

      double upWeigth = 1.0;
      double rightWeight = 1.0;
      double downWeight = 1.0;
      double leftWeight = 1.0;

      if (m_currentPosition.x <= size.width * EdgePercent) {
        rightWeight += weightForCenter;
      }

      if (m_currentPosition.x >= size.width * (1 - EdgePercent)) {
        leftWeight += weightForCenter;
      }

      if (m_currentPosition.y <= size.height * EdgePercent) {
        downWeight += weightForCenter;
      }

      if (m_currentPosition.y >= size.height * (1 - EdgePercent)) {
        upWeigth += weightForCenter;
      }

      switch (m_currentDirection) {
        case gf::Direction::Up:
          upWeigth += weightForPreviousDirection;
          break;
        case gf::Direction::Right:
          rightWeight += weightForPreviousDirection;
          break;
        case gf::Direction::Down:
          downWeight += weightForPreviousDirection;
          break;
        case gf::Direction::Left:
          leftWeight += weightForPreviousDirection;
          break;
        default:
          break;
      }

      std::discrete_distribution<unsigned> distribution({ upWeigth, rightWeight, downWeight, leftWeight });
      unsigned chosenDirection = distribution(random.getEngine());
      gf::Direction newDirection = Directions[chosenDirection];
      gf::Vector2u newPosition = m_currentPosition;

      switch (newDirection) {
        case gf::Direction::Up:
          if (newPosition.y > 2) {
            --newPosition.y;
          }
          break;
        case gf::Direction::Down:
          if (newPosition.y < size.height - 2) {
            ++newPosition.y;
          }
          break;
        case gf::Direction::Left:
          if (newPosition.x > 2) {
            --newPosition.x;
          }
          break;
        case gf::Direction::Right:
          if (newPosition.x < size.width - 2) {
            ++newPosition.x;
          }
          break;
        default:
          break;
      }

      if (m_currentPosition != newPosition) {
        if (m_dungeon(newPosition) == State::Wall) {
          m_dungeon(newPosition) = State::Void;
          ++m_filled;
        }

        m_currentPosition = newPosition;
        m_currentDirection = newDirection;
      }
    }

  private:
    Dungeon m_dungeon;
    unsigned m_filled;
    gf::Direction m_currentDirection;
    gf::Vector2u m_currentPosition;
  };

}


static void computeDisplay(const Dungeon& dungeon, gf::VertexArray& vertices) {
  static constexpr float CellSize = 16.0f;

  vertices.clear();

  for (auto row : dungeon.getRowRange()) {
    for (auto col : dungeon.getColRange()) {
      gf::Vector2u pos(col, row);


      gf::Vertex v[4];
      v[0].position = pos * CellSize;
      v[1].position = pos * CellSize + gf::Vector2f(CellSize, 0.0f);
      v[2].position = pos * CellSize + gf::Vector2f(0.0f, CellSize);
      v[3].position = pos * CellSize + gf::Vector2f(CellSize, CellSize);

      if (dungeon(pos) == State::Void) {
        v[0].color = v[1].color = v[2].color = v[3].color = gf::Color::White;
      } else {
        v[0].color = v[1].color = v[2].color = v[3].color = gf::Color::Black;
      }

      vertices.append(v[0]);
      vertices.append(v[1]);
      vertices.append(v[2]);

      vertices.append(v[2]);
      vertices.append(v[1]);
      vertices.append(v[3]);
    }
  }
}

// inspired by https://github.com/AtTheMatinee/dungeon-generation (MIT)
// see also: https://www.reddit.com/r/roguelikedev/comments/6df0aw/my_implementation_of_a_bunch_of_dungeon_algorithms/

int main() {
  gf::Random random;

  static constexpr int Size = 1024;
  static constexpr int ExtraSize = 250;
  static constexpr float ViewportX = static_cast<float>(Size) / (Size + ExtraSize);

  static constexpr float ComboHeightMax = 200.0f;

  gf::Window window("gf dungeons", { Size + ExtraSize, Size }, ~gf::WindowHints::Resizable);
  gf::RenderWindow renderer(window);

  gf::Font font(gf::Path(GF_DATADIR) / "DroidSans.ttf");
  gf::UI ui(font);

  // views

  gf::ViewContainer views;

  gf::ExtendView automatonView(gf::RectF::fromPositionSize({ 0.0f, 0.0f }, gf::Vector2f(Size, Size)));
  automatonView.setViewport(gf::RectF::fromPositionSize({ 0.0f, 0.0f }, gf::Vector2f(ViewportX, 1.0f)));
  views.addView(automatonView);

  gf::ScreenView uiView;
  views.addView(uiView);

  views.setInitialScreenSize({ Size + ExtraSize, Size });

  gf::ZoomingViewAdaptor adaptor(renderer, automatonView);

  // ui

  static constexpr int AlgorithmCellularAutomaton = 0;
  static constexpr int AlgorithmDrunkardsMarch = 1;
  static constexpr int AlgorithmTunneling = 2;
  static constexpr int AlgorithmBinarySpacePartitioningTree = 3;

  std::vector<std::string> algorithmChoices = { "Cellular Automaton", "Drunkard's March", "Tunneling", "Binary Space Partioning Tree" };
  int algorithmChoice = AlgorithmCellularAutomaton;
  int currentAlgorithmChoice = algorithmChoice;

  std::vector<std::string> modeChoices = { "Diamond-4", "Square-8", "Diamond-12", "Square-24" };
  int modeChoice = 1;
  int currentModeChoice = modeChoice;

  // state

  unsigned dungeonSize = 64;
  int log2DungeonSize = 6;

  CellularAutomaton cellular;
  cellular.threshold = 0.4f;
  cellular.mode = CellularAutomaton::Mode::Square8;
  cellular.survivalThreshold = 4;
  cellular.birthThreshold = 6;
  cellular.iterations = 5;

  DrunkardMarch march;
  march.percentGoal = 0.4f;
  march.weightForCenter = 0.15f;
  march.weightForPreviousDirection = 0.7f;

  Tunneling tunneling;
  tunneling.maxRooms = 30;
  tunneling.roomSizeMinimum = 6;
  tunneling.roomSizeMaximum = 10;

  BinarySpacePartioningTree bsp;
  bsp.leafSizeMinimum = 10;
  bsp.leafSizeMaximum = 24;
  bsp.roomSizeMinimum = 6;
  bsp.roomSizeMaximum = 15;

  DungeonGenerator *currentGenerator = &cellular;
  auto dungeon = currentGenerator->generate({ dungeonSize, dungeonSize }, random);

  gf::VertexArray vertices(gf::PrimitiveType::Triangles);
  computeDisplay(dungeon, vertices);

  renderer.clear(gf::Color::Gray());

  while (window.isOpen()) {
    gf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case gf::EventType::Closed:
          window.close();
          break;

        case gf::EventType::KeyPressed:
          if (event.key.scancode == gf::Scancode::Escape) {
            window.close();
          }
          break;

        default:
          break;
      }

      adaptor.processEvent(event);
      ui.processEvent(event);
      views.processEvent(event);
    }


    ui.begin("Dungeons", gf::RectF::fromPositionSize(gf::Vector2f(Size, 0.0f), gf::Vector2f(ExtraSize, Size)), gf::UIWindow::Title | gf::UIWindow::Border);

    ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
    ui.label("Size");
    ui.label(std::to_string(dungeonSize), gf::UIAlignment::Right);
    ui.layoutRowDynamic(20, 1);
    if (ui.sliderInt(5, log2DungeonSize, 9, 1)) {
      dungeonSize = 1 << log2DungeonSize;
      currentGenerator->setPhase(DungeonGenerator::Phase::Start);
    }

    ui.layoutRowDynamic(20, 1);
    if (ui.buttonLabel("Generate")) {
      currentGenerator->setPhase(DungeonGenerator::Phase::Start);
    }

    ui.layoutRowDynamic(20, 1);
    ui.label("Algorithm");
    auto algorithmBounds = ui.getWidgetBounds();
    ui.combobox(algorithmChoices, algorithmChoice, 20, { algorithmBounds.getWidth(), ComboHeightMax });

    switch (algorithmChoice) {
      case AlgorithmCellularAutomaton: {
        currentGenerator = &cellular;

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Initial Ratio");
        ui.label(gf::niceNum(cellular.threshold, 0.01f), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderFloat(0.0f, cellular.threshold, 1.0f, 0.01f)) {
          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        ui.layoutRowDynamic(20, 1);
        ui.label("Neighborhood");
        auto bounds = ui.getWidgetBounds();
        ui.combobox(modeChoices, modeChoice, 20, { bounds.getWidth(), ComboHeightMax });

        if (currentModeChoice != modeChoice) {
          currentModeChoice = modeChoice;
          cellular.mode = static_cast<CellularAutomaton::Mode>(modeChoice);
          cellular.survivalThreshold = std::min(cellular.survivalThreshold, CellularAutomaton::modeMax(currentModeChoice));
          cellular.birthThreshold = std::min(cellular.birthThreshold, CellularAutomaton::modeMax(currentModeChoice));
          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Survival Threshold");
        ui.label(std::to_string(cellular.survivalThreshold), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderInt(0, cellular.survivalThreshold, CellularAutomaton::modeMax(currentModeChoice), 1)) {
          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Birth Threshold");
        ui.label(std::to_string(cellular.birthThreshold), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderInt(0, cellular.birthThreshold, CellularAutomaton::modeMax(currentModeChoice), 1)) {
          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Number of Iterations");
        ui.label(std::to_string(cellular.iterations), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderInt(0, cellular.iterations, 20, 1)) {
          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        break;
      }

      case AlgorithmDrunkardsMarch: {
        currentGenerator = &march;

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Fill Percentage Goal");
        ui.label(gf::niceNum(march.percentGoal, 0.01f), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderFloat(0.0f, march.percentGoal, 1.0f, 0.01f)) {
          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Weight for Center");
        ui.label(gf::niceNum(march.weightForCenter, 0.01f), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderFloat(0.0f, march.weightForCenter, 1.0f, 0.05f)) {
          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Weight for Previous Direction");
        ui.label(gf::niceNum(march.weightForPreviousDirection, 0.01f), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderFloat(0.0f, march.weightForPreviousDirection, 1.0f, 0.05f)) {
          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        break;
      }

      case AlgorithmTunneling: {
        currentGenerator = &tunneling;

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Maximum Number of Rooms");
        ui.label(std::to_string(tunneling.maxRooms), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderInt(2, tunneling.maxRooms, 100, 1)) {
          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Minimum Size of Rooms");
        ui.label(std::to_string(tunneling.roomSizeMinimum), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderInt(2, tunneling.roomSizeMinimum, dungeonSize / 2, 1)) {
          if (tunneling.roomSizeMinimum > tunneling.roomSizeMaximum) {
            tunneling.roomSizeMaximum = tunneling.roomSizeMinimum;
          }

          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Maximum Size of Rooms");
        ui.label(std::to_string(tunneling.roomSizeMaximum), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderInt(2, tunneling.roomSizeMaximum, dungeonSize / 2, 1)) {
          if (tunneling.roomSizeMaximum < tunneling.roomSizeMinimum) {
            tunneling.roomSizeMinimum = tunneling.roomSizeMaximum;
          }

          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        break;
      }

      case AlgorithmBinarySpacePartitioningTree: {
        currentGenerator = &bsp;

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Minimum Size of Leafs");
        ui.label(std::to_string(bsp.leafSizeMinimum), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderInt(3, bsp.leafSizeMinimum, dungeonSize / 2, 1)) {
          if (bsp.leafSizeMinimum > bsp.leafSizeMaximum) {
            bsp.leafSizeMaximum = bsp.leafSizeMinimum;
          }

          if (bsp.leafSizeMinimum <= bsp.roomSizeMinimum) {
            bsp.roomSizeMinimum = bsp.leafSizeMinimum - 1;
          }

          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Maximum Size of Leafs");
        ui.label(std::to_string(bsp.leafSizeMaximum), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderInt(3, bsp.leafSizeMaximum, dungeonSize / 2, 1)) {
          if (bsp.leafSizeMaximum < bsp.leafSizeMinimum) {
            bsp.leafSizeMinimum = bsp.leafSizeMaximum;
          }

          if (bsp.leafSizeMinimum <= bsp.roomSizeMinimum) {
            bsp.roomSizeMinimum = bsp.leafSizeMinimum - 1;
          }

          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Minimum Size of Rooms");
        ui.label(std::to_string(bsp.roomSizeMinimum), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderInt(2, bsp.roomSizeMinimum, dungeonSize / 2 - 1, 1)) {
          if (bsp.roomSizeMinimum > bsp.roomSizeMaximum) {
            bsp.roomSizeMaximum = bsp.roomSizeMinimum;
          }

          if (bsp.roomSizeMinimum >= bsp.leafSizeMinimum) {
            bsp.leafSizeMinimum = bsp.roomSizeMinimum + 1;
          }

          if (bsp.leafSizeMinimum > bsp.leafSizeMaximum) {
            bsp.leafSizeMaximum = bsp.leafSizeMinimum;
          }

          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }

        ui.layoutRow(gf::UILayout::Dynamic, 20, { 0.75f, 0.25f });
        ui.label("Maximum Size of Rooms");
        ui.label(std::to_string(bsp.roomSizeMaximum), gf::UIAlignment::Right);
        ui.layoutRowDynamic(20, 1);
        if (ui.sliderInt(2, bsp.roomSizeMaximum, dungeonSize / 2 - 1, 1)) {
          if (bsp.roomSizeMaximum < bsp.roomSizeMinimum) {
            bsp.roomSizeMinimum = bsp.roomSizeMaximum;
          }

          currentGenerator->setPhase(DungeonGenerator::Phase::Iterate);
        }
        break;
      }

      default:
        break;
    }

    ui.end();

    if (currentAlgorithmChoice != algorithmChoice) {
      currentAlgorithmChoice = algorithmChoice;
      currentGenerator->setPhase(DungeonGenerator::Phase::Start);
    }

    if (currentGenerator->getPhase() != DungeonGenerator::Phase::Finish) {
      dungeon = currentGenerator->generate({ dungeonSize, dungeonSize }, random);
      computeDisplay(dungeon, vertices);
    }

    renderer.clear();

    renderer.setView(automatonView);
    renderer.draw(vertices);

    renderer.setView(uiView);
    renderer.draw(ui);

    renderer.display();
  }

  return 0;
}
