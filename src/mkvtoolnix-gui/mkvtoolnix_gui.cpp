#include "common/common_pch.h"

#include <QApplication>
#include <QProcess>

#include "mkvtoolnix-gui/app.h"
#include "mkvtoolnix-gui/job_widget/job.h"
#include "mkvtoolnix-gui/mkvtoolnix_gui.h"
#include "mkvtoolnix-gui/main_window/main_window.h"

static void
registerMetaTypes() {
  qRegisterMetaType<Job::Status>("Job::Status");
  qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
}

int
main(int argc,
     char **argv) {
  registerMetaTypes();

  App app(argc, argv);

  MainWindow mainWindow;
  mainWindow.show();

  app.exec();

  mxexit(0);
}
