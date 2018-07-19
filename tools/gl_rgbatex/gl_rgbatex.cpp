
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/resource.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <utils/Timers.h>
#include <cutils/trace.h>

#include <ui/FramebufferNativeWindow.h>
#include <ui/GraphicBuffer.h>
#include <../tests/include/EGLUtils.h>
#include "gl_rgbatex.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <cutils/properties.h>
#include <GraphicsJNI.h>
#include <linux/fb.h>
#include <ui/PixelFormat.h>

#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <utils/String8.h>

#include "gralloc_priv.h"

#define USE_ARRAY   1
#define EGL_VBO_IDS 2

using namespace android;

sp<SurfaceComposerClient> gSurfaceComposerClient = NULL;
sp<SurfaceControl> gSurfaceControl;
sp<Surface> gSurface;

int mSurfaceWidth;
int mSurfaceHeight;
EGLDisplay mDisplay = NULL;
EGLDisplay mContext;
EGLDisplay mSurface;
const EGLint attribs[] = { EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_DEPTH_SIZE, 0,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE };
const EGLint mcontext_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

static EGLint numConfigs;
static EGLConfig config;

static int rgbTexWidth = 0;
static int rgbTexHeight = 0;
static int rgbTexUsage = GraphicBuffer::USAGE_HW_TEXTURE | GraphicBuffer::USAGE_SW_WRITE_OFTEN;
//        | GraphicBuffer::USAGE_SW_READ_OFTEN | GraphicBuffer::USAGE_SOFTWARE_MASK;

/* GRALLOC_USAGE_SW_WRITE_OFTEN | GRALLOC_USAGE_SW_READ_OFTEN */
static int rgbBufferLockFlag = GRALLOC_USAGE_SW_WRITE_OFTEN;

static int rgbTexFormat = HAL_PIXEL_FORMAT_RGBA_8888;

static sp<GraphicBuffer> rgbTexBuffer = NULL;
static GLuint rgbTex;
static EGLClientBuffer clientBuffer;
static EGLImageKHR displayImage = NULL;

/* shader program */
GLuint gl_sh_program = 0;
GLint gvPositionIndex;
GLint texCoordsIndex;

static pthread_mutex_t glsl_clip_gpu_mutex;
static int glsl_has_inited = 0;

/* redirect egl clip */
static GLfloat *gPositionVertices = NULL;
static GLfloat *gTexCoordVertices = NULL;
static int gl_vertices_cur_num = 0;

static int gl_vertices_change = 0;          /* if vertices attribute change, it has to reload vbos */

static GLubyte *gIndices = NULL;
static int gl_indices_num = 0;
static GLubyte gIndicesDefault[6];

static GLuint gl_vboIds[EGL_VBO_IDS];

static int gl_attribute_rects_num = 0;
static int gl_attribute_rect_default = 1;

/* systrace start */

static uint64_t atrace_tag = ATRACE_TAG_GRAPHICS;
int atrace_marker_fd = -1;

/****************************** systrace调试 start ***************************************/
void trace_init()
{
    atrace_marker_fd = open("/sys/kernel/debug/tracing/trace_marker", O_WRONLY);
    if (atrace_marker_fd == -1) { /* do error handling */
        ALOGE("atrace_marker_fd open failed.");
    }
}

void sys_trace_tag(uint64_t tag)
{
    atrace_tag = tag;
}

void sys_trace_begin(const char *name)
{
    atrace_begin(atrace_tag, name);
}

void sys_trace_end()
{
    atrace_end(atrace_tag);
}
/****************************** systrace调试 end***************************************/

static void printGLString(const char *name, GLenum s)
{
    const char *v = (const char *) glGetString(s);
    fprintf(stderr, "GL %s = %s\n", name, v);
}

static void checkEglError(const char* op, EGLBoolean returnVal = EGL_TRUE)
{
    if (returnVal != EGL_TRUE) {
        fprintf(stderr, "%s() returned %d\n", op, returnVal);
    }

    for (EGLint error = eglGetError(); error != EGL_SUCCESS; error = eglGetError()) {
        fprintf(stderr, "after %s() eglError %s (0x%x)\n", op, EGLUtils::strerror(error), error);
    }
}

static void checkGlError(const char* op)
{
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        fprintf(stderr, "after %s() glError (0x%x)\n", op, error);
    }
}

/* 设置EGL的显示位置 */
bool setupGraphics(int w, int h)
{
    glViewport(0, 0, w, h);
    checkGlError("glViewport");
    return true;
}

/* 获取字节对齐 */
int align(int x, int a)
{
    return (x + (a - 1)) & (~(a - 1));
}

bool setRgbDataToGralloc(char *data, int size)
{
    char* buf = NULL;
    status_t err = rgbTexBuffer->lock(rgbBufferLockFlag, (void**) (&buf));
    if (err != 0) {
        fprintf(stderr, "rgbTexBuffer->lock(...) failed: %d\n", err);
        return false;
    }
    memcpy(buf, data, size);

    err = rgbTexBuffer->unlock();
    if (err != 0) {
        fprintf(stderr, "rgbTexBuffer->unlock() failed: %d\n", err);
        return false;
    }

    return true;
}

/**
 * setRgbSurfaceLayer - 设置EGL层次以及设置egl是否隐藏显示
 * layer:层次[0 - INT_MAX]
 * show: 0, 1
 */
