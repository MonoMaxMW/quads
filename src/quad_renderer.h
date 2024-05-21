#ifndef BAD45E83_AAFB_4588_BE58_DDB061AC1772
#define BAD45E83_AAFB_4588_BE58_DDB061AC1772

#include <stdint.h>
#include <stdbool.h>

bool leoQuadRendererInit(void);
void leoQuadRendererDispose(void);
void leoQuadRendererEnd(void);
void leoQuadRendererBegin(void);
void leoQuadRendererResize(const uint32_t width, const uint32_t height);
void leoQuadRendererDrawQuad(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const uint32_t color);

#endif /* BAD45E83_AAFB_4588_BE58_DDB061AC1772 */
