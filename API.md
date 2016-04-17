# API

**TankBattleCommand**  

Byte Offset | Field                              | Description
------------|------------------------------------|-------------------------------
0           | TankBattleMessage msg              | The category or type of the message.
4           | TankMovementOptions tankMove       | Movement direction command for the treads.
8           | CannonMovementOptions cannonMove   | Movement direction command for the cannon.
12          | int fireWish                       | Is the tank attempting to fire?
16          | int messageLength                  | Number of bytes for this message. (Currently not used)

**TankBattleTacticalInfo**

Byte Offset | Field                              | Description
------------|------------------------------------|-------------------------------
0           | int playerID                       | Player ID of the tank being identified.
4           | int inSight                        | Is the enemy tank in sight of the player's tank?
8           | Vector3 lastKnownPosition          | The last known position of the enemy tank.
20          | Vector3 lastKnownDirection         | The last known direction of the enemy tank, relative to the player's tank.

**Conditions**
1. A tank is registered for the first time any information is known for that
particular tank.
2. The last known position for a tank is changed when one of the following is met:
  - The enemy tank is in the view of the tank cannon.
  - The enemy tank has fired.
3. The last known direction for a tank is changed when one of the following is met:
  - The enemy tank is in the view of the tank cannon.
  - The enemy tank is moving (not turning) within a 20m radius.
  - The enemy tank has fired.

**TankBattleStateInfo**  

Byte Offset | Field                              | Description
------------|------------------------------------|-------------------------------
0           | int messageLength                  | Number of bytes for this message. (not implemented)
4           | int playerID                       | Player ID of the tank receiving input.
8           | float currentHealth                | The current health of this tank.
12          | Vector3 position                   | The current position of this tank in world-space.
24          | Vector3 forward                    | The current forward of this tank in world-space.
36          | Vector3 cannonForward              | The current forward of this tank's cannon in world-space.
48          | int canFire                        | Is the tank currently allowed to fire?
52          | int tacticalCount                  | How many tanks have been recorded into the reconnaissance log?
56          | TankTacticalInfo tacticalData[3]   | An array of tactical info.

**tankNet**

### init

```C++
bool init(int port, char * address="127.0.0.1");
```

### update

```C++
bool update(double timeout=0.0);
```

### send

```C++
void send(TankBattleCommand output);
```

### recieve

```C++
TankBattleStateData * recieve();
```

### isConnected

```C++
bool isConnected();
```

### isProvisioned

```C++
bool isProvisioned();
```

### term

```C++
void term();
```