void setRgbSurfaceLayer(int layer, int show)
{
    SurfaceComposerClient::openGlobalTransaction();
    if (layer > 0 && layer < INT_MAX) {
        gSurfaceControl->setLayer(layer);
    }
    if (show > 0) {
        gSurfaceControl->show();
    } else {
        gSurfaceControl->hide();
    }
    SurfaceComposerClient::closeGlobalTransaction();
}

/* 打印EGL的配置信息 */
void printEGLConfiguration(EGLDisplay dpy, EGLConfig config)
{

#define X(VAL) {VAL, #VAL}
    struct {
        EGLint attribute;
        const char* name;
    } names[] = { X(EGL_BUFFER_SIZE), X(EGL_ALPHA_SIZE), X(EGL_BLUE_SIZE), X(EGL_GREEN_SIZE),
            X(EGL_RED_SIZE), X(EGL_DEPTH_SIZE), X(EGL_STENCIL_SIZE), X(EGL_CONFIG_CAVEAT),
            X(EGL_CONFIG_ID), X(EGL_LEVEL), X(EGL_MAX_PBUFFER_HEIGHT),
            X(EGL_MAX_PBUFFER_PIXELS), X(EGL_MAX_PBUFFER_WIDTH), X(EGL_NATIVE_RENDERABLE),
            X(EGL_NATIVE_VISUAL_ID), X(EGL_NATIVE_VISUAL_TYPE), X(EGL_SAMPLES),
            X(EGL_SAMPLE_BUFFERS), X(EGL_SURFACE_TYPE), X(EGL_TRANSPARENT_TYPE),
            X(EGL_TRANSPARENT_RED_VALUE), X(EGL_TRANSPARENT_GREEN_VALUE),
            X(EGL_TRANSPARENT_BLUE_VALUE), X(EGL_BIND_TO_TEXTURE_RGB),
            X(EGL_BIND_TO_TEXTURE_RGBA), X(EGL_MIN_SWAP_INTERVAL), X(EGL_MAX_SWAP_INTERVAL),
            X(EGL_LUMINANCE_SIZE), X(EGL_ALPHA_MASK_SIZE), X(EGL_COLOR_BUFFER_TYPE),
            X(EGL_RENDERABLE_TYPE), X(EGL_CONFORMANT), };
#undef X

    for (size_t j = 0; j < sizeof(names) / sizeof(names[0]); j++) {
        EGLint value = -1;
        EGLint returnVal = eglGetConfigAttrib(dpy, config, names[j].attribute, &value);
        EGLint error = eglGetError();
        if (returnVal && error == EGL_SUCCESS) {
            fprintf(stderr, " %s: ", names[j].name);
            fprintf(stderr, "%d (0x%x)\n", value, value);
        }
    }
    fprintf(stderr, "\n");
}

/* 设置创建界面的格式 */
void gl_init_rgb_format(int pixelFormat, int canvasWidth, int canvasHeight)
{
    rgbTexWidth = canvasWidth;
    rgbTexHeight = canvasHeight;

    switch (pixelFormat) {
    /* "linear" color pixel formats */
    case CANVAS_RGBA_8888:
        rgbTexFormat = HAL_PIXEL_FORMAT_RGBA_8888;
        break;
    case CANVAS_RGBX_8888:
        /**
         * RGBX means, that the pixel format still has an alpha channel,
         * but it is ignored, and is always set to 255
         */
        rgbTexFormat = HAL_PIXEL_FORMAT_RGBX_8888;
        break;
    case CANVAS_RGB_888:
        rgbTexFormat = HAL_PIXEL_FORMAT_RGB_888;
        break;
    case CANVAS_RGB_565:
        rgbTexFormat = HAL_PIXEL_FORMAT_RGB_565;
        break;
    case CANVAS_BGRA_8888:
        rgbTexFormat = HAL_PIXEL_FORMAT_BGRA_8888;
        break;

    /* software decoders format */
    case CANVAS_YV12:
        rgbTexFormat = HAL_PIXEL_FORMAT_YV12;
        break;
    default:
        checkEglError("unknow format.");
        rgbTexFormat = HAL_PIXEL_FORMAT_RGBA_8888;
        break;
    }
}

/* 坐标shder */
static const char gVertexShader[] =
        "attribute vec4 vPosition;\n"
        "attribute vec3 texCoords;\n"
        "varying vec2 outTexCoords;\n"
        "varying float outDraw;\n"
        "void main() {\n"
        "  outTexCoords = texCoords.xy;\n"
        "  outDraw = texCoords.z;\n"
        "  gl_Position = vPosition;\n"
        "}\n";

/* 图像显示shder */
static const char gFragmentShader[] =
        "#extension GL_OES_EGL_image_external : require\n"
        "precision lowp float;\n"
        "uniform samplerExternalOES rgbaTexSampler;\n"
        "varying vec2 outTexCoords;\n"
        "varying float outDraw;\n"
        "void main() {\n"
        "  if (outDraw < 0.0) {gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);}\n"
        "  else {gl_FragColor = texture2D(rgbaTexSampler, outTexCoords);}\n"
        "}\n";

/* 定义shader结构大小 */
#define GL_STRUCT_SIZE_VPOSITION    (8)
#define GL_STRUCT_SIZE_TEX_COORD    (12)

/* 定义结构偏移大小 */
#define GL_VPOSITION_STRIDE (2 * sizeof(GLfloat))
#define GL_TEXCOORD_STRIDE (3 * sizeof(GLfloat))

