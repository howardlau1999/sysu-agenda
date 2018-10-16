#include "AgendaUI.hpp"
#include <iomanip>
#include "Date.hpp"

const std::vector<std::string> AgendaUI::valid_ops_not_logined{"l", "r", "q"};
const std::vector<std::string> AgendaUI::valid_ops_logined{
    "o", "dc", "lu", "cm", "la", "las", "lap", "qm", "qt", "dm", "da"};
std::map<std::string, AgendaUI::NoArgHandler> AgendaUI::no_arg_op_map{
    {"l", &AgendaUI::userLogIn},
    {"r", &AgendaUI::userRegister},
    {"o", &AgendaUI::userLogOut},
    {"dc", &AgendaUI::listAllUsers},
    {"cm", &AgendaUI::createMeeting},
    {"lu", &AgendaUI::listAllUsers},
    {"la", &AgendaUI::listAllMeetings},
    {"las", &AgendaUI::listAllSponsorMeetings},
    {"lap", &AgendaUI::listAllParticipateMeetings},
    {"qm", &AgendaUI::queryMeetingByTitle},
    {"qt", &AgendaUI::queryMeetingByTimeInterval},
    {"dm", &AgendaUI::deleteMeetingByTitle},
    {"da", &AgendaUI::deleteAllMeetings},
};

void AgendaUI::printMenu() {
    if (m_userName.empty()) {
        std::cout << std::endl;
        std::cout << "------------------------------ Agenda "
                     "------------------------------"
                  << std::endl;
        std::cout << "Action : " << std::endl;
        std::cout << "l   - log in Agenda by user name and password"
                  << std::endl;
        std::cout << "r   - register an Agenda account" << std::endl;
        std::cout << "q   - quit Agenda" << std::endl;
        std::cout << "---------------------------------------------------------"
                     "-----------"
                  << std::endl;
        std::cout << "Agenda : ~$ ";
    } else {
        std::cout << std::endl;
        std::cout << "------------------------------ Agenda "
                     "------------------------------"
                  << std::endl;
        std::cout << "Action : " << std::endl;
        std::cout << "o   - log out Agenda" << std::endl;
        std::cout << "dc  - delete Agenda account" << std::endl;
        std::cout << "lu  - list all Agenda user" << std::endl;
        std::cout << "cm  - create a meeting" << std::endl;
        std::cout << "la  - list all meetings" << std::endl;
        std::cout << "las - list all sponsor meetings" << std::endl;
        std::cout << "lap - list all participate meetings" << std::endl;
        std::cout << "qm  - query meeting by title" << std::endl;
        std::cout << "qt  - query meeting by time interval" << std::endl;
        std::cout << "dm  - delete meeting by title" << std::endl;
        std::cout << "da  - delete all meetings" << std::endl;
        std::cout << "---------------------------------------------------------"
                     "-----------"
                  << std::endl;
        std::cout << "Agenda@" << m_userName << " : # ";
    }
}

AgendaUI::AgendaUI() {}

void AgendaUI::startAgenda() {
    m_userName.clear();
    m_userPassword.clear();
    m_agendaService.startAgenda();
}

void AgendaUI::OperationLoop() {
    startAgenda();
    while (executeOperation(getOperation())) continue;
    quitAgenda();
}

bool checkOperation(std::string op, const std::vector<std::string>& table) {
    for (auto item : table) {
        if (op == item) return true;
    }
    return false;
}

std::string AgendaUI::getOperation() {
    while (true) {
        printMenu();
        std::string op;
        std::cin >> op;
        if (m_userName.empty()) {
            if (checkOperation(op, valid_ops_not_logined)) return op;
        } else if (checkOperation(op, valid_ops_logined))
            return op;
    }
}

bool AgendaUI::executeOperation(std::string op) {
    if ("q" == op) return false;
    NoArgHandler handler = AgendaUI::no_arg_op_map[op];
    (this->*handler)();
    return true;
}

void AgendaUI::userLogIn(void) {
    std::cout << "[log in] [user name] [password]" << std::endl;
    std::cout << "[log in] ";
    std::string name, password;
    std::cin >> name >> password;

    if (m_agendaService.userLogIn(name, password)) {
        std::cout << "[log in] succeed!" << std::endl;
        m_userName = name;
        m_userPassword = password;
    } else {
        std::cout << "[error] log in fail!" << std::endl;
    }
}

void AgendaUI::userRegister(void) {
    std::cout << "[register] [user name] [password] [email] [phone]"
              << std::endl;
    std::cout << "[register] ";
    std::string name, password, email, phone;

    std::cin >> name >> password >> email >> phone;
    if (m_agendaService.userRegister(name, password, email, phone)) {
        std::cout << "[register] succeed!" << std::endl;
    } else {
        std::cout << "[error] register fail!" << std::endl;
    }
}

