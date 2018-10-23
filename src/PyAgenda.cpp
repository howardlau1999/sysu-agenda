#include <Python.h>
#include "AgendaService.hpp"
extern "C" {
static AgendaService m_agendaService;

static PyObject* user_login(PyObject* self, PyObject* args) {
    char* username;
    char* password;

    if (!PyArg_ParseTuple(args, "ss", &username, &password)) {
        return NULL;
    }

    bool success = m_agendaService.userLogIn(username, password);

    PyObject* ret = PyBool_FromLong(success);

    return ret;
}

static PyObject* add(PyObject* self, PyObject* args) {
    int a, b;

    if (!PyArg_ParseTuple(args, "ii", &a, &a)) {
        return NULL;
    }

    int sum = a + b;
    PyObject* ret = Py_BuildValue("i", sum);

    return ret;
}


static PyObject* user_register(PyObject* self, PyObject* args, PyObject* kwargs) {
    char* username;
    char* password;
    char* email;
    char* phone;
    char* keywords[] = {"username", "password", "email", "phone", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ssss", keywords, &username, &password, &email, &phone)) {
        return NULL;
    }

    bool success =
        m_agendaService.userRegister(username, password, email, phone);

    PyObject* ret = PyBool_FromLong(success);

    return ret;
}

static PyObject* delete_user(PyObject* self, PyObject* args) {
    char* username;
    char* password;

    if (!PyArg_ParseTuple(args, "ss", &username, &password)) {
        return NULL;
    }

    bool success = m_agendaService.deleteUser(username, password);

    PyObject* ret = PyBool_FromLong(success);

    return ret;
}

static PyObject* create_meeting(PyObject* self, PyObject* args, PyObject* kwargs) {
    char* username;
    char* title;
    char* start_date;
    char* end_date;
    PyObject* participators;

    char* keywords[] = {"username", "title", "start_date", "end_date", "participators", NULL};
 
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ssssO!", keywords, &username, &title, &start_date,
                          &end_date, &PyList_Type, &participators)) {
        return NULL;
    }

    std::vector<std::string> v_participators;
    Py_ssize_t n = PyList_Size(participators);
    if (n < 0) return NULL;
    for (Py_ssize_t i = 0; i < n; ++i) {
        PyObject* item = PyList_GetItem(participators, i);
        std::string participator = PyUnicode_AsUTF8(item);
        v_participators.push_back(participator);
    }

    bool success = m_agendaService.createMeeting(username, title, start_date,
                                                 end_date, v_participators);
    PyObject* ret = PyBool_FromLong(success);

    return ret;
}

static PyObject* delete_meeting(PyObject* self, PyObject* args) {
    char* username;
    char* title;

    if (!PyArg_ParseTuple(args, "ss", &username, &title)) {
        return NULL;
    }

    bool success = m_agendaService.deleteMeeting(username, title);

    PyObject* ret = PyBool_FromLong(success);

    return ret;
}

static PyObject* delete_all_meetings(PyObject* self, PyObject* args) {
    char* username;

    if (!PyArg_ParseTuple(args, "s", &username)) {
        return NULL;
    }

    bool success = m_agendaService.deleteAllMeetings(username);

    PyObject* ret = PyBool_FromLong(success);

    return ret;
}

static PyObject* meetings_to_list(std::list<Meeting> meetings) {
    Py_ssize_t n = meetings.size();
    PyObject* ret = PyList_New(n);
    for (Py_ssize_t i = 0; i < n; ++i) {
        auto meeting = meetings.front();
        meetings.pop_front();
        auto participators = meeting.getParticipator();

        PyObject* meeting_obj = PyDict_New();
        PyObject* meeting_sponsor =
            PyUnicode_FromString(meeting.getSponsor().c_str());
        PyObject* meeting_title =
            PyUnicode_FromString(meeting.getTitle().c_str());
        PyObject* meeting_start_date = PyUnicode_FromString(
            Date::dateToString(meeting.getStartDate()).c_str());
        PyObject* meeting_end_date = PyUnicode_FromString(
            Date::dateToString(meeting.getEndDate()).c_str());
        PyObject* meeting_participators = PyList_New(participators.size());

        for (Py_ssize_t j = 0; j < participators.size(); ++j) {
            PyList_SetItem(meeting_participators, j,
                           PyUnicode_FromString(participators[j].c_str()));
        }

        PyDict_SetItem(meeting_obj, PyUnicode_FromString("sponsor"),
                       meeting_sponsor);
        PyDict_SetItem(meeting_obj, PyUnicode_FromString("title"),
                       meeting_title);
        PyDict_SetItem(meeting_obj, PyUnicode_FromString("start_date"),
                       meeting_start_date);
        PyDict_SetItem(meeting_obj, PyUnicode_FromString("end_date"),
                       meeting_end_date);
        PyDict_SetItem(meeting_obj, PyUnicode_FromString("participators"),
                       meeting_participators);

        PyList_SetItem(ret, i, meeting_obj);
    }
    return ret;
}

