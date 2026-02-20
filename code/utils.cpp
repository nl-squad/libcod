#include "utils.hpp"

qboolean IsNullVector(float *vec)
{
	return vec[0] == 0.0 && vec[1] == 0.0 && vec[2] == 0.0;
}

void VectorClampLength(float *vec, float max)
{
	float length;
	float factor;
	
	length = VectorLength(vec);
	if ( length > max )
	{
		factor = length / max;
		vec[0] /= factor;
		vec[1] /= factor;
		vec[2] /= factor;
	}
}

// https://stackoverflow.com/questions/67842444/modern-practice-to-compare-double-float-for-equality-in-modern-c
#define FLOAT_ALLOWANCE 0.0001 // 0.01% allowed deviation in float value comparison
bool FloatsApproximatelyEqual(float a, float b)
{
	if ( abs(a - b) < FLOAT_ALLOWANCE * std::max(abs(a), abs(b)) )
		return true;

	return false;
}

// https://stackoverflow.com/questions/2264760/efficient-way-of-finding-distance-between-two-3d-points
float Get3DDistance(float *a, float *b)
{
	return hypot(hypot(a[0] - b[0], a[1] - b[1]), a[2] - b[2]);
}

float Get3DDistanceSquared(float *a, float *b)
{
	float dx = b[0] - a[0];
	float dy = b[1] - a[1];
	float dz = b[2] - a[2];
	return dx * dx + dy * dy + dz * dz;
}

void ProjectPointOnLine(float *a, float *b, float *p, float *o)
{
	vec3_t ap;
	vec3_t ab;
	vec3_t scaledAb;
	float abDotAb;
	float apDotAb;

	VectorSubtract(p, a, ap);
	VectorSubtract(b, a, ab);
	abDotAb = DotProduct(ab, ab);
	apDotAb = DotProduct(ap, ab);
	VectorScale(ab, apDotAb / abDotAb, scaledAb);
	VectorAdd(a, scaledAb, o);
}

// Using boost::hash_combine
size_t HashCombine(size_t seed, float v)
{
	std::hash<float>hasher;
	seed ^= hasher(v) + 0x9E3779B9 + ( seed << 6 ) + ( seed >> 2 );
	return seed;
}

time_t sys_timeBase = 0;
uint64_t Sys_Milliseconds64(void)
{
	struct timeval tp;

	gettimeofday(&tp, NULL);

	if ( !sys_timeBase )
	{
		sys_timeBase = tp.tv_sec;
		return tp.tv_usec / 1000;
	}

	return (tp.tv_sec - sys_timeBase) * 1000 + tp.tv_usec / 1000;
}

int GetIndexForProtocol(int protocol)
{
	switch ( protocol )
	{
		case 115: return PROTOCOL_115;
		case 117: return PROTOCOL_117;
		case 118: return PROTOCOL_118;
		case 119: return PROTOCOL_119;
		default: return PROTOCOL_118;
	}
}

int GetProtocolForIndex(int index)
{
	switch ( index )
	{
		case PROTOCOL_115: return 115;
		case PROTOCOL_117: return 117;
		case PROTOCOL_118: return 118;
		case PROTOCOL_119: return 119;
		default: return 118;
	}
}

int GetProtocolFromShortVersion(const char *shortVersion)
{
	if ( strcmp(shortVersion, "1.0") == 0 )
		return 115; // 0x73
	else if ( strcmp(shortVersion, "1.2") == 0 )
		return 117; // 0x75
	else
		return 118; // 0x76, 1.3
	
	// Protocol 119 does not have a unique shortVersion
}

const char * GetShortVersionFromProtocol(int protocol)
{
	switch ( protocol )
	{
		case 115: return "1.0"; // 0x73
		case 117: return "1.2"; // 0x75
		default: return "1.3"; // 0x76, 118 and 0x77, 119
	}
}

qboolean IsLocalIPAddress(unsigned char *ip)
{
    // As defined by IANA, plus link-local and loopback addresses
    if ( ip[0] == 10                                                     // Class A: 10.0.0.0 – 10.255.255.255
         || ( ip[0] == 172 && ( ip[1] >= 16 && ip[1] <= 31 ) )           // Class B: 172.16.0.0 – 172.31.255.255
         || ( ip[0] == 192 && ip[1] == 168 )                             // Class C: 192.168.0.0 – 192.168.255.255
         || ( ip[0] == 169 && ip[1] == 254 )                             // Link local: 169.254.0.0 - 169.254.255.255
         || ( ip[0] == 127 && ip[1] == 0 && ip[2] == 0 && ip[3] == 1 ) ) // Localhost
    {
        return qtrue;
    }
    return qfalse;
}

weapSlot_t GetCurrentWeaponSlot(int clientNum)
{
	playerState_t *ps = SV_GameClientNum(clientNum);

	if ( ps->weapon == 0 )
	{
		return WEAPSLOT_NONE;
	} 
	else if ( (byte)ps->weapon == ps->weaponslots[1] )
	{
		return WEAPSLOT_PRIMARY;
	}
	else if ( (byte)ps->weapon == ps->weaponslots[2] )
	{
		return WEAPSLOT_PRIMARYB;
	}
	else
	{
		return WEAPSLOT_NONE;
	}
}