void AgendaUI::quitAgenda(void) {
    m_agendaService.quitAgenda();
    m_userName.clear();
    m_userPassword.clear();
}

void AgendaUI::userLogOut(void) {
    quitAgenda();
    startAgenda();
}

void AgendaUI::deleteUser(void) {
    m_agendaService.deleteUser(m_userName, m_userPassword);
    std::cout << "[delete agenda account] succeed!" << std::endl;
    userLogOut();
}

void AgendaUI::listAllUsers(void) {
    std::cout << "[list all users]" << std::endl;
    std::cout << "name      email               phone" << std::endl;
    std::list<User> li = m_agendaService.listAllUsers();
    for (std::list<User>::iterator it = li.begin(); it != li.end(); ++it) {
        std::cout << std::setw(10) << std::left << it->getName()
                  << std::setw(20) << std::left << it->getEmail()
                  << std::setw(20) << std::left << it->getPhone() << std::endl;
    }
}

void AgendaUI::createMeeting(void) {
    std::cout << "[create meeting] [the number of participators]" << std::endl;
    std::cout << "[create meeting] ";
    std::string startTime, endTime, title;
    std::vector<std::string> participators;
    int num_participators;
    std::cin >> num_participators;
    for (int i = 1; i <= num_participators; ++i) {
        std::string participator;
        std::cout << "[create meeting] [please enter the participator " << i
                  << "]" << std::endl;
        std::cout << "[create meeting] ";
        std::cin >> participator;
        participators.push_back(participator);
    }
    std::cout << "[create meeting] [title] [start time(";
    std::cout << "yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << std::endl;
    std::cout << "[create meeting] ";
    std::cin >> title >> startTime >> endTime;
    if (m_agendaService.createMeeting(m_userName, title, startTime, endTime,
                                      participators))
        std::cout << "[create meeting] succeed!" << std::endl;
    else
        std::cout << "[error] create meeting fail!" << std::endl;
}

void AgendaUI::listAllMeetings(void) {
    std::cout << "[list all meetings]" << std::endl;
    std::list<Meeting> m = m_agendaService.listAllMeetings(m_userName);
    printMeetings(m);
}

void AgendaUI::listAllSponsorMeetings(void) {
    std::cout << "[list all sponsor meetings]" << std::endl;
    std::list<Meeting> m = m_agendaService.listAllSponsorMeetings(m_userName);
    printMeetings(m);
}

void AgendaUI::listAllParticipateMeetings(void) {
    std::cout << "[list all participate meetings]" << std::endl;
    std::list<Meeting> m =
        m_agendaService.listAllParticipateMeetings(m_userName);
    printMeetings(m);
}

void AgendaUI::queryMeetingByTitle(void) {
    std::cout << "[query meeting] [title]:" << std::endl;
    std::cout << "[query meeting] ";
    std::string title;
    std::cin >> title;
    std::list<Meeting> m = m_agendaService.meetingQuery(m_userName, title);
    printMeetings(m);
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    std::cout << "[query meetings] [start time(";
    std::cout << "yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << std::endl;
    std::cout << "[query meetings] ";
    std::string s, e;
    std::cin >> s >> e;
    std::list<Meeting> m = m_agendaService.meetingQuery(m_userName, s, e);
    std::cout << "[query meetings]" << std::endl;
    printMeetings(m);
}

void AgendaUI::deleteMeetingByTitle(void) {
    std::cout << "[delete meeting] [title]:" << std::endl;
    std::cout << "[delete meeting] ";
    std::string title;
    std::cin >> title;

    if (m_agendaService.deleteMeeting(m_userName, title))
        std::cout << "[delete meeting by title] succed!" << std::endl;
    else
        std::cout << "[error] delete meeting fail!" << std::endl;
}

void AgendaUI::deleteAllMeetings(void) {
    m_agendaService.deleteAllMeetings(m_userName);
}

std::string formatParticipators(const std::vector<std::string>& participators) {
    std::string formatted;
    for (int i = 0; i < participators.size(); ++i) {
        if (i) formatted += ",";
        formatted += participators[i];
    }
    return std::move(formatted);
}

void AgendaUI::printMeetings(std::list<Meeting> meetings) {
    std::cout << "title               sponsor             participator        "
                 "start time          end time"
              << std::endl;
    for (std::list<Meeting>::iterator it = meetings.begin();
         it != meetings.end(); ++it) {
        std::cout << std::setw(20) << std::left << it->getTitle()
                  << std::setw(20) << std::left << it->getSponsor()
                  << std::setw(20) << std::left
                  << formatParticipators(it->getParticipator()) << std::setw(20)
                  << std::left << Date::dateToString(it->getStartDate())
                  << std::setw(20) << std::left
                  << Date::dateToString(it->getEndDate()) << std::endl;
    }
}