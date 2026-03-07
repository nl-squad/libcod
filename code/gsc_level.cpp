#include "gsc_level.hpp"

#if COMPILE_LEVEL == 1

void gsc_level_getmovers()
{
	gentity_t *ent = g_entities;
	int i;

	stackPushArray();
	for ( i = 0; i < level.num_entities; i++, ent++ )
	{
		if ( ent->s.eType == ET_SCRIPTMOVER )
		{
			stackPushEntity(ent);
			stackPushArrayLast();
		}
	}
}

void gsc_level_getnumberofstaticmodels()
{
	stackPushInt(cm.numStaticModels);
}

void gsc_level_getplayersinrange()
{
	vec3_t origin;
	float maxDistSq = 0.0f;
	bool hasMaxDist = false;
	int filterTeam = -1;
	int traceContentMask = 0;
	int hasTraceCheck = 0;
	int args = Scr_GetNumParam();

	if ( args < 1 || Scr_GetType(0) != VAR_VECTOR )
	{
		stackError("gsc_level_getplayersinrange() requires origin");
		stackPushUndefined();
		return;
	}

	Scr_GetVector(0, origin);

	if ( args > 1 && Scr_GetType(1) != VAR_UNDEFINED )
	{
		if ( Scr_GetType(1) != VAR_FLOAT && Scr_GetType(1) != VAR_INTEGER )
		{
			stackError("gsc_level_getplayersinrange() max distance square must be a number");
			stackPushUndefined();
			return;
		}

		maxDistSq = Scr_GetFloat(1);
		hasMaxDist = true;
	}

	if ( hasMaxDist && maxDistSq < 0.0f )
	{
		stackError("gsc_level_getplayersinrange() max distance square must be >= 0");
		stackPushUndefined();
		return;
	}

	if ( args > 2 && Scr_GetType(2) != VAR_UNDEFINED )
	{
		if ( Scr_GetType(2) != VAR_INTEGER )
		{
			stackError("gsc_level_getplayersinrange() team filter must be an int");
			stackPushUndefined();
			return;
		}

		filterTeam = Scr_GetInt(2);
	}

	if ( args > 3 && Scr_GetType(3) != VAR_UNDEFINED )
	{
		if ( Scr_GetType(3) != VAR_INTEGER )
		{
			stackError("gsc_level_getplayersinrange() content mask must be an int");
			stackPushUndefined();
			return;
		}

		traceContentMask = Scr_GetInt(3);
		hasTraceCheck = 1;
	}

	stackPushArray();

	for ( int i = 0; i < level.maxclients; ++i )
	{
		gentity_t *player = &g_entities[i];
		gclient_t *client = player->client;
		vec3_t playerViewOrigin;
		float dx, dy, dz;
		float distSq;

		if ( !client || client->sess.connected != CON_CONNECTED || client->sess.sessionState != STATE_PLAYING )
			continue;

		if ( player->health <= 0 )
			continue;

		if ( filterTeam >= 0 && client->sess.cs.team != filterTeam )
			continue;

		G_GetPlayerViewOrigin(player, playerViewOrigin);

		dx = playerViewOrigin[0] - origin[0];
		dy = playerViewOrigin[1] - origin[1];
		dz = playerViewOrigin[2] - origin[2];
		distSq = dx * dx + dy * dy + dz * dz;

		if ( hasMaxDist && distSq > maxDistSq )
			continue;

		if ( hasTraceCheck && !G_LocationalTracePassed(origin, playerViewOrigin, player->s.number, traceContentMask) )
			continue;

		stackPushEntity(player);
		stackPushArrayLast();
	}
}

