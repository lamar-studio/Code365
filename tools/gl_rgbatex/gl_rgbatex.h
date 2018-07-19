
#ifndef  _GL_RGBTEX_H_
#define _GL_RGBTEX_H_

#ifdef __cplusplus 
extern "C" {
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define ATRACE_MESSAGE_LEN 256

/* gl_rgbtex_surf_init函数的pixelFormat参数类型 */
enum {
    /*
     * "linear" color pixel formats:
     *
     * The pixel formats are otherwise treated
     * as linear formats, i.e.: no special operation is performed when
     * reading or writing into a buffer in one of these formats
     */
	CANVAS_RGBA_8888 = 1,
	CANVAS_RGBX_8888 = 2,  /* RGBX means the pixel format still has an alpha channel, but it is ignored, and is always set to 255 */
	CANVAS_RGB_888 = 3,
	CANVAS_RGB_565 = 4,
	CANVAS_BGRA_8888 = 5,

    /* This format assumes
     * - an even width
     * - an even height
     * - a horizontal stride multiple of 16 pixels
     * - a vertical stride equal to the height
     *
     *   y_size = stride * height
     *   c_stride = ALIGN(stride/2, 16)
     *   c_size = c_stride * height/2
     *   size = y_size + c_size * 2
     *   cr_offset = y_size
     *   cb_offset = y_size + c_size
     */
	CANVAS_YV12 = 6,
};

/* gl_rgbtex_surf_init函数的bufferUsage参数类型 */
enum {
    EGL_USAGE_SW_READ_NEVER     = (1 << 0),
    EGL_USAGE_SW_READ_RARELY    = (1 << 1),
    EGL_USAGE_SW_READ_OFTEN     = (1 << 2),
    EGL_USAGE_SW_READ_MASK      = (1 << 3),

    EGL_USAGE_SW_WRITE_NEVER    = (1 << 4),
    EGL_USAGE_SW_WRITE_RARELY   = (1 << 5),
    EGL_USAGE_SW_WRITE_OFTEN    = (1 << 6),
    EGL_USAGE_SW_WRITE_MASK     = (1 << 7),

    EGL_USAGE_PROTECTED         = (1 << 8),

    EGL_USAGE_HW_TEXTURE        = (1 << 9),
    EGL_USAGE_HW_RENDER         = (1 << 10),
    EGL_USAGE_HW_2D             = (1 << 11),
    EGL_USAGE_HW_COMPOSER       = (1 << 12),
    EGL_USAGE_HW_VIDEO_ENCODER  = (1 << 13),
    EGL_USAGE_HW_MASK           = (1 << 14),
};

/************************ egl clip ************************/

#define EGL_CLIP_SIZE_MAX       1024    /* 限制设置clip的时候，show rect和clip rect的数量之和最大值 */
#define EGL_CLIP_SIZE_DEFAULT   16      /* 视频重定向的时候，clip区域(show rect和clip rect)的默认数量 */

/* 对外隐藏 */
typedef struct egl_rects_s {
    int left;
    int top;
    int right;
    int bottom;
} egl_rects_t;

/************************ egl clip end************************/

/****************************** systrace调试 start ***************************************/
extern int atrace_marker_fd;
void trace_init();

inline void trace_begin(const char *name)
{
    char trace_buf[ATRACE_MESSAGE_LEN];
    int len = snprintf(trace_buf, ATRACE_MESSAGE_LEN, "B|%d|%s", getpid(), name);
    write(atrace_marker_fd, trace_buf, len);
}

inline void trace_end()
{
    char c = 'E';
    write(atrace_marker_fd, &c, 1);
}

inline void trace_counter(const char *name, const int value)
{
    char trace_buf[ATRACE_MESSAGE_LEN];
    int len = snprintf(trace_buf, ATRACE_MESSAGE_LEN, "C|%d|%s|%i", getpid(), name, value);
    write(atrace_marker_fd, trace_buf, len);
}

inline void trace_async_begin(const char *name, const int32_t cookie)
{
    char trace_buf[ATRACE_MESSAGE_LEN];
    int len = snprintf(trace_buf, ATRACE_MESSAGE_LEN, "S|%d|%s|%i", getpid(), name, cookie);
    write(atrace_marker_fd, trace_buf, len);
}

inline void trace_async_end(const char *name, const int32_t cookie)
{
    char trace_buf[ATRACE_MESSAGE_LEN];
    int len = snprintf(trace_buf, ATRACE_MESSAGE_LEN, "F|%d|%s|%i", getpid(), name, cookie);
    write(atrace_marker_fd, trace_buf, len);
}

void sys_trace_tag(uint64_t tag);
void sys_trace_begin(const char* name);
void sys_trace_end();
/****************************** systrace调试 end***************************************/

/* 获取字节对齐 */
int align(int x, int a);

/* 设置EGL层次以及设置egl是否隐藏显示：layer [0 - INT_MAX], show: 0, 1 */
void setRgbSurfaceLayer(int layer, int show);

/* 设置shader需要处理的数据信息 */
void gl_render_set_rect(egl_rects_t *render_rects, int reset_full_screen);

/* 没有clip的时候，恢复默认shader */
void gl_clip_rect_reset_full_screen();

/* gl_set_clip_rects - 设置shader需要处理的数据信息 */
void gl_set_clip_rects(egl_rects_t *show_rects, int show_rects_num, egl_rects_t *clip_rects,
        int clip_num_rects);

/* 初始化EGL的显示界面 layer [0 - INT_MAX] */
int gl_rgbtex_surf_init(int pixelFormat, int canvasWidth, int canvasHeight, int rgbSurfaceWidth,
        int rgbSurfaceHeight, int layer, int bufferUsage);

/* 往EGL内存填入显示数据 */
void rgb_fill_surf_pixel_data(char *data, int left, int top, int right, int bottom, int size);

/* 销毁当前的EGL显示界面，并且释放资源 */
void rgb_surface_destroy();

/* 渲染一帧 */
void rgb_surf_pixel_render(int finish);

/* 独立申请graphicbuffer内存 */
char *alloc_get_graphic_buffer(int pixelFormat, int canvasWidth, int canvasHeight);

/* 锁定grahpicbuffer，并且获取graphic的内存 */
char *rgb_lock_buffer();

/* 解锁grahpicbuffer */
int rgb_get_phy_addr();

/* 获取grahpicbuffer的物理地址 */
void rgb_unlock_buffer();

/* 获取grahpicbuffer实际申请的内存显示界面宽度 */
unsigned int rgb_get_buffer_stride();

#ifdef __cplusplus
}
#endif

#endif    /* _GL_RGBTEX_H_ */