const GLfloat gvPositionDefault[] = {
    //vPosition:x, y
    -1.0f, -1.0f,   // left bottom
    1.0f, -1.0f,    // right bottom
    1.0f, 1.0f,     // right top
    -1.0f, 1.0f,    // left top
};

const GLfloat gTexCoordDefault[] = {
    //texCoords:s, t
    0.0f, 1.0f, 1.0f,      // left top
    1.0f, 1.0f, 1.0f,      // right top
    1.0f, 0.0f, 1.0f,      // right bottom
    0.0f, 0.0f, 1.0f,      // left bottom
};

/* 申请shader内存 */
static void gl_vertices_alloc_memory(int clip_num)
{
    int i;

    pthread_mutex_lock(&glsl_clip_gpu_mutex);
    if (gl_vertices_cur_num == clip_num) {
        pthread_mutex_unlock(&glsl_clip_gpu_mutex);
        return;
    }

    if ((gl_vertices_cur_num == EGL_CLIP_SIZE_DEFAULT) && (clip_num <= EGL_CLIP_SIZE_DEFAULT)) {
        pthread_mutex_unlock(&glsl_clip_gpu_mutex);
        return;
    } else {
        if (clip_num <= EGL_CLIP_SIZE_DEFAULT) {
            gl_vertices_cur_num = EGL_CLIP_SIZE_DEFAULT;
        } else {
            gl_vertices_cur_num = clip_num;
        }
    }

    gl_vertices_change = 1;

    if (gPositionVertices == NULL) {
        gPositionVertices = (GLfloat *)malloc((gl_vertices_cur_num) * GL_STRUCT_SIZE_VPOSITION * sizeof(GLfloat));
    } else {
        gPositionVertices = (GLfloat *)realloc(gPositionVertices,
                (gl_vertices_cur_num) * GL_STRUCT_SIZE_VPOSITION * sizeof(GLfloat));
    }

    if (gTexCoordVertices == NULL) {
        gTexCoordVertices = (GLfloat *)malloc((gl_vertices_cur_num) * GL_STRUCT_SIZE_TEX_COORD * sizeof(GLfloat));
    } else {
        gTexCoordVertices = (GLfloat *)realloc(gTexCoordVertices,
                (gl_vertices_cur_num) * GL_STRUCT_SIZE_TEX_COORD * sizeof(GLfloat));
    }

    gl_attribute_rects_num = 0;
    gl_attribute_rect_default = 1;

#if (!USE_ARRAY)
    gl_indices_num = 0;
    if (gIndices == NULL) {
        gIndices = (GLubyte *) malloc(gl_vertices_cur_num * 6 * sizeof(GLubyte));
    } else {
        gIndices = (GLubyte *) realloc(gIndices, gl_vertices_cur_num * 6 * sizeof(GLubyte));
    }

    gIndicesDefault[0] = 0;
    gIndicesDefault[1] = 1;
    gIndicesDefault[2] = 2;

    gIndicesDefault[3] = 0;
    gIndicesDefault[4] = 3;
    gIndicesDefault[5] = 2;
#endif

    pthread_mutex_unlock(&glsl_clip_gpu_mutex);
}

/* 设置shader需要处理的数据信息 */
void gl_render_set_rect(egl_rects_t *render_rects, int reset_full_screen)
{
    pthread_mutex_lock(&glsl_clip_gpu_mutex);
    if (render_rects->left == 0 && render_rects->top == 0
            && render_rects->right == rgbTexWidth && render_rects->bottom == rgbTexHeight) {
        if (gl_attribute_rect_default == 1) {
            pthread_mutex_unlock(&glsl_clip_gpu_mutex);
            return;
        } else {
            reset_full_screen = 1;
        }
    }

    if (reset_full_screen) {
        gl_attribute_rect_default = 1;
        memcpy(gPositionVertices, gvPositionDefault, GL_STRUCT_SIZE_VPOSITION * sizeof(GLfloat));
        memcpy(gTexCoordVertices, gTexCoordDefault, GL_STRUCT_SIZE_TEX_COORD * sizeof(GLfloat));
    } else {
        float rect[4];
        gl_attribute_rect_default = 0;
        rect[0] = (float) render_rects->left / rgbTexWidth;
        rect[1] = (float) render_rects->top / rgbTexHeight;
        rect[2] = (float) render_rects->right / rgbTexWidth;
        rect[3] = (float) render_rects->bottom / rgbTexHeight;

        // left top, // right top, // right bottom, // left bottom
        gTexCoordVertices[0] = rect[0];
        gTexCoordVertices[1] = rect[1];
        gTexCoordVertices[2] = 0.0f;

        gTexCoordVertices[3] = rect[2];
        gTexCoordVertices[4] = rect[1];
        gTexCoordVertices[5] = 0.0f;

        gTexCoordVertices[6] = rect[2];
        gTexCoordVertices[7] = rect[3];
        gTexCoordVertices[8] = 0.0f;

        gTexCoordVertices[9] = rect[0];
        gTexCoordVertices[10] = rect[3];
        gTexCoordVertices[11] = 0.0f;

        /* set vposition */
        rect[1] = 1.0f - rect[1];
        rect[3] = 1.0f - rect[3];

        /// left top, // right top, // right bottom, // left bottom
        gPositionVertices[0] = (rect[0] * 2 - 1);
        gPositionVertices[1] = (rect[1] * 2 - 1);
        gPositionVertices[2] = (rect[2] * 2 - 1);
        gPositionVertices[3] = (rect[1] * 2 - 1);
        gPositionVertices[4] = (rect[2] * 2 - 1);
        gPositionVertices[5] = (rect[3] * 2 - 1);
        gPositionVertices[6] = (rect[0] * 2 - 1);
        gPositionVertices[7] = (rect[3] * 2 - 1);
    }
    pthread_mutex_unlock(&glsl_clip_gpu_mutex);
}

