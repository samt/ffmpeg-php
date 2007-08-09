/*
   movieinfo - a simple utility for getting info from movie files.

   Copyright (C) 2005 Todd Kirby (ffmpeg.php@gmail.com)

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
#include "quadrupel/qp_util.h"
#include "quadrupel/qp_movie.h"

#define VERSION 0.4

char* prog_name;

static void usage(char *prog_name)
{
  printf("Usage: %s [OPTIONS]... MOVIE_FILES...\n", prog_name);
  printf("\t-terse  Print output in a format that is easily parsable by scripts.\n");
  printf("\t-help   Output this help text.\n");
}


int main (int argc, char** argv)
{
    qp_movie_context *movie_ctx = NULL;
    char *filename;
    int err = 0, terse = 0;

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

    // init quadrupel
    quadrupel_init();

    // parse unamed arguments
    while (argc > 0 && argv[0][0] != '-') {
        filename = argv[0];

        movie_ctx = qp_alloc_movie_ctx(NULL);

        if (qp_open_movie_file(movie_ctx, filename, 0)) {
            err = -1;
            goto done;
        }

        if (qp_has_video(movie_ctx)) {
            if (terse) {
                fprintf(stdout, "%d ",    qp_get_movie_width(movie_ctx));
                fprintf(stdout, "%d ",    qp_get_movie_height(movie_ctx));
                fprintf(stdout, "%0.2f ", qp_get_frame_rate(movie_ctx));
                fprintf(stdout, "%0.2f ", qp_get_duration(movie_ctx));
                fprintf(stdout, "%ld ",   qp_get_frame_count(movie_ctx));
                fprintf(stdout, "%0.3f ", qp_get_pixel_aspect_ratio(movie_ctx));
                fprintf(stdout, "%d\n",   qp_has_audio(movie_ctx));
            } else {
                fprintf(stdout, "width:        %d\n",    qp_get_movie_width(movie_ctx));
                fprintf(stdout, "height:       %d\n",    qp_get_movie_height(movie_ctx));
                fprintf(stdout, "frame rate:   %0.2f\n", qp_get_frame_rate(movie_ctx));
                fprintf(stdout, "duration:     %0.2f\n", qp_get_duration(movie_ctx));
                fprintf(stdout, "frame count:  %ld\n",   qp_get_frame_count(movie_ctx));
                fprintf(stdout, "pixel aspect: %0.3f\n",   qp_get_pixel_aspect_ratio(movie_ctx));
                fprintf(stdout, "has audio:    %d\n\n",  qp_has_audio(movie_ctx));
            }
        } else {
            fprintf(stderr, "Can't find a video stream in this file\n");
            err = -1;
        }

        if (movie_ctx) {
            qp_free_movie_ctx(movie_ctx, NULL);
            movie_ctx = NULL;
        }

        argc--;
        argv++;
    }

done:
    // Clean up qp stuff 
    if (movie_ctx) {
        qp_free_movie_ctx(movie_ctx, NULL);
        movie_ctx = NULL;
    }

    quadrupel_shutdown();

    exit(err);
}   
