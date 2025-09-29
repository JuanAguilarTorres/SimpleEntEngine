### Technical Overview

The program’s infrastructure is outlined through a comprehensive UML diagram, providing a high-level view of the system architecture and relationships between components.

![UML Architecture](../artifacts/Engine_UML.svg)
[Open file](../artifacts/Engine_UML.svg)


This UML diagram showcases the core structure, design patterns, and interactions within the engine, offering insight into how various components work together to create the experience.

This program has been developed to meet high standards of functionality and quality.

### Documentation

The program includes thorough internal documentation that provides clear explanations for all classes and parameters. Additionally, comprehensive external documentation is available through Doxygen, along with a UML diagram that illustrates the structural relationships and design of the system.

### Memory Management

Memory management is a priority in this program, with careful handling of all allocated resources. Key memory management practices include:

- **SDL and Audio**: All audio channels, SDL resources, and related libraries are properly freed upon closure.
- **Textures and Audio Files**: These resources are freed upon exiting each scene to ensure efficient memory use.
- **Entities**: Individual entities are freed immediately upon being removed from the scene.
- **Video Frames**: Frames are freed on a per-frame basis, ensuring smooth video playback without excessive memory buildup.
- **3D Backface Culling**: To optimize rendering performance, 3D backface culling is applied. This ensures that only the visible sides of 3D models are rendered, reducing the number of polygons drawn and improving frame rates.
- 
This approach ensures efficient memory usage, with all libraries and objects properly initialized and released.

### Optimization: Flyweight Pattern

The **Flyweight Pattern** is used to optimize memory and performance by minimizing the creation of duplicate objects, especially useful in cases where multiple similar entities are needed. Instead of creating new instances from scratch, the pattern uses a shared "flyweight" object, which is modified as needed without loading the full data multiple times.

The engine is built to encourage a flyweight approach for efficient memory usage and performance. Rather than repeatedly opening and closing large Lua files for each entity, it provides mechanisms—such as factory instances tagged as "Original" with unique IDs—that make it easy to clone entities on demand. For example, when a program needs a "Player Bullet" after a shoot action, it can clone it from the factory instance instead of reloading the Lua file. Each "Child" entity inherits the properties of its "Original" while behaving as a concrete instance, allowing tailored adjustments without sacrificing memory efficiency. By design, the engine facilitates this workflow, but implementing it in a specific program requires using the provided functions appropriately.

Example of the Flyweight Pattern:

![Flyweight Pattern](https://danielggarcia.wordpress.com/wp-content/uploads/2014/03/032414_2059_patronesest1.png?w=584)

*(Image credit: Daniel Garcia)*

### Security: Revamped Entity ID Allocator

The updated ID allocator now logs every action to security logs, providing detailed tracking and reporting on ID usage. A new system monitors the status of all IDs in real time, automatically handling potential conflicts. For instance, if an entity tries to reuse an existing ID, the system checks the current status of the ID and resolves the conflict, preventing potential runtime errors. This robust ECS state tracking, significantly reduces ID allocation errors and improving overall program stability.

### Good Practice: Warming Up Video Decoder & Buffer

When creating the video system, a notable issue was that the initial frames lacked sufficient data, resulting in flashes of corrupted or “trash” frames on the screen. This could be distracting or discomforting for the player. To address this, a **"warm-up" mechanism** was implemented for the video decoder and buffer.

The warm-up process works by processing but not displaying an adjustable number of frames at the start of each video. This ensures that the first frame shown to the player is fully loaded and clear. The warm-up happens so quickly that it is imperceptible to the user, seamlessly enhancing the quality of video playback from the very first frame.

## 3D Renderer

The 3D system has been integrated into the engine to support rendering 3D models in real-time. Currently, the progress involves parsing OBJ and MTL files, saving the data, and rendering the models using SDL2 for visualization.

The `Render3DSystem` is responsible for handling 3D rendering, including the loading, processing, and display of 3D models. The system is designed to work with triangulated 3D models (make sure to use the `CTRL+T` function in Blender to triangulate models before importing). The following features have been implemented:

### Key Features of the `Render3DSystem`:
- **Model Parsing**: The system parses OBJ files, extracting the vertex data and face definitions, and processes MTL files to associate materials with models.
- **Rendering**: It draws 3D models as wireframes or shaded models, based on their transformations (translation, rotation, and scaling).
- **Backface Culling**: The system calculates the visibility of faces using the normal vector to perform backface culling (eliminating faces that are not visible).
- **Depth Sorting**: Faces are sorted by their average depth to ensure correct rendering order (from back to front).
- **Material Support**: The system applies custom shading to the models based on the materials defined in the MTL file, with basic shading based on vertex normals and light direction.
- **Other features**: You can give a custom shading color to each model instance, likewise, the engine uses the already existing transform component to define the location so that it is versatile.

Workflow:
1. The system loads a 3D model's OBJ and MTL files.
2. It processes the vertex and face data to create a list of renderable faces.
3. The model is rotated (if necessary) based on the object's rotation component.
4. The faces are drawn on the screen, either as wireframes or shaded polygons.

This system provides real-time 3D rendering functionality, enabling complex 3D models to be displayed and manipulated in the game environment.