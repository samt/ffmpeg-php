/*
   This file is part of ffmpeg-php

   Copyright (C) 2004-2008 Todd Kirby (ffmpeg.php AT gmail.com)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

   In addition, as a special exception, the copyright holders of ffmpeg-php
   give you permission to combine ffmpeg-php with code included in the
   standard release of PHP under the PHP license (or modified versions of
   such code, with unchanged license). You may copy and distribute such a
   system following the terms of the GNU GPL for ffmpeg-php and the licenses
   of the other code concerned, provided that you include the source code of
   that other code when and as the GNU GPL requires distribution of source code.

   You must obey the GNU General Public License in all respects for all of the
   code used other than standard release of PHP. If you modify this file, you
   may extend this exception to your version of the file, but you are not
   obligated to do so. If you do not wish to do so, delete this exception
   statement from your version.

 */

#include "php.h"
#include "php_ini.h"
#include "php_globals.h"
#include "ext/standard/info.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_ffmpeg.h"

#include "ffmpeg_frame.h"
#include "ffmpeg_tools.h"

#if HAVE_LIBGD20
#include "gd/libgd/gd.h" 

#ifdef HAVE_SWSCALER
#include <swscale.h>
#endif

#define FFMPEG_PHP_FETCH_IMAGE_RESOURCE(gd_img, ret) { \
    ZEND_GET_RESOURCE_TYPE_ID(le_gd, "gd"); \
    ZEND_FETCH_RESOURCE(gd_img, gdImagePtr, ret, -1, "Image", le_gd); \
}

static int le_gd; // this is only valid after calling 
                  // FFMPEG_PHP_FETCH_IMAGE_RESOURCE() macro

#endif // HAVE_LIBGD20

#define GET_FRAME_RESOURCE(ffmpeg_frame_object, ffmpeg_frame) {\
	zval **_tmp_zval;\
    if (zend_hash_find(Z_OBJPROP_P(ffmpeg_frame_object), "ffmpeg_frame",\
                sizeof("ffmpeg_frame"), (void **)&_tmp_zval) == FAILURE) {\
        zend_error(E_ERROR, "Unable to locate ffmpeg_frame resource in this object.");\
        RETURN_FALSE;\
    }\
\
    ZEND_FETCH_RESOURCE(ffmpeg_frame, ff_frame_context*, _tmp_zval, -1,\
            "ffmpeg_frame", le_ffmpeg_frame);\
}\

int le_ffmpeg_frame; // not static since it is used in ffmpeg_output_movie

static zend_class_entry *ffmpeg_frame_class_entry_ptr;
zend_class_entry ffmpeg_frame_class_entry;
 
/* {{{ ffmpeg_frame methods[]
    Methods of the ffmpeg_frame class 
*/
zend_function_entry ffmpeg_frame_class_methods[] = {
    
    /* object can't be created from user space so no PHP constructor */
    //PHP_ME(ffmpeg_frame, __construct, NULL, 0)
  
#if HAVE_LIBGD20
    /* gd methods */
    FFMPEG_PHP_MALIAS(ffmpeg_frame, togdimage,      toGDImage,     NULL, 0)
#endif // HAVE_LIBGD20

    /* methods */
    FFMPEG_PHP_MALIAS(ffmpeg_frame, getwidth,                    getWidth,                   NULL, 0)
    FFMPEG_PHP_MALIAS(ffmpeg_frame, getheight,                   getHeight,                  NULL, 0)
    FFMPEG_PHP_MALIAS(ffmpeg_frame, resize,                      resize,                     NULL, 0)
    FFMPEG_PHP_MALIAS(ffmpeg_frame, iskeyframe,                  isKeyFrame,                 NULL, 0)
    FFMPEG_PHP_MALIAS(ffmpeg_frame, getpresentationtimestamp,    getPresentationTimestamp,   NULL, 0)
    FFMPEG_PHP_MALIAS(ffmpeg_frame, getpts,                      getPresentationTimestamp,   NULL, 0)
    FFMPEG_PHP_END_METHODS
};
/* }}} */


/* {{{ _php_alloc_ff_frame()
 */
static ff_frame_context* _php_alloc_ff_frame()
{
    ff_frame_context *ff_frame = NULL;

    ff_frame = emalloc(sizeof(ff_frame_context));
    
    if (!ff_frame) {
        zend_error(E_ERROR, "Error allocating ffmpeg_frame");
    }

    ff_frame->av_frame = NULL;
    ff_frame->width = 0;
    ff_frame->height = 0;
    ff_frame->pixel_format = 0;

    return ff_frame;
}
/* }}} */


