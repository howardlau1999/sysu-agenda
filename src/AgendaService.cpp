#include "AgendaService.hpp"
#include <algorithm>

// Confident
bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
  if (m_storage->deleteUser([userName, password] (const User& user) -> bool {
    return user.getName() == userName && user.getPassword() == password;
  }) != 1) return false;

  // Delete sponsored meeting
  deleteAllMeetings(userName);

  // Find all participated meeting
  auto meetings = m_storage->queryMeeting([userName] (const Meeting& m) -> bool { return m.isParticipator(userName); });

  // Remove from participators
  for (auto m : meetings) {
    quitMeeting(userName, m.getTitle());
  }

  return true;
}

bool AgendaService::addMeetingParticipator(const std::string &userName,
                            const std::string &title,
                            const std::string &participator) {
  if (userName == participator) return false;

  if (m_storage->queryUser([&](const User& user) -> bool {
    return user.getName() == participator;
  }).empty()) return false;

  return m_storage->updateMeeting([&](const Meeting& update) {
    return userName == update.getSponsor() && 
           title == update.getTitle() && 
           !update.isParticipator(participator) && // located the meeting to be updated
           m_storage->queryMeeting([update, participator](const Meeting& m) -> bool { // Pariticipator is busy during this meeting
             return (m.isParticipator(participator) || m.getSponsor() == participator) && // Pariticipator is involved
                    !(update.getStartDate() >= m.getEndDate() || update.getEndDate() <= m.getStartDate()); // Participator is busy
           }).empty(); // empty == available
  }, [participator](Meeting& m) {
    m.addParticipator(participator);
  });
}

bool AgendaService::removeMeetingParticipator(const std::string &userName,
                               const std::string &title,
                               const std::string &participator) {
  if (m_storage->queryMeeting([userName, title, participator] (const Meeting& m) {
    return userName == m.getSponsor() && 
           title == m.getTitle() &&
           m.isParticipator(participator);
  }).empty()) return false;
  
  return quitMeeting(participator, title);
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &title) const {
  return m_storage->queryMeeting([&](const Meeting& m) {
    return (userName == m.getSponsor() || m.isParticipator(userName)) 
            && title == m.getTitle();
  });
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                const std::string &startDate,
                                const std::string &endDate) const {
  Date s(startDate); Date e(endDate);

  // Input is invalid
  if (!Date::isValid(s) || !Date::isValid(e) || s > e) return std::list<Meeting>();

  return m_storage->queryMeeting([&](const Meeting& m) {
    return (userName == m.getSponsor() || m.isParticipator(userName)) && 
          !(s > m.getEndDate() || e < m.getStartDate()) ;
  });

}

std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
  return m_storage->queryMeeting([&](const Meeting& m) {
    return userName == m.getSponsor() || m.isParticipator(userName);
  });
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
  return m_storage->queryMeeting([&](const Meeting& m) {
    return userName == m.getSponsor();
  });
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const {
  return m_storage->queryMeeting([&](const Meeting& m) {
    return m.isParticipator(userName);
  });
}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
  return m_storage->deleteMeeting([userName, title](const Meeting& m) {
    return userName == m.getSponsor() && title == m.getTitle();
  });
}

bool AgendaService::deleteAllMeetings(const std::string &userName) {
  return m_storage->deleteMeeting([userName](const Meeting& m) {
    return userName == m.getSponsor();
  });
}

// Passed
bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
  return m_storage->queryUser([userName, password](const User& user) -> bool {
    return user.getName() == userName && user.getPassword() == password;
  }).size() == 1;
}

// Passed
bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                  const std::string &email, const std::string &phone) {
  if (m_storage->queryUser([userName](const User& user) -> bool {
    return user.getName() == userName;
  }).size() >= 1) return false;

  m_storage->createUser(User(userName, password, email, phone));
  return true;
}

// Passed
bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
  int updated = m_storage->updateMeeting([title, userName] (const Meeting& m) {
    return title == m.getTitle() && 
           m.isParticipator(userName) && 
           m.getSponsor() != userName; // Find the meeting containing participator
  }, [userName, this](Meeting& m) {
    m.removeParticipator(userName);
  });

  auto meetings = m_storage->queryMeeting([] (const Meeting&) { return true;});

  // Clean up
  for (auto m : meetings) {
    if (m.getParticipator().empty())
      deleteMeeting(m.getSponsor(), m.getTitle());
  }

  return updated > 0;
}

// Passed
std::list<User> AgendaService::listAllUsers(void) const {
  return m_storage->queryUser([](const User& user) -> bool {
    return true;
  });
}

// Passed
bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                   const std::string &startDate, const std::string &endDate,
                   const std::vector<std::string> &participator) {

  Date s(startDate); Date e(endDate);

  // Input data is invalid
  if (!Date::isValid(s) || !Date::isValid(e) || s >= e || participator.empty()) return false;

  // The title is identical to another
  if (!m_storage->queryMeeting([title](const Meeting& m) -> bool {
    return title == m.getTitle();
  }).empty()) return false;

  // Sponsor does not exist
  if (m_storage->queryUser([userName](const User& user) -> bool {
    return user.getName() == userName;
  }).empty()) return false;

  // Sponsor is busy
  if (!m_storage->queryMeeting([s, e, userName](const Meeting& m) -> bool {
    return !(s >= m.getEndDate() || e <= m.getStartDate()) && 
            (m.getSponsor() == userName || m.isParticipator(userName));
  }).empty()) return false;

  
  Meeting meeting(userName, {}, s, e, title);

  for (auto name : participator) {
    // Participator does not exist
    if (m_storage->queryUser([name](const User& user) -> bool {
      return user.getName() == name;
    }).empty()) return false;

    // Participator is busy
    if (!m_storage->queryMeeting([s,e,name](const Meeting& m) -> bool {
      return  (m.isParticipator(name) || m.getSponsor() == name) && 
              !(s >= m.getEndDate() || e <= m.getStartDate());
    }).empty()) return false;

    // Duplications of participators 
    if (meeting.isParticipator(name) || name == userName) return false;
    
    meeting.addParticipator(name);
  }

  m_storage->createMeeting(meeting);

  return true;
}

AgendaService::AgendaService() { m_storage = Storage::getInstance(); }

AgendaService::~AgendaService() { m_storage->sync(); }

void AgendaService::startAgenda(void) {
  m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
  m_storage->sync();
}