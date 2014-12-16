#include <FL/Fl.H>
#include "include/ui_window_main.h"

int main ()
{
  MainWindow *window;

  window = new MainWindow (800, 600, "xMAS");
  window->show();

  return Fl::run();
}