/* {{{ proto object _php_create_ffmpeg_frame() 
   creates an ffmpeg_frame object, adds a ffmpeg_frame resource to the
   object, registers the resource and returns a direct pointer to the 
   resource.
 */
ff_frame_context* _php_create_ffmpeg_frame(INTERNAL_FUNCTION_PARAMETERS)
{
    int ret;
	ff_frame_context *ff_frame;
    
    ff_frame = _php_alloc_ff_frame();
    
	ret = ZEND_REGISTER_RESOURCE(NULL, ff_frame, le_ffmpeg_frame);
    
    object_init_ex(return_value, ffmpeg_frame_class_entry_ptr);
    add_property_resource(return_value, "ffmpeg_frame", ret);
    return ff_frame;
}
/* }}} */


/* {{{ _php_free_av_frame()
 */
static void _php_free_av_frame(AVFrame *av_frame)
{
    if (av_frame) {
        if (av_frame->data[0]) {
            av_free(av_frame->data[0]);
            av_frame->data[0] = NULL;
        }
        av_free(av_frame);
    }
}
/* }}} */


/* {{{ _php_free_ffmpeg_frame()
 */
static void _php_free_ffmpeg_frame(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    ff_frame_context *ff_frame = (ff_frame_context*)rsrc->ptr;    
    _php_free_av_frame(ff_frame->av_frame);
    efree(ff_frame);
}
/* }}} */


/* {{{ register_ffmpeg_frame_class()
 */
void register_ffmpeg_frame_class(int module_number)
{
    TSRMLS_FETCH();

    le_ffmpeg_frame = zend_register_list_destructors_ex(_php_free_ffmpeg_frame,
            NULL, "ffmpeg_frame", module_number);

    INIT_CLASS_ENTRY(ffmpeg_frame_class_entry, "ffmpeg_frame", 
            ffmpeg_frame_class_methods);

    /* register ffmpeg frame class */
    ffmpeg_frame_class_entry_ptr = 
        zend_register_internal_class(&ffmpeg_frame_class_entry TSRMLS_CC);
}
/* }}} */



/* {{{ _php_convert_frame()
 */
int _php_convert_frame(ff_frame_context *ff_frame, int dst_fmt) {
#ifdef HAVE_SWSCALER
    AVFrame *src_frame;
    AVFrame *dst_frame;
    struct SwsContext *sws_ctx = NULL;

    if (!ff_frame->av_frame) {
        return -1;
    }

    src_frame = ff_frame->av_frame;

    dst_frame = avcodec_alloc_frame();
    avpicture_alloc((AVPicture*)dst_frame, dst_fmt, ff_frame->width,
            ff_frame->height);

    /* TODO: Try to get cached sws_context first */
    sws_ctx = sws_getContext(
            ff_frame->width, ff_frame->height, ff_frame->pixel_format, 
            ff_frame->width, ff_frame->height, dst_fmt, 
            SWS_BICUBIC, NULL, NULL, NULL);

    if (sws_ctx == NULL){
        return 1;
    }

    sws_scale(sws_ctx, src_frame->data, src_frame->linesize, 0, ff_frame->height,
            dst_frame->data, dst_frame->linesize);

    sws_freeContext(sws_ctx);

    ff_frame->av_frame = dst_frame;
    ff_frame->pixel_format = dst_fmt;

    _php_free_av_frame(src_frame);
    return 0;

#else /* Do img_convert */
    AVFrame *dst_fmt_frame;

    if (!ff_frame->av_frame) {
        return -1;
    }

    if (ff_frame->pixel_format == dst_fmt) {
        return 0;
    }

    dst_fmt_frame = avcodec_alloc_frame();
    avpicture_alloc((AVPicture*)dst_fmt_frame, dst_fmt, ff_frame->width,
                            ff_frame->height);
    if (img_convert((AVPicture*)dst_fmt_frame, dst_fmt,
                (AVPicture *)ff_frame->av_frame,
                ff_frame->pixel_format, ff_frame->width,
                ff_frame->height) < 0) {
        zend_error(E_ERROR, "Error converting frame");
    }

    _php_free_av_frame(ff_frame->av_frame);

    ff_frame->av_frame = dst_fmt_frame;
    ff_frame->pixel_format = dst_fmt;

    return 0;
#endif /* HAVE_SWSCALER */
}
/* }}} */


