/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include "PlayListItemFPPEvent.h"
#include "PlayListItemFPPEventPanel.h"
#include "PlayList.h"
#include <wx/xml/xml.h>
#include <wx/notebook.h>
#include <log4cpp/Category.hh>
#include <wx/socket.h>
#include "../xLights/outputs/IPOutput.h"
#include "../Control.h"
#include <wx/protocol/http.h>
#include "../../xLights/UtilFunctions.h"
#include "../xSMSDaemon/Curl.h"

class FPPEventThread : public wxThread
{
    std::string _ip;
    std::string _eventString;

public:
    FPPEventThread(const std::string& ip, const std::string& eventString) : 
        _ip(ip), _eventString(eventString) { }

    virtual void* Entry() override
    {
        log4cpp::Category& logger_base = log4cpp::Category::getInstance(std::string("log_base"));

        logger_base.debug("PlayListFPPEvent in thread.");

        if (IsIPValidOrHostname(_ip) && _ip != "255.255.255.255")
        {
#ifdef EXTREME_FPPEVENT_LOGGING
            logger_base.debug("   Getting URL");
#endif
            std::string url = "http://" + _ip + "/fppxml.php?command=triggerEvent&id=" + _eventString;
            logger_base.debug("FPP Event sent %s%s", (const char*)_ip.c_str(), (const char*)url.c_str());
            auto res = Curl::HTTPSGet(url, "", "", 1);
            logger_base.info("CURL GET: %s", (const char*)res.c_str());
        }
        else
        {
            // I am pretty sure this no longer works in FPP ... at least I dont seem to be able to make it work

            // Open the socket
            wxIPV4address localaddr;
            if (IPOutput::GetLocalIP() == "")
            {
                localaddr.AnyAddress();
            }
            else
            {
                localaddr.Hostname(IPOutput::GetLocalIP());
            }

            wxDatagramSocket* socket = new wxDatagramSocket(localaddr, wxSOCKET_NOWAIT | wxSOCKET_BROADCAST);
            if (socket == nullptr)
            {
                logger_base.error("Error opening datagram for FPP Event send. %s", (const char*)localaddr.IPAddress().c_str());
            }
            else if (!socket->IsOk())
            {
                logger_base.error("Error opening datagram for FPP Event send. %s OK : FALSE", (const char*)localaddr.IPAddress().c_str());
                delete socket;
                socket = nullptr;
            }
            else if (socket->Error())
            {
                logger_base.error("Error opening datagram for FPP Event send. %d : %s %s", socket->LastError(), (const char*)DecodeIPError(socket->LastError()).c_str(), (const char*)localaddr.IPAddress().c_str());
                delete socket;
                socket = nullptr;
            }
            else
            {
                logger_base.info("FPP Event send datagram opened successfully.");
            }

            if (socket != nullptr)
            {
                wxIPV4address remoteAddr;
                //remoteAddr.BroadcastAddress();
                remoteAddr.Hostname("255.255.255.255");
                remoteAddr.Service(FPP_CTRL_PORT);

                wxASSERT(sizeof(ControlPkt) == 7); // ensure data is packed correctly

                int dbufsize = sizeof(ControlPkt) + _eventString.size() + 1;
                unsigned char* dbuffer = (unsigned char*)malloc(dbufsize);
                memset(dbuffer, 0x00, dbufsize);

                if (dbuffer != nullptr)
                {
                    ControlPkt* cp = (ControlPkt*)dbuffer;
                    strncpy(cp->fppd, "FPPD", 4);
                    cp->pktType = CTRL_PKT_EVENT;
                    cp->extraDataLen = dbufsize - sizeof(ControlPkt) - 1;
                    strcpy((char*)(dbuffer + sizeof(ControlPkt)), _eventString.c_str());

                    socket->SendTo(remoteAddr, dbuffer, dbufsize - 1);
                    logger_base.info("FPP Event broadcast %s.", (const char*)_eventString.c_str());

                    free(dbuffer);
                }

                logger_base.info("FPP Event send datagram closed.");
                socket->Close();
                delete socket;
            }
        }

        logger_base.debug("PlayListFPPEvent thread done.");

        return nullptr;
    }
};

PlayListItemFPPEvent::PlayListItemFPPEvent(wxXmlNode* node) : PlayListItem(node)
{
    PlayListItemFPPEvent::Load(node);
}

void PlayListItemFPPEvent::Load(wxXmlNode* node)
{
    PlayListItem::Load(node);
    _major = wxAtoi(node->GetAttribute("Major", "1"));
    _minor = wxAtoi(node->GetAttribute("Minor", "1"));
    _ip = node->GetAttribute("IP", "");
}

PlayListItemFPPEvent::PlayListItemFPPEvent() : PlayListItem()
{
    _type = "PLIFPPEVENT";
}

PlayListItem* PlayListItemFPPEvent::Copy() const
{
    PlayListItemFPPEvent* res = new PlayListItemFPPEvent();
    res->_major = _major;
    res->_minor = _minor;
    res->_ip = _ip;
    res->_started = false;
    PlayListItem::Copy(res);

    return res;
}

wxXmlNode* PlayListItemFPPEvent::Save()
{
    wxXmlNode * node = new wxXmlNode(nullptr, wxXML_ELEMENT_NODE, GetType());

    node->AddAttribute("Major", wxString::Format("%d", _major));
    node->AddAttribute("Minor", wxString::Format("%d", _minor));
    node->AddAttribute("IP", _ip);

    PlayListItem::Save(node);

    return node;
}

std::string PlayListItemFPPEvent::GetTitle() const
{
    return "FPP Event";
}

void PlayListItemFPPEvent::Configure(wxNotebook* notebook)
{
    notebook->AddPage(new PlayListItemFPPEventPanel(notebook, this), GetTitle(), true);
}

std::string PlayListItemFPPEvent::GetNameNoTime() const
{
    if (_name != "") return _name;

    return GetEventString();
}

std::string PlayListItemFPPEvent::GetEventString() const
{
    return wxString::Format("%02d_%02d", _major, _minor).ToStdString();
}

std::string PlayListItemFPPEvent::GetTooltip()
{
    return "";
}

//#define EXTREME_FPPEVENT_LOGGING

void PlayListItemFPPEvent::Frame(uint8_t* buffer, size_t size, size_t ms, size_t framems, bool outputframe)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    if (ms >= _delay && !_started)
    {
        _started = true;

        std::string eventstring = GetEventString();
        logger_base.debug("FPP Event IP '%s' Event String '%s'", (const char*)_ip.c_str(), (const char*)eventstring.c_str());

        FPPEventThread* thread = new FPPEventThread(_ip, eventstring);
        thread->Run();
        wxMicroSleep(1); // encourage the thread to run        }
    }
}

void PlayListItemFPPEvent::Start(long stepLengthMS)
{
    PlayListItem::Start(stepLengthMS);

    _started = false;
}