static PyObject* users_to_list(std::list<User> users) {
    Py_ssize_t n = users.size();
    PyObject* ret = PyList_New(n);
    for (Py_ssize_t i = 0; i < n; ++i) {
        auto user = users.front();
        users.pop_front();
        PyObject* user_obj = PyDict_New();

        PyDict_SetItem(user_obj, PyUnicode_FromString("username"),
                       PyUnicode_FromString(user.getName().c_str()));
        PyDict_SetItem(user_obj, PyUnicode_FromString("email"),
                       PyUnicode_FromString(user.getEmail().c_str()));
        PyDict_SetItem(user_obj, PyUnicode_FromString("phone"),
                       PyUnicode_FromString(user.getPhone().c_str()));

        PyList_SetItem(ret, i, user_obj);
    }
    return ret;
}



static PyObject* list_all_users(PyObject* self, PyObject* args) {

    auto users = m_agendaService.listAllUsers();
    PyObject* ret = users_to_list(users);

    return ret;
}

static PyObject* list_sponsor_meetings(PyObject* self, PyObject* args) {
    char* username;

    if (!PyArg_ParseTuple(args, "s", &username)) {
        return NULL;
    }
    auto meetings = m_agendaService.listAllSponsorMeetings(username);
    PyObject* ret = meetings_to_list(meetings);

    return ret;
}

static PyObject* query_meeting_by_title(PyObject* self, PyObject* args) {
    char* username;
    char* title;

    if (!PyArg_ParseTuple(args, "ss", &username, &title)) {
        return NULL;
    }
    auto meetings = m_agendaService.meetingQuery(username, title);
    PyObject* ret = meetings_to_list(meetings);

    return ret;
}

static PyObject* query_meeting_by_date(PyObject* self, PyObject* args) {
    char* username;
    char* start_date;
    char* end_date;

    if (!PyArg_ParseTuple(args, "sss", &username, &start_date, &end_date)) {
        return NULL;
    }
    auto meetings =
        m_agendaService.meetingQuery(username, start_date, end_date);
    PyObject* ret = meetings_to_list(meetings);

    return ret;
}

static PyObject* list_participate_meetings(PyObject* self, PyObject* args) {
    char* username;

    if (!PyArg_ParseTuple(args, "s", &username)) {
        return NULL;
    }
    auto meetings = m_agendaService.listAllParticipateMeetings(username);
    PyObject* ret = meetings_to_list(meetings);

    return ret;
}

static PyMethodDef PyAgendaMethods[] = {
    {"login", user_login, METH_VARARGS, "Log in Agenda"},
    {"register", (PyCFunction) user_register, METH_VARARGS | METH_KEYWORDS, "Register a new account"},
    {"delete_user", delete_user, METH_VARARGS, "Delete an existing user"},
    {"list_all_users", list_all_users, METH_VARARGS, "List all users"},
    {"create_meeting", (PyCFunction) create_meeting, METH_VARARGS | METH_KEYWORDS, "Create a new meeting"},
    {"delete_meeting", delete_meeting, METH_VARARGS, "Delete meeting"},
    {"delete_all_meetings", delete_all_meetings, METH_VARARGS,
     "Delete all meetings"},
    {"list_sponsor_meetings", list_sponsor_meetings, METH_VARARGS,
     "List sponsored meetings"},
    {"list_participate_meetings", list_participate_meetings, METH_VARARGS,
     "List participated meetings"},
    {"query_meeting_by_date", query_meeting_by_date, METH_VARARGS,
     "Query meetings by date"},
    {"query_meeting_by_title", query_meeting_by_title, METH_VARARGS,
     "Query meetings by title"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef pyagendamodule = {PyModuleDef_HEAD_INIT, "pyagenda",
                                            NULL, -1, PyAgendaMethods};

PyMODINIT_FUNC PyInit_pyagenda() { return PyModule_Create(&pyagendamodule); }
}