#include "AgendaService.hpp"
#include <algorithm>
#include "Storage.hpp"

AgendaService::AgendaService() { m_storage = Storage::getInstance(); }
AgendaService::~AgendaService() { m_storage->sync(); }
bool AgendaService::userLogIn(const std::string userName,
                              const std::string password) {
    auto users =
        m_storage->queryUser([userName, password](const User& user) -> bool {
            return user.getName() == userName && user.getPassword() == password;
        });
    return users.size() == 1;
}

bool AgendaService::userRegister(const std::string userName,
                                 const std::string password,
                                 const std::string email,
                                 const std::string phone) {
    auto users = m_storage->queryUser(
        [userName](const User& user) -> bool { user.getName() == userName; });
    if (!users.empty()) return false;
    m_storage->createUser(User(userName, password, email, phone));
    return true;
}

bool AgendaService::deleteUser(const std::string userName,
                               const std::string password) {
    auto count =
        m_storage->deleteUser([userName, password](const User& user) -> bool {
            return user.getName() == userName && user.getPassword() == password;
        });
    if (count != 1) return false;
    m_storage->deleteMeeting([userName](const Meeting& meeting) {
        auto participators = meeting.getParticipator();
        return userName == meeting.getSponsor() ||
               std::find(participators.begin(), participators.end(),
                         userName) != participators.end();
    });
    return count == 1;
}

std::list<User> AgendaService::listAllUsers(void) const {
    return m_storage->queryUser([](const User&) -> bool { return true; });
}

bool AgendaService::createMeeting(const std::string userName,
                                  const std::string title,
                                  const std::string startDate,
                                  const std::string endDate,
                                  const std::vector<std::string> participator) {
    Date start_date = Date::stringToDate(startDate);
    Date end_date = Date::stringToDate(endDate);
    if (!Date::isValid(start_date) || !Date::isValid(end_date) || start_date >= end_date) return false;
    auto meetings = m_storage->queryMeeting(
        [start_date, end_date, title](const Meeting& meeting) {
            return !(start_date >= meeting.getEndDate() ||
                     end_date <= meeting.getStartDate()) ||
                   title == meeting.getTitle();
        });
    if (meetings.size() > 0) return false;

    auto users = m_storage->queryUser(
        [userName](const User& user) -> bool { return user.getName() == userName; });
    if (users.empty()) return false;

    for (auto name : participator) {
        auto users = m_storage->queryUser([name](const User& user) -> bool {
            return user.getName() == name;
        });
        if (users.empty()) return false;
    }

    m_storage->createMeeting(
        Meeting(userName, participator, start_date, end_date, title));
    return true;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                               const std::string title) const {
    return m_storage->queryMeeting([userName, title](const Meeting& meeting) {
        return userName == meeting.getSponsor() && title == meeting.getTitle();
    });
}

std::list<Meeting> AgendaService::meetingQuery(
    const std::string userName, const std::string startDate,
    const std::string endDate) const {
    Date start_date = Date::stringToDate(startDate);
    Date end_date = Date::stringToDate(endDate);
    if (!Date::isValid(start_date) || !Date::isValid(end_date))
        return std::list<Meeting>();
    return m_storage->queryMeeting(
        [userName, start_date, end_date](const Meeting& meeting) {
            return userName == meeting.getSponsor() &&
                   !(start_date > meeting.getEndDate() ||
                     end_date < meeting.getStartDate());
        });
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(
    const std::string userName) const {
    return std::move(
        m_storage->queryMeeting([userName](const Meeting& meeting) {
            return userName == meeting.getSponsor();
        }));
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string userName) const {
    return m_storage->queryMeeting([userName](const Meeting& meeting) {
        auto participators = meeting.getParticipator();
        return std::find(participators.begin(), participators.end(),
                         userName) != participators.end();
    });
}

std::list<Meeting> AgendaService::listAllMeetings(
    const std::string userName) const {
    return std::move(
        m_storage->queryMeeting([userName](const Meeting& meeting) {
            auto participators = meeting.getParticipator();
            return userName == meeting.getSponsor() ||
                   std::find(participators.begin(), participators.end(),
                             userName) != participators.end();
        }));
}

bool AgendaService::deleteMeeting(const std::string userName,
                                  const std::string title) {
    auto count = m_storage->deleteMeeting([userName, title](
                                              const Meeting& meeting) -> bool {
        return meeting.getSponsor() == userName && meeting.getTitle() == title;
    });
    return count == 1;
}

bool AgendaService::deleteAllMeetings(const std::string userName) {
    auto count =
        m_storage->deleteMeeting([userName](const Meeting& meeting) -> bool {
            return meeting.getSponsor() == userName;
        });
    return count >= 1;
}

void AgendaService::startAgenda() { m_storage = Storage::getInstance(); }

void AgendaService::quitAgenda() { m_storage->sync(); }