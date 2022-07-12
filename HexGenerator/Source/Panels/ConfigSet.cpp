/*  Parent Class for all Configuration Files
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <QFile>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include "Common/Cpp/Exceptions.h"
#include "Common/Cpp/Json/JsonArray.h"
#include "Common/Cpp/Json/JsonObject.h"
#include "Tools/Tools.h"
#include "Tools/PersistentSettings.h"
#include "UI/MainWindow.h"
#include "ConfigSet.h"

namespace PokemonAutomation{


const std::string ConfigSet::JSON_CONFIG_NAME   = "0-ConfigName";
const std::string ConfigSet::JSON_CONFIG_PATH   = "1-ConfigPath";
const std::string ConfigSet::JSON_DOCUMENTATION = "2-Documentation";
const std::string ConfigSet::JSON_DESCRIPTION   = "3-Description";
const std::string ConfigSet::JSON_HEADER        = "4-Header";
const std::string ConfigSet::JSON_OPTIONS       = "5-Options";

ConfigSet::ConfigSet(std::string category, const JsonObject& obj)
    : RightPanel(
        std::move(category),
        obj.get_string_throw(JSON_CONFIG_NAME),
        obj.get_string_throw(JSON_DOCUMENTATION)
    )
    , m_path(obj.get_string_throw(JSON_CONFIG_PATH))
    , m_description(obj.get_string_throw(JSON_DESCRIPTION))
    , m_header(obj.get_string_throw(JSON_HEADER))
{}

JsonObject ConfigSet::to_json() const{
    JsonObject root;
    root[JSON_CONFIG_NAME] = m_name;
    root[JSON_CONFIG_PATH] = m_path;
    root[JSON_DOCUMENTATION] = m_name;
    root[JSON_DESCRIPTION] = m_description;
    root[JSON_HEADER] = m_header;
    root[JSON_OPTIONS] = options_json();
    return root;
}
std::string ConfigSet::to_cfile() const{
    std::string body;
    body += "//  This file is generated by the UI. There's no point in editing.\r\n";
    body += "#include \"";
    body += m_header;
    body += "\"\r\n";
    body += options_cpp();
    return body;
}
std::string ConfigSet::save_json() const{
    std::string name = settings.path + CONFIG_FOLDER_NAME + "/" + m_category + "/" + m_name + ".json";
    to_json().dump(name);
    return name;
}
std::string ConfigSet::save_cfile() const{
    std::string name = settings.path + SOURCE_FOLDER_NAME + "/" + m_path + "/"+ m_name + ".c";
    std::string cpp = to_cfile();
    QFile file(QString::fromStdString(name));
    if (!file.open(QFile::WriteOnly)){
        throw FileException(nullptr, PA_CURRENT_FUNCTION, "Unable to create source file.", name);
    }
    if (file.write(cpp.c_str(), cpp.size()) != cpp.size()){
        throw FileException(nullptr, PA_CURRENT_FUNCTION, "Unable to write source file.", name);
    }
    file.close();
    return name;
}


QWidget* ConfigSet::make_ui(MainWindow& parent){
    QWidget* box = new QWidget(&parent);
    QVBoxLayout* layout = new QVBoxLayout(box);
    layout->setMargin(0);
//    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(new QLabel(QString::fromStdString("<b><font size=4>Name:</font></b> " + m_name)));
    {
        QLabel* text = new QLabel(QString::fromStdString("<b><font size=4>Description:</font></b> " + m_description));
        layout->addWidget(text);
        text->setWordWrap(true);
    }
    {
        std::string path = GITHUB_REPO + m_doc_link;
        QLabel* text = new QLabel(QString::fromStdString("<font size=4><a href=\"" + path + "\">Online Documentation for " + m_name + "</a></font>"));
        layout->addWidget(text);
        text->setTextFormat(Qt::RichText);
        text->setTextInteractionFlags(Qt::TextBrowserInteraction);
        text->setOpenExternalLinks(true);
    }
    layout->addWidget(make_options_body(*box), Qt::AlignBottom);

    QHBoxLayout* row = new QHBoxLayout();
    layout->addLayout(row);
    {
        QPushButton* button = new QPushButton("Save Settings", box);
        connect(
            button, &QPushButton::clicked,
            this, [&](bool){
                try{
                    std::string json = save_json();
                    std::string cfile = save_cfile();
                    QMessageBox box;
                    box.information(nullptr, "Success!", QString::fromStdString("Settings saved to:\n" + json + "\n" + cfile));
                }catch (const Exception& e){
                    QMessageBox box;
                    box.critical(nullptr, "Error", QString::fromStdString(e.message()));
                    return;
                }
            }
        );
        row->addWidget(button);
    }
    {
        QPushButton* button = new QPushButton("Restore Defaults", box);
        connect(
            button, &QPushButton::clicked,
            this, [&](bool){
                restore_defaults();
                parent.change_panel(*this);
            }
        );
        row->addWidget(button);
    }
#if 0
    {
        QPushButton* button = new QPushButton("Save and generate .hex file!", box);
        QFont font = button->font();
        font.setPointSize(16);
        button->setFont(font);
        connect(
            button, &QPushButton::clicked,
            this, [&](bool){
//                save_and_build(mcu_list.get_MCU());
            }
        );
        layout->addWidget(button);
    }
#endif
    return box;
}



JsonArray ConfigSet::options_json() const{
    return JsonArray();
}



}


