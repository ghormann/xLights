#pragma once

/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include <vector>
#include "ViewObject.h"
#include "Image.h"
#include "tiny_obj_loader.h"

class ModelPreview;
namespace DrawGLUtils {
    class xl3DMesh;
}

class MeshObject : public ObjectWithScreenLocation<BoxedScreenLocation>
{
    public:
        MeshObject(wxXmlNode *node, const ViewObjectManager &manager);
        virtual ~MeshObject();

        virtual void InitModel() override;

        virtual void AddTypeProperties(wxPropertyGridInterface* grid) override;
        virtual void UpdateTypeProperties(wxPropertyGridInterface* grid) override {}

        int OnPropertyGridChange(wxPropertyGridInterface *grid, wxPropertyGridEvent& event) override;

        virtual void Draw(ModelPreview* preview, DrawGLUtils::xl3Accumulator &va3, DrawGLUtils::xl3Accumulator &tva3, bool allowSelected = false) override;
        virtual std::list<std::string> GetFileReferences() override;
        virtual bool CleanupFileLocations(xLightsFrame* frame) override;
        virtual std::list<std::string> CheckModelSettings() override;
        virtual void uncacheDisplayObjects();
        virtual void IncrementChangeCount() override;

    protected:

        void loadObject();
    private:
        std::string _objFile;
        float width;
        float height;
        float depth;
        float brightness;
        bool obj_loaded;
        bool mesh_only;
        bool diffuse_colors;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::map<std::string, Image*> textures;
        std::vector<int> lines;
        float bmin[3];
        float bmax[3];

        DrawGLUtils::xl3DMesh *mesh3d;
};

