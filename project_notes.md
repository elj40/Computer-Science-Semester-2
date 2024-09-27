### Hives:
- beehives, wasp hive, flower
- all fixed, cannot share a location

### Bees:
- Generic: 
	- Two states: wander and go to target
	- Can carry pollen
- Honey:
	- Two roles: scouts of forager
	- Scout: 
		- double perception
		- Two states: wander and go to target
        - Stores location
    - Forager: 
        - dormant until hearing about location
        - 3 states: dormant (stay at hive), go to target, search (look around empty flower for 5 moves or pollen)
        - Stores location and pollen
- Desert:
    - Same as generic and...
    - Remembers path to previous flower and will use that back and forth until no more pollen
    - Three states? wander, go to target, follow path?
    - Stores path, location and pollen

### Wasps:
- Moves randomly on map

### Flowers:
- Fixed
- Stores pollen (string or float)

## Movement:
- can move in all directions with 45 degree step
- entity states can change if encountering another object
- magnitude can be 0 to speed of entity (inclusive)
- No interacting with something you dont land on
- if flower is in range of a bees perception:
    - Honey-Scout bees return home with location of flower
    - Other bees move toward flower if in that state
- move diagonally first and then horz/vert

## Action:
- if bee lands on flower and doesnt already have pollen, collect one pollen 
- if 2+ bees land on flower:
    - enough pollen for all: each gets one
    - not enough: no pollen collected (pollen destroyed?), bees continue as if not on flower
- if bee lands on wasp hive, it dies
- if bee lands at same place as wasp, it dies
- bees that die with pollen destroy pollen
- if bee lands on flower with wasp: bees die and flower keeps its pollen
- if 2+ bee scouts inform of a flower at same time, foragers go to closest one

### Action ideas
- have function that checks what objects are in the block it is currently in

# Design Ideas
- OOP
- Must be able to display board in a 2D layout
    - 2D char array
    - update each iteration
- Must be able to remove pollen from the flower
    - remove from flower list (pop)
- Must be able to add pollen to hive
    - add to end of hive pollen
- Must be able to know all the objects that are at a location
    - 3d array (how big)?
    - linked list (how)?
    - looping through all object and getting list (very slow)?
- Must be able to remove bees from world
    - dead variable?
    - remove from hive's list of bees?
- Must be able to ignore flowers if they have no pollen
    - check pollen count when searching?
    - remove visibility from world?
    - add to ignore list?
- Must be able to have each agent act independently
    - OOP


## Pseudocode

`map` can be stored as a 3d array, where for every x and y it stores a string where each char is type of object
Top level:
```
read inputs
loop for every iteration:
    loop for every entity:
        move entity
        do entity action
    display world
end loop
calculate answer
```

move entitiy:
```
use current state to to move according to state rules
place object at next position
update map?
```

do entity action:
```
check what objects are at current location and for objects around location (e.g. flowers)
update state for next iteration
remove any necessary objects
```

display world:
```
loop through map
if string at that location contains a static object, display it
else if there is only one entity display it
else if there are multiple entities display 'm'
else display ' '
```
