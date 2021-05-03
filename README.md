# Tunnel_Man
<img width="570" alt="tman" src="https://user-images.githubusercontent.com/66845657/116927114-17949600-ac10-11eb-85cc-81002a9e8695.png">

Fundamentally, this is a video game is composed of a bunch of objects (i.e., class variables); in 
Tunnelman, those objects include the Tunnelman, Regular Protesters, Hardcore
Protesters, Goodies (e.g., Sonar Kits, Gold Nuggets, Water, Barrels of oil) that can be
picked up, Boulders, Earth, and Squirts (water squirted by the Tunnelman at Protesters). 
Let’s call these objects “actors,” since each object is an actor in our video game. Each 
actor has its own x, y location in the oil field, its own internal state (e.g., the location of 
the Protester, what direction the Protester is moving, etc.) and its own special algorithms 
that control its actions in the game based on its own state and the state of the other objects 
in the oil field. In the case of the Tunnelman, the algorithm that controls the Tunnelman’s
actor object is the player’s own brain and hand, and the keyboard! In the case of other 
actors (e.g., Protesters), each object has an internal autonomous algorithm and state that 
dictates how the object behaves in the game world.