/* 没有clip的时候，恢复默认shader */
void gl_clip_rect_reset_full_screen()
{
    pthread_mutex_lock(&glsl_clip_gpu_mutex);
    gl_attribute_rects_num = 0;
    gl_indices_num = 0;
    gl_vertices_change = 1;
    pthread_mutex_unlock(&glsl_clip_gpu_mutex);
}

/* 设置drawelement索引信息 */
static void gl_generate_indices(int index)
{
    int u;

    u = (index + 1) * 4;

    gIndices[u] = u;
    gIndices[u + 1] = u + 1;
    gIndices[u + 2] = u + 2;

    gIndices[u + 3] = u;
    gIndices[u + 4] = u + 3;
    gIndices[u + 5] = u + 2;

    gl_indices_num = (index + 1) * 6;
}

/**
 * gl_set_clip_rects - 设置shader需要处理的数据信息
 * show_rects: 需要显示出来的区域
 * show_rects_num：显示区域的数量
 * clip_rects：需要设置为透明的区域
 * clip_num_rects透明区域的数量
 *
 */
void gl_set_clip_rects(egl_rects_t *show_rects, int show_rects_num, egl_rects_t *clip_rects,
        int clip_num_rects)
{
    if (rgbTexWidth == 0 || rgbTexHeight == 0) {
        fprintf(stderr, "rgbTexWidth:%d,  rgbTexHeight:%d\n", rgbTexWidth, rgbTexHeight);
        return;
    }

    if (show_rects_num + clip_num_rects > EGL_CLIP_SIZE_MAX) {
        fprintf(stderr, "clip_rects show_rects_num:%d, clip_num_rects:%d\n", show_rects_num, clip_num_rects);
        pthread_mutex_lock(&glsl_clip_gpu_mutex);
        gl_attribute_rects_num = 0;
        pthread_mutex_unlock(&glsl_clip_gpu_mutex);
        return;
    }

    gl_vertices_alloc_memory(show_rects_num + clip_num_rects);

    pthread_mutex_lock(&glsl_clip_gpu_mutex);
    gl_vertices_change = 1;

    gl_attribute_rects_num = show_rects_num + clip_num_rects;

    float rect[4];
    int i, u, v, idx = 0;
    for (i = 0; i < show_rects_num; i++, idx++) {
        rect[0] = (float) show_rects[i].left / rgbTexWidth;
        rect[1] = (float) show_rects[i].top / rgbTexHeight;
        rect[2] = (float) show_rects[i].right / rgbTexWidth;
        rect[3] = (float) show_rects[i].bottom / rgbTexHeight;

        v = idx * GL_STRUCT_SIZE_TEX_COORD;
        // left top, // right top, // right bottom, // left bottom
        gTexCoordVertices[v] = rect[0];
        gTexCoordVertices[v + 1] = rect[1];
        gTexCoordVertices[v + 2] = 0.0f;

        gTexCoordVertices[v + 3] = rect[2];
        gTexCoordVertices[v + 4] = rect[1];
        gTexCoordVertices[v + 5] = 0.0f;

        gTexCoordVertices[v + 6] = rect[2];
        gTexCoordVertices[v + 7] = rect[3];
        gTexCoordVertices[v + 8] = 0.0f;

        gTexCoordVertices[v + 9] = rect[0];
        gTexCoordVertices[v + 10] = rect[3];
        gTexCoordVertices[v + 11] = 0.0f;

        /* set vposition */
        rect[1] = 1.0f - rect[1];
        rect[3] = 1.0f - rect[3];

        u = idx * GL_STRUCT_SIZE_VPOSITION;
        /// left top, // right top, // right bottom, // left bottom
        gPositionVertices[u] = (rect[0] * 2 - 1);
        gPositionVertices[u + 1] = (rect[1] * 2 - 1);
        gPositionVertices[u + 2] = (rect[2] * 2 - 1);
        gPositionVertices[u + 3] = (rect[1] * 2 - 1);
        gPositionVertices[u + 4] = (rect[2] * 2 - 1);
        gPositionVertices[u + 5] = (rect[3] * 2 - 1);
        gPositionVertices[u + 6] = (rect[0] * 2 - 1);
        gPositionVertices[u + 7] = (rect[3] * 2 - 1);

#if (!USE_ARRAY)
        gl_generate_indices(idx);
#endif
    }

    for (i = 0; i < clip_num_rects; i++, idx++) {
        rect[0] = (float) clip_rects[i].left / rgbTexWidth;
        rect[1] = (float) clip_rects[i].top / rgbTexHeight;
        rect[2] = (float) clip_rects[i].right / rgbTexWidth;
        rect[3] = (float) clip_rects[i].bottom / rgbTexHeight;

        v = idx * GL_STRUCT_SIZE_TEX_COORD;
        // left bottom, // right bottom, // right top, // left top
        gTexCoordVertices[v] = rect[0];
        gTexCoordVertices[v + 1] = rect[1];
        gTexCoordVertices[v + 2] = -1.0f;

        gTexCoordVertices[v + 3] = rect[2];
        gTexCoordVertices[v + 4] = rect[1];
        gTexCoordVertices[v + 5] = -1.0f;

        gTexCoordVertices[v + 6] = rect[2];
        gTexCoordVertices[v + 7] = rect[3];
        gTexCoordVertices[v + 8] = -1.0f;

        gTexCoordVertices[v + 9] = rect[0];
        gTexCoordVertices[v + 10] = rect[3];
        gTexCoordVertices[v + 11] = -1.0f;

        /* set vposition */
        rect[1] = 1.0f - rect[1];
        rect[3] = 1.0f - rect[3];

        u = idx * GL_STRUCT_SIZE_VPOSITION;
        // left top, // right top, // right bottom, // left bottom
        gPositionVertices[u] = (rect[0] * 2 - 1);
        gPositionVertices[u + 1] = (rect[1] * 2 - 1);
        gPositionVertices[u + 2] = (rect[2] * 2 - 1);
        gPositionVertices[u + 3] = (rect[1] * 2 - 1);
        gPositionVertices[u + 4] = (rect[2] * 2 - 1);
        gPositionVertices[u + 5] = (rect[3] * 2 - 1);
        gPositionVertices[u + 6] = (rect[0] * 2 - 1);
        gPositionVertices[u + 7] = (rect[3] * 2 - 1);

#if (!USE_ARRAY)
        gl_generate_indices(idx);
#endif
    }
    pthread_mutex_unlock(&glsl_clip_gpu_mutex);
}

