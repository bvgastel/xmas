/*
    Copyright 2015  xmas-design
	// we don't have a central email address Authors <xmas-design-devel@kde.org>

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the Free
	Software Foundation; version 3 or any later version

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ui_window_main.h"

constexpr int MENU_BAR_HEIGHT {30};     // test C++11 constexpr feature

MainWindow::MainWindow(int width, int height, const char* title) : Fl_Window(width, height, title)
{
    begin();
        Fl_Menu_Bar *menu = new Fl_Menu_Bar(0,0,this->w(),MENU_BAR_HEIGHT);
        menu->add("&File/&Open", FL_ALT+'o', open_callback,0,FL_MENU_DIVIDER);
        menu->add("&File/E&xit", FL_ALT+FL_F+4, exit_callback,0,0);
        Fl_Group *grp = new Fl_Group(0,MENU_BAR_HEIGHT,this->w(),this->h()-MENU_BAR_HEIGHT);
        grp->end();
    end();
    resizable(grp);
    show();
}

MainWindow::~MainWindow()
{
}

/*
   Handles the open event
*/
void MainWindow::open_callback(Fl_Widget*, void* v) {
  char *newfile;
  char  filename[1024] = "";
  newfile = fl_file_chooser("Open File?", "*.wck", filename);
  if (newfile != NULL) fl_message("File Opened!");

}

/*
   Handles the exit event
*/
void MainWindow::exit_callback(Fl_Widget*, void* v) {
  exit(0);
}



