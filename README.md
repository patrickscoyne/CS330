# CS-330: Computer Graphics and Visualization

**How do I approach designing software?**

My approach to software design starts with analyzing requirements. Next, I break the project down into manageable pieces. For this project, the task was to create an interactive 3D scene from a 2D image of my choosing. I could source a 2D image from online or take one of my own. I chose to take an image of my own. At least one object in the scene needed to be complex (constructed from more than one basic shape). I chose a coffee table scene because it allowed me to combine multiple primitives while creating something visually realistic.

The course was thoughtfully structured with milestones that helped break the overall project into manageable pieces. This approach allowed me to focus on one aspect of the scene at a time. The project evolved from putting together basic shapes to adding camera controls, and then applying textures and lighting. Early in the course, my instructor suggested encapsulating each object into its own class. As my design decisions evolved and became more complex (especially with the coffee cup), I immediately saw the benefit of this approach. If all of those shapes, transformations, and texture calls had been placed directly in RenderScene(), the code would have become messy. Instead, each object now has its own dedicated Render() method, which made the code much cleaner.

**How do I approach developing programs?**

I use an iterative approach when developing programs. I start with the basic functionality and then layer on additional features. This project required visual testing at every step, which was new to me, as I had never created a 3D scene before. One of the most interesting things was learning to create my own seamless textures. For the diamond pattern on the coffee cup, the wooden table, and the grapefruit skin, I took photos of the actual objects and then used a graphics editor and tiling to produce textures that repeat without visible seams. Creating seamless textures was completely new to me and required a lot of trial and error.

Iteration played a major role throughout development. The coffee cup, for example, went through many versions before I arrived at the final design with nested tapered cylinders for realistic wall thickness. The same iterative process was applied to lighting the scene. I spent considerable time adjusting positions, colors, and intensities of the two lights until the scene had the right balance. I also started using constants for all texture tags in PrepareScene() and added error checking with a MessageBox.

My approach to developing code has evolved significantly throughout the milestones. Early in the course, I had the cup, plate, grapefruit, table, and coaster all being drawn directly in RenderScene(). By the final project, I moved each object into its own clean class with a dedicated Render() method. I also added a loop in SetupSceneLights() that makes it easy to add more lights in the future. These changes resulted in a final project that is much more readable, modular, and maintainable.

**How can computer science help me in reaching my goals?**

As someone transitioning from construction to software engineering, this course has shown me how powerful 3D modeling and interactive visualization can be. In construction, we currently rely heavily on 2D building plans. With what I have learned, I can now take those plans and create interactive 3D models of homes or buildings. Customers could virtually walk through a proposed design, explore different layouts, and see material options in real time before construction even begins. Therefore, this course has opened up exciting new possibilities for combining my construction background with software development.

