/*
   ffmpeg-php - a php module for accessing video info from movie files.
   
   Copyright (C) 2004  Todd Kirby (doubleshot at pacbell dot net)

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
 */

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include <assert.h>
#include <avcodec.h>
#include <avformat.h>
#include <inttypes.h>
#include <math.h>

#include "php.h"
#include "php_ini.h"
#include "php_globals.h"
#include "ext/standard/info.h"

#include "php_ffmpeg.h"

/* FIXME: make this work without the ../../ when compiling standalone  */
#if HAVE_GD_BUNDLED
  #include "../../ext/gd/libgd/gd.h"
#else
  #include "gd.h"
#endif

#define INBUF_SIZE 4096

#define GET_MOVIE_RESOURCE(im) {\
	zval **_tmp_zval;\
    if (zend_hash_find(Z_OBJPROP_P(getThis()), "ffmpeg_movie",\
                sizeof("ffmpeg_movie"), (void **)&_tmp_zval) == FAILURE) {\
        zend_error(E_ERROR, "Unable to find ffmpeg_movie property");\
        RETURN_FALSE;\
    }\
\
    ZEND_FETCH_RESOURCE(im, ffmpeg_movie_context*, _tmp_zval, -1,\
            "ffmpeg_movie", le_ffmpeg_movie);\
}\


static int le_ffmpeg_movie;

static int le_gd;

static zend_class_entry *ffmpeg_movie_class_entry_ptr;

zend_class_entry ffmpeg_movie_class_entry;

typedef struct {
    AVFormatContext* ic;
} ffmpeg_movie_context;


/* {{{ ffmpeg_movie methods[]
    Methods of the ffmpeg_movie class 
*/
zend_function_entry ffmpeg_movie_class_methods[] = {
   
	PHP_FE(ffmpeg_movie, NULL)

	PHP_FE(getDuration, NULL)
    PHP_FALIAS(getduration, getDuration, NULL)

	PHP_FE(getFrameCount, NULL)
    PHP_FALIAS(getframecount, getFrameCount, NULL)

	PHP_FE(getFrameRate, NULL)
    PHP_FALIAS(getframerate, getFrameRate, NULL)
    
	PHP_FE(getFileName, NULL)
    PHP_FALIAS(getfilename, getFileName, NULL)
    
	PHP_FE(getComment, NULL)
    PHP_FALIAS(getcomment, getComment, NULL)
 
	PHP_FE(getTitle, NULL)
    PHP_FALIAS(gettitle, getTitle, NULL)

    PHP_FE(getAuthor, NULL)
    PHP_FALIAS(getauthor, getAuthor, NULL)
 
	PHP_FE(getCopyright, NULL)
    PHP_FALIAS(getcopyright, getCopyright, NULL)

    PHP_FE(getFrameWidth, NULL)
    PHP_FALIAS(getframewidth, getFrameWidth, NULL)

    PHP_FE(getFrameHeight, NULL)
    PHP_FALIAS(getframeheight, getFrameHeight, NULL)

#if HAVE_LIBGD20
    PHP_FE(getFrame, NULL)
    PHP_FALIAS(getframe, getFrame, NULL)
#endif /* HAVE_LIBGD20 */

	{NULL, NULL, NULL}
};
/* }}} */

zend_module_entry ffmpeg_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"ffmpeg",
	NULL,
	PHP_MINIT(ffmpeg),
	PHP_MSHUTDOWN(ffmpeg),
	NULL,
	NULL,
	PHP_MINFO(ffmpeg),
#if ZEND_MODULE_API_NO >= 20010901
	"0.2", /* version number for ffmpeg-php */
#endif
	STANDARD_MODULE_PROPERTIES
};


#ifdef COMPILE_DL_FFMPEG
ZEND_GET_MODULE(ffmpeg)
#endif

/* {{{ _php_free_ffmpeg_movie
 */
static void _php_free_ffmpeg_movie(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    ffmpeg_movie_context *input = (ffmpeg_movie_context*)rsrc->ptr;    
    
    av_close_input_file(input->ic);
	efree(input);
}
/* }}} */


/* {{{ _php_get_stream_index()
 */