#if HAVE_LIBGD20

/* {{{ _php_get_gd_image()
 */
static int _php_get_gd_image(int w, int h)
{
    zval *function_name, *width, *height;
    zval **argv[2];
    zend_function *func;
    zval *retval;
    char *function_cname = "imagecreatetruecolor";
    int ret;
    TSRMLS_FETCH();
   
    if (zend_hash_find(EG(function_table), function_cname, 
                strlen(function_cname) + 1, (void **)&func) == FAILURE) {
        zend_error(E_ERROR, "Error can't find %s function", function_cname);
    }

    MAKE_STD_ZVAL(function_name);
    MAKE_STD_ZVAL(width);
    MAKE_STD_ZVAL(height);

    ZVAL_STRING(function_name, function_cname, 0);
    ZVAL_LONG(width, w);
    ZVAL_LONG(height, h);

    argv[0] = &width;
    argv[1] = &height;

    if (call_user_function_ex(EG(function_table), NULL, function_name, 
                &retval, 2, argv, 0, NULL TSRMLS_CC) == FAILURE) {
        zend_error(E_ERROR, "Error calling %s function", function_cname);
    }
    
    FREE_ZVAL(function_name); 
    FREE_ZVAL(width); 
    FREE_ZVAL(height); 
    
    if (!retval || retval->type != IS_RESOURCE) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR,
                "Error creating GD Image");
    }

    ret = retval->value.lval;
    zend_list_addref(ret); 
    if (retval) {
        zval_ptr_dtor(&retval);
    }

    return ret;
}
/* }}} */


/* {{{ _php_avframe_to_gd_image()
 */
static int _php_avframe_to_gd_image(AVFrame *frame, gdImage *dest,
        int width, int height)
{
    int x, y;
    long int *src = (long int*)frame->data[0];

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            gdImageSetPixel(dest, x, y, src[x] & 0x00ffffff);
        }
        src += width;
    }
    return 0;
}
/* }}} */


/* {{{ proto resource toGDImage()
 */
FFMPEG_PHP_METHOD(ffmpeg_frame, toGDImage)
{
    ff_frame_context *ff_frame;
    gdImage *gd_img;

    GET_FRAME_RESOURCE(getThis(), ff_frame);

    _php_convert_frame(ff_frame, PIX_FMT_RGBA32);

    return_value->value.lval = _php_get_gd_image(ff_frame->width, 
            ff_frame->height);

    return_value->type = IS_RESOURCE;

    FFMPEG_PHP_FETCH_IMAGE_RESOURCE(gd_img, &return_value);

    if (_php_avframe_to_gd_image(ff_frame->av_frame, gd_img,
            ff_frame->width, ff_frame->height)) {
        zend_error(E_ERROR, "failed to convert frame to gd image");
    }
}
/* }}} */


#endif /* HAVE_LIBGD20 */


/* {{{ proto int getPresentationTimestamp()
 */
FFMPEG_PHP_METHOD(ffmpeg_frame, getPresentationTimestamp)
{
    ff_frame_context *ff_frame;

    GET_FRAME_RESOURCE(getThis(), ff_frame);
    
    RETURN_DOUBLE((double)ff_frame->pts / AV_TIME_BASE);
}
/* }}} */


/* {{{ proto int isKeyFrame()
 */
FFMPEG_PHP_METHOD(ffmpeg_frame, isKeyFrame)
{
    ff_frame_context *ff_frame;

    GET_FRAME_RESOURCE(getThis(), ff_frame);
    
    RETURN_LONG(ff_frame->keyframe);
}
/* }}} */


/* {{{ proto int getWidth()
 */
FFMPEG_PHP_METHOD(ffmpeg_frame, getWidth)
{
    ff_frame_context *ff_frame;

    GET_FRAME_RESOURCE(getThis(), ff_frame);
    
    RETURN_LONG(ff_frame->width);
}
/* }}} */


/* {{{ proto int getHeight()
 */
FFMPEG_PHP_METHOD(ffmpeg_frame, getHeight)
{
    ff_frame_context *ff_frame;

    GET_FRAME_RESOURCE(getThis(), ff_frame);
    
    RETURN_LONG(ff_frame->height);
}
/* }}} */


/* {{{ _php_resample_frame()
 */
