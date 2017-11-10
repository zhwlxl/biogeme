#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>




GdkPixbuf *create_pixbuf(const gchar * filename) {
    
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   
   if (!pixbuf) {
       
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

int main( int   argc,
          char *argv[] ) {
    
  GtkWidget *window;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Biogeme");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  
  // Icon 
  GdkPixbuf *icon = create_pixbuf("smiley.png");  
  gtk_window_set_icon(GTK_WINDOW(window), icon);

  // Menu
  GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  GtkWidget *menubar = gtk_menu_bar_new();

  // File menu
  GtkWidget *fileMenu = gtk_menu_new();
  GtkWidget *fileMenuTitle = gtk_menu_item_new_with_label("File");
  GtkWidget *modelFileMenuItem = gtk_menu_item_new_with_label("Model file...") ;
  GtkWidget *dataFileMenuItem = gtk_menu_item_new_with_label("Data file...") ;
  GtkWidget *quitFileMenuItem = gtk_menu_item_new_with_label("Quit");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMenuTitle), fileMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), modelFileMenuItem);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), dataFileMenuItem);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitFileMenuItem);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMenuTitle);



  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);



  g_signal_connect(window, "destroy",
		   G_CALLBACK(gtk_main_quit), NULL);  

  g_signal_connect(G_OBJECT(quitFileMenuItem), "activate",
		   G_CALLBACK(gtk_main_quit), NULL);

  g_object_unref(icon) ;

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
