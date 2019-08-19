import traceback
try:
    from Npp import Editor

    ruler_header = ''.join(['{:>10}'.format(x) for x in range(1,21)])
    ruler_footer = ''.join(['{0} '.format('123456789') for x in range(1,21)])

    Editor.annotationSetText(0, '{}\r\n{}'.format(ruler_header,ruler_footer))
    Editor.annotationSetStyle(0, 0)
    Editor.annotationSetVisible(not Editor.annotationGetVisible())
except:
    trace = traceback.format_exc()
    open(r"c:\temp\npp_py.txt","w").write(trace)