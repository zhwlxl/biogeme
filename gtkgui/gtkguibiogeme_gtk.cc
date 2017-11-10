#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include "patType.h"
#include "patConst.h"
#include "gtkguibiogeme.h"
#include "patDisplay.h"

// Callback functions
void getModelFileName(GtkWidget* widget, gpointer *data) {
  bioRunCallbackData *theCallbackData = (bioRunCallbackData*) data ;
  theCallbackData->modelSelected = patTRUE ;
  gchar* theFile =  gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget)) ;
  theCallbackData->modelFileWithPath = patString(theFile) ;
  DEBUG_MESSAGE(theCallbackData->modelFileWithPath) ;
}


void getDataFileName(GtkWidget* widget, gpointer *data) {
  bioRunCallbackData *theCallbackData = (bioRunCallbackData*) data ;
  theCallbackData->dataSelected = patTRUE ;
  gchar* theFile = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget)) ;
  theCallbackData->dataFileWithPath =  patString(theFile) ;
  DEBUG_MESSAGE(theCallbackData->dataFileWithPath) ;

}


void runBiogeme(GtkWidget* widget, gpointer *data) {
  bioRunCallbackData *theCallbackData = (bioRunCallbackData*) data ;
  
}



static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  bioRunCallbackData theCallbackData ;
  theCallbackData.modelSelected = patFALSE ;
  theCallbackData.dataSelected = patFALSE ;
  theCallbackData.theVersion = bioBison ;
  GtkWidget *window;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Biogeme");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_widget_show_all (window);

  GtkWidget *grid = gtk_grid_new() ;
  gtk_container_add (GTK_CONTAINER (window), grid);
  gtk_grid_set_column_spacing(GTK_GRID(grid),5) ;
  gtk_grid_set_row_spacing(GTK_GRID(grid),10) ;

  GtkWidget *wVersion = gtk_label_new("Version:") ;
  gtk_label_set_justify(GTK_LABEL(wVersion),GTK_JUSTIFY_RIGHT) ;

  gtk_grid_attach(GTK_GRID(grid),wVersion,0,0,1,1) ;

  GtkWidget *wButtonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;

  GtkWidget *wChooseBiogeme = gtk_radio_button_new_with_label (NULL,"Biogeme") ;
  gtk_container_add(GTK_CONTAINER(wButtonBox),wChooseBiogeme) ;
  GtkWidget *wChooseBiosim = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(wChooseBiogeme),"Biosim") ;
  gtk_container_add(GTK_CONTAINER(wButtonBox),wChooseBiosim) ;
  GtkWidget *wChoosePython = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(wChooseBiogeme),"PythonBiogeme") ;
  gtk_container_add(GTK_CONTAINER(wButtonBox),wChoosePython) ;
  gtk_grid_attach(GTK_GRID(grid),wButtonBox,1,0,1,1) ;

  GtkWidget *wChooseModel = gtk_label_new("Model file:") ;
  gtk_label_set_justify(GTK_LABEL(wChooseModel),GTK_JUSTIFY_RIGHT) ;
  gtk_grid_attach(GTK_GRID(grid),wChooseModel,0,1,1,1) ;

  GtkWidget *modelFileButton = gtk_file_chooser_button_new("Select a model file",
							   GTK_FILE_CHOOSER_ACTION_OPEN) ;



  gtk_grid_attach(GTK_GRID(grid),modelFileButton,1,1,1,1) ;


  GtkWidget *wChooseData = gtk_label_new("Data file:") ;
  gtk_label_set_justify(GTK_LABEL(wChooseData),GTK_JUSTIFY_RIGHT) ;
  gtk_grid_attach(GTK_GRID(grid),wChooseData,0,2,1,1) ;


  GtkWidget *dataFileButton = gtk_file_chooser_button_new("Select a data file",
							   GTK_FILE_CHOOSER_ACTION_OPEN) ;
 
  gtk_grid_attach(GTK_GRID(grid),dataFileButton,1,2,1,1) ;

  GtkWidget *runButton = gtk_button_new_with_label("Run...") ;
  gtk_grid_attach(GTK_GRID(grid),runButton,0,3,2,1) ;

  // Signals

  g_signal_connect (modelFileButton, "file-set", G_CALLBACK (getModelFileName), &theCallbackData);
  g_signal_connect (dataFileButton, "file-set", G_CALLBACK (getDataFileName), &theCallbackData);
  g_signal_connect (runButton, "clicked", G_CALLBACK (runBiogeme), &theCallbackData);

  gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
  
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;


}
