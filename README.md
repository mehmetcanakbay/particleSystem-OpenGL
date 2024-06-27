# Particle System in OpenGL

Most implementations on GitHub use shaders or compute shaders to move particles. In this implementation I went for a different approach and moved particles using a thread pool.

<video src='media/sample.mp4' width=480></video>

## Implementation Details

You need to have an instance of a renderer class and a layer. Sample usage is shown in [App.cpp](https://github.com/mehmetcanakbay/particleSystem-OpenGL/blob/main/particleSystem/src/App.cpp#L5C1-L26C2). Layer is an abstract class and you can push several of these layer objects into the renderer.

Layers have several virtual functions and these will get called in certain parts of the rendering process. For example, OnInit function gets called after OpenGL initializes. You can find where they are called at which point by looking at [Renderer.cpp](https://github.com/mehmetcanakbay/particleSystem-OpenGL/blob/main/particleSystem/src/Core/Private/Renderer.cpp)

Current example spawns 1M particles at once. 

## TODOs

- Instead of particles having a random velocity set during initialization, make a vortex force.
- Several particle systems at once.  
- Refactor magic numbers in Particle.cpp