static int _php_get_stream_index(AVFormatContext *ic, int type)
{
    int i;
    
    for (i = 0; i < ic->nb_streams; i++) {
        if (ic->streams[i] && ic->streams[i]->codec.codec_type == type) {
            return i;
        }
    }
    /* stream not found */
    return -1;
}
/* }}} */


/* {{{ _php_get_video_stream()
 */
static AVStream *_php_get_video_stream(AVFormatContext *ic)
{
    int i = _php_get_stream_index(ic, CODEC_TYPE_VIDEO);
    
    return i < 0 ? NULL : ic->streams[i];
}
/* }}} */


/* {{{ php module init function
 */
PHP_MINIT_FUNCTION(ffmpeg)
{
	le_ffmpeg_movie = zend_register_list_destructors_ex(_php_free_ffmpeg_movie,
            NULL, "ffmpeg_movie", module_number);

    INIT_CLASS_ENTRY(ffmpeg_movie_class_entry, "ffmpeg_movie", 
            ffmpeg_movie_class_methods);
    
    /* register ffmpeg movie class */
    ffmpeg_movie_class_entry_ptr = 
        zend_register_internal_class(&ffmpeg_movie_class_entry TSRMLS_CC);

    /* must be called before using avcodec libraries. */ 
    avcodec_init();

    /* register all codecs */
    av_register_all();
    return SUCCESS;
}
/* }}} */


/* {{{ php module shutdown function
 */
PHP_MSHUTDOWN_FUNCTION(ffmpeg)
{
    av_free_static();
	return SUCCESS;
}
/* }}} */


/* {{{ php info function
   Add an entry for ffmpeg support in phpinfo() */
PHP_MINFO_FUNCTION(ffmpeg)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ffmpeg support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ _php_open_movie_file()
 */
static void _php_open_movie_file(ffmpeg_movie_context *ffmovie_ctx, 
        char* filename)
{
    AVFormatParameters params;

    /* open the file with generic libav function */
    if (av_open_input_file(&(ffmovie_ctx->ic), filename, NULL, 0, &params)) {
        zend_error(E_ERROR, "Can't open movie file %s", filename);
    }
    
    /* If not enough info to get the stream parameters, we decode the
       first frames to get it. */
    if (av_find_stream_info(ffmovie_ctx->ic)) {
        zend_error(E_ERROR, "Can't find codec parameters for %s", filename);
    }
}
/* }}} */


/* {{{ proto object ffmpeg_movie(string filename) 
   Constructor for ffmpeg_movie objects
 */
PHP_FUNCTION(ffmpeg_movie)
{
    int argc, ret;
    zval **argv[0];
    ffmpeg_movie_context *ffmovie_ctx;
    
    /* get the number of arguments */
    argc = ZEND_NUM_ARGS();

    if(argc != 1) {
        WRONG_PARAM_COUNT;
    }

    /* argument count is correct, now retrieve arguments */
    if(zend_get_parameters_array_ex(argc, argv) != SUCCESS) {
        WRONG_PARAM_COUNT;
    }
  
	ffmovie_ctx = emalloc(sizeof(ffmpeg_movie_context));
    
    convert_to_string_ex(argv[0]);
    
    _php_open_movie_file(ffmovie_ctx, Z_STRVAL_PP(argv[0]));

    /* pass NULL for resource result since we're not returning the resource
     directly, but adding it to the returned object. */
	ret = ZEND_REGISTER_RESOURCE(NULL, ffmovie_ctx, le_ffmpeg_movie);
    
    object_init_ex(getThis(), &ffmpeg_movie_class_entry);
    add_property_resource(getThis(), "ffmpeg_movie", ret);
}
/* }}} */


/* {{{ proto int getDuration()
 */
PHP_FUNCTION(getDuration)
{
    ffmpeg_movie_context *ffmovie_ctx;
       
    GET_MOVIE_RESOURCE(ffmovie_ctx);
    
    RETURN_DOUBLE((float)ffmovie_ctx->ic->duration / AV_TIME_BASE);
}
/* }}} */


/* {{{ _php_get_framecount()
 */
