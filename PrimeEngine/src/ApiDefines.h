#pragma once

#ifdef PRIME_EXPORT
#define PRIME_API __declspec(dllexport)
#else
#define PRIME_API __declspec(dllimport)
#endif // PRIME_EXPORT
