#include "AgendaService.hpp"
#include <algorithm>
#include "Logger.hpp"
static Logger classLogger("AgendaService::",
                          {std::make_shared<FileAppender>("/tmp/log.txt")});
// Confident
bool AgendaService::deleteUser(const std::string &userName,
                               const std::string &password) {
    Logger logger(classLogger, __func__);
    if (m_storage->deleteUser([userName, password](const User &user) -> bool {
            return user.getName() == userName && user.getPassword() == password;
        }) != 1) {
        logger.warning("Delete user " + userName + " failed");
        return false;
    }

    // Delete sponsored meeting
    deleteAllMeetings(userName);

    // Find all participated meeting
    auto meetings =
        m_storage->queryMeeting([userName](const Meeting &m) -> bool {
            return m.isParticipator(userName);
        });

    // Remove from participators
    for (auto m : meetings) {
        quitMeeting(userName, m.getTitle());
    }

    logger.info("User " + userName + " was deleted successfully");

    m_storage->sync();
    return true;
}

bool AgendaService::addMeetingParticipator(const std::string &userName,
                                           const std::string &title,
                                           const std::string &participator) {
    Logger logger(classLogger, __func__);
    if (userName == participator) {
        logger.error("Sponsor " + userName +
                     " is the same as the participator");
        return false;
    }

    if (m_storage
            ->queryUser([&](const User &user) -> bool {
                return user.getName() == participator;
            })
            .empty()) {
        logger.error("Sponsor " + userName + " does not exist");
        return false;
    }

    int count = m_storage->updateMeeting(
        [&](const Meeting &update) {
            return userName == update.getSponsor() &&
                   title == update.getTitle() &&
                   !update.isParticipator(
                       participator) &&  // located the meeting to be updated
                   m_storage
                       ->queryMeeting([update, participator](const Meeting &m)
                                          -> bool {  // Pariticipator is busy
                                                     // during this meeting
                           return (m.isParticipator(participator) ||
                                   m.getSponsor() ==
                                       participator) &&  // Pariticipator is
                                                         // involved
                                  !(update.getStartDate() >= m.getEndDate() ||
                                    update.getEndDate() <=
                                        m.getStartDate());  // Participator is
                                                            // busy
                       })
                       .empty();  // empty == available
        },
        [participator](Meeting &m) { m.addParticipator(participator); });
    m_storage->sync();
    return count;
}

bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                              const std::string &title,
                                              const std::string &participator) {
    Logger logger(classLogger, __func__);
    if (m_storage
            ->queryMeeting([userName, title, participator](const Meeting &m) {
                return userName == m.getSponsor() && title == m.getTitle() &&
                       m.isParticipator(participator);
            })
            .empty())
        return false;

    return quitMeeting(participator, title);
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &title) const {
    Logger logger(classLogger, __func__);
    logger.info("Query meeting by title" + title);
    return m_storage->queryMeeting([&](const Meeting &m) {
        return (userName == m.getSponsor() || m.isParticipator(userName)) &&
               title == m.getTitle();
    });
}

std::list<Meeting> AgendaService::meetingQuery(
    const std::string &userName, const std::string &startDate,
    const std::string &endDate) const {
    Date s(startDate);
    Date e(endDate);
    Logger logger(classLogger, __func__);
    logger.info("Query info by date " + startDate + " to " + endDate);
    // Input is invalid
    if (!Date::isValid(s) || !Date::isValid(e) || s > e)
        return std::list<Meeting>();

    return m_storage->queryMeeting([&](const Meeting &m) {
        return (userName == m.getSponsor() || m.isParticipator(userName)) &&
               !(s > m.getEndDate() || e < m.getStartDate());
    });
}

