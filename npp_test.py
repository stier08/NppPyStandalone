import datetime
open(r"c:\temp\npp_py.txt","a").write("\n" + str(datetime.datetime.now())+" Started")

import traceback

import pynpp


import os.path

registar={}

def scripted():
    open(r"c:\temp\npp_py.txt","a").write("\n" + str(datetime.datetime.now())+" SCRIPTED")

def registerScript(group,name,ref,f):
    pynpp.RegisterScript(ref,group,name)
    registar[ref]=(group,name,f)


def runScript(ref):
    try:
        open(r"c:\temp\npp_py.txt","a").write("\n" + str(datetime.datetime.now())+" runScript " + str(ref) )
        registar[ref][2]()
    except:
        trace = traceback.format_exc()
        open(r"c:\temp\npp_error.txt","a").write(trace)

registerScript("cool group/sub group","my script",scripted.__name__,scripted)

open(r"c:\temp\npp_py.txt","a").write("\n" + str(datetime.datetime.now())+" Finished")