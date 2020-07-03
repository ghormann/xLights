/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include "SubModel.h"

#include <wx/xml/xml.h>
#include <wx/tokenzr.h>

#include <log4cpp/Category.hh>

SubModel::SubModel(Model* p, wxXmlNode* n) : Model(p->GetModelManager()), parent(p) {

    // copy change count from owning model ... otherwise we lose track of changes when the model is recreated
    changeCount = p->changeCount;

    _nodesAllValid = true;
    ModelXml = n;
    StrobeRate = 0;
    Nodes.clear();
    DisplayAs = "SubModel";

    name = n->GetAttribute("name").Trim(true).Trim(false).ToStdString();
    parm1 = 1;
    parm2 = 1;
    parm3 = 1;

    //static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    //logger_base.debug("Submodel init %s:%s", (const char*)p->GetFullName().c_str(), (const char*)name.c_str());

    bool vert = n->GetAttribute("layout") == "vertical";
    bool isRanges = n->GetAttribute("type", "ranges") == "ranges";

    unsigned int startChannel = UINT32_MAX;
    if (isRanges) {
        int row = 0;
        int col = 0;
        int line = 0;
        int maxRow = 0;
        int maxCol = 0;
        while (n->HasAttribute(wxString::Format("line%d", line))) {
            wxString nodes = n->GetAttribute(wxString::Format("line%d", line));
            //logger_base.debug("    Line %d: %s", line, (const char*)nodes.c_str());
            wxStringTokenizer wtkz(nodes, ",");
            while (wtkz.HasMoreTokens()) {
                wxString valstr = wtkz.GetNextToken();

                int start, end;
                if (valstr.Contains("-")) {
                    int idx = valstr.Index('-');
                    start = wxAtoi(valstr.Left(idx));
                    end = wxAtoi(valstr.Right(valstr.size() - idx - 1));
                }
                else {
                    start = end = wxAtoi(valstr);
                }
                if (start == 0)
                {
                    if (vert) {
                        row++;
                    }
                    else {
                        col++;
                    }
                }
                else
                {
                    start--;
                    end--;
                    bool done = false;
                    int nn = start;
                    while (!done) {
                        if (nn < p->GetNodeCount()) {
                            NodeBaseClass* node = p->Nodes[nn]->clone();
                            startChannel = (std::min)(startChannel, node->ActChan);
                            Nodes.push_back(NodeBaseClassPtr(node));
                            for (auto c = node->Coords.begin(); c != node->Coords.end(); ++c) {
                                c->bufX = col;
                                c->bufY = row;
                            }
                            if (vert) {
                                row++;
                            }
                            else {
                                col++;
                            }
                        }
                        else
                        {
                            _nodesAllValid = false;
                        }
                        if (start > end) {
                            nn--;
                            done = nn < end;
                        }
                        else {
                            nn++;
                            done = nn > end;
                        }
                    }
                }
            }
            if (vert) {
                row--;
            }
            else {
                col--;
            }
            if (maxRow < row) {
                maxRow = row;
            }
            if (maxCol < col) {
                maxCol = col;
            }
            if (vert) {
                row = 0;
                col++;
            }
            else {
                col = 0;
                row++;
            }
            line++;
        }
        SetBufferSize(maxRow + 1, maxCol + 1);
    }
    else {
        wxString range = n->GetAttribute("subBuffer");
        float x1 = 0;
        float x2 = 100;
        float y1 = 0;
        float y2 = 100;
        if (range != "") {
            wxArrayString v = wxSplit(range, 'x');
            x1 = v.size() > 0 ? wxAtof(v[0]) : 0.0;
            y1 = v.size() > 1 ? wxAtof(v[1]) : 0.0;
            x2 = v.size() > 2 ? wxAtof(v[2]) : 100.0;
            y2 = v.size() > 3 ? wxAtof(v[3]) : 100.0;
        }

        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);

        x1 *= (float)p->GetDefaultBufferWi();
        x2 *= (float)p->GetDefaultBufferWi();
        y1 *= (float)p->GetDefaultBufferHt();
        y2 *= (float)p->GetDefaultBufferHt();
        x1 /= 100.0;
        x2 /= 100.0;
        y1 /= 100.0;
        y2 /= 100.0;
        x1 = std::ceil(x1);
        y1 = std::ceil(y1);

        float minx = 10000;
        float maxx = -1;
        float miny = 10000;
        float maxy = -1;

        int nn = p->GetNodeCount();
        for (int m = 0; m < nn; m++) {
            if (p->IsNodeInBufferRange(m, x1, y1, x2, y2)) {
                NodeBaseClass* node = p->Nodes[m]->clone();
                for (auto c = node->Coords.begin(); c != node->Coords.end(); ++c) {

                    if (c->bufX < minx) minx = c->bufX;
                    if (c->bufY < miny) miny = c->bufY;
                    if (c->bufX > maxx) maxx = c->bufX;
                    if (c->bufY > maxy) maxy = c->bufY;
                }
                delete node;
            }
        }

        for (int m = 0; m < nn; m++) {
            if (p->IsNodeInBufferRange(m, x1, y1, x2, y2)) {
                NodeBaseClass* node = p->Nodes[m]->clone();
                startChannel = (std::min)(startChannel, node->ActChan);
                Nodes.push_back(NodeBaseClassPtr(node));
                for (auto c = node->Coords.begin(); c != node->Coords.end(); ++c) {
                    c->bufX -= minx;
                    c->bufY -= miny;
                }
            }
        }

        if (maxx < minx || maxy < miny || Nodes.size() == 0) {
            // invalid buffer, set it to just a 1x1 as 0x0 can cause some render issues
            SetBufferSize(1, 1);
        }
        else {
            x2 = int(std::ceil(maxx - minx)) + 1;
            y2 = int(std::ceil(maxy - miny)) + 1;
            SetBufferSize(y2, x2);
        }
    }
    //ModelStartChannel is 1 based
    this->ModelStartChannel = wxString::Format("%u", (startChannel + 1));

    // inheret pixel properties from parent model
    pixelStyle = p->pixelStyle;
    transparency = p->transparency;
    blackTransparency = p->blackTransparency;
    pixelSize = p->pixelSize;
}
