#! /usr/bin/env python3

from ctypes import *
import zenipy
import os
testlib = cdll.LoadLibrary('/usr/local/lib/libbiogemedylib.dylib')

zenipy.message("Biogeme","Welcome to biogeme. Select the model specification file (*.py) and the data file")
fullModelFile = zenipy.file_selection(title="Model specification file (*.py)")
if (fullModelFile is None):
    exit(0)
modelDir, modelFile = os.path.split(fullModelFile)
os.chdir(modelDir)
modelName, modelExtension = os.path.splitext(modelFile)
if (modelExtension != ".py"):
    zenipy.error(title="Error",text="The extension of the model specification file must be .py")
fullDataFile = zenipy.file_selection(title="Data file")
if (fullDataFile is None):
    exit(0)
dataDir, dataFile = os.path.split(fullDataFile)

modelFile_c = c_char_p(modelName.encode("utf8"))
if (dataDir == modelDir):
    dataFile_c = c_char_p(dataFile.encode("utf8"))
else:
    dataFile_c = c_char_p(fullDataFile.encode("utf8"))

testlib.runBiogeme(modelFile_c,dataFile_c)