int _php_resample_frame(ff_frame_context *ff_frame, 
        int dst_width, int dst_height)
{
#ifndef HAVE_SWSCALER /* No swscaler, use img_convert */
    AVFrame *resampled_frame;
    ImgReSampleContext *img_resample_ctx = NULL;

    if (!ff_frame->av_frame) {
        return -1;
    }

    // do nothing if dimensions are same as current
    if (dst_width == ff_frame->width && dst_height == ff_frame->height) {
        return 0;
    }

    /* convert to PIX_FMT_YUV420P required for resampling */
    _php_convert_frame(ff_frame, PIX_FMT_YUV420P);

    img_resample_ctx = img_resample_full_init(dst_width, dst_height,
            ff_frame->width, ff_frame->height, 0, 0, 0, 0, 0, 0, 0, 0);

    if (!img_resample_ctx) {
        return -1;
    }

    resampled_frame = avcodec_alloc_frame();
    avpicture_alloc((AVPicture*)resampled_frame, PIX_FMT_YUV420P,
            dst_width, dst_height);

    img_resample(img_resample_ctx, (AVPicture*)resampled_frame,
            (AVPicture*)ff_frame->av_frame);

    _php_free_av_frame(ff_frame->av_frame);

    img_resample_close(img_resample_ctx);

    ff_frame->av_frame = resampled_frame;
    ff_frame->width = dst_width;
    ff_frame->height = dst_height;
#else
    AVFrame *src_frame;
    AVFrame *dst_frame;
    struct SwsContext *sws_ctx = NULL;
    int result = 0;

    if (!ff_frame->av_frame) {
        return -1;
    }

    src_frame = ff_frame->av_frame;

    dst_frame = avcodec_alloc_frame();
    avpicture_alloc((AVPicture*)dst_frame, PIX_FMT_RGBA32, ff_frame->width,
            ff_frame->height);

    /* TODO: Try to get cached sws_context first */
    sws_ctx = sws_getContext(ff_frame->width, ff_frame->height, PIX_FMT_RGBA32,
            dst_width, dst_height, ff_frame->pixel_format, 
            SWS_BICUBIC, NULL, NULL, NULL);

    if (sws_ctx == NULL){
        return 1;
    }

    result = sws_scale(sws_ctx, src_frame->data, src_frame->linesize, 0,
            ff_frame->height, dst_frame->data, dst_frame->linesize);

    sws_freeContext(sws_ctx);

    if (result) {
        zend_error(E_ERROR, "Error converting frame");
        goto fail;
    }

    ff_frame->av_frame = dst_frame;
    //ff_frame_ctx->pixel_format = dst_fmt;

fail:
    _php_free_av_frame(src_frame);
    return result;

#endif /* HAVE_SWSCALER */

    return 0;
}
/* }}} */


/* {{{ proto boolean resize(int width, int height)
*/
FFMPEG_PHP_METHOD(ffmpeg_frame, resize)
{
    zval ***argv;
    ff_frame_context *ff_frame;
    int wanted_width = 0, wanted_height = 0;

    GET_FRAME_RESOURCE(getThis(), ff_frame);

    /* retrieve arguments */
    argv = (zval ***) safe_emalloc(sizeof(zval **), ZEND_NUM_ARGS(), 0);

    if (zend_get_parameters_array_ex(ZEND_NUM_ARGS(), argv) != SUCCESS) {
        efree(argv);
        php_error_docref(NULL TSRMLS_CC, E_ERROR,
                "Error parsing arguments");
    }

    if (ZEND_NUM_ARGS() != 2) {
        WRONG_PARAM_COUNT
    }
 
    /* width arg */
    convert_to_long_ex(argv[0]);
    wanted_width = Z_LVAL_PP(argv[0]);

    /* bounds check wanted width */
    if (wanted_width < 1) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR,
                "Frame width must be greater than zero");
    }

    /* wanted width must be even number for lavc resample */
    if (wanted_width % 2) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR,
                "Frame width must be an even number");
    }
   
    /* height arg */
    convert_to_long_ex(argv[1]);
    wanted_height = Z_LVAL_PP(argv[1]);

    /* bounds check wanted height */
    if (wanted_height < 1) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR,
                "Frame height must be greater than zero");
    }

    /* wanted height must be even number for lavc resample */
    if (wanted_height % 2) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR,
                "Frame height must be an even number");
    }

    efree(argv);

    /* resize frame */
    _php_resample_frame(ff_frame, wanted_width, wanted_height);

    RETURN_TRUE;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4
 * vim<600: noet sw=4 ts=4
 */
