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

#include "include/ui_window_main.h"

#define MENU_BAR_HEIGHT 30

MainWindow::MainWindow(int width, int height, const char* title) : Fl_Window(width, height, title)
{
    begin();
        Fl_Menu_Bar *menu = new Fl_Menu_Bar(0,0,this->w(),MENU_BAR_HEIGHT);
        menu->add("&File/&New", FL_ALT+'n', new_callback,0,0);
        menu->add("&File/&Open", FL_ALT+'o', open_callback,0,0);
        menu->add("&File/&Save", FL_ALT+'s', save_callback,0,0);
        menu->add("&File/&SaveAs...", 0, saveas_callback,0,FL_MENU_DIVIDER);
        menu->add("&File/E&xit", FL_ALT+FL_F+4, exit_callback,0,0);

        menu->add("&Edit/Cut", FL_CTRL+'x', cut_callback,0,0);
        menu->add("&Edit/Copy", FL_CTRL+'c', copy_callback,0,0);
        menu->add("&Edit/Cut", FL_CTRL+'v', paste_callback,0,0);

        menu->add("&View/Project (Example)", 0, project_callback,0,FL_MENU_TOGGLE);
        menu->add("&View/Output (Example)", 0, output_callback,0,FL_MENU_TOGGLE);

        menu->add("&Window/Close", 0, 0,0,0);
        menu->add("&Window/Close All", 0, 0,0,FL_MENU_DIVIDER);
        menu->add("&Window/Tile", 0, 0,0,0);
        menu->add("&Window/Cascade", 0, 0,0,FL_MENU_DIVIDER);
        menu->add("&Window/Next", 0, 0,0,0);
        menu->add("&Window/Previous", 0, 0,0,FL_MENU_DIVIDER);


        menu->add("&Help/About", 0, exit_callback,0,0);
        Fl_Group *grp = new Fl_Group(0,MENU_BAR_HEIGHT,this->w(),this->h()-MENU_BAR_HEIGHT);
        grp->end();
    end();
    resizable(grp);
}

MainWindow::~MainWindow()
{
}

/*
   Handles the new event
*/
void MainWindow::new_callback(Fl_Widget*, void* v) {

}


/*
   Handles the open event
*/
void MainWindow::open_callback(Fl_Widget*, void* v) {
  //char *newfile;
  Fl_Native_File_Chooser fnfc;
  fnfc.title("Open File?");
  fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
  fnfc.filter("xMAS\t*.wck\n");

  switch ( fnfc.show() ) {
      case -1:
            fl_message("ERROR: %s\n", fnfc.errmsg());
            break;  // ERROR
      case  1:
            fl_message("CANCEL\n");
            break;  // CANCEL
      default:
            fl_message("PICKED: %s\n", fnfc.filename());
            break;  // FILE CHOSEN}
    }

}





/*
   Handles the save event
*/
void MainWindow::save_callback(Fl_Widget*, void* v) {

}

/*
   Handles the save as event
*/
void MainWindow::saveas_callback(Fl_Widget*, void* v) {

}

/*
   Handles the cut event
*/
void MainWindow::cut_callback(Fl_Widget*, void* v) {

}
/*
   Handles the copy event
*/
void MainWindow::copy_callback(Fl_Widget*, void* v) {

}
/*
   Handles the paste event
*/
void MainWindow::paste_callback(Fl_Widget*, void* v) {

}

/*
   Handles the project event
*/
void MainWindow::project_callback(Fl_Widget*, void* v) {

}

/*
   Handles the output event
*/
void MainWindow::output_callback(Fl_Widget*, void* v) {

}

/*
   Handles the exit event
*/
void MainWindow::exit_callback(Fl_Widget*, void* v) {
  exit(0);
}



