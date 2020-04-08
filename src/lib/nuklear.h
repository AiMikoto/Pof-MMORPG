#ifndef LIB_NUKLEAR_H
#define LIB_NUKLEAR_H

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT

#include <stdarg.h>

#include "include/nuklear.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "include/nuklear_glfw_gl4.h"

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LIB_NUKLEAR_H
