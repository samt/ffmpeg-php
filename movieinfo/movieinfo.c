/*
   movieinfo - a simple utility for getting info from movie files.

   Copyright (C) 2004,2005 Todd Kirby (ffmpeg.php@gmail.com)

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <ffmpeg/avformat.h>

#define VERSION 0.1

#define LRINT(x) ((long) ((x)+0.5))

char* prog_name;

#define STREAM_NOT_FOUND -1
static int get_stream_index(AVFormatContext *fmt_ctx, int type)
{
    int i;

    for (i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i] &&
                fmt_ctx->streams[i]->codec.codec_type == type) {
            return i;
        }
    }

    return STREAM_NOT_FOUND;
}


static AVStream *get_video_stream(AVFormatContext *fmt_ctx)
{
    int i = get_stream_index(fmt_ctx, CODEC_TYPE_VIDEO);
    return i == STREAM_NOT_FOUND ? NULL : fmt_ctx->streams[i];
}


static AVStream *get_audio_stream(AVFormatContext *fmt_ctx)
{
    int i = get_stream_index(fmt_ctx, CODEC_TYPE_AUDIO);
    return i == STREAM_NOT_FOUND ? NULL : fmt_ctx->streams[i];
}


static float get_duration(AVFormatContext *fmt_ctx)
{
    float duration;

    duration = (float)fmt_ctx->duration / AV_TIME_BASE;

    if (duration < 0) {
        duration = 0.0f;
    }
    return duration;
}


static float get_framerate(AVStream *st)
{
#if LIBAVCODEC_BUILD > 4753
    return av_q2d(st->r_frame_rate);
#else
    return (float)st->r_frame_rate / st->r_frame_rate_base;
#endif
}


static long get_framecount(AVFormatContext *fmt_ctx, AVStream *st)
{
    return LRINT(get_framerate(st) * get_duration(fmt_ctx));
}


static int has_audio(AVFormatContext *fmt_ctx)
{
    return get_audio_stream(fmt_ctx) ? 1 : 0;
}


static void usage(char *prog_name)
{
  printf("Usage: %s [OPTIONS]... MOVIE_FILES...\n", prog_name);
  printf("\t-t -terse  Print output in a format that is easily parsable by scripts.\n");
  printf("\t-h -help   Output this help text.\n");
}


int main (int argc, char** argv)
{
    AVFormatContext *fmt_ctx = NULL;
    AVFormatParameters params;
    AVStream *st;
    char *filename;
    int err = 0, terse = 0;

    // TODO: Add formatted and unformatted views.
    // parse args
    /* set program name */
    prog_name = argv[0];
    argc--;
    argv++;

    if (argc < 1) {
        usage(prog_name);
        exit(0);
    }

    /* Parse named arguments */
    while (argc > 0 && argv[0][0] == '-') {
        if (strcmp(argv[0], "-terse") == 0) {
            terse = 1;
        } else if (strcmp(argv[0], "-help") == 0) {
            usage(prog_name);
            exit(0);
        } else {
            fprintf(stderr,"%s: unrecognized option: %s (try -help)\n",
                    prog_name, argv[0]);
            exit(1);
        }
        argc--;
        argv++;
    }

    // init ffmpeg libs
    av_register_all();

    // parse unamed arguments
    while (argc > 0 && argv[0][0] != '-') {
        filename = argv[0];

        /* open the file with generic libav function */
        if (av_open_input_file(&fmt_ctx, filename, NULL, 0, &params)) {
            err = -1;
            goto done;
        }

        /* If not enough info to get the stream parameters, decode the
         *        first frames to get it. */
        av_find_stream_info(fmt_ctx);

        st = get_video_stream(fmt_ctx);

        if (st) {
            if (terse) {
                fprintf(stdout, "%d ",    st->codec.width);
                fprintf(stdout, "%d ",    st->codec.height);
                fprintf(stdout, "%0.2f ", get_framerate(st));
                fprintf(stdout, "%0.2f ", get_duration(fmt_ctx));
                fprintf(stdout, "%ld ",   get_framecount(fmt_ctx, st));
                fprintf(stdout, "%d\n",   has_audio(fmt_ctx));
            } else {
                fprintf(stdout, "width:       %d\n",    st->codec.width);
                fprintf(stdout, "height:      %d\n",    st->codec.height);
                fprintf(stdout, "frame rate:  %0.2f\n", get_framerate(st));
                fprintf(stdout, "duration:    %0.2f\n", get_duration(fmt_ctx));
                fprintf(stdout, "frame count: %ld\n",   get_framecount(fmt_ctx, st));
                fprintf(stdout, "has audio:   %d\n\n",  has_audio(fmt_ctx));
            }
        } else {
            fprintf(stderr, "Can't find a video stream in this file\n");
            err = -1;
        }

        av_close_input_file(fmt_ctx);
        fmt_ctx = NULL;

        argc--;
        argv++;
    }

done:
    // Clean up ffmpeg stuff 
    av_free_static();
    if (fmt_ctx) {
        av_close_input_file(fmt_ctx);
    }

    exit(err);
}   
