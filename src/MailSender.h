#ifndef MAILSENDER_H
#define MAILSENDER_H

#include <string>
#include <vector>

namespace Mail
{
    std::string StringReplace(std::string s, const std::string &toReplace, const std::string &replaceWith);
 
    bool CreateMailSendingScript();
    int SendMail(const std::string &subject, const std::string &body, const std::string &attachments, const bool &delScript);
    int SendMail(const std::string &subject, const std::string &body, const std::vector<std::string> &att, const bool &delScript);
}

#endif