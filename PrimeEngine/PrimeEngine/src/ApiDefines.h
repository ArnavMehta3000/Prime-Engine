#pragma once

#ifdef PRIME_EXPORTS
#define PRIME_API __declspec(dllexport)
#else
#define PRIME_API _declspec(dllimport)
#endif // PRIME_EXPORTS