int GetAnimationId(const char *string)
{
	int i;
	int hash;
	animation_t *anim;

	hash = BG_StringHashValue(string);

	for ( i = 0; i < globalScriptData->numAnimations; i++ )
	{
		anim = &globalScriptData->animations[i];
		if ( ( hash == anim->nameHash ) && !I_stricmp(string, anim->name) )
		{
			return i;
		}
	}

	return -1;
}

qboolean IsValidWeaponId(int id)
{
	int weapons = BG_GetNumWeapons();

	if ( id > weapons || id <= 0 || weapons == 0 )
		return qfalse;

	return qtrue;
}

static inline uint32_t Sha256Rotr32(uint32_t x, uint32_t n)
{
	return ( x >> n ) | ( x << ( 32 - n ) );
}

static inline uint32_t Sha256Ch(uint32_t x, uint32_t y, uint32_t z)
{
	return ( x & y ) ^ ( ~x & z );
}

static inline uint32_t Sha256Maj(uint32_t x, uint32_t y, uint32_t z)
{
	return ( x & y ) ^ ( x & z ) ^ ( y & z );
}

static inline uint32_t Sha256BigSigma0(uint32_t x)
{
	return Sha256Rotr32(x, 2) ^ Sha256Rotr32(x, 13) ^ Sha256Rotr32(x, 22);
}

static inline uint32_t Sha256BigSigma1(uint32_t x)
{
	return Sha256Rotr32(x, 6) ^ Sha256Rotr32(x, 11) ^ Sha256Rotr32(x, 25);
}

static inline uint32_t Sha256SmallSigma0(uint32_t x)
{
	return Sha256Rotr32(x, 7) ^ Sha256Rotr32(x, 18) ^ ( x >> 3 );
}

static inline uint32_t Sha256SmallSigma1(uint32_t x)
{
	return Sha256Rotr32(x, 17) ^ Sha256Rotr32(x, 19) ^ ( x >> 10 );
}

static const uint32_t kSha256RoundConstants[64] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static void Sha256CompressBlock(const uint8_t block[64], uint32_t state[8])
{
	uint32_t w[64];

	for ( int i = 0; i < 16; ++i )
	{
		const int j = i * 4;
		w[i] = ( (uint32_t)block[j] << 24 ) | ( (uint32_t)block[j + 1] << 16 ) | ( (uint32_t)block[j + 2] << 8 ) | (uint32_t)block[j + 3];
	}

	for ( int i = 16; i < 64; ++i )
	{
		w[i] = Sha256SmallSigma1(w[i - 2]) + w[i - 7] + Sha256SmallSigma0(w[i - 15]) + w[i - 16];
	}

	uint32_t a = state[0];
	uint32_t b = state[1];
	uint32_t c = state[2];
	uint32_t d = state[3];
	uint32_t e = state[4];
	uint32_t f = state[5];
	uint32_t g = state[6];
	uint32_t h = state[7];

	for ( int i = 0; i < 64; ++i )
	{
		const uint32_t t1 = h + Sha256BigSigma1(e) + Sha256Ch(e, f, g) + kSha256RoundConstants[i] + w[i];
		const uint32_t t2 = Sha256BigSigma0(a) + Sha256Maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
	state[5] += f;
	state[6] += g;
	state[7] += h;
}

void Sha256Bytes(const uint8_t *data, size_t len, uint8_t digest[32])
{
	uint32_t state[8] =
	{
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};

	uint8_t block[64];
	size_t offset = 0;

	while ( len - offset >= 64 )
	{
		Sha256CompressBlock(data + offset, state);
		offset += 64;
	}

	const size_t rem = len - offset;
	for ( size_t i = 0; i < rem; ++i )
	{
		block[i] = data[offset + i];
	}
	block[rem] = 0x80;

	if ( rem + 1 > 56 )
	{
		for ( size_t i = rem + 1; i < 64; ++i )
		{
			block[i] = 0;
		}
		Sha256CompressBlock(block, state);
		for ( size_t i = 0; i < 56; ++i )
		{
			block[i] = 0;
		}
	}
	else
	{
		for ( size_t i = rem + 1; i < 56; ++i )
		{
			block[i] = 0;
		}
	}

	const uint64_t bitLen = (uint64_t)len * 8;
	for ( int i = 0; i < 8; ++i )
	{
		block[56 + i] = (uint8_t)( bitLen >> ( 56 - ( i * 8 ) ) );
	}

	Sha256CompressBlock(block, state);

	for ( int i = 0; i < 8; ++i )
	{
		digest[i * 4] = (uint8_t)( state[i] >> 24 );
		digest[i * 4 + 1] = (uint8_t)( state[i] >> 16 );
		digest[i * 4 + 2] = (uint8_t)( state[i] >> 8 );
		digest[i * 4 + 3] = (uint8_t)state[i];
	}
}

void Sha256Hex(const uint8_t *data, size_t len, char hex[65])
{
	static const char lut[] = "0123456789abcdef";
	uint8_t digest[32];
	Sha256Bytes(data, len, digest);

	for ( int i = 0; i < 32; ++i )
	{
		hex[i * 2] = lut[digest[i] >> 4];
		hex[i * 2 + 1] = lut[digest[i] & 0x0F];
	}

	hex[64] = '\0';
}