static long _php_get_framecount(ffmpeg_movie_context *ffmovie_ctx)
{
    float duration = 0.0, frame_rate = 0.0;
    AVStream *st = _php_get_video_stream(ffmovie_ctx->ic);

    duration = (float)ffmovie_ctx->ic->duration / AV_TIME_BASE;
    frame_rate = (float)st->codec.frame_rate / st->codec.frame_rate_base;

    return lrint(frame_rate * duration);
}
/* }}} */


/* {{{ proto int getFrameCount()
 */
PHP_FUNCTION(getFrameCount)
{
    ffmpeg_movie_context *ffmovie_ctx;
    GET_MOVIE_RESOURCE(ffmovie_ctx);
    RETURN_LONG(_php_get_framecount(ffmovie_ctx));
}
/* }}} */


/* {{{ _php_get_framerate()
 */
static float _php_get_framerate(ffmpeg_movie_context *ffmovie_ctx)
{
    AVStream *st = _php_get_video_stream(ffmovie_ctx->ic);

    return (float)st->codec.frame_rate / st->codec.frame_rate_base;
}
/* }}} */


/* {{{ proto int getFrameRate()
 */
PHP_FUNCTION(getFrameRate)
{
    ffmpeg_movie_context *ffmovie_ctx;
    GET_MOVIE_RESOURCE(ffmovie_ctx);
    RETURN_DOUBLE(_php_get_framerate(ffmovie_ctx));
}
/* }}} */


/* {{{ _php_get_filename()
 */
static char* _php_get_filename(ffmpeg_movie_context *ffmovie_ctx)
{
    return ffmovie_ctx->ic->filename;
}
/* }}} */


/* {{{ proto string getFileName()
 */
PHP_FUNCTION(getFileName)
{
    ffmpeg_movie_context *ffmovie_ctx;
    char* filename;
    
    GET_MOVIE_RESOURCE(ffmovie_ctx);

    filename = _php_get_filename(ffmovie_ctx);
    RETURN_STRINGL(filename, strlen(filename), 1);
}
/* }}} */


/* {{{ proto string getComment()
 */
PHP_FUNCTION(getComment)
{
    ffmpeg_movie_context *ffmovie_ctx;

    GET_MOVIE_RESOURCE(ffmovie_ctx);
    
    RETURN_STRINGL(ffmovie_ctx->ic->comment, strlen(ffmovie_ctx->ic->comment), 1);
}
/* }}} */


/* {{{ proto string getTitle()
 */
PHP_FUNCTION(getTitle)
{
    ffmpeg_movie_context *ffmovie_ctx;
    
    GET_MOVIE_RESOURCE(ffmovie_ctx);

    RETURN_STRINGL(ffmovie_ctx->ic->title, strlen(ffmovie_ctx->ic->title), 1);
}
/* }}} */


/* {{{ proto string getAuthor()
 */
PHP_FUNCTION(getAuthor)
{
    ffmpeg_movie_context *ffmovie_ctx;
    
    GET_MOVIE_RESOURCE(ffmovie_ctx);

    RETURN_STRINGL(ffmovie_ctx->ic->author, strlen(ffmovie_ctx->ic->author), 1);
}
/* }}} */


/* {{{ proto string getCopyright()
 */
PHP_FUNCTION(getCopyright)
{
    ffmpeg_movie_context *ffmovie_ctx;
    
    GET_MOVIE_RESOURCE(ffmovie_ctx);

    RETURN_STRINGL(ffmovie_ctx->ic->copyright, strlen(ffmovie_ctx->ic->author), 1);
}
/* }}} */

/* {{{ _php_get_framewidth()
 */
static float _php_get_framewidth(ffmpeg_movie_context *ffmovie_ctx)
{
    AVStream *st = _php_get_video_stream(ffmovie_ctx->ic);

    return st->codec.width;
}
/* }}} */


/* {{{ proto int getFrameWidth()
 */
PHP_FUNCTION(getFrameWidth) {
    ffmpeg_movie_context *ffmovie_ctx;
    GET_MOVIE_RESOURCE(ffmovie_ctx);
    RETURN_LONG(_php_get_framewidth(ffmovie_ctx));
}


