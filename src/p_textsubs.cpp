/*
  mkvmerge -- utility for splicing together matroska files
      from component media subtypes

  r_srt.cpp

  Written by Moritz Bunkus <moritz@bunkus.org>

  Distributed under the GPL
  see the file COPYING for details
  or visit http://www.gnu.org/copyleft/gpl.html
*/

/*!
    \file
    \version $Id$
    \brief Subripper subtitle reader
    \author Moritz Bunkus <moritz@bunkus.org>
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "pr_generic.h"
#include "p_textsubs.h"
#include "matroska.h"

using namespace libmatroska;

textsubs_packetizer_c::textsubs_packetizer_c(generic_reader_c *nreader,
                                             const char *ncodec_id,
                                             const void *nglobal_data,
                                             int nglobal_size, bool nrecode,
                                             bool is_utf8, track_info_t *nti)
  throw (error_c): generic_packetizer_c(nreader, nti) {
  packetno = 0;
  recode = nrecode;
  if (recode) {
    if ((ti->sub_charset != NULL) || !is_utf8)
      cc_utf8 = utf8_init(ti->sub_charset);
    else
      cc_utf8 = utf8_init("UTF-8");
  }
  
  global_size = nglobal_size;
  global_data = safememdup(nglobal_data, global_size);
  codec_id = safestrdup(ncodec_id);

  set_track_type(track_subtitle);
}

textsubs_packetizer_c::~textsubs_packetizer_c() {
  safefree(global_data);
  safefree(codec_id);
}

void textsubs_packetizer_c::set_headers() {
  set_codec_id(codec_id);
  if (global_data != NULL)
    set_codec_private((unsigned char *)global_data, global_size);

  generic_packetizer_c::set_headers();

  track_entry->EnableLacing(false);
}

int textsubs_packetizer_c::process(unsigned char *_subs, int, int64_t start,
                                   int64_t length, int64_t, int64_t) {
  int num_newlines;
  char *subs, *idx1, *idx2, *utf8_subs;
  int64_t end;

  end = start + length;
  // Adjust the start and end values according to the audio adjustment.
  start += ti->async.displacement;
  start = (int64_t)(ti->async.linear * start);
  end += ti->async.displacement;
  end = (int64_t)(ti->async.linear * end);

  if (end < 0)
    return EMOREDATA;
  else if (start < 0)
    start = 0;

  if (length < 0) {
    mxwarn("textsubs_packetizer: Ignoring an entry which starts after it ends."
           "\n");
    return EMOREDATA;
  }

  // Count the number of lines.
  idx1 = (char *)_subs;
  subs = NULL;
  num_newlines = 0;
  while (*idx1 != 0) {
    if (*idx1 == '\n')
      num_newlines++;
    idx1++;
  }
  subs = (char *)safemalloc(strlen((char *)_subs) + num_newlines * 2 + 1);

  // Unify the new lines into DOS style newlines.
  idx1 = (char *)_subs;
  idx2 = subs;
  while (*idx1 != 0) {
    if (*idx1 == '\n') {
      *idx2 = '\r';
      idx2++;
      *idx2 = '\n';
      idx2++;
    } else if (*idx1 != '\r') {
      *idx2 = *idx1;
      idx2++;
    }
    idx1++;
  }
  if (idx2 != subs) {
    while (((idx2 - 1) != subs) &&
           ((*(idx2 - 1) == '\n') || (*(idx2 - 1) == '\r'))) {
      *idx2 = 0;
      idx2--;
    }
  }
  *idx2 = 0;

  if (recode) {
    utf8_subs = to_utf8(cc_utf8, subs);
    add_packet((unsigned char *)utf8_subs, strlen(utf8_subs), start, length,
               true);
    safefree(utf8_subs);
  } else
    add_packet((unsigned char *)subs, strlen(subs), start, length, true);

  safefree(subs);

  return EMOREDATA;
}

void textsubs_packetizer_c::dump_debug_info() {
  mxdebug("textsubs_packetizer_c: queue: %d\n",
          packet_queue.size());
}
