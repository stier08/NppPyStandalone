import traceback
import datetime

open(r"c:\temp\npp_py.txt","a").write("\n" + str(datetime.datetime.now()))
import os.path

try:
    open(r"c:\temp\npp_py.txt","a").write(str("\n".join(dir(os.path))))  
except:
    trace = traceback.format_exc()
    open(r"c:\temp\npp_error.txt","a").write(trace)
    
try:
    from Npp import editor
    #open(r"c:\temp\npp_py.txt","a").write("\n".join(dir(editor)))
    open(r"c:\temp\npp_py.txt","a").write("\nselection\n"+editor.getSelText()+"\n")
    

except:
    trace = traceback.format_exc()
    open(r"c:\temp\npp_error.txt","a").write(trace)