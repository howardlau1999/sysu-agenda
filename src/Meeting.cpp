#include "Meeting.hpp"
#include <algorithm>

Meeting::Meeting(const Meeting& t_meeting) {
    m_sponsor = t_meeting.m_sponsor;
    m_participators = t_meeting.m_participators;
    m_startDate = t_meeting.m_startDate;
    m_endDate = t_meeting.m_endDate;
    m_title = t_meeting.m_title;
}

std::string Meeting::getSponsor() const {
    return m_sponsor;
}


void Meeting::setSponsor(std::string t_sponsor) {
    m_sponsor = t_sponsor;
}


std::vector<std::string> Meeting::getParticipator() const {
    return m_participators;
}


void Meeting::setParticipator(std::vector<std::string> t_participators) {
    m_participators = t_participators;
}


Date Meeting::getStartDate() const {
    return m_startDate;
}


void Meeting::setStartDate(Date t_startDate) {
    m_startDate = t_startDate;
}


Date Meeting::getEndDate() const {
    return m_endDate;
}


void Meeting::setEndDate(Date t_endDate) {
    m_endDate = t_endDate;
}


std::string Meeting::getTitle() const {
    return m_title;
}


void Meeting::setTitle(std::string t_title) {
    m_title = t_title;
}



bool Meeting::isParticipator(const std::string t_username) const {
    return std::find(m_participators.begin(), m_participators.end(), t_username) != m_participators.end();
}