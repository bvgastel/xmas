#ifndef UI_WINDOW_MAIN_H
#define UI_WINDOW_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>

class MainWindow : public Fl_Window
{
    public:
        MainWindow(int width, int height, const char* title);
        virtual ~MainWindow();
    protected:
        static void open_callback(Fl_Widget*, void* v);
        static void exit_callback(Fl_Widget*, void* v);
    private:

};

#endif // UI_WINDOW_MAIN_H