/* {{{ _php_get_frameheight()
 */
static float _php_get_frameheight(ffmpeg_movie_context *ffmovie_ctx)
{
    AVStream *st = _php_get_video_stream(ffmovie_ctx->ic);

    return st->codec.height;
}
/* }}} */


/* {{{ proto int getFrameHeight()
 */
PHP_FUNCTION(getFrameHeight) {
    ffmpeg_movie_context *ffmovie_ctx;
    GET_MOVIE_RESOURCE(ffmovie_ctx);
    RETURN_LONG(_php_get_frameheight(ffmovie_ctx));
}
/* }}} */


/* {{{ _php_get_pixelformat()
 */
/*
   static float _php_get_pixelformat(ffmpeg_movie_context *im)
{
    AVStream *st = _php_get_video_stream(im->ic);
    zend_printf("pix fmt = %s\n", avcodec_get_pix_fmt_name(c->pix_fmt)); 
    return st->codec.height;
}
*/
/* }}} */


/* {{{ proto int getPixelFormat()
 */
/*
PHP_FUNCTION(getPixelFormat) {
    ffmpeg_movie_context *im;
    char *fmt;
    GET_MOVIE_RESOURCE(im);
   
    fmt = _php_get_pixelformat(im);
    RETURN_STRINGL(fmt, strlen(fmt), 1);
}
*/


/* {{{ _php_get_gd_image()
 */
zval* _php_get_gd_image(int w, int h)
{
    zval *function_name, *width, *height;
    zval **params[2];
    zval *return_value;
    zend_function *func;
    char *function_cname = "imagecreatetruecolor";
    
    if (zend_hash_find(EG(function_table), function_cname, 
                strlen(function_cname) + 1, (void **)&func) == FAILURE) {
        zend_error(E_ERROR, "Error can't find %s function", function_cname);
    }
    
    MAKE_STD_ZVAL(function_name);
    MAKE_STD_ZVAL(width);
    MAKE_STD_ZVAL(height);

    ZVAL_STRING(function_name, function_cname, 1);
    ZVAL_LONG(width, w);
    ZVAL_LONG(height, h);

    params[0] = &width;
    params[1] = &height;
    
    if(call_user_function_ex(EG(function_table), NULL, function_name, 
               &return_value, 2, params, 0, NULL TSRMLS_CC) == FAILURE) {
        zend_error(E_ERROR, "Error calling %s function", function_cname);
   }

   FREE_ZVAL(function_name); 
   FREE_ZVAL(width); 
   FREE_ZVAL(height); 

   return return_value;
}
/* }}} */


/* {{{ _php_rgba32_to_gd_image()
 */
int _php_rgba32_to_gd_image(int *src, gdImage *dest, int width, int height) 
{
    int x, y;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (gdImageBoundsSafe(dest, x, y)) {
                dest->tpixels[y][x] = src[x];
            } else {
                return -1;
            }
        }
        src += width;
    }
    return 0;
}
/* }}} */

#if HAVE_LIBGD20

/* {{{ proto resource getFrame(int frame)
 */
