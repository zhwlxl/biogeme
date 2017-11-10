Use up.py to start the WebUI.
The only option available is 'openBrowser', this will open a browser on the correct port.

Two configurations files are available:
-BiogemeWebUI.conf:
	contains settings related to the user interface.
	-datafolder: choose the path of your data folder, can be relative or absolute
	-defaultsort: can be [nameAsc/nameDes/dateAsc/dateDes], the order in which your files will be sorted by default.
	-shutdown_allowd: can be [True/False], indicates if you want the shutdown and settings buttons to appear. (can not be changed via user interface)
	
-serv.conf:
	contains settings related to the server:
	-log.error_file: file containing the logs of the server, used for debug purposes. (can not be changed via user interface)
	-server.socket_host: can be  ["127.0.0.1"/"0.0.0.0"], sets the visibility of the server "127.0.0.1" means that only connections from the same computer are accepted.
	-server.socket_port: port used to access the WebUI in the browser