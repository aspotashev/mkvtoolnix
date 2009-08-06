/*
   mkvmerge GUI -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   command line options dialog definitions

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#ifndef __MMG_CLI_OPTIONS_DLG_H
#define __MMG_CLI_OPTIONS_DLG_H

#include "common/os.h"

#include <vector>
#include <wx/wx.h>

#include "mmg/mmg.h"
#include "common/wx.h"

#define ID_CLIOPTIONS_COB 2000
#define ID_CLIOPTIONS_ADD 2001

struct cli_option_t {
  wxString option, description;

  cli_option_t(const wxString &n_option,
               const wxString &n_description)
    : option(n_option)
    , description(n_description) {
  };
};

class cli_options_dlg: public wxDialog {
  DECLARE_CLASS(cli_options_dlg);
  DECLARE_EVENT_TABLE();
public:
  static std::vector<cli_option_t> all_cli_options;

public:
  wxMTX_COMBOBOX_TYPE *cob_option;
  wxTextCtrl *tc_options, *tc_description;

public:
  cli_options_dlg(wxWindow *parent);
  void on_option_changed(wxCommandEvent &evt);
  void on_add_clicked(wxCommandEvent &evt);
  bool go(wxString &options);

public:
  static void init_cli_option_list();
};

#endif  // __MMG_CLI_OPTIONS_DLG_H
