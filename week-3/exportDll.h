#ifndef MYDLL_H
#define MYDLL_H

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) void printArgument(const char *input);
__declspec(dllexport) void writeFile(const char *filename);

#ifdef __cplusplus
}
#endif

#endif // MYDLL_H