PHP_FUNCTION(getFrame)
{
	zval **argv[0], *gd_img_resource;
    gdImage *gd_img;
    int argc, frame, size, got_frame, video_stream, rgba_frame_size;
    long wanted_frame;
    uint8_t *converted_frame_buf = NULL;
    AVCodec *codec;
    AVPacket packet;
    AVFrame *decoded_frame, converted_frame, *final_frame;
    AVCodecContext *codec_ctx = NULL;
    
    ffmpeg_movie_context *ffmovie_ctx;

    /* get the number of arguments */
    argc = ZEND_NUM_ARGS();

    if(argc != 1) {
        WRONG_PARAM_COUNT;
    }

    /* retrieve arguments */ 
    if(zend_get_parameters_array_ex(argc, argv) != SUCCESS) {
        WRONG_PARAM_COUNT;
    }
   
    GET_MOVIE_RESOURCE(ffmovie_ctx);
    
    video_stream = _php_get_stream_index(ffmovie_ctx->ic, CODEC_TYPE_VIDEO);
    if (video_stream < 0) {
        zend_error(E_ERROR, "Video stream not found in %s",
                _php_get_filename(ffmovie_ctx));
    }

    /* find the decoder */
    codec = avcodec_find_decoder(
            ffmovie_ctx->ic->streams[video_stream]->codec.codec_id);
    if (!codec) {
        zend_error(E_ERROR, "Codec not found for %s", 
                _php_get_filename(ffmovie_ctx));
    }

    codec_ctx = avcodec_alloc_context();
    decoded_frame = avcodec_alloc_frame();

    /* open the decoder */
    if (avcodec_open(codec_ctx, codec) < 0) {
        /* TODO: must clean up before erroring */
        zend_error(E_ERROR, "Could not open codec for %s", 
                _php_get_filename(ffmovie_ctx));
    }

    convert_to_long_ex(argv[0]);
    wanted_frame = Z_LVAL_PP(argv[0]);

    if (wanted_frame < 1) {
        zend_error(E_ERROR, "Frame number must be greater than zero");
    }
    
    if (wanted_frame > _php_get_framecount(ffmovie_ctx)) {
        /* FIXME: rewrite so _php_get_framecount is not called twice */
        wanted_frame = _php_get_framecount(ffmovie_ctx);
        zend_error(E_WARNING, "%s only has %d frames, getting last frame.", 
                _php_get_filename(ffmovie_ctx), wanted_frame);
    }

    /* read frames looking for wanted_frame */ 
    frame = 1;
    while (av_read_frame(ffmovie_ctx->ic, &packet) >= 0) {
        
        if (packet.stream_index == video_stream) {
            avcodec_decode_video(codec_ctx, decoded_frame, &got_frame,
                    packet.data, packet.size);

            if (got_frame) {
                if (frame == wanted_frame) {
                    goto found_frame;
                }
            }
        }

        /* free the packet allocated by av_read_frame */
        av_free_packet(&packet);
        frame++;
    }
    
    zend_error(E_ERROR, "Couldn't find frame %d in %s", wanted_frame,
            _php_get_filename(ffmovie_ctx));

found_frame:
    gd_img_resource = _php_get_gd_image(codec_ctx->width, codec_ctx->height);
    
    if (!gd_img_resource || gd_img_resource->type != IS_RESOURCE) {
       zend_error(E_ERROR, "Error creating GD Image");
    }
    
    ZEND_GET_RESOURCE_TYPE_ID(le_gd, "gd");
    ZEND_FETCH_RESOURCE(gd_img, gdImagePtr, &gd_img_resource, -1, "Image", le_gd);
   
    /* make sure frame data is RGBA32 */
    if (codec_ctx->pix_fmt == PIX_FMT_RGBA32) {
        final_frame = decoded_frame;

    } else { /* convert frame to rgba */

        /* create a temporary picture for conversion to RGBA32 */
        rgba_frame_size = avpicture_get_size(PIX_FMT_RGBA32, codec_ctx->width, 
                codec_ctx->height);

        if (! (converted_frame_buf = av_malloc(rgba_frame_size)) ) {
            zend_error(E_ERROR, "Error allocating memory for RGBA conversion");
        }

        final_frame = &converted_frame;
        avpicture_fill((AVPicture*)final_frame, converted_frame_buf, 
                PIX_FMT_RGBA32, codec_ctx->width,codec_ctx->height);

        if (img_convert((AVPicture*)final_frame, PIX_FMT_RGBA32,
                    (AVPicture*)decoded_frame, codec_ctx->pix_fmt, 
                    codec_ctx->width, codec_ctx->height) < 0) {
            zend_error(E_ERROR, "Error converting frame");
        }
    }

    _php_rgba32_to_gd_image((int*)final_frame->data[0], gd_img, codec_ctx->width,
            codec_ctx->height);

    avcodec_close(codec_ctx);
    av_free(codec_ctx);
    av_free(converted_frame_buf);
    av_free(decoded_frame);
   
    RETURN_RESOURCE(gd_img_resource->value.lval);
}
/* }}} */

#endif /* HAVE_LIBGD20 */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