/* shader加载 */
GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    ALOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }

    return shader;
}

/* 创建program，并且附加shader */
static int gl_create_shader_program()
{
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, gVertexShader);
    if (!vertexShader) {
        fprintf(stderr, "loadShader vertexShader.\n");
        return -1;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, gFragmentShader);
    if (!pixelShader) {
        fprintf(stderr, "loadShader pixelShader.\n");
        return -1;
    }

    gl_sh_program = glCreateProgram();
    if (gl_sh_program) {
        glAttachShader(gl_sh_program, vertexShader);
        checkGlError("glAttachShader vertexShader");
        glAttachShader(gl_sh_program, pixelShader);
        checkGlError("glAttachShader pixelShader");
        glLinkProgram(gl_sh_program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(gl_sh_program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(gl_sh_program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char *buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(gl_sh_program, bufLength, NULL, buf);
                    ALOGE("Could not link gl_sh_program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(gl_sh_program);
            gl_sh_program = 0;
            return -1;
        }
    }

    return 0;
}

static void gl_get_vertices_idx()
{
    gvPositionIndex = glGetAttribLocation(gl_sh_program, "vPosition");
    checkGlError("glGetAttribLocation");
    fprintf(stderr, "EGL: glGetAttribLocation(\"vPosition\") = %d\n", gvPositionIndex);

    texCoordsIndex = glGetAttribLocation(gl_sh_program, "texCoords");
    checkGlError("glGetAttribLocation");
    fprintf(stderr, "EGL: glGetAttribLocation(\"texCoords\") = %d\n", texCoordsIndex);

    GLint gYuvTexSamplerHandle = glGetUniformLocation(gl_sh_program, "rgbaTexSampler");
    checkGlError("glGetUniformLocation");
    fprintf(stderr, "EGL: glGetUniformLocation(\"rgbaTexSampler\") = %d\n", gYuvTexSamplerHandle);
}

/* clip改变之后，需要重新加载vbos */
static void gl_set_attribute_vbos(int first, int attr_num, const GLfloat *v_position, const GLfloat *tex_coord)
{
    glDeleteBuffers(EGL_VBO_IDS, gl_vboIds);
    glGenBuffers(EGL_VBO_IDS, gl_vboIds);

    glBindBuffer(GL_ARRAY_BUFFER, gl_vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, GL_VPOSITION_STRIDE * 4 * attr_num, v_position, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, gl_vboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, GL_TEXCOORD_STRIDE * 4 * attr_num, tex_coord, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, gl_vboIds[0]);
    glVertexAttribPointer(gvPositionIndex, 2, GL_FLOAT, GL_FALSE, GL_VPOSITION_STRIDE, 0);
    if (first > 0) {
        glEnableVertexAttribArray(gvPositionIndex);
    }

    glBindBuffer(GL_ARRAY_BUFFER, gl_vboIds[1]);
    glVertexAttribPointer(texCoordsIndex, 3, GL_FLOAT, GL_FALSE, GL_TEXCOORD_STRIDE, 0);
    if (first > 0) {
        glEnableVertexAttribArray(texCoordsIndex);
    }

    gl_vertices_change = 0;
}

/* 判断clip是否改变 */
static void gl_set_vertex_attrib_vbos()
{
    if (gl_attribute_rects_num == 0) {
        gl_set_attribute_vbos(0, 1, gvPositionDefault, gTexCoordDefault);
        return;
    }

    gl_set_attribute_vbos(0, gl_attribute_rects_num, gPositionVertices, gTexCoordVertices);
}

/* 配置graphicbuffer以及设置显示参数 */
static int gl_rgbtex_img_init(int rgbSurfaceWidth, int rgbSurfaceHeight)
{
    char *gra_buf = NULL;

    printGLString("OpenGL factory", GL_VENDOR);
    printGLString("GL_RENDERER", GL_RENDERER);
    printGLString("OpenGLVersion", GL_VERSION);

    printGLString("GL_EXTENSIONS", GL_EXTENSIONS);
    fprintf(stderr, "GL eglGetCurrentDisplay = %s\n", eglQueryString(eglGetCurrentDisplay(), EGL_EXTENSIONS));

    ALOGE("EGL: rgbTexWidth=%d, rgbTexHeight=%d\n", rgbTexWidth, rgbTexHeight);

    rgbTexBuffer = new GraphicBuffer(rgbTexWidth, rgbTexHeight, rgbTexFormat, rgbTexUsage);
    status_t err = rgbTexBuffer->initCheck();
    if (err != NO_ERROR) {
        ALOGE("EGL: %s\n", strerror(-err));
        return false;
    }

    err = rgbTexBuffer->lock(rgbBufferLockFlag, (void**) (&gra_buf));
    if (err != 0) {
        fprintf(stderr, "rgbTexBuffer->lock(...) failed: %d\n", err);
        return false;
    }
    if (rgbTexFormat <= HAL_PIXEL_FORMAT_RGBX_8888) {
        memset(gra_buf, 0, rgbTexWidth * rgbTexHeight * 4);
    } else if (rgbTexFormat == HAL_PIXEL_FORMAT_BGRA_8888) {
        memset(gra_buf, 0, rgbTexWidth * rgbTexHeight * 4);
    } else if (rgbTexFormat == HAL_PIXEL_FORMAT_RGB_565) {
        memset(gra_buf, 0, rgbTexWidth * rgbTexHeight * 2);
    }
    err = rgbTexBuffer->unlock();

    clientBuffer = (EGLClientBuffer) rgbTexBuffer->getNativeBuffer();

    displayImage = eglCreateImageKHR(mDisplay, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID,
            clientBuffer, 0);

    checkEglError("eglCreateImageKHR");
    if (displayImage == EGL_NO_IMAGE_KHR) {
        return false;
    }

    /* generate one texture */
    glGenTextures(1, &rgbTex);
    checkGlError("glGenTextures");

    glBindTexture(GL_TEXTURE_EXTERNAL_OES, rgbTex);
    checkGlError("glBindTexture");
    glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, (GLeglImageOES) displayImage);
    checkGlError("glEGLImageTargetTexture2DOES");

    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    checkGlError("glTexParameteri");

    gl_create_shader_program();

    /* progrem info */
    gl_get_vertices_idx();

    glViewport(0, 0, rgbSurfaceWidth, rgbSurfaceHeight);
    ALOGE("EGL: eglViewport surface width:%d, height:%d", mSurfaceWidth, mSurfaceHeight);
    checkGlError("glViewport");
    glUseProgram(gl_sh_program);
    checkGlError("glUseProgram");

#if ORIGINAL_VERTEX
    glVertexAttribPointer(gvPositionIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), gvPositionDefault);
    checkGlError("glVertexAttribPointer1");
    glEnableVertexAttribArray(gvPositionIndex);
    checkGlError("glEnableVertexAttribArray1");

    glVertexAttribPointer(texCoordsIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), gTexCoordDefault);
    checkGlError("glVertexAttribPointer2");
    glEnableVertexAttribArray(texCoordsIndex);
    checkGlError("glEnableVertexAttribArray2");
