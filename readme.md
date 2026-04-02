# OPENGL_Engine

Real-time 3D graphics engine in **C++20** using **OpenGL**, featuring a scene graph inspired by Unreal Engine, a decoupled event system (Event Bus), and a rigid-body physics engine.

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
- [Inputs](#inputs)
- [TODO / Roadmap](#todo--roadmap)
- [Global Diagram](#global-diagram)

---

## Dependencies

| Library | Role | Management |
|---|---|---|
| **OpenGL** | Graphics API | System |
| **GLFW 3** | Windowing, GL context, input | vcpkg |
| **GLAD** | OpenGL function loading | vcpkg |
| **GLM** | Mathematics (vectors, matrices) | vcpkg |
| **Assimp** | 3D model import (FBX, OBJ…) | vcpkg |
| **stb_image** | Texture loading (PNG, JPG…) | Header-only included |

---

## Build

```bash
cmake -B out/build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build out/build --config Release
```

---

## File Architecture
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
│   │   ├── world.h                #   World singleton (actors + physics)
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

---

## Game Loop

Input → Events → Logic/Physics → Rendering

---

## Scene Graph

Actor
├── RootComponent (SceneComponent)     ← Root of the transform graph
│   ├── MeshComponent                  ← 3D model (child)
│   └── other SceneComponents...
└── ActorComponents[]                  ← Logical components without transform
    └── RigidBodyComponent             ← Physics (mass, velocity, forces)
        └── ColliderComponent (owned)

---

## Event Bus

Decoupled Publish/Subscribe system (`eventBus.h` / `eventBus.cpp`).

### How it works

1. **Producers** (e.g., `InputManager`) push events into the queue via  
   `pushEvent(makeEvent(type, name, args...))`

2. Each frame, `dispatchEvents()` iterates over the queue, calls callbacks registered by `EventType`, then clears the queue

3. Subscribers register via `subscribe()`

### Available Events (bitmask)

| EventType   | Payload           | Consumers       |
|------------|------------------|-----------------|
| `KeyPressed`  | `int` (key code) | Camera, Window  |
| `KeyReleased` | `int`            |                 |


### `subscribe` overloads

```cpp
// Lambda / std::function
eventBus->subscribe(type, [](const Event& e) { ... });

// Member function + raw pointer
eventBus->subscribe(EventType::KeyPressed, &Camera::onKeyBoardInput, this);

// Member function + shared_ptr (safe via internal weak_ptr)
eventBus->subscribe(type, &MyClass::onEvent, sharedInstance);

---

## Physics Engine

Rigid-body simulation with fixed timestep (1/60 s).

### Body types

| Type      | Behavior                                      |
|-----------|-----------------------------------------------|
| `Static`  | Immovable, inverse mass = 0                   |
| `Dynamic` | Affected by gravity and collisions            |
| `Sleeping`| At rest (velocity < threshold), wakes on collision |

### Collision shapes

| Shape         | Data                         |
|---------------|------------------------------|
| `SphereShape` | `float radius`               |
| `BoxShape`    | `vec3 halfExtents`           |
| `PlaneShape`  | `vec3 normal, float offset`  |

### Integration

Semi-implicit Euler:

```cpp
acceleration = force x inverseMass
velocity     += acceleration x dt
velocity     *= damping^dt
position     += velocity x dt

### Collision pipeline 

Broadphase (AABB overlap)
    | no -> skip
    | yes
    v
Narrowphase (precise test)
    v
Position correction (anti-jitter slop)
    v
Impulse resolution

---

## Rendering

Forward OpenGL pipeline:

1. Load models via Assimp → Mesh (VAO/VBO/IBO)
2. Traverse scene graph → collect RenderProxies
3. Draw each proxy with Model/View/Projection + lighting

---

## Inputs


    GLFW callbacks
     ├─ key_callback → InputManager::processKey
     └─ mouse        → InputManager::processMouse
                            |
                    pushEvent(...) → EventBus

---

## TODO / Roadmap

- Angular physics
- Multi-contact collisions
- Spatial partitioning
- Sphere vs Box collision
- Friction
- EventBus unsubscribe
- Advanced lighting
- Center of mass calculation


## Global diagram


mermaid
flowchart TD
    subgraph INIT["🚀 Initialization (main.cpp)"]
        M1["main()"] --> M2["Create EventBus"]
        M2 --> M3["Create InputManager\n(ref. to EventBus)"]
        M3 --> M4["Create Window\n(GLFW, 1920x1080)"]
        M4 --> M5["World::getWorld()\n(Singleton)"]
        M5 --> M6["spawnActor&lt;ACamera&gt;()"]
        M6 --> M7["Create Renderer\n(OpenGL, Shader, Projection)"]
        M7 --> M8["spawnActor&lt;ATown&gt;()\nStatic + PlaneShape"]
        M8 --> M9["spawnActor&lt;ACrate&gt;()\nDynamic + BoxShape"]
    end

    subgraph LOOP["🔄 Game loop (each frame)"]
        direction TB
        L0["deltaTime = currentFrame - lastFrame"]
        L0 --> L1

        subgraph L1["1. Window::update()"]
            W1["glfwPollEvents()"] --> W2["glfwSwapBuffers()"]
            W2 --> W3["GLFW callbacks triggered"]
            W3 --> W4["key_callback → InputManager::processKey()"]
            W3 --> W5["HandleMouse → InputManager::processMouse()"]
        end

        L1 --> L2

        subgraph L2["2. EventBus::dispatchEvents()"]
            E1["Iterate over myEventQueue"] --> E2{"For each Event:\nfind subscribers\nby EventType"}
            E2 -->|"KeyPressed"| E3["Camera::onKeyBoardInput(int)\nWindow lambda (Esc, ←, →)"]
            E2 -->|"KeyReleased"| E4["Camera::onKeyBoardRelease(int)\nWindow lambda"]
            E2 -->|"MouseMoved"| E5["Camera::onMouseInput(double, double)\nWindow lambda"]
            E3 --> E6["clearEvents()"]
            E4 --> E6
            E5 --> E6
        end

        L2 --> L3

        subgraph L3["3. World::Tick()"]
            T1["For each Actor → Tick()"] --> T1b["ACamera::Tick()\n→ Camera::update()\n→ WASD/QE movement\n→ CalculateViewMatrix()"]
            T1b --> T2

            subgraph T2["PhysicsWorld::Step(1/60)"]
                direction TB
                P0["Sync collider positions\n(all objects)"]
                P0 --> P1

                subgraph P1["For each Dynamic object"]
                    PH1["Apply gravity\n(force += g × mass)"]
                    PH1 --> PH2["Integrate(dt)\n─ acceleration = force / mass\n─ velocity += acceleration × dt\n─ velocity ×= damping^dt\n─ position += velocity × dt"]
                    PH2 --> PH3["ClearForces()"]
                    PH3 --> PH4["Sync collider position"]
                end

                P1 --> P2

                subgraph P2["Collision detection &amp; resolution"]
                    direction TB
                    C1["For each pair (A, B)\ndeduplication by address"]
                    C1 --> C2["Broadphase: AABB overlap?"]
                    C2 -->|"No"| C2b["Skip"]
                    C2 -->|"Yes"| C3["Narrowphase: checkCollision()\nSphere/Sphere, Box/Box,\nSphere/Plane, Box/Plane"]
                    C3 --> C4{"Collision?"}
                    C4 -->|"No"| C2b
                    C4 -->|"Yes"| C5["Position correction\n(with anti-jitter slop)"]
                    C5 --> C6["Impulse resolution\n─ vRel = vA − vB\n─ j = -(1+e)·vn / (1/mA + 1/mB)\n─ apply impulse"]
                    C6 --> C7["Wake sleeping objects\nif impact is sufficient"]
                end

                P2 --> P3["Set to Sleeping\nif velocity &lt; threshold"]
            end
        end

        L3 --> L4

        subgraph L4["4. Renderer::update()"]
            R1["Get ViewMatrix\nfrom Camera"] --> R2["For each Actor\n→ collectRenderProxies()"]
            R2 --> R3["For each RenderProxy\n→ glDraw with\nModel, Projection, View"]
            R3 --> R4["Apply lighting\n(Ambient + PointLight)"]
        end
    end

    INIT --> LOOP

    subgraph ARCH["📐 Component architecture (Unreal-inspired ECS)"]
        direction LR
        CO["Component\n(base)"] --> AC["ActorComponent"]
        CO --> SC["SceneComponent\n(position, rotation, scale,\nparent/children)"]
        AC --> RB["RigidBodyComponent\n(mass, velocity, forces,\nbodyType)"]
        SC --> MC["MeshComponent\n(Model, RenderProxy)"]
        SC --> CAM["Camera\n(view, yaw, pitch)"]
        SC --> COL["ColliderComponent\n(ShapeData = variant&lt;\nBoxShape, SphereShape,\nPlaneShape&gt;)"]
        RB -.->|"owns"| COL
        ACT["Actor\n(RootComponent,\nComponents[])"] -->|"contains"| SC
        ACT -->|"contains"| AC
    end

    subgraph EVBUS["📨 Event Bus (Publish / Subscribe)"]
        direction LR
        IM["InputManager"] -->|"pushEvent(\nmakeEvent(type, args...))"| EB["EventBus\nmyEventQueue[]"]
        EB -->|"dispatchEvents()\n→ callbacks by type"| SUB["Subscribers\n(lambdas, &amp;Class::Method)"]
    end