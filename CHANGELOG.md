# Weapons

- `slotId = player getCurrentSlotId()` - Returns current slot ID (0=none, 1=primary, 2=primaryb); returns `undefined` if the entity is not a player.
- `weaponId = player getCurrentWeaponId()` - Returns current weapon ID; returns `undefined` if the entity is not a player.
- `weaponId = player getWeaponIdInSlotId(slotId)` - Returns weapon ID in slot (may be `0` if empty); returns `undefined` if the entity is not a player, the arg is wrong type, or the slot ID is invalid.
- `slotId = player getSlotIdClipAmmo(slotId)` - Returns clip ammo for the weapon in the slot; returns `0` if slot is invalid, empty, or has no clip; returns `undefined` if the entity is not a player or the arg is wrong type.
- `ammo = player getSlotIdAmmo(slotId)` - Returns ammo for the weapon in the slot (clip-only weapons return clip ammo); returns `0` if slot is invalid, empty, or has no ammo type; returns `undefined` if the entity is not a player or the arg is wrong type.
- `clip = player getWeaponIdClipAmmo(weaponId)` - Returns clip ammo for weapon ID; returns `0` if the weapon has no clip; returns `undefined` if the entity is not a player, the arg is wrong type, or the weapon ID is invalid.
- `ammo = player getWeaponIdAmmo(weaponId)` - Returns ammo for weapon ID (clip-only weapons return clip ammo); returns `0` if the weapon has no ammo type; returns `undefined` if the entity is not a player, the arg is wrong type, or the weapon ID is invalid.
- `slotId = player setCurrentSlotId(slotId)` - Returns `true` on success, `false` if slot is invalid, player is not active, or slot has no weapon; returns `undefined` if the entity is not a player or the arg is wrong type.
- `isSuccess = player setCurrentWeaponId(weaponId)` - Returns `true` on success, `false` if weapon ID is invalid, player is not active, or weapon is not in primary/primaryb; returns `undefined` if the entity is not a player or the arg is wrong type.
- `isSuccess = player setWeaponIdInSlotId(slotId, weaponId)` - Returns `true` on success, `false` if slot is invalid, player is not active, or weapon ID is invalid (non-zero); returns `undefined` if the entity is not a player or args are wrong type.
- `isSuccess = player setSlotIdClipAmmo(slotId, clipAmmo)` - Returns `true` on success (clamps to valid range), `false` if slot is invalid, player is not active, slot is empty, or weapon has no clip; returns `undefined` if the entity is not a player or args are wrong type.
- `isSuccess = player setSlotIdAmmo(slotId, ammo)` - Returns `true` on success (clamps to valid range), `false` if slot is invalid, player is not active, slot is empty, or weapon has no ammo type; returns `undefined` if the entity is not a player or args are wrong type.
- `isSuccess = player setWeaponIdClipAmmo(weaponId, clipAmmo)` - Returns `true` on success (clamps to valid range), `false` if weapon ID is invalid, player is not active, or weapon has no clip; returns `undefined` if the entity is not a player or args are wrong type.
- `isSuccess = player setWeaponIdAmmo(weaponId, ammo)` - Returns `true` on success (clamps to valid range), `false` if weapon ID is invalid, player is not active, or weapon has no ammo type; returns `undefined` if the entity is not a player or args are wrong type.
- `isSuccess = player canReloadSlotId(slotId, [reloadThreshold])` - Returns `true`/`false` based on reload eligibility; returns `false` for invalid slot, empty slot, clip-only weapon, or missing clip; returns `undefined` if the entity is not a player, arg count is wrong, or args are wrong type.
- `weaponId = weaponNameToId(weaponName)` - Returns weapon ID (often `0` if not found); returns `undefined` if the arg is wrong type or missing.


# Graph

- `graphId = graphCreate([persist], [nodeCapacity])` - Returns new graph ID; `nodeCapacity` is NOT a hard limit.
- `isSuccess = graphRemove(graphId)` - Returns `true` on success; returns `undefined` if graph does not exist.
- `isSuccess = graphRemoveAll()` - Returns `true`.
- `nodeId = graphAddNode(graphId, origin, [type], [nodeId])` - Returns node ID; returns `undefined` if graph does not exist, node ID already exists, or (FSA) graph hit max nodes.
- `properties = graphGetNodeProperties(graphId, nodeId)` - Returns array with `origin` and `type`; returns `undefined` if graph or node does not exist.
- `isSuccess = graphRemoveNode(graphId, nodeId)` - Returns `true` if removed, `false` if node not found; returns `undefined` if graph does not exist.
- `isSuccess = graphAddEdge(graphId, fromNodeId, toNodeId, [type], [cost])` - Returns `true` on success; returns `undefined` if graph does not exist, start/end missing, edge already exists, start=end, or (FSA) start node hit max edges.
- `properties = graphGetEdgeProperties(graphId, fromNodeId, toNodeId)` - Returns array with `start`, `end`, `type`, `cost`; returns `undefined` if graph/start/end/edge not found.
- `isSuccess = graphRemoveEdge(graphId, fromNodeId, toNodeId)` - Returns `true` if removed, `false` if edge not found; returns `undefined` if graph/start/end not found or (FSA) start has no edges.
- `path = graphFindPath(graphId, startNodeId, endNodeId, [skipNodeIds], [skipNodeTypes], [skipEdgeTypes])` - Returns array of node IDs (includes `start`) if path found; returns `undefined` if graph is missing, nodes invalid, skip list invalid, or no path found.
- `isSuccess = graphPrecomputePathsToNode(graphId, nodeId, [skipNodeTypes], [skipEdgeTypes])` - Returns `true` on success; returns `undefined` if graph missing, no nodes, goal missing, or precompute failed.
- `nodeId = graphFindClosestNode(graphId, origin)` - Returns closest node ID; returns `undefined` if graph missing or has no nodes.
- `edgeInfo = graphFindClosestEdge(graphId, origin)` - Returns array with `start`, `end`, `origin` (closest point), `cost`, `type`; returns `undefined` if graph missing, has <2 nodes, or has no edges.
