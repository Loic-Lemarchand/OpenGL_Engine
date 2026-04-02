# OPENGL_Engine

Real-time 3D graphics engine in **C++20** using **OpenGL**, featuring an Unreal Engine–inspired scene graph, a decoupled event system (Event Bus), and a rigid-body physics engine.

---

## Table of Contents

- Dependencies
- Build
- File Architecture
- Game Loop
- Scene Graph
- Event Bus
- Physics Engine
- Rendering
- Inputs
- TODO / Roadmap
- Global Diagrams

---

## Dependencies

| Library | Role | Management |
|---|---|---|
| OpenGL | Graphics API | System |
| GLFW 3 | Windowing, GL context, input | vcpkg |
| GLAD | OpenGL loading | vcpkg |
| GLM | Mathematics | vcpkg |
| Assimp | 3D model import | vcpkg |
| stb_image | Texture loading | Header-only |

---

## Build

cmake -B out/build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake  
cmake --build out/build --config Release  

---

## File Architecture

OPENGL_Engine/
├── main.cpp
├── CMakeLists.txt
├── headers/
├── src/
├── sceneGraph/
├── render/
├── physics/
└── Assets/

---

## Game Loop

Input → Events → Logic/Physics → Rendering

---

## Scene Graph

Actor  
├── RootComponent (SceneComponent)  
│   ├── MeshComponent  
│   └── other SceneComponents  
└── ActorComponents[]  
    └── RigidBodyComponent  

---

## Event Bus

Decoupled Publish/Subscribe system.

### How it works

1. Producers push events  
2. dispatchEvents() triggers callbacks  
3. Subscribers register via subscribe()  

---

## Physics Engine

Rigid-body simulation with fixed timestep (1/60 s)

### Body types

| Type | Behavior |
|------|---------|
| Static | Immovable |
| Dynamic | Affected by gravity |
| Sleeping | At rest |

### Integration

Semi-implicit Euler:

    acceleration = force x inverseMass
    velocity     += acceleration x dt
    velocity     *= damping^dt
    position     += velocity x dt

---

## Rendering

Forward OpenGL pipeline:

1. Load models  
2. Traverse scene  
3. Draw with matrices + lighting  

---

## Inputs

GLFW callbacks → InputManager → EventBus

---

## TODO

- Angular physics  
- Multi-contact collisions  
- Spatial partitioning  
- Friction  
- Advanced lighting  

---

## Global Diagrams

### Initialization

mermaid
flowchart TD
    M1["main()"] --> M2["EventBus"]
    M2 --> M3["InputManager"]
    M3 --> M4["Window"]
    M4 --> M5["World"]
    M5 --> M6["Camera"]
    M6 --> M7["Renderer"]

---

### Game Loop

mermaid
flowchart TD
    A["Input"] --> B["EventBus"]
    B --> C["Physics"]
    C --> D["Rendering"]

---

### ECS Architecture

mermaid
flowchart LR
    Actor --> SceneComponent
    Actor --> ActorComponent
    SceneComponent --> MeshComponent
    ActorComponent --> RigidBody

---

### Event Bus Flow

mermaid
flowchart LR
    InputManager --> EventBus
    EventBus --> Subscribers
