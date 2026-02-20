#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include "gsc.hpp"
#include <stdint.h>

qboolean IsNullVector(float *vec);
void VectorClampLength(float *vec, float max);
bool FloatsApproximatelyEqual(float a, float b);
float Get3DDistance(float *a, float *b);
float Get3DDistanceSquared(float *a, float *b);
void ProjectPointOnLine(float *a, float *b, float *p, float *o);

size_t HashCombine(size_t seed, float v);

void Sha256Bytes(const uint8_t *data, size_t len, uint8_t digest[32]);
void Sha256Hex(const uint8_t *data, size_t len, char hex[65]);

uint64_t Sys_Milliseconds64(void);

int GetIndexForProtocol(int protocol);
int GetProtocolForIndex(int index);
int GetProtocolFromShortVersion(const char *shortVersion);
const char * GetShortVersionFromProtocol(int protocol);

qboolean IsLocalIPAddress(unsigned char *ip);

#ifndef Q_vsnprintf
int Q_vsnprintf(char *s0, size_t size, const char *fmt, va_list args);
#endif

#ifndef strcmp_constant_time
int strcmp_constant_time(const char *s1, const char *s2);
#endif

weapSlot_t GetCurrentWeaponSlot(int clientNum);

int GetAnimationId(const char *string);

qboolean IsValidWeaponId(int id);

#endif
