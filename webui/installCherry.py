import subprocess
import sys

onWindows = False
if sys.platform == "nt" or sys.platform == "win32":
    onWindows = True


try:
    import cherrypy
    print ("cherrypy already installed") 
except ImportError:
    getpip = ["python3","get-pip.py"]
    getcpy = ["python3","-m","pip","install","cherrypy"]
    if onWindows:
       getpip[0] = "python"
       getcpy[0] = "python"
    else:
        getpip = ["sudo"] + getpip
        getcpy = ["sudo"] + getcpy

    print("CherryPy is missing, trying to install")
    #stderr is not redirected to subprocess.PIPE so the user can see the errors
    print(getpip)
    subprocess.call(getpip, stdout=subprocess.PIPE, shell=onWindows)
    print("pip installed")
    print(getcpy)
    subprocess.call(getcpy,stdout=subprocess.PIPE, shell=onWindows)
    print("cherrypy installed")

