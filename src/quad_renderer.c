#define GLAD_GL_IMPLEMENTATION
#include "glad.h"
#include "quad_renderer.h"

#define MAX_QUADS 1024

typedef struct {
    float       width;
    float       height;
    float       aspect;
    uint32_t    count;
} RendererData;

typedef struct {
    float x;
    float y;
    float width;
    float height;
    uint32_t color;
} PosScaleColor;

static RendererData mRendererData;
static uint32_t mVbo;
static uint32_t mVao;
static uint32_t mShader;
static PosScaleColor mData[MAX_QUADS];

static float ndcx(float x) {
    return (x * 2 / ((float)mRendererData.width) - 1);
}

static float ndcy(float y) {
    return (y * 2 / ((float)mRendererData.height) - 1) * -1;
}

static void convertScreenToNDC(PosScaleColor *item) {
    PosScaleColor orig = *item;
    
    float x = ndcx(orig.x + orig.width * 0.5f);
    float y = ndcy(orig.y + orig.height * 0.5f);
    float sx = orig.width   / (float)mRendererData.width;
    float sy = orig.height  / (float)mRendererData.height;
    *item = (PosScaleColor) {
        x,
        y,
        sx,
        sy,
        orig.color
    };
}

static char* readFile(char *filename)
{
   char *buffer = NULL;
   int string_size, read_size;
   FILE *handler = fopen(filename, "r");

   if (handler)
   {
       // Seek the last byte of the file
       fseek(handler, 0, SEEK_END);
       // Offset from the first to the last byte, or in other words, filesize
       string_size = ftell(handler);
       // go back to the start of the file
       rewind(handler);

       // Allocate a string that can hold it all
       buffer = (char*) malloc(sizeof(char) * (string_size + 1) );

       // Read it all in one operation
       read_size = fread(buffer, sizeof(char), string_size, handler);

       // fread doesn't set it so put a \0 in the last position
       // and buffer is now officially a string
       buffer[string_size] = '\0';

       if (string_size != read_size)
       {
           // Something went wrong, throw away the memory and set
           // the buffer to NULL
           free(buffer);
           buffer = NULL;
       }

       // Always remember to close the file.
       fclose(handler);
    }

    return buffer;
}

static uint32_t compileAndAttachShader(const uint32_t prg, const uint32_t shaderType, const char *src) {
    uint32_t res = glCreateShader(shaderType);
    glShaderSource(res, 1, &src, NULL);
    glCompileShader(res);
    int success = 0;
    glGetShaderiv(res, GL_COMPILE_STATUS, &success);
    if(!success)  {
        exit(EXIT_FAILURE);
    }
    glAttachShader(prg, res);
    return res;
}

bool leoQuadRendererInit(void) {
    const uint32_t ver = gladLoaderLoadGL();
    const uint32_t major = GLAD_VERSION_MAJOR(ver);
    const uint32_t minor = GLAD_VERSION_MINOR(ver);
    bool res = major >= 3 && minor >= 3;

    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PosScaleColor) * MAX_QUADS, NULL, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(PosScaleColor), (void*)(sizeof(float) * 0));
    glEnableVertexAttribArray(0);
    glVertexAttribDivisor(0, 1);

    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(PosScaleColor), (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, true, sizeof(PosScaleColor), (void*)(sizeof(float) * 4));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    const char* vsSrc = readFile("quads.vs.glsl");
    const char* fsSrc = readFile("quads.fs.glsl");

    mShader = glCreateProgram();
    uint32_t shaders[2] = {
        [0] = compileAndAttachShader(mShader, GL_VERTEX_SHADER,     vsSrc),
        [1] = compileAndAttachShader(mShader, GL_FRAGMENT_SHADER,   fsSrc)
    };

    glLinkProgram(mShader);
    glDetachShader(mShader, shaders[0]);
    glDetachShader(mShader, shaders[1]);
    glDeleteShader(shaders[0]);
    glDeleteShader(shaders[1]);

    free(vsSrc);
    free(fsSrc);

    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    return res;
}

void leoQuadRendererDispose(void) {
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mVbo);
    glDeleteShader(mShader);



    gladLoaderUnloadGL();
}

void leoQuadRendererResize(const uint32_t width, const uint32_t height) {
    mRendererData.width =   (float)width;
    mRendererData.height =  (float)height;
    mRendererData.aspect = mRendererData.width / mRendererData.height;
    glViewport(0, 0, width, height);
}

void leoQuadRendererDrawQuad(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height, const uint32_t color) {

    if(mRendererData.count == MAX_QUADS) {
        leoQuadRendererEnd();
    }

    PosScaleColor *item = &mData[mRendererData.count];
    item->x = x;
    item->y = y;
    item->width = width;
    item->height = height;
    item->color = color;
    convertScreenToNDC(item);
    mRendererData.count++;
}

void leoQuadRendererBegin(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(mVao);
    glUseProgram(mShader);
}    

void leoQuadRendererEnd(void) {
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(PosScaleColor) * mRendererData.count, mData);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, mRendererData.count);
    mRendererData.count = 0u;
}

