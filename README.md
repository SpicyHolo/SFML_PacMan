# SFML Poli Pac-Man

My attempt of re-creating the original 1980 Pac-Man game in C++ using SFML library.


## Compilation

I'm using Pre-compiled headers, so when .cpp files are not in the main direcotry (like entites/entity.cpp)
You have to change headers directory for those files in entities/ to "../stdafx.cpp" instead of "stdafx.cpp"

## Ghost AI
The ghosts are trying to get to their target.
When entering a new tile they make their decision based on the distance from their target tile.
They choose the tile that minimizes the distance from the target (if distance is the same the priority is : UP, DOWN, LEFT, RIGHT)
They never choose the tile directly behind them (there's an exception from this rule)

#In Scatter mode each ghosts targets a different corner of the maze (the tile they target is outside the maze)

#In Chase mode each ghost has a different way to choose it's target:
  - Blinky: Targets a tile directly on top of Pac-Man
  - Pinky: Targets a tile 4 tiles ahead of Pac-Man (due to error in original Pac-Man when facing up the target tile is 4 tiles up and 4 tiles to the left)
  - Inky: Creates a vector between Blinky and a point 2 tiles ahead of Pac-Man (the same error when facing up occurs) and rotates it by 180 degress
    The point at the end of that vector is it's target tile
  - Clyde: When is 8 or more tiles away from pacman it targets Pac-Man directly, when closer it targets it's Scatter Mode target
  
#In frightened mode the ghosts first turn away from their current direction and choose the next tile randomly (excluding the tile behind them)

#source: https://www.youtube.com/watch?v=ataGotQ7ir8 (Pac-Man Ghost AI Explained)

## Credits
I used the graphics from this repository
I used similar map loading system and based my Pac-Man controls on that repository a bit
https://github.com/HexagonNico/Pac-Man

#Font: Arcadepix Plus by Jimmy Campbell based on Arcadepix by Reekee of Dimenzioned


## TODO

#Gameplay
- Add multiple levels
- Difficulty scaling with levels
- Ghosts actually spawning inside the base
- More points for consecutive ghost killing in the same frightened mode
- Spawning Fruits

#Graphics and sounds
- Sounds :)
- Death animation
- Flickering of ghosts to white just before changing back from STATE::FRIGHTENED

#Misc
- Game Menu
- Keeping High Score
- Global Player Ranking with MongoDB

#BUGS
- When ghosts entering MODE::FRIGHTENED they turn 180 degress
	When a wall is behind them during that state, they can glitch into a wall.
	//Temporary fix: When entering into the frightened stage - check if the tile behind them isSolid(), if true don't turn around.
  
  
