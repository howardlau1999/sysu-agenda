#include "AgendaService.hpp"
#include "Storage.hpp"
#include <algorithm>

AgendaService::AgendaService() { m_storage = Storage::getInstance(); }

bool AgendaService::userLogIn(const std::string userName,
                              const std::string password) {
    auto users =
        m_storage->queryUser([userName, password](const User& user) -> bool {
            user.getName() == userName;
            user.getPassword() == password;
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
    return count == 1;
}

std::list<User> AgendaService::listAllUsers(void) const {
    return m_storage->queryUser([](const User&) -> bool { return true; });
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
                   start_date <= meeting.getStartDate() &&
                   end_date >= meeting.getEndDate();
        });
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(
    const std::string userName) const {
    return m_storage->queryMeeting([userName](const Meeting& meeting) {
        return userName == meeting.getSponsor();
    });
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string userName) const {
    return m_storage->queryMeeting([userName](const Meeting& meeting) {
        auto participators = meeting.getParticipator();
        return std::find(participators.begin(), participators.end(), userName) != participators.end();
    });
}

std::list<Meeting> AgendaService::listAllMeetings(
    const std::string userName) const {
    return m_storage->queryMeeting([userName](const Meeting& meeting) {
        auto participators = meeting.getParticipator();
        return userName == meeting.getSponsor() ||
               std::find(participators.begin(), participators.end(),
                         userName) != participators.end();
    });
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