#endif

    gl_set_attribute_vbos(1, 1, gvPositionDefault, gTexCoordDefault);

    return 0;
}

static void gl_rgatex_parse_usage(int bufferUsage)
{
    int buffer_flag = 0;
    
    /* read */
    if (bufferUsage & EGL_USAGE_SW_READ_NEVER) {
        buffer_flag |= GraphicBuffer::USAGE_SW_READ_NEVER;
    }
    if (bufferUsage & EGL_USAGE_SW_READ_RARELY) {
        buffer_flag |= GraphicBuffer::USAGE_SW_READ_RARELY;
    }
    if (bufferUsage & EGL_USAGE_SW_READ_OFTEN) {
        buffer_flag |= GraphicBuffer::USAGE_SW_READ_OFTEN;
    }
    if (bufferUsage & EGL_USAGE_SW_READ_MASK) {
        buffer_flag |= GraphicBuffer::USAGE_SW_READ_MASK;
    }

    /* write */
    if (bufferUsage & EGL_USAGE_SW_WRITE_NEVER) {
        buffer_flag |= GraphicBuffer::USAGE_SW_WRITE_NEVER;
    }
    if (bufferUsage & EGL_USAGE_SW_WRITE_RARELY) {
        buffer_flag |= GraphicBuffer::USAGE_SW_WRITE_RARELY;
    }
    if (bufferUsage & EGL_USAGE_SW_WRITE_OFTEN) {
        buffer_flag |= GraphicBuffer::USAGE_SW_WRITE_OFTEN;
    }
    if (bufferUsage & EGL_USAGE_SW_WRITE_MASK) {
        buffer_flag |= GraphicBuffer::USAGE_SW_WRITE_MASK;
    }

    if (bufferUsage & EGL_USAGE_PROTECTED) {
        buffer_flag |= GraphicBuffer::USAGE_PROTECTED;
    }

    /* hardware */
    if (bufferUsage & EGL_USAGE_HW_TEXTURE) {
        buffer_flag |= GraphicBuffer::USAGE_HW_TEXTURE;
    }
    if (bufferUsage & EGL_USAGE_HW_RENDER) {
        buffer_flag |= GraphicBuffer::USAGE_HW_RENDER;
    }

    if (bufferUsage & EGL_USAGE_HW_2D) {
        buffer_flag |= GraphicBuffer::USAGE_HW_2D;
    }
    if (bufferUsage & EGL_USAGE_HW_COMPOSER) {
        buffer_flag |= GraphicBuffer::USAGE_HW_COMPOSER;
    }
    if (bufferUsage & EGL_USAGE_HW_VIDEO_ENCODER) {
        buffer_flag |= GraphicBuffer::USAGE_HW_VIDEO_ENCODER;
    }
    if (bufferUsage & EGL_USAGE_HW_MASK) {
        buffer_flag |= GraphicBuffer::USAGE_HW_MASK;
    }

    if (buffer_flag != 0) {
        rgbTexUsage = buffer_flag;
        ALOGE("graphicbuffer rgbTexUsage:%x\n", rgbTexUsage);
    } else {
        ALOGE("graphicbuffer default rgbTexUsage:%x\n", rgbTexUsage);
    }
}

