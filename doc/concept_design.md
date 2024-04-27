# ICP Project: Simple 2D robot collision simulator

## Divided libraries
Application is made out of two parts, **2DSimulationLib** and **QtSpecific** (names of the directories). The reasoning is that simulation where all collisions and calculations are being done to be separated from the Qt application (window/GUI). This way the GUI is simply copying positions, collisions etc... from the simulation and doesn't perform any calculation in terms of collisions and 2D simulation of space.

GUI interacts with Simulator by method calls (for example for adding new robot).

![Image showing how 2DSimulationLib and QtSpecific dir are supposed to work](icp_img1.drawio.svg)

## Multiple threads
The **Simulator** can be run on either one or multiple threads at once, for this **SimulationCore** objects within the Simulator are used. SimulationCores are run on independent threads, waiting for the main thread (where Simulator is running) to signal them to run the simulation cycle. Number of robots to be simulated is balanced between all initialized SimulationCores. Multiple threads trying to access the same data at the same time could cause a problem with synchronization, however since the SimulationCores have a fixed set of robots to simulation worst case scenario "error" is read after read situation, which in this case shouldn't cause many problems.

## 2D Simulation library
For this project a custom 2D simulation library was written, Qt does contain a collision system, however, as an author I decided to not use it even at the cost of performance (since Qt collision system might be better optimized).

![Inheritance for 2D Simulation Library](icp_img2.drawio.svg)<br>
*class diagram of inheritance in 2DSimulationLib*

In the diagram of class inheritance, it can be seen that the class Rectangle inherits from the Circle class. This is so that detailed collision detection can be skipped, in case Rectangle is far away from Robot. This is because the function for an intersection between two circles is easier to compute than the intersection of Rectangle with Circle, or Rectangle with Rectangle. For this reason, Rectangles have a radius stored for faster collision detection.

![Inheritance for 2D Simulation Library](icp_img3.drawio.svg)<br>

Robots move forward in a given direction, this means that the simple circle intersection will not be enough. For this a Rectangle with Rectangle intersection is used which can decide more precisely whenever robot is colliding with obstacles or not. Rectangle collision is the same as a 4-sided convex polygon intersection because these rectangles can be rotated. This is done by checking if lines of two rectangles intersect, if yes, a collision occurred.
*(Source for line intersection: [https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/](https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/))*

Robot is however supposed to be have 