import traceback
import datetime

open(r"c:\temp\npp_py.txt","a").write("\n" + str(datetime.datetime.now()))
import os.path

try:
    import pynpp
    pynpp.RegisterScript("__reference1___","lev1/lev2/lev3","name")
    pynpp.RegisterScript("__reference2___","lev1/lev3/lev4","name")
    pynpp.RegisterScript("__reference3___","lev4/lev3/lev4","name")
    pynpp.RegisterScript("__referenc4e___","group1","name")

    

except:
    trace = traceback.format_exc()
    open(r"c:\temp\npp_error.txt","a").write(trace)
    
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