/* 创建egl的显示surface*/
static int gl_rgbtex_create_surf(int pixelFormat, int canvasWidth, int canvasHeight,
        int rgbSurfaceWidth, int rgbSurfaceHeight, int layer, int bufferUsage)
{
    int ret = 0;

    gl_init_rgb_format(pixelFormat, canvasWidth, canvasHeight);

    if (gSurfaceComposerClient != NULL) {
        return 0;
    }

    gSurfaceComposerClient = new SurfaceComposerClient();
    ret = gSurfaceComposerClient->initCheck();
    if (ret != 0) {
        ALOGE("new SurfaceComposerClient() ERROR:%d\n", ret);
        return -1;
    }

    gSurfaceControl = gSurfaceComposerClient->createSurface(String8("GlSurface_rgbx"), rgbSurfaceWidth,
            rgbSurfaceHeight, rgbTexFormat);
    if (gSurfaceControl != NULL && gSurfaceControl->isValid()) {
        ALOGE("EGL: createSurface %dx%d sucess!\n", rgbSurfaceWidth, rgbSurfaceHeight);
    } else {
        ALOGE("EGL: gSurfaceControl has error\n");
        return -1;
    }

    SurfaceComposerClient::openGlobalTransaction();
    gSurfaceControl->setLayer(layer);
    gSurfaceControl->setPosition(0, 0);
    gSurfaceControl->show();
    SurfaceComposerClient::closeGlobalTransaction();

    gSurface = gSurfaceControl->getSurface();

    // initialize opengl and egl
    mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(mDisplay, 0, 0);
    eglChooseConfig(mDisplay, attribs, &config, 1, &numConfigs);
    mSurface = eglCreateWindowSurface(mDisplay, config, gSurface.get(), NULL);
    mContext = eglCreateContext(mDisplay, config, NULL, mcontext_attribs);

    printEGLConfiguration(mDisplay, config);

    eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &mSurfaceWidth);
    eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &mSurfaceHeight);
    ALOGE("EGL: numConfigs=%d, surface=%p,  context=%p, W=%d, H=%d\n", numConfigs, mSurface,
            mContext, mSurfaceWidth, mSurfaceHeight);
    if (eglMakeCurrent(mDisplay, mSurface, mSurface, mContext) == EGL_FALSE) {
        ALOGE("eglMakeCurrent return EGL_FALSE\n");
        return -1;
    }

    if (bufferUsage > 0) {
        gl_rgatex_parse_usage(bufferUsage);
    }
    gl_rgbtex_img_init(rgbSurfaceWidth, rgbSurfaceHeight);

    return ret;
}

/**
 * gl_rgbtex_surf_init - 初始化EGL的显示界面
 *
 * pixelFormat: 创建界面的格式rgba，rgb565，yuv等
 * canvasWidth：需要创建的画面宽度
 * canvasHeight：需要创建的画面高度
 * rgbSurfaceWidth：当前显示的分辨率宽度
 * rgbSurfaceHeight：当前显示的分辨率高度
 * layer：创建EGL界面后，设置显示的层次
 * bufferUsage：graphicbuffer创建的配置类型
 *
 * return：0 - 创建新的EGL
 *         -1 - 未创建
 */
int gl_rgbtex_surf_init(int pixelFormat, int canvasWidth, int canvasHeight, int rgbSurfaceWidth,
        int rgbSurfaceHeight, int layer, int bufferUsage)
{
    if (canvasWidth == 0 || canvasHeight == 0) {
        return -1;
    }

    if (glsl_has_inited == 0) {
        glsl_has_inited = 1;
        pthread_mutex_init(&glsl_clip_gpu_mutex, NULL);

        glGenBuffers(EGL_VBO_IDS, gl_vboIds);
        gl_vertices_alloc_memory(EGL_CLIP_SIZE_DEFAULT);
    }

    gl_rgbtex_create_surf(pixelFormat, canvasWidth, canvasHeight, rgbSurfaceWidth,
            rgbSurfaceHeight, layer, bufferUsage);

    return 0;
}

/**
 * rgb_surf_pixel_render - 渲染一帧
 *
 * finish：渲染时候，是否需要等待渲染指令处理完成才返回
 *
 */
