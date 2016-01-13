#ifndef _TSSP_EIT_SECTION_FILTER_IMPL_HPP_
#define _TSSP_EIT_SECTION_FILTER_IMPL_HPP_

#include "context.hpp"
#include "section.hpp"
#include "eit.hpp"

namespace tssp
{

void eit_section_filter::do_handle_section(
    context& c,
    const char* section_buffer,
    size_t section_length) {
  section s;
  s.unpack(section_buffer, section_length);
  event_information_table eit;
  s.convert(eit);

  c.get_view().print(
      c.get_packet_num(),
      s.header,
      eit,
      subtable_is_changed(s.header, eit));

  // FIXME
  if(s.header.section_number == s.header.last_section_number) {
    version_
      [s.header.table_id]
      [s.header.table_id_extension]
      [eit.transport_stream_id]
      [eit.original_network_id]
        = s.header.version;
  }
}

inline
bool eit_section_filter::subtable_is_changed(
    const section_header& header,
    const event_information_table& eit) {
  auto i = version_.find(
      header.table_id);
  if(i == version_.end())
    return true;

  auto j = i->second.find(
      header.table_id_extension);
  if(j == i->second.end())
    return true;

  auto k = j->second.find(
      eit.transport_stream_id);
  if(k == j->second.end())
    return true;

  auto l = k->second.find(
      eit.original_network_id);

  return 
    l == k->second.end() ||
    l->second != header.version;
}

}

#endif