std::list<Meeting> AgendaService::listAllMeetings(
    const std::string &userName) const {
    Logger logger(classLogger, __func__);
    logger.info("User: " + userName);
    return m_storage->queryMeeting([&](const Meeting &m) {
        return userName == m.getSponsor() || m.isParticipator(userName);
    });
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(
    const std::string &userName) const {
    Logger logger(classLogger, __func__);
    logger.info("User: " + userName);
    return m_storage->queryMeeting(
        [&](const Meeting &m) { return userName == m.getSponsor(); });
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string &userName) const {
    Logger logger(classLogger, __func__);
    logger.info("User: " + userName);
    return m_storage->queryMeeting(
        [&](const Meeting &m) { return m.isParticipator(userName); });
}

bool AgendaService::deleteMeeting(const std::string &userName,
                                  const std::string &title) {
    Logger logger(classLogger, __func__);
    logger.info("User " + userName + " Try delete meeting " + title);
    int count = m_storage->deleteMeeting([userName, title](const Meeting &m) {
        return userName == m.getSponsor() && title == m.getTitle();
    });
    m_storage->sync();
    return count;
}

bool AgendaService::deleteAllMeetings(const std::string &userName) {
    Logger logger(classLogger, __func__);
    logger.info("Delete all meetings by " + userName);
    m_storage->deleteMeeting(
        [userName](const Meeting &m) { return userName == m.getSponsor(); });
    m_storage->sync();
    return true;
}

// Passed
bool AgendaService::userLogIn(const std::string &userName,
                              const std::string &password) {
    Logger logger(classLogger, __func__);
    if (m_storage
            ->queryUser([userName, password](const User &user) -> bool {
                return user.getName() == userName &&
                       user.getPassword() == password;
            })
            .size() == 1) {
        logger.info("User " + userName + " logged in");
        return true;
    } else {
        logger.warning("User " + userName + " logged in failed");
        return false;
    }
}

// Passed
bool AgendaService::userRegister(const std::string &userName,
                                 const std::string &password,
                                 const std::string &email,
                                 const std::string &phone) {
    Logger logger(classLogger, __func__);
    if (m_storage
            ->queryUser([userName](const User &user) -> bool {
                return user.getName() == userName;
            })
            .size() >= 1) {
        logger.warning("User " + userName + " already exists");
        return false;
    }

    m_storage->createUser(User(userName, password, email, phone));
    logger.info(userName + " registered successfully");
    m_storage->sync();
    return true;
}

// Passed
bool AgendaService::quitMeeting(const std::string &userName,
                                const std::string &title) {
    Logger logger(classLogger, __func__);
    int updated = m_storage->updateMeeting(
        [title, userName](const Meeting &m) {
            return title == m.getTitle() && m.isParticipator(userName) &&
                   m.getSponsor() !=
                       userName;  // Find the meeting containing participator
        },
        [userName](Meeting &m) { m.removeParticipator(userName); });

    if (!updated) {
        logger.warning("User " + userName + " failed to quit meeting " + title);
        return false;
    }

    m_storage->deleteMeeting(
        [](const Meeting &m) { return m.getParticipator().empty(); });
    logger.info("User " + userName + " quitted meeting " + title);
    m_storage->sync();
    return updated;
}

// Passed
std::list<User> AgendaService::listAllUsers(void) const {
    return m_storage->queryUser([](const User &user) -> bool { return true; });
}

// Passed
bool AgendaService::createMeeting(
    const std::string &userName, const std::string &title,
    const std::string &startDate, const std::string &endDate,
    const std::vector<std::string> &participator) {
    Logger logger(classLogger, __func__);
    Date s(startDate);
    Date e(endDate);

    // Input data is invalid
    if (!Date::isValid(s) || !Date::isValid(e) || s >= e ||
        participator.empty()) {
        logger.warning("Input is invalid");
        return false;
    }

    // The title is identical to another
    if (!m_storage
             ->queryMeeting([title](const Meeting &m) -> bool {
                 return title == m.getTitle();
             })
             .empty()) {
        logger.warning("There is already a meeting named " + title);
        return false;
    }

    // Sponsor does not exist
    if (m_storage
            ->queryUser([userName](const User &user) -> bool {
                return user.getName() == userName;
            })
            .empty()) {
        logger.warning("User " + userName + "does not exist");
        return false;
    }

    // Sponsor is busy
    if (!m_storage
             ->queryMeeting([s, e, userName](const Meeting &m) -> bool {
                 return !(s >= m.getEndDate() || e <= m.getStartDate()) &&
                        (m.getSponsor() == userName ||
                         m.isParticipator(userName));
             })
             .empty()) {
        logger.warning("Sponsor " + userName + " is busy");
        return false;
    }

    Meeting meeting(userName, {}, s, e, title);

    for (auto name : participator) {
        // Participator does not exist
        if (m_storage
                ->queryUser([name](const User &user) -> bool {
                    return user.getName() == name;
                })
                .empty()) {
            logger.warning("Participator " + name + " does not exist");
            return false;
        }

        // Participator is busy
        if (!m_storage
                 ->queryMeeting([s, e, name](const Meeting &m) -> bool {
                     return (m.isParticipator(name) ||
                             m.getSponsor() == name) &&
                            !(s >= m.getEndDate() || e <= m.getStartDate());
                 })
                 .empty()) {
            logger.warning("Participator " + name + "  is busy");
            return false;
        }

        // Duplications of participators
        if (meeting.isParticipator(name) || name == userName) {
            logger.warning("Participator " + name + "  is duplicated");
            return false;
        }

        meeting.addParticipator(name);
    }

    m_storage->createMeeting(meeting);
    logger.info("Meeting " + title + " was created");
    m_storage->sync();
    return true;
}

AgendaService::AgendaService() { m_storage = Storage::getInstance(); }

AgendaService::~AgendaService() { m_storage->sync(); }

void AgendaService::startAgenda(void) {
    Logger logger(classLogger, __func__);
    logger.info("Agenda started");
    m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
    Logger logger(classLogger, __func__);
    logger.info("Agenda quitted");
    m_storage->sync();
}