void rgb_surf_pixel_render(int finish)
{
    pthread_mutex_lock(&glsl_clip_gpu_mutex);

    if (gl_vertices_change != 0) {
        gl_set_vertex_attrib_vbos();
    }

    if (gl_attribute_rects_num == 0) {
#if USE_ARRAY
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#else
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
#endif
    } else {
#if USE_ARRAY
        for (int i = 0; i < gl_attribute_rects_num; i++) {
            glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
        }
#else
        glDrawElements(GL_TRIANGLES, gl_indices_num, GL_UNSIGNED_BYTE, gIndices);
#endif
    }
    pthread_mutex_unlock(&glsl_clip_gpu_mutex);

    if (finish > 0) {
        glFinish();
    } else {
        glFlush();
    }
    eglSwapBuffers(mDisplay, mSurface);
}

/* 往EGL内存填入显示数据 */
void rgb_fill_surf_pixel_data(char *data, int left, int top, int right, int bottom, int size)
{
    setRgbDataToGralloc(data, size);

    rgb_surf_pixel_render(0);
}

/* 销毁当前的EGL显示界面，并且释放资源 */
void rgb_surface_destroy()
{
    if (mDisplay == NULL) {
        return;
    }

    eglDestroyImageKHR(mDisplay, displayImage);
    eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(mDisplay, mContext);
    eglDestroySurface(mDisplay, mSurface);

    gSurface.clear();
    gSurfaceControl.clear();

    gSurfaceComposerClient = NULL;
    mDisplay == NULL;
    displayImage = NULL;

    rgbTexWidth = 0;
    rgbTexHeight = 0;

    pthread_mutex_lock(&glsl_clip_gpu_mutex);
    gl_attribute_rects_num = 0;
    gl_vertices_change = 1;
    pthread_mutex_unlock(&glsl_clip_gpu_mutex);
}

/* rgba convert to rgb 565*/
void rgba8888_to_rgb565(char *outPixel16, char *inPixel32, int pixelCount)
{
    int i, j, k;
    unsigned char r, g, b;

    for (i = 0, j = 0, k = 0; i < pixelCount; i++, j += 2, k += 4) {
        r = ((inPixel32[k]) & 0xFF);
        g = ((inPixel32[k + 1]) & 0xFF);
        b = ((inPixel32[k + 2]) & 0xFF);
        outPixel16[j + 1] = ((r >> 3) << 3) | ((g >> 5) << 0);
        outPixel16[j] = ((g >> 2) << 5) | ((b >> 3) << 0);
    }
}

/* rgba convert to rgb 888 */
void rgba8888_to_rgb888(char *outPixel24, char *inPixel32, int pixelCount)
{
    int i, j, k;
    unsigned char r, g, b;

    for (i = 0, j = 0, k = 0; i < pixelCount; i++, j += 3, k += 4) {
        r = ((inPixel32[k]) & 0xFF);
        g = ((inPixel32[k + 1]) & 0xFF);
        b = ((inPixel32[k + 2]) & 0xFF);

        outPixel24[j] = ((r) << 0);
        outPixel24[j + 1] = ((g) << 0);
        outPixel24[j + 2] = ((b) << 0);
    }
}

/* 独立申请graphicbuffer内存 */
char *alloc_get_graphic_buffer(int pixelFormat, int canvasWidth, int canvasHeight)
{
    char *buf = NULL;

    gl_init_rgb_format(pixelFormat, canvasWidth, canvasHeight);

    rgbTexBuffer = new GraphicBuffer(rgbTexWidth, rgbTexHeight, rgbTexFormat, rgbTexUsage);
    status_t err = rgbTexBuffer->initCheck();
    if (err != NO_ERROR) {
        fprintf(stderr, "%s\n", strerror(-err));
        return NULL;
    }

    err = rgbTexBuffer->lock(rgbBufferLockFlag, (void**) (&buf));
    if (err != 0) {
        fprintf(stderr, "rgbTexBuffer->lock(...) failed: %d\n", err);
        return NULL;
    }

    if (rgbTexFormat <= HAL_PIXEL_FORMAT_RGBX_8888) {
        memset(buf, 0, rgbTexWidth * rgbTexHeight * 4);
    } else if (rgbTexFormat == HAL_PIXEL_FORMAT_BGRA_8888) {
        memset(buf, 0, rgbTexWidth * rgbTexHeight * 4);
    } else if (rgbTexFormat == HAL_PIXEL_FORMAT_RGB_565) {
        memset(buf, 0, rgbTexWidth * rgbTexHeight * 2);
    }
    err = rgbTexBuffer->unlock();

    return buf;
}

/* 锁定grahpicbuffer，并且获取graphic的内存 */
char *rgb_lock_buffer()
{
    char *buf;

    status_t err = rgbTexBuffer->lock(rgbBufferLockFlag, (void**) (&buf));
    if (err != 0) {
        fprintf(stderr, "rgbTexBuffer->lock(...) failed: %d\n", err);
        return NULL;
    }

    return buf;
}

/* 解锁grahpicbuffer */
void rgb_unlock_buffer()
{
    rgbTexBuffer->unlock();
}

/* 获取grahpicbuffer的物理地址 */
int rgb_get_phy_addr()
{
    struct private_handle_t *buffhnd = (struct private_handle_t*) rgbTexBuffer->handle;

    return buffhnd->phy_addr;
}

/* 获取grahpicbuffer实际申请的内存显示界面宽度 */
unsigned int rgb_get_buffer_stride()
{
    return rgbTexBuffer->getStride();
}