void gsc_level_getclosestplayerinrange()
{
	vec3_t origin;
	float maxDistSq = 0.0f;
	bool hasMaxDist = false;
	int filterTeam = -1;
	int traceContentMask = 0;
	int hasTraceCheck = 0;
	int args = Scr_GetNumParam();

	if ( args < 1 || Scr_GetType(0) != VAR_VECTOR )
	{
		stackError("gsc_level_getclosestplayerinrange() requires origin");
		stackPushUndefined();
		return;
	}

	Scr_GetVector(0, origin);

	if ( args > 1 && Scr_GetType(1) != VAR_UNDEFINED )
	{
		if ( Scr_GetType(1) != VAR_FLOAT && Scr_GetType(1) != VAR_INTEGER )
		{
			stackError("gsc_level_getclosestplayerinrange() max distance square must be a number");
			stackPushUndefined();
			return;
		}

		maxDistSq = Scr_GetFloat(1);
		hasMaxDist = true;
	}

	if ( hasMaxDist && maxDistSq < 0.0f )
	{
		stackError("gsc_level_getclosestplayerinrange() max distance square must be >= 0");
		stackPushUndefined();
		return;
	}

	if ( args > 2 && Scr_GetType(2) != VAR_UNDEFINED )
	{
		if ( Scr_GetType(2) != VAR_INTEGER )
		{
			stackError("gsc_level_getclosestplayerinrange() team filter must be an int");
			stackPushUndefined();
			return;
		}

		filterTeam = Scr_GetInt(2);
	}

	if ( args > 3 && Scr_GetType(3) != VAR_UNDEFINED )
	{
		if ( Scr_GetType(3) != VAR_INTEGER )
		{
			stackError("gsc_level_getclosestplayerinrange() content mask must be an int");
			stackPushUndefined();
			return;
		}

		traceContentMask = Scr_GetInt(3);
		hasTraceCheck = 1;
	}

	gentity_t *closestPlayer = NULL;
	float closestDistSq = hasMaxDist ? maxDistSq : 0.0f;
	bool foundPlayer = false;

	for ( int i = 0; i < level.maxclients; ++i )
	{
		gentity_t *player = &g_entities[i];
		gclient_t *client = player->client;
		float dx, dy, dz;
		float distSq;

		if ( !client || client->sess.connected != CON_CONNECTED || client->sess.sessionState != STATE_PLAYING )
			continue;

		if ( player->health <= 0 )
			continue;

		if ( filterTeam >= 0 && client->sess.cs.team != filterTeam )
			continue;

		dx = player->r.currentOrigin[0] - origin[0];
		dy = player->r.currentOrigin[1] - origin[1];
		dz = player->r.currentOrigin[2] - origin[2];
		distSq = dx * dx + dy * dy + dz * dz;

		if ( hasMaxDist && distSq > maxDistSq )
			continue;

		if ( hasTraceCheck && !G_LocationalTracePassed(origin, player->r.currentOrigin, player->s.number, traceContentMask) )
			continue;

		if ( !foundPlayer || distSq < closestDistSq )
		{
			closestPlayer = player;
			closestDistSq = distSq;
			foundPlayer = true;
		}
	}

	if ( !foundPlayer )
	{
		stackPushUndefined();
		return;
	}

	stackPushEntity(closestPlayer);
}

void gsc_level_getstaticmodelname()
{
	int index;

	if ( !stackGetParams("i", &index) )
	{
		stackError("gsc_level_getstaticmodelname() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( index < 0 || index >= (int)cm.numStaticModels )
	{
		stackError("gsc_level_getstaticmodelname() index is out of range");
		stackPushUndefined();
		return;
	}

	stackPushString(cm.staticModelList[index].xmodel->name);
}

void gsc_level_getstaticmodelorigin()
{
	int index;

	if ( !stackGetParams("i", &index) )
	{
		stackError("gsc_level_getstaticmodelorigin() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if ( index < 0 || index >= (int)cm.numStaticModels )
	{
		stackError("gsc_level_getstaticmodelorigin() index is out of range");
		stackPushUndefined();
		return;
	}

	stackPushVector(cm.staticModelList[index].origin);
}

void gsc_level_getentitycount()
{
	int inUseOnly;
	gentity_t *ent;
	int i;
	int entities = 0;

	if ( !stackGetParams("i", &inUseOnly) )
	{
		inUseOnly = 0;
	}

	if ( inUseOnly )
	{
		ent = g_entities;
		for ( i = 0; i < level.num_entities; i++, ent++ )
		{
			if ( ent->r.inuse != 0 )
			{
				entities++;
			}
		}
		stackPushInt(entities);
	}
	else
	{
		stackPushInt(level.num_entities);
	}
}

void gsc_level_getsavepersist()
{
	stackPushBool(level.savepersist);
}

void gsc_level_setsavepersist()
{
	int save;

	if ( !stackGetParams("i",  &save) )
	{
		stackError("gsc_utils_setsavepersist() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	level.savepersist = save;

	stackPushBool(qtrue);
}

void gsc_level_setnorthyaw()
{
	float fYaw;

	if ( !stackGetParams("f", &fYaw) )
	{
		stackError("gsc_level_setnorthyaw() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	int len = snprintf(NULL, 0, "%g", fYaw);
	if ( len <= 0 || len >= MAX_STRINGLENGTH )
	{
		stackError("gsc_level_setnorthyaw() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	char *szYaw = (char *)Z_MallocInternal(len + 1);
	snprintf(szYaw, len + 1, "%g", fYaw);
	SV_SetConfigstring(11, szYaw);
	Z_FreeInternal(szYaw);

	stackPushBool(qtrue);
}

#endif
