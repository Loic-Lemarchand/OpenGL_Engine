# OPENGL_Engine

Real-time 3D graphics engine in **C++20** with **OpenGL**, integrating a scene graph inspired by Unreal Engine, a decoupled event system (Event Bus), and a rigid-body physics engine.

---

## Table of Contents

- [Dependencies](#dependencies)
- [Build](#build)
- [File Architecture](#file-architecture)
- [Game Loop](#game-loop)
- [Scene Graph (Unreal-inspired ECS)](#scene-graph)
- [Event Bus](#event-bus)
- [Physics Engine](#physics-engine)
- [Rendering](#rendering)
- [Input](#input)
- [Architecture Diagrams](#architecture-diagrams)
- [TODO / Roadmap](#todo--roadmap)

---

## Dependencies

| Library | Role | Management |
|---|---|---|
| **OpenGL** | Graphics API | System |
| **GLFW 3** | Windowing, GL context, inputs | vcpkg |
| **GLAD** | OpenGL function loading | vcpkg |
| **GLM** | Mathematics (vectors, matrices) | vcpkg |
| **Assimp** | 3D model import (FBX, OBJ…) | vcpkg |
| **stb_image** | Texture loading (PNG, JPG…) | Header-only included |

---

## Build

```bash
# Requirements: CMake >= 3.21, vcpkg with packages above
cmake -B out/build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build out/build --config Release
```

Or open the folder directly in **Visual Studio** (native CMake).

---

## File Architecture

```
OPENGL_Engine/
├── main.cpp                       # Entry point, game loop
├── CMakeLists.txt                 # CMake configuration (C++20)
├── headers/                       # Global headers
│   ├── eventBus.h                 #   Publish/Subscribe system
│   ├── inputManager.h             #   Keyboard/mouse input handling
│   ├── window.h                   #   GLFW window
│   └── utilities.h                #   Logs, globals (deltaTime)
├── src/                           # Global implementations
│   ├── eventBus.cpp
│   ├── inputManager.cpp
│   └── window.cpp
├── sceneGraph/                    # Scene Graph & ECS
│   ├── headers/
│   │   ├── world.h                #   Singleton World (actors + physics)
│   │   ├── actor.h                #   Base entity (Actor)
│   │   ├── component.h            #   Component, SceneComponent, MeshComponent
│   │   └── camera.h               #   Camera + ACamera
│   └── src/
│       ├── world.cpp
│       ├── actor.cpp
│       ├── component.cpp
│       └── camera.cpp
├── render/                        # OpenGL rendering pipeline
│   ├── headers/
│   │   ├── renderer.h             #   Rendering orchestrator
│   │   ├── shader.h               #   Vertex/fragment shader compilation
│   │   ├── model.h                #   Model loading (Assimp)
│   │   ├── mesh.h                 #   VAO/VBO/IBO, vertices, draw calls
│   │   ├── texture.h              #   OpenGL textures
│   │   └── light.h                #   Lighting (Ambient + PointLight)
│   └── src/
├── physics/                       # Rigid-body physics engine
│   ├── headers/
│   │   ├── physicsWorld.h         #   Simulation (Step, detection, resolution)
│   │   └── physicsComponent.h     #   RigidBody, Collider, geometric shapes
│   └── src/
│       ├── physicsWorld.cpp
│       └── physicsComponent.cpp
└── Assets/                        # 3D models (FBX)
```

---

## Game Loop

Defined in `main.cpp`, the loop follows the classic **Input → Events → Logic/Physics → Render** pattern:

```
while (!window->shouldClose())
│
├─ 1. window->update()              // glfwPollEvents → GLFW callbacks
│     └─ InputManager pushes events to EventBus
│
├─ 2. eventBus->dispatchEvents()    // Distributes events to subscribers
│     └─ Calls callbacks (Camera, Window…) then clears queue
│
├─ 3. world.Tick()
│     ├─ Actor::Tick() for each actor (ex: Camera updates view)
│     └─ PhysicsWorld::Step(1/60)
│         ├─ Gravity + Integration
│         ├─ Collision detection (Broadphase AABB + Narrowphase)
│         └─ Resolution (position correction + impulse)
│
└─ 4. renderer->update()            // Collects RenderProxies → OpenGL draw
```

---

## Scene Graph

Hierarchy of components attached to **Actors**, inspired by Unreal Engine:

```
Actor
├── RootComponent (SceneComponent)     ← Root of transformation graph
│   ├── MeshComponent                  ← 3D model (child)
│   └── other SceneComponents...
└── ActorComponents[]                  ← Logic components without transform
    └── RigidBodyComponent             ← Physics (mass, velocity, forces)
        └── ColliderComponent (owned)
```

| Class | Inherits from | Role |
|---|---|---|
| `Component` | — | Base: owner, active, valid |
| `ActorComponent` | `Component` | Logic components without transformation |
| `SceneComponent` | `ActorComponent` | Position, rotation, scale + parent/children tree |
| `MeshComponent` | `SceneComponent` | Contains a `Model`, provides `RenderProxy` |
| `Camera` | `SceneComponent` | FPS view (yaw/pitch), view matrix |
| `RigidBodyComponent` | `ActorComponent` | Mass, velocity, forces, body type |
| `ColliderComponent` | `SceneComponent` | Geometric shape (`variant<Box, Sphere, Plane>`) |

**Tick** is recursive: `Actor::Tick()` → `RootComponent::Tick()` → propagates to children, then ticks `ActorComponent` not in the graph.

---

## Event Bus

Decoupled **Publish/Subscribe** system (`eventBus.h` / `eventBus.cpp`).

### How It Works

1. **Producers** (ex: `InputManager`) push events to queue via `pushEvent(makeEvent(type, name, args...))`
2. **Each frame**, `dispatchEvents()` iterates queue, calls callbacks registered by `EventType`, then clears queue
3. **Subscribers** register via `subscribe()`

### Available Events (bitmask)

| EventType | Payload | Consumers |
|---|---|---|
| `KeyPressed` | `int` (key code) | Camera, Window |
| `KeyReleased` | `int` | Camera, Window |
| `MouseMoved` | `double, double` (x, y) | Camera, Window |
| `WindowResized` | `int, int` (w, h) | — |

### `subscribe` Overloads

```cpp
// Lambda / std::function
eventBus->subscribe(type, [](const Event& e) { ... });

// Member method + raw pointer (auto-unpacks payload)
eventBus->subscribe(EventType::KeyPressed, &Camera::onKeyBoardInput, this);

// Member method + shared_ptr (safe via internal weak_ptr)
eventBus->subscribe(type, &MyClass::onEvent, sharedInstance);
```

Template overloads use `dynamic_cast<const PayloadEvent<Args...>*>` + `std::apply` to call member method directly with unpacked arguments.

---

## Physics Engine

Fixed-step rigid-body simulation (1/60 s), in `physicsWorld.cpp` / `physicsComponent.cpp`.

### Body Types (`bodyType`)

| Type | Behavior |
|---|---|
| `Static` | Immobile, inverse mass = 0 |
| `Dynamic` | Subject to gravity and collisions |
| `Sleeping` | Dynamic at rest (velocity < threshold), woken by collision |

### Collision Shapes (`std::variant<BoxShape, SphereShape, PlaneShape>`)

| Shape | Data |
|---|---|
| `SphereShape` | `float radius` |
| `BoxShape` | `vec3 halfExtents` |
| `PlaneShape` | `vec3 normal, float offset` |

### Integration (`RigidBodyComponent::Integrate`)

Semi-implicit Euler:

```
acceleration  = resultantForce × inverseMass
velocity     += acceleration × dt
velocity     *= damping^dt                       // air friction (0.99)
position     += velocity × dt
```

### Collision Pipeline

```
Broadphase (AABB overlap)
    │ no → skip
    ▼ yes
Narrowphase (precise geometric test)
    ├─ Sphere vs Sphere : distance between centers < sum of radii
    ├─ Box vs Box       : simplified SAT (3 axes, least penetration)
    ├─ Sphere vs Plane  : signed distance < radius
    └─ Box vs Plane     : project half-extents on normal
    ▼
Position correction (with anti-jitter slop 0.01)
    ▼
Impulse resolution:
    j = -(1 + restitution) × dot(vRel, n) / (1/mA + 1/mB)
    vA += j × n / mA
    vB -= j × n / mB
```

---

## Rendering

Forward OpenGL pipeline defined in `renderer.cpp`:

1. **Loading**: `Model::Load()` via Assimp → node tree → `Mesh` (VAO/VBO/IBO) + textures
2. **Collection**: `Actor::collectRenderProxies()` traverses scene graph; each `MeshComponent` provides `RenderProxy` (world matrix + model)
3. **Draw**: for each proxy → bind shader, upload matrices (Model, View, Projection), lighting (Ambient + PointLight), `glDrawElements`

---

## Input

```
GLFW callbacks (static)
    ├─ key_callback   → InputManager::processKey(key, action)
    └─ HandleMouse    → InputManager::processMouse(xPos, yPos)
                              │
                   pushEvent(makeEvent(...)) → EventBus
```

`InputManager` transforms raw GLFW callbacks into typed events (`KeyPressedEvent`, `MouseEvent`) and pushes them to `EventBus`.

---

## Architecture Diagrams

### Diagram 1: Initialization

```mermaid
flowchart TD
    M1["main()"] --> M2["Create EventBus"]
    M2 --> M3["Create InputManager\n(ref. to EventBus)"]
    M3 --> M4["Create Window\n(GLFW, 1920x1080)"]
    M4 --> M5["World::getWorld()\n(Singleton)"]
    M5 --> M6["spawnActor&lt;ACamera&gt;()"]
    M6 --> M7["Create Renderer\n(OpenGL, Shader, Projection)"]
    M7 --> M8["spawnActor&lt;ATown&gt;()\nStatic + PlaneShape"]
    M8 --> M9["spawnActor&lt;ACrate&gt;()\nDynamic + BoxShape"]
```

### Diagram 2: Game Loop - Input & Events

```mermaid
flowchart TD
    L0["deltaTime = currentFrame - lastFrame"]
    L0 --> W1["glfwPollEvents()"]
    W1 --> W2["glfwSwapBuffers()"]
    W2 --> W3["GLFW callbacks triggered"]
    W3 --> W4["key_callback → InputManager::processKey()"]
    W3 --> W5["HandleMouse → InputManager::processMouse()"]
    W4 --> E1["pushEvent to EventBus"]
    W5 --> E1
    E1 --> E2["EventBus::dispatchEvents()"]
    E2 --> E3["Iterate myEventQueue"]
    E3 --> E4{"For each Event:\nfind subscribers\nby EventType"}
    E4 -->|"KeyPressed"| E5["Camera::onKeyBoardInput(int)\nWindow lambda"]
    E4 -->|"KeyReleased"| E6["Camera::onKeyBoardRelease(int)"]
    E4 -->|"MouseMoved"| E7["Camera::onMouseInput(double, double)"]
    E5 --> E8["clearEvents()"]
    E6 --> E8
    E7 --> E8
```

### Diagram 3: Game Loop - Ticking & Physics

```mermaid
flowchart TD
    T1["World::Tick()"] --> T2["For each Actor → Tick()"]
    T2 --> T3["ACamera::Tick()\n→ update view matrix\n→ WASD/QE movement"]
    T3 --> P0["PhysicsWorld::Step(1/60)"]
    P0 --> P1["Sync collider positions"]
    P1 --> P2["For each Dynamic object:\nApply gravity"]
    P2 --> P3["Integrate(dt)\n- acceleration = force / mass\n- velocity += acceleration × dt\n- velocity ×= damping^dt\n- position += velocity × dt"]
    P3 --> P4["ClearForces()"]
    P4 --> P5["Sync collider position"]
    P5 --> P6["Collision Detection"]
```

### Diagram 4: Game Loop - Collision & Resolution

```mermaid
flowchart TD
    C1["For each pair A, B\n(deduplication by address)"]
    C1 --> C2["Broadphase: AABB overlap?"]
    C2 -->|"No"| C2b["Skip"]
    C2 -->|"Yes"| C3["Narrowphase: checkCollision()\nSphere/Sphere, Box/Box,\nSphere/Plane, Box/Plane"]
    C3 --> C4{"Collision\ndetected?"}
    C4 -->|"No"| C2b
    C4 -->|"Yes"| C5["Position correction\n(anti-jitter slop)"]
    C5 --> C6["Impulse resolution\n- vRel = vA − vB\n- j = -(1+e)·vn / (1/mA + 1/mB)\n- apply impulse"]
    C6 --> C7["Wake sleeping objects\nif impact sufficient"]
    C7 --> C8["Set to Sleeping\nif velocity &lt; threshold"]
```

### Diagram 5: Game Loop - Rendering

```mermaid
flowchart TD
    R1["Renderer::update()"]
    R1 --> R2["Get ViewMatrix from Camera"]
    R2 --> R3["For each Actor\n→ collectRenderProxies()"]
    R3 --> R4["For each RenderProxy"]
    R4 --> R5["Bind shader"]
    R5 --> R6["Upload matrices\n(Model, View, Projection)"]
    R6 --> R7["Apply lighting\n(Ambient + PointLight)"]
    R7 --> R8["glDrawElements"]
```

### Diagram 6: Component Architecture

```mermaid
flowchart LR
    CO["Component\n(base)"] --> AC["ActorComponent"]
    CO --> SC["SceneComponent\n(position, rotation, scale,\nparent/children)"]
    AC --> RB["RigidBodyComponent\n(mass, velocity, forces)"]
    SC --> MC["MeshComponent\n(Model, RenderProxy)"]
    SC --> CAM["Camera\n(view, yaw, pitch)"]
    SC --> COL["ColliderComponent\n(variant&lt;Box,\nSphere, Plane&gt;)"]
    RB -.->|"owns"| COL
    ACT["Actor\n(RootComponent,\nComponents[])"] -->|"contains"| SC
    ACT -->|"contains"| AC
```

### Diagram 7: Event Bus System

```mermaid
flowchart LR
    IM["InputManager"] -->|"pushEvent(\nmakeEvent(...)))"| EB["EventBus\nmyEventQueue[]"]
    EB -->|"dispatchEvents()\n→ callbacks by type"| SUB["Subscribers\n(lambdas, &Class::Method)"]
```

---

## TODO / Roadmap

- [ ] **Angular Physics** — Torques, inertia tensor, angular velocity
- [ ] **Multi-contact** — `CollisionManyFold` prepared for multiple contact points, only one generated currently
- [ ] **Spatial Partitioning** — Detection is O(n²); add grid or BVH
- [ ] **Sphere vs Box** — Collider combination not yet implemented
- [ ] **Tangential Friction** — No friction in impulse resolution
- [ ] **EventBus Unsubscribe** — No `unsubscribe` mechanism
- [ ] **Advanced Lighting** — Specular, multiple lights, shadow mapping
- [ ] **Center of Mass** — `calculateCenterOfMassFromMesh` always returns `(0,0,0)`