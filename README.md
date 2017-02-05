# SPAAAAAACE

A simple 2D infinite space shooter made using C++ with SDL2.

The player controls a ship and has to attack and destroy enemy ships to increase the score. Player can move the ship using WASD, and move the mouse to rotate. Clicking left mouse button will fire a laser in the direction the player's ship is facing. Colliding with an enemy ship will push the enemy ship backwards. Enemy's HP will be decreased when the laser hits it. Enemy ships will respawn randomly after being destroyed, but not in the player's current area.

Enemies will be harder to destroy as the player destroys more enemies. However, they won't be impossible to destroy. Enemies will detect and attack the player if they get too close. Player's HP will regenerate after taking damage.

Game ends when player’s HP goes down to zero. 
