/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL v2
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   class definition for the AAC output module

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#ifndef MTX_P_AAC_H
#define MTX_P_AAC_H

#include "common/common_pch.h"

#include "common/aac.h"
#include "common/samples_timecode_conv.h"
#include "merge/generic_packetizer.h"

class aac_packetizer_c: public generic_packetizer_c {
private:
  int64_t m_packetno, m_last_timecode, m_num_packets_same_tc;
  int m_samples_per_sec, m_channels, m_id, m_profile;
  bool m_headerless;
  samples_to_timecode_converter_c m_s2tc;
  int64_t m_single_packet_duration;
  aac::parser_c m_parser;

public:
  aac_packetizer_c(generic_reader_c *p_reader, track_info_c &p_ti, int id, int profile, int samples_per_sec, int channels, bool headerless);
  virtual ~aac_packetizer_c();

  virtual int process(packet_cptr packet);
  virtual void set_headers();

  virtual translatable_string_c get_format_name() const {
    return YT("AAC");
  }

  virtual connection_result_e can_connect_to(generic_packetizer_c *src, std::string &error_message);

private:
  virtual int process_headerless(packet_cptr packet);
};

#endif // MTX_P_AAC_H
