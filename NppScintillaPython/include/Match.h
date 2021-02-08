#ifndef MATCH_H_123091238901
#define MATCH_H_123091238901

#include "NppScintillaPython/include/NotSupportedException.h"

namespace NppPythonScript
{
    
    class GroupDetail 
    {
    public:
        virtual long start() const = 0;
        virtual long end() const = 0;
        virtual bool matched() const = 0;
    
    };

    class Match
    {
    public:
        virtual size_t groupCount() = 0;
        virtual GroupDetail* group(std::size_t groupNumber) = 0;
        virtual GroupDetail* groupName(const char *groupName) = 0;
        virtual void expand(const char* format, char **result, size_t *resultLength) = 0;
        virtual std::string getTextForGroup(GroupDetail* group) = 0;
        virtual int groupIndexFromName(const char *groupName) = 0;

        boost::python::str py_group_number(std::size_t groupNumber);
        boost::python::str py_group_name(boost::python::str groupName);
        boost::python::str py_expand(boost::python::object replaceFormat);
        boost::python::tuple py_groups();
        long py_start_group_0() { return py_start(0); }
        long py_start(long group);
        long py_start_name(boost::python::str groupName);

        long py_end_group_0() { return py_end(0); }
        long py_end(long group);
        long py_end_name(boost::python::str groupName);

        boost::python::tuple py_span_group_0() { return py_span(0); }
        boost::python::tuple py_span(long groupIndex);
        boost::python::tuple py_span_name(boost::python::str groupName);

        long py_lastindex();

        boost::python::object py_group_variable(boost::python::tuple args, boost::python::dict kwargs);

    private:
        boost::python::str getGroup(boost::python::object groupIdentifier);

    };


    boost::python::object py_group_variable(boost::python::tuple args, boost::python::dict kwargs);

}


#endif // MATCH_H_123091238901