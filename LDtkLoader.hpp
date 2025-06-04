//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     welcome data = nlohmann::json::parse(jsonString);

#pragma once

#include <optional>
#include "json.hpp"

#include <optional>
#include <stdexcept>
#include <regex>

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>> {
        static void to_json(json & j, const std::shared_ptr<T> & opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::shared_ptr<T> from_json(const json & j) {
            if (j.is_null()) return std::make_shared<T>(); else return std::make_shared<T>(j.get<T>());
        }
    };
    template <typename T>
    struct adl_serializer<std::optional<T>> {
        static void to_json(json & j, const std::optional<T> & opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::optional<T> from_json(const json & j) {
            if (j.is_null()) return std::make_optional<T>(); else return std::make_optional<T>(j.get<T>());
        }
    };
}
#endif

namespace quicktype {
    using nlohmann::json;

    #ifndef NLOHMANN_UNTYPED_quicktype_HELPER
    #define NLOHMANN_UNTYPED_quicktype_HELPER
    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }
    #endif

    #ifndef NLOHMANN_OPTIONAL_quicktype_HELPER
    #define NLOHMANN_OPTIONAL_quicktype_HELPER
    template <typename T>
    inline std::shared_ptr<T> get_heap_optional(const json & j, const char * property) {
        auto it = j.find(property);
        if (it != j.end() && !it->is_null()) {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_heap_optional(const json & j, std::string property) {
        return get_heap_optional<T>(j, property.data());
    }
    template <typename T>
    inline std::optional<T> get_stack_optional(const json & j, const char * property) {
        auto it = j.find(property);
        if (it != j.end() && !it->is_null()) {
            return j.at(property).get<std::optional<T>>();
        }
        return std::optional<T>();
    }

    template <typename T>
    inline std::optional<T> get_stack_optional(const json & j, std::string property) {
        return get_stack_optional<T>(j, property.data());
    }
    #endif

    enum class forcedRefsType : int { boolean, integer, null, number, object, string };

    class appBuildId {
    public:
        appBuildId() = default;
        virtual ~appBuildId() = default;

    private:
        std::string description;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class itemsValue {
    public:
        itemsValue() = default;
        virtual ~itemsValue() = default;

    private:
        std::string ref;

    public:
        const std::string & getRef() const { return ref; }
        std::string & getMutableRef() { return ref; }
        void setRef(const std::string & value) { this->ref = value; }
    };

    enum class customCommandsType : int { array, null };

    class customCommands {
    public:
        customCommands() = default;
        virtual ~customCommands() = default;

    private:
        std::string description;
        itemsValue items;
        std::vector<customCommandsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const itemsValue & getItems() const { return items; }
        itemsValue & getMutableItems() { return items; }
        void setItems(const itemsValue & value) { this->items = value; }

        const std::vector<customCommandsType> & getType() const { return type; }
        std::vector<customCommandsType> & getMutableType() { return type; }
        void setType(const std::vector<customCommandsType> & value) { this->type = value; }
    };

    class defs {
    public:
        defs() = default;
        virtual ~defs() = default;

    private:
        std::string description;
        std::string ref;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getRef() const { return ref; }
        std::string & getMutableRef() { return ref; }
        void setRef(const std::string & value) { this->ref = value; }
    };

    class flagsItems {
    public:
        flagsItems() = default;
        virtual ~flagsItems() = default;

    private:
        std::vector<std::string> itemsEnum;

    public:
        const std::vector<std::string> & getItemsEnum() const { return itemsEnum; }
        std::vector<std::string> & getMutableItemsEnum() { return itemsEnum; }
        void setItemsEnum(const std::vector<std::string> & value) { this->itemsEnum = value; }
    };

    class flags {
    public:
        flags() = default;
        virtual ~flags() = default;

    private:
        std::string description;
        flagsItems items;
        std::vector<customCommandsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const flagsItems & getItems() const { return items; }
        flagsItems & getMutableItems() { return items; }
        void setItems(const flagsItems & value) { this->items = value; }

        const std::vector<customCommandsType> & getType() const { return type; }
        std::vector<customCommandsType> & getMutableType() { return type; }
        void setType(const std::vector<customCommandsType> & value) { this->type = value; }
    };

    class forcedRefs {
    public:
        forcedRefs() = default;
        virtual ~forcedRefs() = default;

    private:
        std::string description;
        std::map<std::string, itemsValue> properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::map<std::string, itemsValue> & getProperties() const { return properties; }
        std::map<std::string, itemsValue> & getMutableProperties() { return properties; }
        void setProperties(const std::map<std::string, itemsValue> & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class identifierStyle {
    public:
        identifierStyle() = default;
        virtual ~identifierStyle() = default;

    private:
        std::string description;
        std::vector<std::string> identifierStyleEnum;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::vector<std::string> & getIdentifierStyleEnum() const { return identifierStyleEnum; }
        std::vector<std::string> & getMutableIdentifierStyleEnum() { return identifierStyleEnum; }
        void setIdentifierStyleEnum(const std::vector<std::string> & value) { this->identifierStyleEnum = value; }
    };

    class worldLayout {
    public:
        worldLayout() = default;
        virtual ~worldLayout() = default;

    private:
        std::string description;
        std::vector<std::optional<std::string>> worldLayoutEnum;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::vector<std::optional<std::string>> & getWorldLayoutEnum() const { return worldLayoutEnum; }
        std::vector<std::optional<std::string>> & getMutableWorldLayoutEnum() { return worldLayoutEnum; }
        void setWorldLayoutEnum(const std::vector<std::optional<std::string>> & value) { this->worldLayoutEnum = value; }
    };

    class ldtkJsonRootProperties {
    public:
        ldtkJsonRootProperties() = default;
        virtual ~ldtkJsonRootProperties() = default;

    private:
        appBuildId backupLimit;
        appBuildId defaultEntityWidth;
        appBuildId backupOnSave;
        appBuildId worldGridWidth;
        appBuildId iid;
        appBuildId defaultLevelBgColor;
        appBuildId bgColor;
        customCommands worlds;
        customCommands toc;
        appBuildId nextUid;
        identifierStyle imageExportMode;
        identifierStyle identifierStyle;
        appBuildId defaultPivotY;
        appBuildId dummyWorldIid;
        customCommands customCommands;
        appBuildId worldGridHeight;
        appBuildId appBuildId;
        appBuildId defaultGridSize;
        worldLayout worldLayout;
        flags flags;
        appBuildId levelNamePattern;
        appBuildId exportPng;
        appBuildId defaultLevelWidth;
        appBuildId pngFilePattern;
        forcedRefs forcedRefs;
        appBuildId exportTiled;
        defs defs;
        customCommands levels;
        appBuildId jsonVersion;
        appBuildId defaultEntityHeight;
        appBuildId defaultPivotX;
        appBuildId defaultLevelHeight;
        appBuildId simplifiedExport;
        appBuildId externalLevels;
        appBuildId tutorialDesc;
        appBuildId minifyJson;
        appBuildId exportLevelBg;
        appBuildId backupRelPath;

    public:
        const appBuildId & getBackupLimit() const { return backupLimit; }
        appBuildId & getMutableBackupLimit() { return backupLimit; }
        void setBackupLimit(const appBuildId & value) { this->backupLimit = value; }

        const appBuildId & getDefaultEntityWidth() const { return defaultEntityWidth; }
        appBuildId & getMutableDefaultEntityWidth() { return defaultEntityWidth; }
        void setDefaultEntityWidth(const appBuildId & value) { this->defaultEntityWidth = value; }

        const appBuildId & getBackupOnSave() const { return backupOnSave; }
        appBuildId & getMutableBackupOnSave() { return backupOnSave; }
        void setBackupOnSave(const appBuildId & value) { this->backupOnSave = value; }

        const appBuildId & getWorldGridWidth() const { return worldGridWidth; }
        appBuildId & getMutableWorldGridWidth() { return worldGridWidth; }
        void setWorldGridWidth(const appBuildId & value) { this->worldGridWidth = value; }

        const appBuildId & getIid() const { return iid; }
        appBuildId & getMutableIid() { return iid; }
        void setIid(const appBuildId & value) { this->iid = value; }

        const appBuildId & getDefaultLevelBgColor() const { return defaultLevelBgColor; }
        appBuildId & getMutableDefaultLevelBgColor() { return defaultLevelBgColor; }
        void setDefaultLevelBgColor(const appBuildId & value) { this->defaultLevelBgColor = value; }

        const appBuildId & getBgColor() const { return bgColor; }
        appBuildId & getMutableBgColor() { return bgColor; }
        void setBgColor(const appBuildId & value) { this->bgColor = value; }

        const customCommands & getWorlds() const { return worlds; }
        customCommands & getMutableWorlds() { return worlds; }
        void setWorlds(const customCommands & value) { this->worlds = value; }

        const customCommands & getToc() const { return toc; }
        customCommands & getMutableToc() { return toc; }
        void setToc(const customCommands & value) { this->toc = value; }

        const appBuildId & getNextUid() const { return nextUid; }
        appBuildId & getMutableNextUid() { return nextUid; }
        void setNextUid(const appBuildId & value) { this->nextUid = value; }

        const identifierStyle & getImageExportMode() const { return imageExportMode; }
        identifierStyle & getMutableImageExportMode() { return imageExportMode; }
        void setImageExportMode(const identifierStyle & value) { this->imageExportMode = value; }

        const identifierStyle & getIdentifierStyle() const { return identifierStyle; }
        identifierStyle & getMutableIdentifierStyle() { return identifierStyle; }
        void setIdentifierStyle(const identifierStyle & value) { this->identifierStyle = value; }

        const appBuildId & getDefaultPivotY() const { return defaultPivotY; }
        appBuildId & getMutableDefaultPivotY() { return defaultPivotY; }
        void setDefaultPivotY(const appBuildId & value) { this->defaultPivotY = value; }

        const appBuildId & getDummyWorldIid() const { return dummyWorldIid; }
        appBuildId & getMutableDummyWorldIid() { return dummyWorldIid; }
        void setDummyWorldIid(const appBuildId & value) { this->dummyWorldIid = value; }

        const customCommands & getCustomCommands() const { return customCommands; }
        customCommands & getMutableCustomCommands() { return customCommands; }
        void setCustomCommands(const customCommands & value) { this->customCommands = value; }

        const appBuildId & getWorldGridHeight() const { return worldGridHeight; }
        appBuildId & getMutableWorldGridHeight() { return worldGridHeight; }
        void setWorldGridHeight(const appBuildId & value) { this->worldGridHeight = value; }

        const appBuildId & getAppBuildId() const { return appBuildId; }
        appBuildId & getMutableAppBuildId() { return appBuildId; }
        void setAppBuildId(const appBuildId & value) { this->appBuildId = value; }

        const appBuildId & getDefaultGridSize() const { return defaultGridSize; }
        appBuildId & getMutableDefaultGridSize() { return defaultGridSize; }
        void setDefaultGridSize(const appBuildId & value) { this->defaultGridSize = value; }

        const worldLayout & getWorldLayout() const { return worldLayout; }
        worldLayout & getMutableWorldLayout() { return worldLayout; }
        void setWorldLayout(const worldLayout & value) { this->worldLayout = value; }

        const flags & getFlags() const { return flags; }
        flags & getMutableFlags() { return flags; }
        void setFlags(const flags & value) { this->flags = value; }

        const appBuildId & getLevelNamePattern() const { return levelNamePattern; }
        appBuildId & getMutableLevelNamePattern() { return levelNamePattern; }
        void setLevelNamePattern(const appBuildId & value) { this->levelNamePattern = value; }

        const appBuildId & getExportPng() const { return exportPng; }
        appBuildId & getMutableExportPng() { return exportPng; }
        void setExportPng(const appBuildId & value) { this->exportPng = value; }

        const appBuildId & getDefaultLevelWidth() const { return defaultLevelWidth; }
        appBuildId & getMutableDefaultLevelWidth() { return defaultLevelWidth; }
        void setDefaultLevelWidth(const appBuildId & value) { this->defaultLevelWidth = value; }

        const appBuildId & getPngFilePattern() const { return pngFilePattern; }
        appBuildId & getMutablePngFilePattern() { return pngFilePattern; }
        void setPngFilePattern(const appBuildId & value) { this->pngFilePattern = value; }

        const forcedRefs & getForcedRefs() const { return forcedRefs; }
        forcedRefs & getMutableForcedRefs() { return forcedRefs; }
        void setForcedRefs(const forcedRefs & value) { this->forcedRefs = value; }

        const appBuildId & getExportTiled() const { return exportTiled; }
        appBuildId & getMutableExportTiled() { return exportTiled; }
        void setExportTiled(const appBuildId & value) { this->exportTiled = value; }

        const defs & getDefs() const { return defs; }
        defs & getMutableDefs() { return defs; }
        void setDefs(const defs & value) { this->defs = value; }

        const customCommands & getLevels() const { return levels; }
        customCommands & getMutableLevels() { return levels; }
        void setLevels(const customCommands & value) { this->levels = value; }

        const appBuildId & getJsonVersion() const { return jsonVersion; }
        appBuildId & getMutableJsonVersion() { return jsonVersion; }
        void setJsonVersion(const appBuildId & value) { this->jsonVersion = value; }

        const appBuildId & getDefaultEntityHeight() const { return defaultEntityHeight; }
        appBuildId & getMutableDefaultEntityHeight() { return defaultEntityHeight; }
        void setDefaultEntityHeight(const appBuildId & value) { this->defaultEntityHeight = value; }

        const appBuildId & getDefaultPivotX() const { return defaultPivotX; }
        appBuildId & getMutableDefaultPivotX() { return defaultPivotX; }
        void setDefaultPivotX(const appBuildId & value) { this->defaultPivotX = value; }

        const appBuildId & getDefaultLevelHeight() const { return defaultLevelHeight; }
        appBuildId & getMutableDefaultLevelHeight() { return defaultLevelHeight; }
        void setDefaultLevelHeight(const appBuildId & value) { this->defaultLevelHeight = value; }

        const appBuildId & getSimplifiedExport() const { return simplifiedExport; }
        appBuildId & getMutableSimplifiedExport() { return simplifiedExport; }
        void setSimplifiedExport(const appBuildId & value) { this->simplifiedExport = value; }

        const appBuildId & getExternalLevels() const { return externalLevels; }
        appBuildId & getMutableExternalLevels() { return externalLevels; }
        void setExternalLevels(const appBuildId & value) { this->externalLevels = value; }

        const appBuildId & getTutorialDesc() const { return tutorialDesc; }
        appBuildId & getMutableTutorialDesc() { return tutorialDesc; }
        void setTutorialDesc(const appBuildId & value) { this->tutorialDesc = value; }

        const appBuildId & getMinifyJson() const { return minifyJson; }
        appBuildId & getMutableMinifyJson() { return minifyJson; }
        void setMinifyJson(const appBuildId & value) { this->minifyJson = value; }

        const appBuildId & getExportLevelBg() const { return exportLevelBg; }
        appBuildId & getMutableExportLevelBg() { return exportLevelBg; }
        void setExportLevelBg(const appBuildId & value) { this->exportLevelBg = value; }

        const appBuildId & getBackupRelPath() const { return backupRelPath; }
        appBuildId & getMutableBackupRelPath() { return backupRelPath; }
        void setBackupRelPath(const appBuildId & value) { this->backupRelPath = value; }
    };

    class ldtkJsonRoot {
    public:
        ldtkJsonRoot() = default;
        virtual ~ldtkJsonRoot() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        ldtkJsonRootProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const ldtkJsonRootProperties & getProperties() const { return properties; }
        ldtkJsonRootProperties & getMutableProperties() { return properties; }
        void setProperties(const ldtkJsonRootProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class oneOf {
    public:
        oneOf() = default;
        virtual ~oneOf() = default;

    private:
        std::optional<std::vector<forcedRefsType>> type;
        std::optional<std::string> ref;

    public:
        std::optional<std::vector<forcedRefsType>> getType() const { return type; }
        void setType(std::optional<std::vector<forcedRefsType>> value) { this->type = value; }

        std::optional<std::string> getRef() const { return ref; }
        void setRef(std::optional<std::string> value) { this->ref = value; }
    };

    class icon {
    public:
        icon() = default;
        virtual ~icon() = default;

    private:
        std::string description;
        std::vector<oneOf> oneOf;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::vector<oneOf> & getOneOf() const { return oneOf; }
        std::vector<oneOf> & getMutableOneOf() { return oneOf; }
        void setOneOf(const std::vector<oneOf> & value) { this->oneOf = value; }
    };

    class requiredBiomeValuesItems {
    public:
        requiredBiomeValuesItems() = default;
        virtual ~requiredBiomeValuesItems() = default;

    private:
        std::vector<forcedRefsType> type;

    public:
        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class requiredBiomeValues {
    public:
        requiredBiomeValues() = default;
        virtual ~requiredBiomeValues() = default;

    private:
        std::string description;
        requiredBiomeValuesItems items;
        std::vector<customCommandsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const requiredBiomeValuesItems & getItems() const { return items; }
        requiredBiomeValuesItems & getMutableItems() { return items; }
        void setItems(const requiredBiomeValuesItems & value) { this->items = value; }

        const std::vector<customCommandsType> & getType() const { return type; }
        std::vector<customCommandsType> & getMutableType() { return type; }
        void setType(const std::vector<customCommandsType> & value) { this->type = value; }
    };

    class autoLayerRuleGroupProperties {
    public:
        autoLayerRuleGroupProperties() = default;
        virtual ~autoLayerRuleGroupProperties() = default;

    private:
        appBuildId name;
        appBuildId collapsed;
        appBuildId biomeRequirementMode;
        appBuildId color;
        appBuildId isOptional;
        icon icon;
        appBuildId usesWizard;
        appBuildId uid;
        requiredBiomeValues requiredBiomeValues;
        appBuildId active;
        customCommands rules;

    public:
        const appBuildId & getName() const { return name; }
        appBuildId & getMutableName() { return name; }
        void setName(const appBuildId & value) { this->name = value; }

        const appBuildId & getCollapsed() const { return collapsed; }
        appBuildId & getMutableCollapsed() { return collapsed; }
        void setCollapsed(const appBuildId & value) { this->collapsed = value; }

        const appBuildId & getBiomeRequirementMode() const { return biomeRequirementMode; }
        appBuildId & getMutableBiomeRequirementMode() { return biomeRequirementMode; }
        void setBiomeRequirementMode(const appBuildId & value) { this->biomeRequirementMode = value; }

        const appBuildId & getColor() const { return color; }
        appBuildId & getMutableColor() { return color; }
        void setColor(const appBuildId & value) { this->color = value; }

        const appBuildId & getIsOptional() const { return isOptional; }
        appBuildId & getMutableIsOptional() { return isOptional; }
        void setIsOptional(const appBuildId & value) { this->isOptional = value; }

        const icon & getIcon() const { return icon; }
        icon & getMutableIcon() { return icon; }
        void setIcon(const icon & value) { this->icon = value; }

        const appBuildId & getUsesWizard() const { return usesWizard; }
        appBuildId & getMutableUsesWizard() { return usesWizard; }
        void setUsesWizard(const appBuildId & value) { this->usesWizard = value; }

        const appBuildId & getUid() const { return uid; }
        appBuildId & getMutableUid() { return uid; }
        void setUid(const appBuildId & value) { this->uid = value; }

        const requiredBiomeValues & getRequiredBiomeValues() const { return requiredBiomeValues; }
        requiredBiomeValues & getMutableRequiredBiomeValues() { return requiredBiomeValues; }
        void setRequiredBiomeValues(const requiredBiomeValues & value) { this->requiredBiomeValues = value; }

        const appBuildId & getActive() const { return active; }
        appBuildId & getMutableActive() { return active; }
        void setActive(const appBuildId & value) { this->active = value; }

        const customCommands & getRules() const { return rules; }
        customCommands & getMutableRules() { return rules; }
        void setRules(const customCommands & value) { this->rules = value; }
    };

    class autoLayerRuleGroup {
    public:
        autoLayerRuleGroup() = default;
        virtual ~autoLayerRuleGroup() = default;

    private:
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        autoLayerRuleGroupProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const autoLayerRuleGroupProperties & getProperties() const { return properties; }
        autoLayerRuleGroupProperties & getMutableProperties() { return properties; }
        void setProperties(const autoLayerRuleGroupProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class tileRectsIdsItems {
    public:
        tileRectsIdsItems() = default;
        virtual ~tileRectsIdsItems() = default;

    private:
        requiredBiomeValuesItems items;
        std::vector<customCommandsType> type;

    public:
        const requiredBiomeValuesItems & getItems() const { return items; }
        requiredBiomeValuesItems & getMutableItems() { return items; }
        void setItems(const requiredBiomeValuesItems & value) { this->items = value; }

        const std::vector<customCommandsType> & getType() const { return type; }
        std::vector<customCommandsType> & getMutableType() { return type; }
        void setType(const std::vector<customCommandsType> & value) { this->type = value; }
    };

    class tileRectsIds {
    public:
        tileRectsIds() = default;
        virtual ~tileRectsIds() = default;

    private:
        std::string description;
        tileRectsIdsItems items;
        std::vector<customCommandsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const tileRectsIdsItems & getItems() const { return items; }
        tileRectsIdsItems & getMutableItems() { return items; }
        void setItems(const tileRectsIdsItems & value) { this->items = value; }

        const std::vector<customCommandsType> & getType() const { return type; }
        std::vector<customCommandsType> & getMutableType() { return type; }
        void setType(const std::vector<customCommandsType> & value) { this->type = value; }
    };

    class autoRuleDefProperties {
    public:
        autoRuleDefProperties() = default;
        virtual ~autoRuleDefProperties() = default;

    private:
        appBuildId flipX;
        appBuildId pivotX;
        appBuildId perlinActive;
        tileRectsIds tileRectsIds;
        appBuildId perlinScale;
        appBuildId outOfBoundsValue;
        requiredBiomeValues pattern;
        appBuildId tileRandomXMin;
        identifierStyle checker;
        appBuildId perlinOctaves;
        requiredBiomeValues tileIds;
        appBuildId alpha;
        appBuildId tileXOffset;
        appBuildId invalidated;
        appBuildId xModulo;
        appBuildId size;
        appBuildId chance;
        appBuildId breakOnMatch;
        appBuildId tileYOffset;
        appBuildId uid;
        appBuildId perlinSeed;
        appBuildId yOffset;
        appBuildId tileRandomYMax;
        appBuildId tileRandomYMin;
        identifierStyle tileMode;
        appBuildId flipY;
        appBuildId tileRandomXMax;
        appBuildId pivotY;
        appBuildId yModulo;
        appBuildId active;
        appBuildId xOffset;

    public:
        const appBuildId & getFlipX() const { return flipX; }
        appBuildId & getMutableFlipX() { return flipX; }
        void setFlipX(const appBuildId & value) { this->flipX = value; }

        const appBuildId & getPivotX() const { return pivotX; }
        appBuildId & getMutablePivotX() { return pivotX; }
        void setPivotX(const appBuildId & value) { this->pivotX = value; }

        const appBuildId & getPerlinActive() const { return perlinActive; }
        appBuildId & getMutablePerlinActive() { return perlinActive; }
        void setPerlinActive(const appBuildId & value) { this->perlinActive = value; }

        const tileRectsIds & getTileRectsIds() const { return tileRectsIds; }
        tileRectsIds & getMutableTileRectsIds() { return tileRectsIds; }
        void setTileRectsIds(const tileRectsIds & value) { this->tileRectsIds = value; }

        const appBuildId & getPerlinScale() const { return perlinScale; }
        appBuildId & getMutablePerlinScale() { return perlinScale; }
        void setPerlinScale(const appBuildId & value) { this->perlinScale = value; }

        const appBuildId & getOutOfBoundsValue() const { return outOfBoundsValue; }
        appBuildId & getMutableOutOfBoundsValue() { return outOfBoundsValue; }
        void setOutOfBoundsValue(const appBuildId & value) { this->outOfBoundsValue = value; }

        const requiredBiomeValues & getPattern() const { return pattern; }
        requiredBiomeValues & getMutablePattern() { return pattern; }
        void setPattern(const requiredBiomeValues & value) { this->pattern = value; }

        const appBuildId & getTileRandomXMin() const { return tileRandomXMin; }
        appBuildId & getMutableTileRandomXMin() { return tileRandomXMin; }
        void setTileRandomXMin(const appBuildId & value) { this->tileRandomXMin = value; }

        const identifierStyle & getChecker() const { return checker; }
        identifierStyle & getMutableChecker() { return checker; }
        void setChecker(const identifierStyle & value) { this->checker = value; }

        const appBuildId & getPerlinOctaves() const { return perlinOctaves; }
        appBuildId & getMutablePerlinOctaves() { return perlinOctaves; }
        void setPerlinOctaves(const appBuildId & value) { this->perlinOctaves = value; }

        const requiredBiomeValues & getTileIds() const { return tileIds; }
        requiredBiomeValues & getMutableTileIds() { return tileIds; }
        void setTileIds(const requiredBiomeValues & value) { this->tileIds = value; }

        const appBuildId & getAlpha() const { return alpha; }
        appBuildId & getMutableAlpha() { return alpha; }
        void setAlpha(const appBuildId & value) { this->alpha = value; }

        const appBuildId & getTileXOffset() const { return tileXOffset; }
        appBuildId & getMutableTileXOffset() { return tileXOffset; }
        void setTileXOffset(const appBuildId & value) { this->tileXOffset = value; }

        const appBuildId & getInvalidated() const { return invalidated; }
        appBuildId & getMutableInvalidated() { return invalidated; }
        void setInvalidated(const appBuildId & value) { this->invalidated = value; }

        const appBuildId & getXModulo() const { return xModulo; }
        appBuildId & getMutableXModulo() { return xModulo; }
        void setXModulo(const appBuildId & value) { this->xModulo = value; }

        const appBuildId & getSize() const { return size; }
        appBuildId & getMutableSize() { return size; }
        void setSize(const appBuildId & value) { this->size = value; }

        const appBuildId & getChance() const { return chance; }
        appBuildId & getMutableChance() { return chance; }
        void setChance(const appBuildId & value) { this->chance = value; }

        const appBuildId & getBreakOnMatch() const { return breakOnMatch; }
        appBuildId & getMutableBreakOnMatch() { return breakOnMatch; }
        void setBreakOnMatch(const appBuildId & value) { this->breakOnMatch = value; }

        const appBuildId & getTileYOffset() const { return tileYOffset; }
        appBuildId & getMutableTileYOffset() { return tileYOffset; }
        void setTileYOffset(const appBuildId & value) { this->tileYOffset = value; }

        const appBuildId & getUid() const { return uid; }
        appBuildId & getMutableUid() { return uid; }
        void setUid(const appBuildId & value) { this->uid = value; }

        const appBuildId & getPerlinSeed() const { return perlinSeed; }
        appBuildId & getMutablePerlinSeed() { return perlinSeed; }
        void setPerlinSeed(const appBuildId & value) { this->perlinSeed = value; }

        const appBuildId & getYOffset() const { return yOffset; }
        appBuildId & getMutableYOffset() { return yOffset; }
        void setYOffset(const appBuildId & value) { this->yOffset = value; }

        const appBuildId & getTileRandomYMax() const { return tileRandomYMax; }
        appBuildId & getMutableTileRandomYMax() { return tileRandomYMax; }
        void setTileRandomYMax(const appBuildId & value) { this->tileRandomYMax = value; }

        const appBuildId & getTileRandomYMin() const { return tileRandomYMin; }
        appBuildId & getMutableTileRandomYMin() { return tileRandomYMin; }
        void setTileRandomYMin(const appBuildId & value) { this->tileRandomYMin = value; }

        const identifierStyle & getTileMode() const { return tileMode; }
        identifierStyle & getMutableTileMode() { return tileMode; }
        void setTileMode(const identifierStyle & value) { this->tileMode = value; }

        const appBuildId & getFlipY() const { return flipY; }
        appBuildId & getMutableFlipY() { return flipY; }
        void setFlipY(const appBuildId & value) { this->flipY = value; }

        const appBuildId & getTileRandomXMax() const { return tileRandomXMax; }
        appBuildId & getMutableTileRandomXMax() { return tileRandomXMax; }
        void setTileRandomXMax(const appBuildId & value) { this->tileRandomXMax = value; }

        const appBuildId & getPivotY() const { return pivotY; }
        appBuildId & getMutablePivotY() { return pivotY; }
        void setPivotY(const appBuildId & value) { this->pivotY = value; }

        const appBuildId & getYModulo() const { return yModulo; }
        appBuildId & getMutableYModulo() { return yModulo; }
        void setYModulo(const appBuildId & value) { this->yModulo = value; }

        const appBuildId & getActive() const { return active; }
        appBuildId & getMutableActive() { return active; }
        void setActive(const appBuildId & value) { this->active = value; }

        const appBuildId & getXOffset() const { return xOffset; }
        appBuildId & getMutableXOffset() { return xOffset; }
        void setXOffset(const appBuildId & value) { this->xOffset = value; }
    };

    class autoRuleDef {
    public:
        autoRuleDef() = default;
        virtual ~autoRuleDef() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        autoRuleDefProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const autoRuleDefProperties & getProperties() const { return properties; }
        autoRuleDefProperties & getMutableProperties() { return properties; }
        void setProperties(const autoRuleDefProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class customCommandProperties {
    public:
        customCommandProperties() = default;
        virtual ~customCommandProperties() = default;

    private:
        identifierStyle when;
        appBuildId command;

    public:
        const identifierStyle & getWhen() const { return when; }
        identifierStyle & getMutableWhen() { return when; }
        void setWhen(const identifierStyle & value) { this->when = value; }

        const appBuildId & getCommand() const { return command; }
        appBuildId & getMutableCommand() { return command; }
        void setCommand(const appBuildId & value) { this->command = value; }
    };

    class customCommand {
    public:
        customCommand() = default;
        virtual ~customCommand() = default;

    private:
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        customCommandProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const customCommandProperties & getProperties() const { return properties; }
        customCommandProperties & getMutableProperties() { return properties; }
        void setProperties(const customCommandProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class definitionsProperties {
    public:
        definitionsProperties() = default;
        virtual ~definitionsProperties() = default;

    private:
        customCommands tilesets;
        customCommands layers;
        customCommands levelFields;
        customCommands enums;
        customCommands entities;
        customCommands externalEnums;

    public:
        const customCommands & getTilesets() const { return tilesets; }
        customCommands & getMutableTilesets() { return tilesets; }
        void setTilesets(const customCommands & value) { this->tilesets = value; }

        const customCommands & getLayers() const { return layers; }
        customCommands & getMutableLayers() { return layers; }
        void setLayers(const customCommands & value) { this->layers = value; }

        const customCommands & getLevelFields() const { return levelFields; }
        customCommands & getMutableLevelFields() { return levelFields; }
        void setLevelFields(const customCommands & value) { this->levelFields = value; }

        const customCommands & getEnums() const { return enums; }
        customCommands & getMutableEnums() { return enums; }
        void setEnums(const customCommands & value) { this->enums = value; }

        const customCommands & getEntities() const { return entities; }
        customCommands & getMutableEntities() { return entities; }
        void setEntities(const customCommands & value) { this->entities = value; }

        const customCommands & getExternalEnums() const { return externalEnums; }
        customCommands & getMutableExternalEnums() { return externalEnums; }
        void setExternalEnums(const customCommands & value) { this->externalEnums = value; }
    };

    class definitions {
    public:
        definitions() = default;
        virtual ~definitions() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        definitionsProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const definitionsProperties & getProperties() const { return properties; }
        definitionsProperties & getMutableProperties() { return properties; }
        void setProperties(const definitionsProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class entityDefProperties {
    public:
        entityDefProperties() = default;
        virtual ~entityDefProperties() = default;

    private:
        appBuildId tileId;
        appBuildId showName;
        appBuildId tilesetId;
        appBuildId maxHeight;
        identifierStyle limitScope;
        appBuildId pivotX;
        appBuildId maxCount;
        appBuildId allowOutOfBounds;
        appBuildId hollow;
        appBuildId minHeight;
        appBuildId keepAspectRatio;
        appBuildId color;
        appBuildId minWidth;
        icon tileRect;
        appBuildId doc;
        customCommands fieldDefs;
        identifierStyle tileRenderMode;
        identifierStyle limitBehavior;
        appBuildId tileOpacity;
        requiredBiomeValues nineSliceBorders;
        appBuildId resizableX;
        icon uiTileRect;
        appBuildId uid;
        appBuildId lineOpacity;
        appBuildId maxWidth;
        appBuildId resizableY;
        appBuildId exportToToc;
        appBuildId fillOpacity;
        appBuildId height;
        appBuildId identifier;
        appBuildId pivotY;
        identifierStyle renderMode;
        requiredBiomeValues tags;
        appBuildId width;

    public:
        const appBuildId & getTileId() const { return tileId; }
        appBuildId & getMutableTileId() { return tileId; }
        void setTileId(const appBuildId & value) { this->tileId = value; }

        const appBuildId & getShowName() const { return showName; }
        appBuildId & getMutableShowName() { return showName; }
        void setShowName(const appBuildId & value) { this->showName = value; }

        const appBuildId & getTilesetId() const { return tilesetId; }
        appBuildId & getMutableTilesetId() { return tilesetId; }
        void setTilesetId(const appBuildId & value) { this->tilesetId = value; }

        const appBuildId & getMaxHeight() const { return maxHeight; }
        appBuildId & getMutableMaxHeight() { return maxHeight; }
        void setMaxHeight(const appBuildId & value) { this->maxHeight = value; }

        const identifierStyle & getLimitScope() const { return limitScope; }
        identifierStyle & getMutableLimitScope() { return limitScope; }
        void setLimitScope(const identifierStyle & value) { this->limitScope = value; }

        const appBuildId & getPivotX() const { return pivotX; }
        appBuildId & getMutablePivotX() { return pivotX; }
        void setPivotX(const appBuildId & value) { this->pivotX = value; }

        const appBuildId & getMaxCount() const { return maxCount; }
        appBuildId & getMutableMaxCount() { return maxCount; }
        void setMaxCount(const appBuildId & value) { this->maxCount = value; }

        const appBuildId & getAllowOutOfBounds() const { return allowOutOfBounds; }
        appBuildId & getMutableAllowOutOfBounds() { return allowOutOfBounds; }
        void setAllowOutOfBounds(const appBuildId & value) { this->allowOutOfBounds = value; }

        const appBuildId & getHollow() const { return hollow; }
        appBuildId & getMutableHollow() { return hollow; }
        void setHollow(const appBuildId & value) { this->hollow = value; }

        const appBuildId & getMinHeight() const { return minHeight; }
        appBuildId & getMutableMinHeight() { return minHeight; }
        void setMinHeight(const appBuildId & value) { this->minHeight = value; }

        const appBuildId & getKeepAspectRatio() const { return keepAspectRatio; }
        appBuildId & getMutableKeepAspectRatio() { return keepAspectRatio; }
        void setKeepAspectRatio(const appBuildId & value) { this->keepAspectRatio = value; }

        const appBuildId & getColor() const { return color; }
        appBuildId & getMutableColor() { return color; }
        void setColor(const appBuildId & value) { this->color = value; }

        const appBuildId & getMinWidth() const { return minWidth; }
        appBuildId & getMutableMinWidth() { return minWidth; }
        void setMinWidth(const appBuildId & value) { this->minWidth = value; }

        const icon & getTileRect() const { return tileRect; }
        icon & getMutableTileRect() { return tileRect; }
        void setTileRect(const icon & value) { this->tileRect = value; }

        const appBuildId & getDoc() const { return doc; }
        appBuildId & getMutableDoc() { return doc; }
        void setDoc(const appBuildId & value) { this->doc = value; }

        const customCommands & getFieldDefs() const { return fieldDefs; }
        customCommands & getMutableFieldDefs() { return fieldDefs; }
        void setFieldDefs(const customCommands & value) { this->fieldDefs = value; }

        const identifierStyle & getTileRenderMode() const { return tileRenderMode; }
        identifierStyle & getMutableTileRenderMode() { return tileRenderMode; }
        void setTileRenderMode(const identifierStyle & value) { this->tileRenderMode = value; }

        const identifierStyle & getLimitBehavior() const { return limitBehavior; }
        identifierStyle & getMutableLimitBehavior() { return limitBehavior; }
        void setLimitBehavior(const identifierStyle & value) { this->limitBehavior = value; }

        const appBuildId & getTileOpacity() const { return tileOpacity; }
        appBuildId & getMutableTileOpacity() { return tileOpacity; }
        void setTileOpacity(const appBuildId & value) { this->tileOpacity = value; }

        const requiredBiomeValues & getNineSliceBorders() const { return nineSliceBorders; }
        requiredBiomeValues & getMutableNineSliceBorders() { return nineSliceBorders; }
        void setNineSliceBorders(const requiredBiomeValues & value) { this->nineSliceBorders = value; }

        const appBuildId & getResizableX() const { return resizableX; }
        appBuildId & getMutableResizableX() { return resizableX; }
        void setResizableX(const appBuildId & value) { this->resizableX = value; }

        const icon & getUiTileRect() const { return uiTileRect; }
        icon & getMutableUiTileRect() { return uiTileRect; }
        void setUiTileRect(const icon & value) { this->uiTileRect = value; }

        const appBuildId & getUid() const { return uid; }
        appBuildId & getMutableUid() { return uid; }
        void setUid(const appBuildId & value) { this->uid = value; }

        const appBuildId & getLineOpacity() const { return lineOpacity; }
        appBuildId & getMutableLineOpacity() { return lineOpacity; }
        void setLineOpacity(const appBuildId & value) { this->lineOpacity = value; }

        const appBuildId & getMaxWidth() const { return maxWidth; }
        appBuildId & getMutableMaxWidth() { return maxWidth; }
        void setMaxWidth(const appBuildId & value) { this->maxWidth = value; }

        const appBuildId & getResizableY() const { return resizableY; }
        appBuildId & getMutableResizableY() { return resizableY; }
        void setResizableY(const appBuildId & value) { this->resizableY = value; }

        const appBuildId & getExportToToc() const { return exportToToc; }
        appBuildId & getMutableExportToToc() { return exportToToc; }
        void setExportToToc(const appBuildId & value) { this->exportToToc = value; }

        const appBuildId & getFillOpacity() const { return fillOpacity; }
        appBuildId & getMutableFillOpacity() { return fillOpacity; }
        void setFillOpacity(const appBuildId & value) { this->fillOpacity = value; }

        const appBuildId & getHeight() const { return height; }
        appBuildId & getMutableHeight() { return height; }
        void setHeight(const appBuildId & value) { this->height = value; }

        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }

        const appBuildId & getPivotY() const { return pivotY; }
        appBuildId & getMutablePivotY() { return pivotY; }
        void setPivotY(const appBuildId & value) { this->pivotY = value; }

        const identifierStyle & getRenderMode() const { return renderMode; }
        identifierStyle & getMutableRenderMode() { return renderMode; }
        void setRenderMode(const identifierStyle & value) { this->renderMode = value; }

        const requiredBiomeValues & getTags() const { return tags; }
        requiredBiomeValues & getMutableTags() { return tags; }
        void setTags(const requiredBiomeValues & value) { this->tags = value; }

        const appBuildId & getWidth() const { return width; }
        appBuildId & getMutableWidth() { return width; }
        void setWidth(const appBuildId & value) { this->width = value; }
    };

    class entityDef {
    public:
        entityDef() = default;
        virtual ~entityDef() = default;

    private:
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        entityDefProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const entityDefProperties & getProperties() const { return properties; }
        entityDefProperties & getMutableProperties() { return properties; }
        void setProperties(const entityDefProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class entityInstanceProperties {
    public:
        entityInstanceProperties() = default;
        virtual ~entityInstanceProperties() = default;

    private:
        appBuildId iid;
        appBuildId defUid;
        appBuildId identifier;
        icon tile;
        requiredBiomeValues px;
        appBuildId worldX;
        appBuildId worldY;
        appBuildId smartColor;
        requiredBiomeValues grid;
        requiredBiomeValues pivot;
        customCommands fieldInstances;
        appBuildId height;
        requiredBiomeValues tags;
        appBuildId width;

    public:
        const appBuildId & getIid() const { return iid; }
        appBuildId & getMutableIid() { return iid; }
        void setIid(const appBuildId & value) { this->iid = value; }

        const appBuildId & getDefUid() const { return defUid; }
        appBuildId & getMutableDefUid() { return defUid; }
        void setDefUid(const appBuildId & value) { this->defUid = value; }

        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }

        const icon & getTile() const { return tile; }
        icon & getMutableTile() { return tile; }
        void setTile(const icon & value) { this->tile = value; }

        const requiredBiomeValues & getPx() const { return px; }
        requiredBiomeValues & getMutablePx() { return px; }
        void setPx(const requiredBiomeValues & value) { this->px = value; }

        const appBuildId & getWorldX() const { return worldX; }
        appBuildId & getMutableWorldX() { return worldX; }
        void setWorldX(const appBuildId & value) { this->worldX = value; }

        const appBuildId & getWorldY() const { return worldY; }
        appBuildId & getMutableWorldY() { return worldY; }
        void setWorldY(const appBuildId & value) { this->worldY = value; }

        const appBuildId & getSmartColor() const { return smartColor; }
        appBuildId & getMutableSmartColor() { return smartColor; }
        void setSmartColor(const appBuildId & value) { this->smartColor = value; }

        const requiredBiomeValues & getGrid() const { return grid; }
        requiredBiomeValues & getMutableGrid() { return grid; }
        void setGrid(const requiredBiomeValues & value) { this->grid = value; }

        const requiredBiomeValues & getPivot() const { return pivot; }
        requiredBiomeValues & getMutablePivot() { return pivot; }
        void setPivot(const requiredBiomeValues & value) { this->pivot = value; }

        const customCommands & getFieldInstances() const { return fieldInstances; }
        customCommands & getMutableFieldInstances() { return fieldInstances; }
        void setFieldInstances(const customCommands & value) { this->fieldInstances = value; }

        const appBuildId & getHeight() const { return height; }
        appBuildId & getMutableHeight() { return height; }
        void setHeight(const appBuildId & value) { this->height = value; }

        const requiredBiomeValues & getTags() const { return tags; }
        requiredBiomeValues & getMutableTags() { return tags; }
        void setTags(const requiredBiomeValues & value) { this->tags = value; }

        const appBuildId & getWidth() const { return width; }
        appBuildId & getMutableWidth() { return width; }
        void setWidth(const appBuildId & value) { this->width = value; }
    };

    class entityInstance {
    public:
        entityInstance() = default;
        virtual ~entityInstance() = default;

    private:
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        entityInstanceProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const entityInstanceProperties & getProperties() const { return properties; }
        entityInstanceProperties & getMutableProperties() { return properties; }
        void setProperties(const entityInstanceProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class entityReferenceInfosProperties {
    public:
        entityReferenceInfosProperties() = default;
        virtual ~entityReferenceInfosProperties() = default;

    private:
        appBuildId worldIid;
        appBuildId entityIid;
        appBuildId layerIid;
        appBuildId levelIid;

    public:
        const appBuildId & getWorldIid() const { return worldIid; }
        appBuildId & getMutableWorldIid() { return worldIid; }
        void setWorldIid(const appBuildId & value) { this->worldIid = value; }

        const appBuildId & getEntityIid() const { return entityIid; }
        appBuildId & getMutableEntityIid() { return entityIid; }
        void setEntityIid(const appBuildId & value) { this->entityIid = value; }

        const appBuildId & getLayerIid() const { return layerIid; }
        appBuildId & getMutableLayerIid() { return layerIid; }
        void setLayerIid(const appBuildId & value) { this->layerIid = value; }

        const appBuildId & getLevelIid() const { return levelIid; }
        appBuildId & getMutableLevelIid() { return levelIid; }
        void setLevelIid(const appBuildId & value) { this->levelIid = value; }
    };

    class entityReferenceInfos {
    public:
        entityReferenceInfos() = default;
        virtual ~entityReferenceInfos() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        entityReferenceInfosProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const entityReferenceInfosProperties & getProperties() const { return properties; }
        entityReferenceInfosProperties & getMutableProperties() { return properties; }
        void setProperties(const entityReferenceInfosProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class enumDefProperties {
    public:
        enumDefProperties() = default;
        virtual ~enumDefProperties() = default;

    private:
        appBuildId externalFileChecksum;
        appBuildId externalRelPath;
        appBuildId uid;
        customCommands values;
        appBuildId iconTilesetUid;
        appBuildId identifier;
        requiredBiomeValues tags;

    public:
        const appBuildId & getExternalFileChecksum() const { return externalFileChecksum; }
        appBuildId & getMutableExternalFileChecksum() { return externalFileChecksum; }
        void setExternalFileChecksum(const appBuildId & value) { this->externalFileChecksum = value; }

        const appBuildId & getExternalRelPath() const { return externalRelPath; }
        appBuildId & getMutableExternalRelPath() { return externalRelPath; }
        void setExternalRelPath(const appBuildId & value) { this->externalRelPath = value; }

        const appBuildId & getUid() const { return uid; }
        appBuildId & getMutableUid() { return uid; }
        void setUid(const appBuildId & value) { this->uid = value; }

        const customCommands & getValues() const { return values; }
        customCommands & getMutableValues() { return values; }
        void setValues(const customCommands & value) { this->values = value; }

        const appBuildId & getIconTilesetUid() const { return iconTilesetUid; }
        appBuildId & getMutableIconTilesetUid() { return iconTilesetUid; }
        void setIconTilesetUid(const appBuildId & value) { this->iconTilesetUid = value; }

        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }

        const requiredBiomeValues & getTags() const { return tags; }
        requiredBiomeValues & getMutableTags() { return tags; }
        void setTags(const requiredBiomeValues & value) { this->tags = value; }
    };

    class enumDef {
    public:
        enumDef() = default;
        virtual ~enumDef() = default;

    private:
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        enumDefProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const enumDefProperties & getProperties() const { return properties; }
        enumDefProperties & getMutableProperties() { return properties; }
        void setProperties(const enumDefProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class enumDefValuesProperties {
    public:
        enumDefValuesProperties() = default;
        virtual ~enumDefValuesProperties() = default;

    private:
        appBuildId tileId;
        appBuildId color;
        icon tileRect;
        appBuildId id;
        requiredBiomeValues tileSrcRect;

    public:
        const appBuildId & getTileId() const { return tileId; }
        appBuildId & getMutableTileId() { return tileId; }
        void setTileId(const appBuildId & value) { this->tileId = value; }

        const appBuildId & getColor() const { return color; }
        appBuildId & getMutableColor() { return color; }
        void setColor(const appBuildId & value) { this->color = value; }

        const icon & getTileRect() const { return tileRect; }
        icon & getMutableTileRect() { return tileRect; }
        void setTileRect(const icon & value) { this->tileRect = value; }

        const appBuildId & getId() const { return id; }
        appBuildId & getMutableId() { return id; }
        void setId(const appBuildId & value) { this->id = value; }

        const requiredBiomeValues & getTileSrcRect() const { return tileSrcRect; }
        requiredBiomeValues & getMutableTileSrcRect() { return tileSrcRect; }
        void setTileSrcRect(const requiredBiomeValues & value) { this->tileSrcRect = value; }
    };

    class enumDefValues {
    public:
        enumDefValues() = default;
        virtual ~enumDefValues() = default;

    private:
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        enumDefValuesProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const enumDefValuesProperties & getProperties() const { return properties; }
        enumDefValuesProperties & getMutableProperties() { return properties; }
        void setProperties(const enumDefValuesProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class enumTagValueProperties {
    public:
        enumTagValueProperties() = default;
        virtual ~enumTagValueProperties() = default;

    private:
        requiredBiomeValues tileIds;
        appBuildId enumValueId;

    public:
        const requiredBiomeValues & getTileIds() const { return tileIds; }
        requiredBiomeValues & getMutableTileIds() { return tileIds; }
        void setTileIds(const requiredBiomeValues & value) { this->tileIds = value; }

        const appBuildId & getEnumValueId() const { return enumValueId; }
        appBuildId & getMutableEnumValueId() { return enumValueId; }
        void setEnumValueId(const appBuildId & value) { this->enumValueId = value; }
    };

    class enumTagValue {
    public:
        enumTagValue() = default;
        virtual ~enumTagValue() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        enumTagValueProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const enumTagValueProperties & getProperties() const { return properties; }
        enumTagValueProperties & getMutableProperties() { return properties; }
        void setProperties(const enumTagValueProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class defaultOverride {
    public:
        defaultOverride() = default;
        virtual ~defaultOverride() = default;

    private:
        std::string description;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }
    };

    class fieldDefProperties {
    public:
        fieldDefProperties() = default;
        virtual ~fieldDefProperties() = default;

    private:
        requiredBiomeValues acceptFileTypes;
        appBuildId editorDisplayScale;
        appBuildId searchable;
        appBuildId useForSmartColor;
        appBuildId editorShowInWorld;
        identifierStyle allowedRefs;
        appBuildId editorAlwaysShow;
        appBuildId arrayMinLength;
        appBuildId editorTextSuffix;
        appBuildId min;
        appBuildId type;
        identifierStyle editorDisplayMode;
        appBuildId editorDisplayColor;
        appBuildId canBeNull;
        appBuildId autoChainRef;
        appBuildId doc;
        appBuildId allowedRefsEntityUid;
        appBuildId tilesetUid;
        requiredBiomeValues allowedRefTags;
        appBuildId symmetricalRef;
        appBuildId uid;
        appBuildId editorTextPrefix;
        appBuildId isArray;
        appBuildId exportToToc;
        identifierStyle editorDisplayPos;
        worldLayout textLanguageMode;
        appBuildId max;
        appBuildId allowOutOfLevelRef;
        appBuildId editorCutLongValues;
        defaultOverride defaultOverride;
        identifierStyle editorLinkStyle;
        appBuildId regex;
        appBuildId propertiesType;
        appBuildId identifier;
        appBuildId arrayMaxLength;

    public:
        const requiredBiomeValues & getAcceptFileTypes() const { return acceptFileTypes; }
        requiredBiomeValues & getMutableAcceptFileTypes() { return acceptFileTypes; }
        void setAcceptFileTypes(const requiredBiomeValues & value) { this->acceptFileTypes = value; }

        const appBuildId & getEditorDisplayScale() const { return editorDisplayScale; }
        appBuildId & getMutableEditorDisplayScale() { return editorDisplayScale; }
        void setEditorDisplayScale(const appBuildId & value) { this->editorDisplayScale = value; }

        const appBuildId & getSearchable() const { return searchable; }
        appBuildId & getMutableSearchable() { return searchable; }
        void setSearchable(const appBuildId & value) { this->searchable = value; }

        const appBuildId & getUseForSmartColor() const { return useForSmartColor; }
        appBuildId & getMutableUseForSmartColor() { return useForSmartColor; }
        void setUseForSmartColor(const appBuildId & value) { this->useForSmartColor = value; }

        const appBuildId & getEditorShowInWorld() const { return editorShowInWorld; }
        appBuildId & getMutableEditorShowInWorld() { return editorShowInWorld; }
        void setEditorShowInWorld(const appBuildId & value) { this->editorShowInWorld = value; }

        const identifierStyle & getAllowedRefs() const { return allowedRefs; }
        identifierStyle & getMutableAllowedRefs() { return allowedRefs; }
        void setAllowedRefs(const identifierStyle & value) { this->allowedRefs = value; }

        const appBuildId & getEditorAlwaysShow() const { return editorAlwaysShow; }
        appBuildId & getMutableEditorAlwaysShow() { return editorAlwaysShow; }
        void setEditorAlwaysShow(const appBuildId & value) { this->editorAlwaysShow = value; }

        const appBuildId & getArrayMinLength() const { return arrayMinLength; }
        appBuildId & getMutableArrayMinLength() { return arrayMinLength; }
        void setArrayMinLength(const appBuildId & value) { this->arrayMinLength = value; }

        const appBuildId & getEditorTextSuffix() const { return editorTextSuffix; }
        appBuildId & getMutableEditorTextSuffix() { return editorTextSuffix; }
        void setEditorTextSuffix(const appBuildId & value) { this->editorTextSuffix = value; }

        const appBuildId & getMin() const { return min; }
        appBuildId & getMutableMin() { return min; }
        void setMin(const appBuildId & value) { this->min = value; }

        const appBuildId & getType() const { return type; }
        appBuildId & getMutableType() { return type; }
        void setType(const appBuildId & value) { this->type = value; }

        const identifierStyle & getEditorDisplayMode() const { return editorDisplayMode; }
        identifierStyle & getMutableEditorDisplayMode() { return editorDisplayMode; }
        void setEditorDisplayMode(const identifierStyle & value) { this->editorDisplayMode = value; }

        const appBuildId & getEditorDisplayColor() const { return editorDisplayColor; }
        appBuildId & getMutableEditorDisplayColor() { return editorDisplayColor; }
        void setEditorDisplayColor(const appBuildId & value) { this->editorDisplayColor = value; }

        const appBuildId & getCanBeNull() const { return canBeNull; }
        appBuildId & getMutableCanBeNull() { return canBeNull; }
        void setCanBeNull(const appBuildId & value) { this->canBeNull = value; }

        const appBuildId & getAutoChainRef() const { return autoChainRef; }
        appBuildId & getMutableAutoChainRef() { return autoChainRef; }
        void setAutoChainRef(const appBuildId & value) { this->autoChainRef = value; }

        const appBuildId & getDoc() const { return doc; }
        appBuildId & getMutableDoc() { return doc; }
        void setDoc(const appBuildId & value) { this->doc = value; }

        const appBuildId & getAllowedRefsEntityUid() const { return allowedRefsEntityUid; }
        appBuildId & getMutableAllowedRefsEntityUid() { return allowedRefsEntityUid; }
        void setAllowedRefsEntityUid(const appBuildId & value) { this->allowedRefsEntityUid = value; }

        const appBuildId & getTilesetUid() const { return tilesetUid; }
        appBuildId & getMutableTilesetUid() { return tilesetUid; }
        void setTilesetUid(const appBuildId & value) { this->tilesetUid = value; }

        const requiredBiomeValues & getAllowedRefTags() const { return allowedRefTags; }
        requiredBiomeValues & getMutableAllowedRefTags() { return allowedRefTags; }
        void setAllowedRefTags(const requiredBiomeValues & value) { this->allowedRefTags = value; }

        const appBuildId & getSymmetricalRef() const { return symmetricalRef; }
        appBuildId & getMutableSymmetricalRef() { return symmetricalRef; }
        void setSymmetricalRef(const appBuildId & value) { this->symmetricalRef = value; }

        const appBuildId & getUid() const { return uid; }
        appBuildId & getMutableUid() { return uid; }
        void setUid(const appBuildId & value) { this->uid = value; }

        const appBuildId & getEditorTextPrefix() const { return editorTextPrefix; }
        appBuildId & getMutableEditorTextPrefix() { return editorTextPrefix; }
        void setEditorTextPrefix(const appBuildId & value) { this->editorTextPrefix = value; }

        const appBuildId & getIsArray() const { return isArray; }
        appBuildId & getMutableIsArray() { return isArray; }
        void setIsArray(const appBuildId & value) { this->isArray = value; }

        const appBuildId & getExportToToc() const { return exportToToc; }
        appBuildId & getMutableExportToToc() { return exportToToc; }
        void setExportToToc(const appBuildId & value) { this->exportToToc = value; }

        const identifierStyle & getEditorDisplayPos() const { return editorDisplayPos; }
        identifierStyle & getMutableEditorDisplayPos() { return editorDisplayPos; }
        void setEditorDisplayPos(const identifierStyle & value) { this->editorDisplayPos = value; }

        const worldLayout & getTextLanguageMode() const { return textLanguageMode; }
        worldLayout & getMutableTextLanguageMode() { return textLanguageMode; }
        void setTextLanguageMode(const worldLayout & value) { this->textLanguageMode = value; }

        const appBuildId & getMax() const { return max; }
        appBuildId & getMutableMax() { return max; }
        void setMax(const appBuildId & value) { this->max = value; }

        const appBuildId & getAllowOutOfLevelRef() const { return allowOutOfLevelRef; }
        appBuildId & getMutableAllowOutOfLevelRef() { return allowOutOfLevelRef; }
        void setAllowOutOfLevelRef(const appBuildId & value) { this->allowOutOfLevelRef = value; }

        const appBuildId & getEditorCutLongValues() const { return editorCutLongValues; }
        appBuildId & getMutableEditorCutLongValues() { return editorCutLongValues; }
        void setEditorCutLongValues(const appBuildId & value) { this->editorCutLongValues = value; }

        const defaultOverride & getDefaultOverride() const { return defaultOverride; }
        defaultOverride & getMutableDefaultOverride() { return defaultOverride; }
        void setDefaultOverride(const defaultOverride & value) { this->defaultOverride = value; }

        const identifierStyle & getEditorLinkStyle() const { return editorLinkStyle; }
        identifierStyle & getMutableEditorLinkStyle() { return editorLinkStyle; }
        void setEditorLinkStyle(const identifierStyle & value) { this->editorLinkStyle = value; }

        const appBuildId & getRegex() const { return regex; }
        appBuildId & getMutableRegex() { return regex; }
        void setRegex(const appBuildId & value) { this->regex = value; }

        const appBuildId & getPropertiesType() const { return propertiesType; }
        appBuildId & getMutablePropertiesType() { return propertiesType; }
        void setPropertiesType(const appBuildId & value) { this->propertiesType = value; }

        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }

        const appBuildId & getArrayMaxLength() const { return arrayMaxLength; }
        appBuildId & getMutableArrayMaxLength() { return arrayMaxLength; }
        void setArrayMaxLength(const appBuildId & value) { this->arrayMaxLength = value; }
    };

    class fieldDef {
    public:
        fieldDef() = default;
        virtual ~fieldDef() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        fieldDefProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const fieldDefProperties & getProperties() const { return properties; }
        fieldDefProperties & getMutableProperties() { return properties; }
        void setProperties(const fieldDefProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class realEditorValuesItems {
    public:
        realEditorValuesItems() = default;
        virtual ~realEditorValuesItems() = default;

    private:

    public:
    };

    class realEditorValues {
    public:
        realEditorValues() = default;
        virtual ~realEditorValues() = default;

    private:
        std::string description;
        realEditorValuesItems items;
        std::vector<customCommandsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const realEditorValuesItems & getItems() const { return items; }
        realEditorValuesItems & getMutableItems() { return items; }
        void setItems(const realEditorValuesItems & value) { this->items = value; }

        const std::vector<customCommandsType> & getType() const { return type; }
        std::vector<customCommandsType> & getMutableType() { return type; }
        void setType(const std::vector<customCommandsType> & value) { this->type = value; }
    };

    class fieldInstanceProperties {
    public:
        fieldInstanceProperties() = default;
        virtual ~fieldInstanceProperties() = default;

    private:
        appBuildId type;
        appBuildId defUid;
        appBuildId identifier;
        icon tile;
        realEditorValues realEditorValues;
        defaultOverride value;

    public:
        const appBuildId & getType() const { return type; }
        appBuildId & getMutableType() { return type; }
        void setType(const appBuildId & value) { this->type = value; }

        const appBuildId & getDefUid() const { return defUid; }
        appBuildId & getMutableDefUid() { return defUid; }
        void setDefUid(const appBuildId & value) { this->defUid = value; }

        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }

        const icon & getTile() const { return tile; }
        icon & getMutableTile() { return tile; }
        void setTile(const icon & value) { this->tile = value; }

        const realEditorValues & getRealEditorValues() const { return realEditorValues; }
        realEditorValues & getMutableRealEditorValues() { return realEditorValues; }
        void setRealEditorValues(const realEditorValues & value) { this->realEditorValues = value; }

        const defaultOverride & getValue() const { return value; }
        defaultOverride & getMutableValue() { return value; }
        void setValue(const defaultOverride & value) { this->value = value; }
    };

    class fieldInstance {
    public:
        fieldInstance() = default;
        virtual ~fieldInstance() = default;

    private:
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        fieldInstanceProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const fieldInstanceProperties & getProperties() const { return properties; }
        fieldInstanceProperties & getMutableProperties() { return properties; }
        void setProperties(const fieldInstanceProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class gridPointProperties {
    public:
        gridPointProperties() = default;
        virtual ~gridPointProperties() = default;

    private:
        appBuildId cy;
        appBuildId cx;

    public:
        const appBuildId & getCy() const { return cy; }
        appBuildId & getMutableCy() { return cy; }
        void setCy(const appBuildId & value) { this->cy = value; }

        const appBuildId & getCx() const { return cx; }
        appBuildId & getMutableCx() { return cx; }
        void setCx(const appBuildId & value) { this->cx = value; }
    };

    class gridPoint {
    public:
        gridPoint() = default;
        virtual ~gridPoint() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        gridPointProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const gridPointProperties & getProperties() const { return properties; }
        gridPointProperties & getMutableProperties() { return properties; }
        void setProperties(const gridPointProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class intGridValueDefProperties {
    public:
        intGridValueDefProperties() = default;
        virtual ~intGridValueDefProperties() = default;

    private:
        icon tile;
        appBuildId color;
        appBuildId identifier;
        appBuildId value;
        appBuildId groupUid;

    public:
        const icon & getTile() const { return tile; }
        icon & getMutableTile() { return tile; }
        void setTile(const icon & value) { this->tile = value; }

        const appBuildId & getColor() const { return color; }
        appBuildId & getMutableColor() { return color; }
        void setColor(const appBuildId & value) { this->color = value; }

        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }

        const appBuildId & getValue() const { return value; }
        appBuildId & getMutableValue() { return value; }
        void setValue(const appBuildId & value) { this->value = value; }

        const appBuildId & getGroupUid() const { return groupUid; }
        appBuildId & getMutableGroupUid() { return groupUid; }
        void setGroupUid(const appBuildId & value) { this->groupUid = value; }
    };

    class intGridValueDef {
    public:
        intGridValueDef() = default;
        virtual ~intGridValueDef() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        intGridValueDefProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const intGridValueDefProperties & getProperties() const { return properties; }
        intGridValueDefProperties & getMutableProperties() { return properties; }
        void setProperties(const intGridValueDefProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class intGridValueGroupDefProperties {
    public:
        intGridValueGroupDefProperties() = default;
        virtual ~intGridValueGroupDefProperties() = default;

    private:
        appBuildId color;
        appBuildId uid;
        appBuildId identifier;

    public:
        const appBuildId & getColor() const { return color; }
        appBuildId & getMutableColor() { return color; }
        void setColor(const appBuildId & value) { this->color = value; }

        const appBuildId & getUid() const { return uid; }
        appBuildId & getMutableUid() { return uid; }
        void setUid(const appBuildId & value) { this->uid = value; }

        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }
    };

    class intGridValueGroupDef {
    public:
        intGridValueGroupDef() = default;
        virtual ~intGridValueGroupDef() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        intGridValueGroupDefProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const intGridValueGroupDefProperties & getProperties() const { return properties; }
        intGridValueGroupDefProperties & getMutableProperties() { return properties; }
        void setProperties(const intGridValueGroupDefProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class intGridValueInstanceProperties {
    public:
        intGridValueInstanceProperties() = default;
        virtual ~intGridValueInstanceProperties() = default;

    private:
        appBuildId v;
        appBuildId coordId;

    public:
        const appBuildId & getV() const { return v; }
        appBuildId & getMutableV() { return v; }
        void setV(const appBuildId & value) { this->v = value; }

        const appBuildId & getCoordId() const { return coordId; }
        appBuildId & getMutableCoordId() { return coordId; }
        void setCoordId(const appBuildId & value) { this->coordId = value; }
    };

    class intGridValueInstance {
    public:
        intGridValueInstance() = default;
        virtual ~intGridValueInstance() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        intGridValueInstanceProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const intGridValueInstanceProperties & getProperties() const { return properties; }
        intGridValueInstanceProperties & getMutableProperties() { return properties; }
        void setProperties(const intGridValueInstanceProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class layerDefProperties {
    public:
        layerDefProperties() = default;
        virtual ~layerDefProperties() = default;

    private:
        appBuildId pxOffsetX;
        appBuildId tilePivotX;
        requiredBiomeValues uiFilterTags;
        appBuildId displayOpacity;
        appBuildId parallaxFactorY;
        appBuildId hideInList;
        appBuildId type;
        appBuildId guideGridHei;
        appBuildId uiColor;
        appBuildId doc;
        appBuildId tilesetDefUid;
        appBuildId canSelectWhenInactive;
        appBuildId useAsyncRender;
        appBuildId autoSourceLayerDefUid;
        appBuildId autoTilesetDefUid;
        appBuildId parallaxScaling;
        appBuildId renderInWorldView;
        customCommands intGridValuesGroups;
        appBuildId inactiveOpacity;
        appBuildId uid;
        requiredBiomeValues excludedTags;
        appBuildId hideFieldsWhenInactive;
        customCommands intGridValues;
        customCommands autoRuleGroups;
        identifierStyle propertiesType;
        appBuildId identifier;
        appBuildId guideGridWid;
        requiredBiomeValues requiredTags;
        appBuildId pxOffsetY;
        appBuildId tilePivotY;
        appBuildId biomeFieldUid;
        appBuildId gridSize;
        appBuildId parallaxFactorX;
        appBuildId autoTilesKilledByOtherLayerUid;

    public:
        const appBuildId & getPxOffsetX() const { return pxOffsetX; }
        appBuildId & getMutablePxOffsetX() { return pxOffsetX; }
        void setPxOffsetX(const appBuildId & value) { this->pxOffsetX = value; }

        const appBuildId & getTilePivotX() const { return tilePivotX; }
        appBuildId & getMutableTilePivotX() { return tilePivotX; }
        void setTilePivotX(const appBuildId & value) { this->tilePivotX = value; }

        const requiredBiomeValues & getUiFilterTags() const { return uiFilterTags; }
        requiredBiomeValues & getMutableUiFilterTags() { return uiFilterTags; }
        void setUiFilterTags(const requiredBiomeValues & value) { this->uiFilterTags = value; }

        const appBuildId & getDisplayOpacity() const { return displayOpacity; }
        appBuildId & getMutableDisplayOpacity() { return displayOpacity; }
        void setDisplayOpacity(const appBuildId & value) { this->displayOpacity = value; }

        const appBuildId & getParallaxFactorY() const { return parallaxFactorY; }
        appBuildId & getMutableParallaxFactorY() { return parallaxFactorY; }
        void setParallaxFactorY(const appBuildId & value) { this->parallaxFactorY = value; }

        const appBuildId & getHideInList() const { return hideInList; }
        appBuildId & getMutableHideInList() { return hideInList; }
        void setHideInList(const appBuildId & value) { this->hideInList = value; }

        const appBuildId & getType() const { return type; }
        appBuildId & getMutableType() { return type; }
        void setType(const appBuildId & value) { this->type = value; }

        const appBuildId & getGuideGridHei() const { return guideGridHei; }
        appBuildId & getMutableGuideGridHei() { return guideGridHei; }
        void setGuideGridHei(const appBuildId & value) { this->guideGridHei = value; }

        const appBuildId & getUiColor() const { return uiColor; }
        appBuildId & getMutableUiColor() { return uiColor; }
        void setUiColor(const appBuildId & value) { this->uiColor = value; }

        const appBuildId & getDoc() const { return doc; }
        appBuildId & getMutableDoc() { return doc; }
        void setDoc(const appBuildId & value) { this->doc = value; }

        const appBuildId & getTilesetDefUid() const { return tilesetDefUid; }
        appBuildId & getMutableTilesetDefUid() { return tilesetDefUid; }
        void setTilesetDefUid(const appBuildId & value) { this->tilesetDefUid = value; }

        const appBuildId & getCanSelectWhenInactive() const { return canSelectWhenInactive; }
        appBuildId & getMutableCanSelectWhenInactive() { return canSelectWhenInactive; }
        void setCanSelectWhenInactive(const appBuildId & value) { this->canSelectWhenInactive = value; }

        const appBuildId & getUseAsyncRender() const { return useAsyncRender; }
        appBuildId & getMutableUseAsyncRender() { return useAsyncRender; }
        void setUseAsyncRender(const appBuildId & value) { this->useAsyncRender = value; }

        const appBuildId & getAutoSourceLayerDefUid() const { return autoSourceLayerDefUid; }
        appBuildId & getMutableAutoSourceLayerDefUid() { return autoSourceLayerDefUid; }
        void setAutoSourceLayerDefUid(const appBuildId & value) { this->autoSourceLayerDefUid = value; }

        const appBuildId & getAutoTilesetDefUid() const { return autoTilesetDefUid; }
        appBuildId & getMutableAutoTilesetDefUid() { return autoTilesetDefUid; }
        void setAutoTilesetDefUid(const appBuildId & value) { this->autoTilesetDefUid = value; }

        const appBuildId & getParallaxScaling() const { return parallaxScaling; }
        appBuildId & getMutableParallaxScaling() { return parallaxScaling; }
        void setParallaxScaling(const appBuildId & value) { this->parallaxScaling = value; }

        const appBuildId & getRenderInWorldView() const { return renderInWorldView; }
        appBuildId & getMutableRenderInWorldView() { return renderInWorldView; }
        void setRenderInWorldView(const appBuildId & value) { this->renderInWorldView = value; }

        const customCommands & getIntGridValuesGroups() const { return intGridValuesGroups; }
        customCommands & getMutableIntGridValuesGroups() { return intGridValuesGroups; }
        void setIntGridValuesGroups(const customCommands & value) { this->intGridValuesGroups = value; }

        const appBuildId & getInactiveOpacity() const { return inactiveOpacity; }
        appBuildId & getMutableInactiveOpacity() { return inactiveOpacity; }
        void setInactiveOpacity(const appBuildId & value) { this->inactiveOpacity = value; }

        const appBuildId & getUid() const { return uid; }
        appBuildId & getMutableUid() { return uid; }
        void setUid(const appBuildId & value) { this->uid = value; }

        const requiredBiomeValues & getExcludedTags() const { return excludedTags; }
        requiredBiomeValues & getMutableExcludedTags() { return excludedTags; }
        void setExcludedTags(const requiredBiomeValues & value) { this->excludedTags = value; }

        const appBuildId & getHideFieldsWhenInactive() const { return hideFieldsWhenInactive; }
        appBuildId & getMutableHideFieldsWhenInactive() { return hideFieldsWhenInactive; }
        void setHideFieldsWhenInactive(const appBuildId & value) { this->hideFieldsWhenInactive = value; }

        const customCommands & getIntGridValues() const { return intGridValues; }
        customCommands & getMutableIntGridValues() { return intGridValues; }
        void setIntGridValues(const customCommands & value) { this->intGridValues = value; }

        const customCommands & getAutoRuleGroups() const { return autoRuleGroups; }
        customCommands & getMutableAutoRuleGroups() { return autoRuleGroups; }
        void setAutoRuleGroups(const customCommands & value) { this->autoRuleGroups = value; }

        const identifierStyle & getPropertiesType() const { return propertiesType; }
        identifierStyle & getMutablePropertiesType() { return propertiesType; }
        void setPropertiesType(const identifierStyle & value) { this->propertiesType = value; }

        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }

        const appBuildId & getGuideGridWid() const { return guideGridWid; }
        appBuildId & getMutableGuideGridWid() { return guideGridWid; }
        void setGuideGridWid(const appBuildId & value) { this->guideGridWid = value; }

        const requiredBiomeValues & getRequiredTags() const { return requiredTags; }
        requiredBiomeValues & getMutableRequiredTags() { return requiredTags; }
        void setRequiredTags(const requiredBiomeValues & value) { this->requiredTags = value; }

        const appBuildId & getPxOffsetY() const { return pxOffsetY; }
        appBuildId & getMutablePxOffsetY() { return pxOffsetY; }
        void setPxOffsetY(const appBuildId & value) { this->pxOffsetY = value; }

        const appBuildId & getTilePivotY() const { return tilePivotY; }
        appBuildId & getMutableTilePivotY() { return tilePivotY; }
        void setTilePivotY(const appBuildId & value) { this->tilePivotY = value; }

        const appBuildId & getBiomeFieldUid() const { return biomeFieldUid; }
        appBuildId & getMutableBiomeFieldUid() { return biomeFieldUid; }
        void setBiomeFieldUid(const appBuildId & value) { this->biomeFieldUid = value; }

        const appBuildId & getGridSize() const { return gridSize; }
        appBuildId & getMutableGridSize() { return gridSize; }
        void setGridSize(const appBuildId & value) { this->gridSize = value; }

        const appBuildId & getParallaxFactorX() const { return parallaxFactorX; }
        appBuildId & getMutableParallaxFactorX() { return parallaxFactorX; }
        void setParallaxFactorX(const appBuildId & value) { this->parallaxFactorX = value; }

        const appBuildId & getAutoTilesKilledByOtherLayerUid() const { return autoTilesKilledByOtherLayerUid; }
        appBuildId & getMutableAutoTilesKilledByOtherLayerUid() { return autoTilesKilledByOtherLayerUid; }
        void setAutoTilesKilledByOtherLayerUid(const appBuildId & value) { this->autoTilesKilledByOtherLayerUid = value; }
    };

    class layerDef {
    public:
        layerDef() = default;
        virtual ~layerDef() = default;

    private:
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        layerDefProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const layerDefProperties & getProperties() const { return properties; }
        layerDefProperties & getMutableProperties() { return properties; }
        void setProperties(const layerDefProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class layerInstanceProperties {
    public:
        layerInstanceProperties() = default;
        virtual ~layerInstanceProperties() = default;

    private:
        appBuildId cHei;
        appBuildId pxOffsetX;
        appBuildId tilesetRelPath;
        appBuildId iid;
        appBuildId levelId;
        appBuildId type;
        customCommands autoLayerTiles;
        requiredBiomeValues optionalRules;
        appBuildId identifier;
        appBuildId gridSize;
        appBuildId pxTotalOffsetY;
        requiredBiomeValues intGridCsv;
        appBuildId overrideTilesetUid;
        appBuildId visible;
        customCommands entityInstances;
        appBuildId opacity;
        appBuildId seed;
        appBuildId layerDefUid;
        appBuildId pxTotalOffsetX;
        appBuildId cWid;
        appBuildId pxOffsetY;
        appBuildId tilesetDefUid;
        customCommands gridTiles;
        customCommands intGrid;

    public:
        const appBuildId & getCHei() const { return cHei; }
        appBuildId & getMutableCHei() { return cHei; }
        void setCHei(const appBuildId & value) { this->cHei = value; }

        const appBuildId & getPxOffsetX() const { return pxOffsetX; }
        appBuildId & getMutablePxOffsetX() { return pxOffsetX; }
        void setPxOffsetX(const appBuildId & value) { this->pxOffsetX = value; }

        const appBuildId & getTilesetRelPath() const { return tilesetRelPath; }
        appBuildId & getMutableTilesetRelPath() { return tilesetRelPath; }
        void setTilesetRelPath(const appBuildId & value) { this->tilesetRelPath = value; }

        const appBuildId & getIid() const { return iid; }
        appBuildId & getMutableIid() { return iid; }
        void setIid(const appBuildId & value) { this->iid = value; }

        const appBuildId & getLevelId() const { return levelId; }
        appBuildId & getMutableLevelId() { return levelId; }
        void setLevelId(const appBuildId & value) { this->levelId = value; }

        const appBuildId & getType() const { return type; }
        appBuildId & getMutableType() { return type; }
        void setType(const appBuildId & value) { this->type = value; }

        const customCommands & getAutoLayerTiles() const { return autoLayerTiles; }
        customCommands & getMutableAutoLayerTiles() { return autoLayerTiles; }
        void setAutoLayerTiles(const customCommands & value) { this->autoLayerTiles = value; }

        const requiredBiomeValues & getOptionalRules() const { return optionalRules; }
        requiredBiomeValues & getMutableOptionalRules() { return optionalRules; }
        void setOptionalRules(const requiredBiomeValues & value) { this->optionalRules = value; }

        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }

        const appBuildId & getGridSize() const { return gridSize; }
        appBuildId & getMutableGridSize() { return gridSize; }
        void setGridSize(const appBuildId & value) { this->gridSize = value; }

        const appBuildId & getPxTotalOffsetY() const { return pxTotalOffsetY; }
        appBuildId & getMutablePxTotalOffsetY() { return pxTotalOffsetY; }
        void setPxTotalOffsetY(const appBuildId & value) { this->pxTotalOffsetY = value; }

        const requiredBiomeValues & getIntGridCsv() const { return intGridCsv; }
        requiredBiomeValues & getMutableIntGridCsv() { return intGridCsv; }
        void setIntGridCsv(const requiredBiomeValues & value) { this->intGridCsv = value; }

        const appBuildId & getOverrideTilesetUid() const { return overrideTilesetUid; }
        appBuildId & getMutableOverrideTilesetUid() { return overrideTilesetUid; }
        void setOverrideTilesetUid(const appBuildId & value) { this->overrideTilesetUid = value; }

        const appBuildId & getVisible() const { return visible; }
        appBuildId & getMutableVisible() { return visible; }
        void setVisible(const appBuildId & value) { this->visible = value; }

        const customCommands & getEntityInstances() const { return entityInstances; }
        customCommands & getMutableEntityInstances() { return entityInstances; }
        void setEntityInstances(const customCommands & value) { this->entityInstances = value; }

        const appBuildId & getOpacity() const { return opacity; }
        appBuildId & getMutableOpacity() { return opacity; }
        void setOpacity(const appBuildId & value) { this->opacity = value; }

        const appBuildId & getSeed() const { return seed; }
        appBuildId & getMutableSeed() { return seed; }
        void setSeed(const appBuildId & value) { this->seed = value; }

        const appBuildId & getLayerDefUid() const { return layerDefUid; }
        appBuildId & getMutableLayerDefUid() { return layerDefUid; }
        void setLayerDefUid(const appBuildId & value) { this->layerDefUid = value; }

        const appBuildId & getPxTotalOffsetX() const { return pxTotalOffsetX; }
        appBuildId & getMutablePxTotalOffsetX() { return pxTotalOffsetX; }
        void setPxTotalOffsetX(const appBuildId & value) { this->pxTotalOffsetX = value; }

        const appBuildId & getCWid() const { return cWid; }
        appBuildId & getMutableCWid() { return cWid; }
        void setCWid(const appBuildId & value) { this->cWid = value; }

        const appBuildId & getPxOffsetY() const { return pxOffsetY; }
        appBuildId & getMutablePxOffsetY() { return pxOffsetY; }
        void setPxOffsetY(const appBuildId & value) { this->pxOffsetY = value; }

        const appBuildId & getTilesetDefUid() const { return tilesetDefUid; }
        appBuildId & getMutableTilesetDefUid() { return tilesetDefUid; }
        void setTilesetDefUid(const appBuildId & value) { this->tilesetDefUid = value; }

        const customCommands & getGridTiles() const { return gridTiles; }
        customCommands & getMutableGridTiles() { return gridTiles; }
        void setGridTiles(const customCommands & value) { this->gridTiles = value; }

        const customCommands & getIntGrid() const { return intGrid; }
        customCommands & getMutableIntGrid() { return intGrid; }
        void setIntGrid(const customCommands & value) { this->intGrid = value; }
    };

    class layerInstance {
    public:
        layerInstance() = default;
        virtual ~layerInstance() = default;

    private:
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        layerInstanceProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const layerInstanceProperties & getProperties() const { return properties; }
        layerInstanceProperties & getMutableProperties() { return properties; }
        void setProperties(const layerInstanceProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class levelProperties {
    public:
        levelProperties() = default;
        virtual ~levelProperties() = default;

    private:
        customCommands neighbours;
        appBuildId bgColor;
        appBuildId worldX;
        appBuildId externalRelPath;
        appBuildId useAutoIdentifier;
        appBuildId iid;
        appBuildId propertiesBgColor;
        worldLayout propertiesBgPos;
        appBuildId pxHei;
        appBuildId worldY;
        icon bgPos;
        appBuildId uid;
        appBuildId smartColor;
        customCommands fieldInstances;
        appBuildId pxWid;
        appBuildId identifier;
        appBuildId bgPivotY;
        appBuildId bgPivotX;
        customCommands layerInstances;
        appBuildId bgRelPath;
        appBuildId worldDepth;

    public:
        const customCommands & getNeighbours() const { return neighbours; }
        customCommands & getMutableNeighbours() { return neighbours; }
        void setNeighbours(const customCommands & value) { this->neighbours = value; }

        const appBuildId & getBgColor() const { return bgColor; }
        appBuildId & getMutableBgColor() { return bgColor; }
        void setBgColor(const appBuildId & value) { this->bgColor = value; }

        const appBuildId & getWorldX() const { return worldX; }
        appBuildId & getMutableWorldX() { return worldX; }
        void setWorldX(const appBuildId & value) { this->worldX = value; }

        const appBuildId & getExternalRelPath() const { return externalRelPath; }
        appBuildId & getMutableExternalRelPath() { return externalRelPath; }
        void setExternalRelPath(const appBuildId & value) { this->externalRelPath = value; }

        const appBuildId & getUseAutoIdentifier() const { return useAutoIdentifier; }
        appBuildId & getMutableUseAutoIdentifier() { return useAutoIdentifier; }
        void setUseAutoIdentifier(const appBuildId & value) { this->useAutoIdentifier = value; }

        const appBuildId & getIid() const { return iid; }
        appBuildId & getMutableIid() { return iid; }
        void setIid(const appBuildId & value) { this->iid = value; }

        const appBuildId & getPropertiesBgColor() const { return propertiesBgColor; }
        appBuildId & getMutablePropertiesBgColor() { return propertiesBgColor; }
        void setPropertiesBgColor(const appBuildId & value) { this->propertiesBgColor = value; }

        const worldLayout & getPropertiesBgPos() const { return propertiesBgPos; }
        worldLayout & getMutablePropertiesBgPos() { return propertiesBgPos; }
        void setPropertiesBgPos(const worldLayout & value) { this->propertiesBgPos = value; }

        const appBuildId & getPxHei() const { return pxHei; }
        appBuildId & getMutablePxHei() { return pxHei; }
        void setPxHei(const appBuildId & value) { this->pxHei = value; }

        const appBuildId & getWorldY() const { return worldY; }
        appBuildId & getMutableWorldY() { return worldY; }
        void setWorldY(const appBuildId & value) { this->worldY = value; }

        const icon & getBgPos() const { return bgPos; }
        icon & getMutableBgPos() { return bgPos; }
        void setBgPos(const icon & value) { this->bgPos = value; }

        const appBuildId & getUid() const { return uid; }
        appBuildId & getMutableUid() { return uid; }
        void setUid(const appBuildId & value) { this->uid = value; }

        const appBuildId & getSmartColor() const { return smartColor; }
        appBuildId & getMutableSmartColor() { return smartColor; }
        void setSmartColor(const appBuildId & value) { this->smartColor = value; }

        const customCommands & getFieldInstances() const { return fieldInstances; }
        customCommands & getMutableFieldInstances() { return fieldInstances; }
        void setFieldInstances(const customCommands & value) { this->fieldInstances = value; }

        const appBuildId & getPxWid() const { return pxWid; }
        appBuildId & getMutablePxWid() { return pxWid; }
        void setPxWid(const appBuildId & value) { this->pxWid = value; }

        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }

        const appBuildId & getBgPivotY() const { return bgPivotY; }
        appBuildId & getMutableBgPivotY() { return bgPivotY; }
        void setBgPivotY(const appBuildId & value) { this->bgPivotY = value; }

        const appBuildId & getBgPivotX() const { return bgPivotX; }
        appBuildId & getMutableBgPivotX() { return bgPivotX; }
        void setBgPivotX(const appBuildId & value) { this->bgPivotX = value; }

        const customCommands & getLayerInstances() const { return layerInstances; }
        customCommands & getMutableLayerInstances() { return layerInstances; }
        void setLayerInstances(const customCommands & value) { this->layerInstances = value; }

        const appBuildId & getBgRelPath() const { return bgRelPath; }
        appBuildId & getMutableBgRelPath() { return bgRelPath; }
        void setBgRelPath(const appBuildId & value) { this->bgRelPath = value; }

        const appBuildId & getWorldDepth() const { return worldDepth; }
        appBuildId & getMutableWorldDepth() { return worldDepth; }
        void setWorldDepth(const appBuildId & value) { this->worldDepth = value; }
    };

    class level {
    public:
        level() = default;
        virtual ~level() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        levelProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const levelProperties & getProperties() const { return properties; }
        levelProperties & getMutableProperties() { return properties; }
        void setProperties(const levelProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class levelBgPosInfosProperties {
    public:
        levelBgPosInfosProperties() = default;
        virtual ~levelBgPosInfosProperties() = default;

    private:
        requiredBiomeValues cropRect;
        requiredBiomeValues scale;
        requiredBiomeValues topLeftPx;

    public:
        const requiredBiomeValues & getCropRect() const { return cropRect; }
        requiredBiomeValues & getMutableCropRect() { return cropRect; }
        void setCropRect(const requiredBiomeValues & value) { this->cropRect = value; }

        const requiredBiomeValues & getScale() const { return scale; }
        requiredBiomeValues & getMutableScale() { return scale; }
        void setScale(const requiredBiomeValues & value) { this->scale = value; }

        const requiredBiomeValues & getTopLeftPx() const { return topLeftPx; }
        requiredBiomeValues & getMutableTopLeftPx() { return topLeftPx; }
        void setTopLeftPx(const requiredBiomeValues & value) { this->topLeftPx = value; }
    };

    class levelBgPosInfos {
    public:
        levelBgPosInfos() = default;
        virtual ~levelBgPosInfos() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        levelBgPosInfosProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const levelBgPosInfosProperties & getProperties() const { return properties; }
        levelBgPosInfosProperties & getMutableProperties() { return properties; }
        void setProperties(const levelBgPosInfosProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class neighbourLevelProperties {
    public:
        neighbourLevelProperties() = default;
        virtual ~neighbourLevelProperties() = default;

    private:
        appBuildId levelIid;
        appBuildId levelUid;
        appBuildId dir;

    public:
        const appBuildId & getLevelIid() const { return levelIid; }
        appBuildId & getMutableLevelIid() { return levelIid; }
        void setLevelIid(const appBuildId & value) { this->levelIid = value; }

        const appBuildId & getLevelUid() const { return levelUid; }
        appBuildId & getMutableLevelUid() { return levelUid; }
        void setLevelUid(const appBuildId & value) { this->levelUid = value; }

        const appBuildId & getDir() const { return dir; }
        appBuildId & getMutableDir() { return dir; }
        void setDir(const appBuildId & value) { this->dir = value; }
    };

    class neighbourLevel {
    public:
        neighbourLevel() = default;
        virtual ~neighbourLevel() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        neighbourLevelProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const neighbourLevelProperties & getProperties() const { return properties; }
        neighbourLevelProperties & getMutableProperties() { return properties; }
        void setProperties(const neighbourLevelProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class tableOfContentEntryProperties {
    public:
        tableOfContentEntryProperties() = default;
        virtual ~tableOfContentEntryProperties() = default;

    private:
        appBuildId identifier;
        customCommands instancesData;
        customCommands instances;

    public:
        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }

        const customCommands & getInstancesData() const { return instancesData; }
        customCommands & getMutableInstancesData() { return instancesData; }
        void setInstancesData(const customCommands & value) { this->instancesData = value; }

        const customCommands & getInstances() const { return instances; }
        customCommands & getMutableInstances() { return instances; }
        void setInstances(const customCommands & value) { this->instances = value; }
    };

    class tableOfContentEntry {
    public:
        tableOfContentEntry() = default;
        virtual ~tableOfContentEntry() = default;

    private:
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        tableOfContentEntryProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const tableOfContentEntryProperties & getProperties() const { return properties; }
        tableOfContentEntryProperties & getMutableProperties() { return properties; }
        void setProperties(const tableOfContentEntryProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class tileProperties {
    public:
        tileProperties() = default;
        virtual ~tileProperties() = default;

    private:
        appBuildId t;
        requiredBiomeValues d;
        requiredBiomeValues px;
        appBuildId a;
        appBuildId f;
        requiredBiomeValues src;

    public:
        const appBuildId & getT() const { return t; }
        appBuildId & getMutableT() { return t; }
        void setT(const appBuildId & value) { this->t = value; }

        const requiredBiomeValues & getD() const { return d; }
        requiredBiomeValues & getMutableD() { return d; }
        void setD(const requiredBiomeValues & value) { this->d = value; }

        const requiredBiomeValues & getPx() const { return px; }
        requiredBiomeValues & getMutablePx() { return px; }
        void setPx(const requiredBiomeValues & value) { this->px = value; }

        const appBuildId & getA() const { return a; }
        appBuildId & getMutableA() { return a; }
        void setA(const appBuildId & value) { this->a = value; }

        const appBuildId & getF() const { return f; }
        appBuildId & getMutableF() { return f; }
        void setF(const appBuildId & value) { this->f = value; }

        const requiredBiomeValues & getSrc() const { return src; }
        requiredBiomeValues & getMutableSrc() { return src; }
        void setSrc(const requiredBiomeValues & value) { this->src = value; }
    };

    class tile {
    public:
        tile() = default;
        virtual ~tile() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        tileProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const tileProperties & getProperties() const { return properties; }
        tileProperties & getMutableProperties() { return properties; }
        void setProperties(const tileProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class tileCustomMetadataProperties {
    public:
        tileCustomMetadataProperties() = default;
        virtual ~tileCustomMetadataProperties() = default;

    private:
        appBuildId tileId;
        appBuildId data;

    public:
        const appBuildId & getTileId() const { return tileId; }
        appBuildId & getMutableTileId() { return tileId; }
        void setTileId(const appBuildId & value) { this->tileId = value; }

        const appBuildId & getData() const { return data; }
        appBuildId & getMutableData() { return data; }
        void setData(const appBuildId & value) { this->data = value; }
    };

    class tileCustomMetadata {
    public:
        tileCustomMetadata() = default;
        virtual ~tileCustomMetadata() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        tileCustomMetadataProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const tileCustomMetadataProperties & getProperties() const { return properties; }
        tileCustomMetadataProperties & getMutableProperties() { return properties; }
        void setProperties(const tileCustomMetadataProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class tilesetDefProperties {
    public:
        tilesetDefProperties() = default;
        virtual ~tilesetDefProperties() = default;

    private:
        appBuildId cachedPixelData;
        appBuildId cHei;
        appBuildId pxHei;
        customCommands customData;
        appBuildId tagsSourceEnumUid;
        appBuildId uid;
        appBuildId padding;
        customCommands enumTags;
        appBuildId pxWid;
        appBuildId cWid;
        appBuildId spacing;
        appBuildId identifier;
        requiredBiomeValues savedSelections;
        requiredBiomeValues tags;
        worldLayout embedAtlas;
        appBuildId relPath;
        appBuildId tileGridSize;

    public:
        const appBuildId & getCachedPixelData() const { return cachedPixelData; }
        appBuildId & getMutableCachedPixelData() { return cachedPixelData; }
        void setCachedPixelData(const appBuildId & value) { this->cachedPixelData = value; }

        const appBuildId & getCHei() const { return cHei; }
        appBuildId & getMutableCHei() { return cHei; }
        void setCHei(const appBuildId & value) { this->cHei = value; }

        const appBuildId & getPxHei() const { return pxHei; }
        appBuildId & getMutablePxHei() { return pxHei; }
        void setPxHei(const appBuildId & value) { this->pxHei = value; }

        const customCommands & getCustomData() const { return customData; }
        customCommands & getMutableCustomData() { return customData; }
        void setCustomData(const customCommands & value) { this->customData = value; }

        const appBuildId & getTagsSourceEnumUid() const { return tagsSourceEnumUid; }
        appBuildId & getMutableTagsSourceEnumUid() { return tagsSourceEnumUid; }
        void setTagsSourceEnumUid(const appBuildId & value) { this->tagsSourceEnumUid = value; }

        const appBuildId & getUid() const { return uid; }
        appBuildId & getMutableUid() { return uid; }
        void setUid(const appBuildId & value) { this->uid = value; }

        const appBuildId & getPadding() const { return padding; }
        appBuildId & getMutablePadding() { return padding; }
        void setPadding(const appBuildId & value) { this->padding = value; }

        const customCommands & getEnumTags() const { return enumTags; }
        customCommands & getMutableEnumTags() { return enumTags; }
        void setEnumTags(const customCommands & value) { this->enumTags = value; }

        const appBuildId & getPxWid() const { return pxWid; }
        appBuildId & getMutablePxWid() { return pxWid; }
        void setPxWid(const appBuildId & value) { this->pxWid = value; }

        const appBuildId & getCWid() const { return cWid; }
        appBuildId & getMutableCWid() { return cWid; }
        void setCWid(const appBuildId & value) { this->cWid = value; }

        const appBuildId & getSpacing() const { return spacing; }
        appBuildId & getMutableSpacing() { return spacing; }
        void setSpacing(const appBuildId & value) { this->spacing = value; }

        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }

        const requiredBiomeValues & getSavedSelections() const { return savedSelections; }
        requiredBiomeValues & getMutableSavedSelections() { return savedSelections; }
        void setSavedSelections(const requiredBiomeValues & value) { this->savedSelections = value; }

        const requiredBiomeValues & getTags() const { return tags; }
        requiredBiomeValues & getMutableTags() { return tags; }
        void setTags(const requiredBiomeValues & value) { this->tags = value; }

        const worldLayout & getEmbedAtlas() const { return embedAtlas; }
        worldLayout & getMutableEmbedAtlas() { return embedAtlas; }
        void setEmbedAtlas(const worldLayout & value) { this->embedAtlas = value; }

        const appBuildId & getRelPath() const { return relPath; }
        appBuildId & getMutableRelPath() { return relPath; }
        void setRelPath(const appBuildId & value) { this->relPath = value; }

        const appBuildId & getTileGridSize() const { return tileGridSize; }
        appBuildId & getMutableTileGridSize() { return tileGridSize; }
        void setTileGridSize(const appBuildId & value) { this->tileGridSize = value; }
    };

    class tilesetDef {
    public:
        tilesetDef() = default;
        virtual ~tilesetDef() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        tilesetDefProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const tilesetDefProperties & getProperties() const { return properties; }
        tilesetDefProperties & getMutableProperties() { return properties; }
        void setProperties(const tilesetDefProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class tilesetRectProperties {
    public:
        tilesetRectProperties() = default;
        virtual ~tilesetRectProperties() = default;

    private:
        appBuildId tilesetUid;
        appBuildId h;
        appBuildId x;
        appBuildId y;
        appBuildId w;

    public:
        const appBuildId & getTilesetUid() const { return tilesetUid; }
        appBuildId & getMutableTilesetUid() { return tilesetUid; }
        void setTilesetUid(const appBuildId & value) { this->tilesetUid = value; }

        const appBuildId & getH() const { return h; }
        appBuildId & getMutableH() { return h; }
        void setH(const appBuildId & value) { this->h = value; }

        const appBuildId & getX() const { return x; }
        appBuildId & getMutableX() { return x; }
        void setX(const appBuildId & value) { this->x = value; }

        const appBuildId & getY() const { return y; }
        appBuildId & getMutableY() { return y; }
        void setY(const appBuildId & value) { this->y = value; }

        const appBuildId & getW() const { return w; }
        appBuildId & getMutableW() { return w; }
        void setW(const appBuildId & value) { this->w = value; }
    };

    class tilesetRect {
    public:
        tilesetRect() = default;
        virtual ~tilesetRect() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        tilesetRectProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const tilesetRectProperties & getProperties() const { return properties; }
        tilesetRectProperties & getMutableProperties() { return properties; }
        void setProperties(const tilesetRectProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class tocInstanceDataProperties {
    public:
        tocInstanceDataProperties() = default;
        virtual ~tocInstanceDataProperties() = default;

    private:
        appBuildId worldX;
        appBuildId widPx;
        appBuildId worldY;
        appBuildId heiPx;
        defaultOverride fields;
        defs iids;

    public:
        const appBuildId & getWorldX() const { return worldX; }
        appBuildId & getMutableWorldX() { return worldX; }
        void setWorldX(const appBuildId & value) { this->worldX = value; }

        const appBuildId & getWidPx() const { return widPx; }
        appBuildId & getMutableWidPx() { return widPx; }
        void setWidPx(const appBuildId & value) { this->widPx = value; }

        const appBuildId & getWorldY() const { return worldY; }
        appBuildId & getMutableWorldY() { return worldY; }
        void setWorldY(const appBuildId & value) { this->worldY = value; }

        const appBuildId & getHeiPx() const { return heiPx; }
        appBuildId & getMutableHeiPx() { return heiPx; }
        void setHeiPx(const appBuildId & value) { this->heiPx = value; }

        const defaultOverride & getFields() const { return fields; }
        defaultOverride & getMutableFields() { return fields; }
        void setFields(const defaultOverride & value) { this->fields = value; }

        const defs & getIids() const { return iids; }
        defs & getMutableIids() { return iids; }
        void setIids(const defs & value) { this->iids = value; }
    };

    class tocInstanceData {
    public:
        tocInstanceData() = default;
        virtual ~tocInstanceData() = default;

    private:
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        tocInstanceDataProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const tocInstanceDataProperties & getProperties() const { return properties; }
        tocInstanceDataProperties & getMutableProperties() { return properties; }
        void setProperties(const tocInstanceDataProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class worldProperties {
    public:
        worldProperties() = default;
        virtual ~worldProperties() = default;

    private:
        appBuildId worldGridWidth;
        appBuildId iid;
        appBuildId worldGridHeight;
        worldLayout worldLayout;
        appBuildId defaultLevelWidth;
        customCommands levels;
        appBuildId defaultLevelHeight;
        appBuildId identifier;

    public:
        const appBuildId & getWorldGridWidth() const { return worldGridWidth; }
        appBuildId & getMutableWorldGridWidth() { return worldGridWidth; }
        void setWorldGridWidth(const appBuildId & value) { this->worldGridWidth = value; }

        const appBuildId & getIid() const { return iid; }
        appBuildId & getMutableIid() { return iid; }
        void setIid(const appBuildId & value) { this->iid = value; }

        const appBuildId & getWorldGridHeight() const { return worldGridHeight; }
        appBuildId & getMutableWorldGridHeight() { return worldGridHeight; }
        void setWorldGridHeight(const appBuildId & value) { this->worldGridHeight = value; }

        const worldLayout & getWorldLayout() const { return worldLayout; }
        worldLayout & getMutableWorldLayout() { return worldLayout; }
        void setWorldLayout(const worldLayout & value) { this->worldLayout = value; }

        const appBuildId & getDefaultLevelWidth() const { return defaultLevelWidth; }
        appBuildId & getMutableDefaultLevelWidth() { return defaultLevelWidth; }
        void setDefaultLevelWidth(const appBuildId & value) { this->defaultLevelWidth = value; }

        const customCommands & getLevels() const { return levels; }
        customCommands & getMutableLevels() { return levels; }
        void setLevels(const customCommands & value) { this->levels = value; }

        const appBuildId & getDefaultLevelHeight() const { return defaultLevelHeight; }
        appBuildId & getMutableDefaultLevelHeight() { return defaultLevelHeight; }
        void setDefaultLevelHeight(const appBuildId & value) { this->defaultLevelHeight = value; }

        const appBuildId & getIdentifier() const { return identifier; }
        appBuildId & getMutableIdentifier() { return identifier; }
        void setIdentifier(const appBuildId & value) { this->identifier = value; }
    };

    class world {
    public:
        world() = default;
        virtual ~world() = default;

    private:
        std::string description;
        std::string title;
        std::vector<std::string> required;
        bool additionalProperties;
        worldProperties properties;
        std::vector<forcedRefsType> type;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::vector<std::string> & getRequired() const { return required; }
        std::vector<std::string> & getMutableRequired() { return required; }
        void setRequired(const std::vector<std::string> & value) { this->required = value; }

        const bool & getAdditionalProperties() const { return additionalProperties; }
        bool & getMutableAdditionalProperties() { return additionalProperties; }
        void setAdditionalProperties(const bool & value) { this->additionalProperties = value; }

        const worldProperties & getProperties() const { return properties; }
        worldProperties & getMutableProperties() { return properties; }
        void setProperties(const worldProperties & value) { this->properties = value; }

        const std::vector<forcedRefsType> & getType() const { return type; }
        std::vector<forcedRefsType> & getMutableType() { return type; }
        void setType(const std::vector<forcedRefsType> & value) { this->type = value; }
    };

    class otherTypes {
    public:
        otherTypes() = default;
        virtual ~otherTypes() = default;

    private:
        tilesetRect tilesetRect;
        fieldInstance fieldInstance;
        entityInstance entityInstance;
        definitions definitions;
        enumTagValue enumTagValue;
        autoRuleDef autoRuleDef;
        fieldDef fieldDef;
        customCommand customCommand;
        entityDef entityDef;
        autoLayerRuleGroup autoLayerRuleGroup;
        intGridValueGroupDef intGridValueGroupDef;
        intGridValueInstance intGridValueInstance;
        tocInstanceData tocInstanceData;
        neighbourLevel neighbourLevel;
        layerInstance layerInstance;
        world world;
        entityReferenceInfos entityReferenceInfos;
        tileCustomMetadata tileCustomMetadata;
        tilesetDef tilesetDef;
        enumDefValues enumDefValues;
        tile tile;
        layerDef layerDef;
        levelBgPosInfos levelBgPosInfos;
        level level;
        tableOfContentEntry tableOfContentEntry;
        enumDef enumDef;
        gridPoint gridPoint;
        intGridValueDef intGridValueDef;

    public:
        const tilesetRect & getTilesetRect() const { return tilesetRect; }
        tilesetRect & getMutableTilesetRect() { return tilesetRect; }
        void setTilesetRect(const tilesetRect & value) { this->tilesetRect = value; }

        const fieldInstance & getFieldInstance() const { return fieldInstance; }
        fieldInstance & getMutableFieldInstance() { return fieldInstance; }
        void setFieldInstance(const fieldInstance & value) { this->fieldInstance = value; }

        const entityInstance & getEntityInstance() const { return entityInstance; }
        entityInstance & getMutableEntityInstance() { return entityInstance; }
        void setEntityInstance(const entityInstance & value) { this->entityInstance = value; }

        const definitions & getDefinitions() const { return definitions; }
        definitions & getMutableDefinitions() { return definitions; }
        void setDefinitions(const definitions & value) { this->definitions = value; }

        const enumTagValue & getEnumTagValue() const { return enumTagValue; }
        enumTagValue & getMutableEnumTagValue() { return enumTagValue; }
        void setEnumTagValue(const enumTagValue & value) { this->enumTagValue = value; }

        const autoRuleDef & getAutoRuleDef() const { return autoRuleDef; }
        autoRuleDef & getMutableAutoRuleDef() { return autoRuleDef; }
        void setAutoRuleDef(const autoRuleDef & value) { this->autoRuleDef = value; }

        const fieldDef & getFieldDef() const { return fieldDef; }
        fieldDef & getMutableFieldDef() { return fieldDef; }
        void setFieldDef(const fieldDef & value) { this->fieldDef = value; }

        const customCommand & getCustomCommand() const { return customCommand; }
        customCommand & getMutableCustomCommand() { return customCommand; }
        void setCustomCommand(const customCommand & value) { this->customCommand = value; }

        const entityDef & getEntityDef() const { return entityDef; }
        entityDef & getMutableEntityDef() { return entityDef; }
        void setEntityDef(const entityDef & value) { this->entityDef = value; }

        const autoLayerRuleGroup & getAutoLayerRuleGroup() const { return autoLayerRuleGroup; }
        autoLayerRuleGroup & getMutableAutoLayerRuleGroup() { return autoLayerRuleGroup; }
        void setAutoLayerRuleGroup(const autoLayerRuleGroup & value) { this->autoLayerRuleGroup = value; }

        const intGridValueGroupDef & getIntGridValueGroupDef() const { return intGridValueGroupDef; }
        intGridValueGroupDef & getMutableIntGridValueGroupDef() { return intGridValueGroupDef; }
        void setIntGridValueGroupDef(const intGridValueGroupDef & value) { this->intGridValueGroupDef = value; }

        const intGridValueInstance & getIntGridValueInstance() const { return intGridValueInstance; }
        intGridValueInstance & getMutableIntGridValueInstance() { return intGridValueInstance; }
        void setIntGridValueInstance(const intGridValueInstance & value) { this->intGridValueInstance = value; }

        const tocInstanceData & getTocInstanceData() const { return tocInstanceData; }
        tocInstanceData & getMutableTocInstanceData() { return tocInstanceData; }
        void setTocInstanceData(const tocInstanceData & value) { this->tocInstanceData = value; }

        const neighbourLevel & getNeighbourLevel() const { return neighbourLevel; }
        neighbourLevel & getMutableNeighbourLevel() { return neighbourLevel; }
        void setNeighbourLevel(const neighbourLevel & value) { this->neighbourLevel = value; }

        const layerInstance & getLayerInstance() const { return layerInstance; }
        layerInstance & getMutableLayerInstance() { return layerInstance; }
        void setLayerInstance(const layerInstance & value) { this->layerInstance = value; }

        const world & getWorld() const { return world; }
        world & getMutableWorld() { return world; }
        void setWorld(const world & value) { this->world = value; }

        const entityReferenceInfos & getEntityReferenceInfos() const { return entityReferenceInfos; }
        entityReferenceInfos & getMutableEntityReferenceInfos() { return entityReferenceInfos; }
        void setEntityReferenceInfos(const entityReferenceInfos & value) { this->entityReferenceInfos = value; }

        const tileCustomMetadata & getTileCustomMetadata() const { return tileCustomMetadata; }
        tileCustomMetadata & getMutableTileCustomMetadata() { return tileCustomMetadata; }
        void setTileCustomMetadata(const tileCustomMetadata & value) { this->tileCustomMetadata = value; }

        const tilesetDef & getTilesetDef() const { return tilesetDef; }
        tilesetDef & getMutableTilesetDef() { return tilesetDef; }
        void setTilesetDef(const tilesetDef & value) { this->tilesetDef = value; }

        const enumDefValues & getEnumDefValues() const { return enumDefValues; }
        enumDefValues & getMutableEnumDefValues() { return enumDefValues; }
        void setEnumDefValues(const enumDefValues & value) { this->enumDefValues = value; }

        const tile & getTile() const { return tile; }
        tile & getMutableTile() { return tile; }
        void setTile(const tile & value) { this->tile = value; }

        const layerDef & getLayerDef() const { return layerDef; }
        layerDef & getMutableLayerDef() { return layerDef; }
        void setLayerDef(const layerDef & value) { this->layerDef = value; }

        const levelBgPosInfos & getLevelBgPosInfos() const { return levelBgPosInfos; }
        levelBgPosInfos & getMutableLevelBgPosInfos() { return levelBgPosInfos; }
        void setLevelBgPosInfos(const levelBgPosInfos & value) { this->levelBgPosInfos = value; }

        const level & getLevel() const { return level; }
        level & getMutableLevel() { return level; }
        void setLevel(const level & value) { this->level = value; }

        const tableOfContentEntry & getTableOfContentEntry() const { return tableOfContentEntry; }
        tableOfContentEntry & getMutableTableOfContentEntry() { return tableOfContentEntry; }
        void setTableOfContentEntry(const tableOfContentEntry & value) { this->tableOfContentEntry = value; }

        const enumDef & getEnumDef() const { return enumDef; }
        enumDef & getMutableEnumDef() { return enumDef; }
        void setEnumDef(const enumDef & value) { this->enumDef = value; }

        const gridPoint & getGridPoint() const { return gridPoint; }
        gridPoint & getMutableGridPoint() { return gridPoint; }
        void setGridPoint(const gridPoint & value) { this->gridPoint = value; }

        const intGridValueDef & getIntGridValueDef() const { return intGridValueDef; }
        intGridValueDef & getMutableIntGridValueDef() { return intGridValueDef; }
        void setIntGridValueDef(const intGridValueDef & value) { this->intGridValueDef = value; }
    };

    class welcome {
    public:
        welcome() = default;
        virtual ~welcome() = default;

    private:
        std::string description;
        std::string title;
        std::string schema;
        std::string ref;
        std::string version;
        ldtkJsonRoot ldtkJsonRoot;
        otherTypes otherTypes;

    public:
        const std::string & getDescription() const { return description; }
        std::string & getMutableDescription() { return description; }
        void setDescription(const std::string & value) { this->description = value; }

        const std::string & getTitle() const { return title; }
        std::string & getMutableTitle() { return title; }
        void setTitle(const std::string & value) { this->title = value; }

        const std::string & getSchema() const { return schema; }
        std::string & getMutableSchema() { return schema; }
        void setSchema(const std::string & value) { this->schema = value; }

        const std::string & getRef() const { return ref; }
        std::string & getMutableRef() { return ref; }
        void setRef(const std::string & value) { this->ref = value; }

        const std::string & getVersion() const { return version; }
        std::string & getMutableVersion() { return version; }
        void setVersion(const std::string & value) { this->version = value; }

        const ldtkJsonRoot & getLdtkJsonRoot() const { return ldtkJsonRoot; }
        ldtkJsonRoot & getMutableLdtkJsonRoot() { return ldtkJsonRoot; }
        void setLdtkJsonRoot(const ldtkJsonRoot & value) { this->ldtkJsonRoot = value; }

        const otherTypes & getOtherTypes() const { return otherTypes; }
        otherTypes & getMutableOtherTypes() { return otherTypes; }
        void setOtherTypes(const otherTypes & value) { this->otherTypes = value; }
    };
}

namespace quicktype {
    void from_json(const json & j, appBuildId & x);
    void to_json(json & j, const appBuildId & x);

    void from_json(const json & j, itemsValue & x);
    void to_json(json & j, const itemsValue & x);

    void from_json(const json & j, customCommands & x);
    void to_json(json & j, const customCommands & x);

    void from_json(const json & j, defs & x);
    void to_json(json & j, const defs & x);

    void from_json(const json & j, flagsItems & x);
    void to_json(json & j, const flagsItems & x);

    void from_json(const json & j, flags & x);
    void to_json(json & j, const flags & x);

    void from_json(const json & j, forcedRefs & x);
    void to_json(json & j, const forcedRefs & x);

    void from_json(const json & j, identifierStyle & x);
    void to_json(json & j, const identifierStyle & x);

    void from_json(const json & j, worldLayout & x);
    void to_json(json & j, const worldLayout & x);

    void from_json(const json & j, ldtkJsonRootProperties & x);
    void to_json(json & j, const ldtkJsonRootProperties & x);

    void from_json(const json & j, ldtkJsonRoot & x);
    void to_json(json & j, const ldtkJsonRoot & x);

    void from_json(const json & j, oneOf & x);
    void to_json(json & j, const oneOf & x);

    void from_json(const json & j, icon & x);
    void to_json(json & j, const icon & x);

    void from_json(const json & j, requiredBiomeValuesItems & x);
    void to_json(json & j, const requiredBiomeValuesItems & x);

    void from_json(const json & j, requiredBiomeValues & x);
    void to_json(json & j, const requiredBiomeValues & x);

    void from_json(const json & j, autoLayerRuleGroupProperties & x);
    void to_json(json & j, const autoLayerRuleGroupProperties & x);

    void from_json(const json & j, autoLayerRuleGroup & x);
    void to_json(json & j, const autoLayerRuleGroup & x);

    void from_json(const json & j, tileRectsIdsItems & x);
    void to_json(json & j, const tileRectsIdsItems & x);

    void from_json(const json & j, tileRectsIds & x);
    void to_json(json & j, const tileRectsIds & x);

    void from_json(const json & j, autoRuleDefProperties & x);
    void to_json(json & j, const autoRuleDefProperties & x);

    void from_json(const json & j, autoRuleDef & x);
    void to_json(json & j, const autoRuleDef & x);

    void from_json(const json & j, customCommandProperties & x);
    void to_json(json & j, const customCommandProperties & x);

    void from_json(const json & j, customCommand & x);
    void to_json(json & j, const customCommand & x);

    void from_json(const json & j, definitionsProperties & x);
    void to_json(json & j, const definitionsProperties & x);

    void from_json(const json & j, definitions & x);
    void to_json(json & j, const definitions & x);

    void from_json(const json & j, entityDefProperties & x);
    void to_json(json & j, const entityDefProperties & x);

    void from_json(const json & j, entityDef & x);
    void to_json(json & j, const entityDef & x);

    void from_json(const json & j, entityInstanceProperties & x);
    void to_json(json & j, const entityInstanceProperties & x);

    void from_json(const json & j, entityInstance & x);
    void to_json(json & j, const entityInstance & x);

    void from_json(const json & j, entityReferenceInfosProperties & x);
    void to_json(json & j, const entityReferenceInfosProperties & x);

    void from_json(const json & j, entityReferenceInfos & x);
    void to_json(json & j, const entityReferenceInfos & x);

    void from_json(const json & j, enumDefProperties & x);
    void to_json(json & j, const enumDefProperties & x);

    void from_json(const json & j, enumDef & x);
    void to_json(json & j, const enumDef & x);

    void from_json(const json & j, enumDefValuesProperties & x);
    void to_json(json & j, const enumDefValuesProperties & x);

    void from_json(const json & j, enumDefValues & x);
    void to_json(json & j, const enumDefValues & x);

    void from_json(const json & j, enumTagValueProperties & x);
    void to_json(json & j, const enumTagValueProperties & x);

    void from_json(const json & j, enumTagValue & x);
    void to_json(json & j, const enumTagValue & x);

    void from_json(const json & j, defaultOverride & x);
    void to_json(json & j, const defaultOverride & x);

    void from_json(const json & j, fieldDefProperties & x);
    void to_json(json & j, const fieldDefProperties & x);

    void from_json(const json & j, fieldDef & x);
    void to_json(json & j, const fieldDef & x);

    void from_json(const json & j, realEditorValuesItems & x);
    void to_json(json & j, const realEditorValuesItems & x);

    void from_json(const json & j, realEditorValues & x);
    void to_json(json & j, const realEditorValues & x);

    void from_json(const json & j, fieldInstanceProperties & x);
    void to_json(json & j, const fieldInstanceProperties & x);

    void from_json(const json & j, fieldInstance & x);
    void to_json(json & j, const fieldInstance & x);

    void from_json(const json & j, gridPointProperties & x);
    void to_json(json & j, const gridPointProperties & x);

    void from_json(const json & j, gridPoint & x);
    void to_json(json & j, const gridPoint & x);

    void from_json(const json & j, intGridValueDefProperties & x);
    void to_json(json & j, const intGridValueDefProperties & x);

    void from_json(const json & j, intGridValueDef & x);
    void to_json(json & j, const intGridValueDef & x);

    void from_json(const json & j, intGridValueGroupDefProperties & x);
    void to_json(json & j, const intGridValueGroupDefProperties & x);

    void from_json(const json & j, intGridValueGroupDef & x);
    void to_json(json & j, const intGridValueGroupDef & x);

    void from_json(const json & j, intGridValueInstanceProperties & x);
    void to_json(json & j, const intGridValueInstanceProperties & x);

    void from_json(const json & j, intGridValueInstance & x);
    void to_json(json & j, const intGridValueInstance & x);

    void from_json(const json & j, layerDefProperties & x);
    void to_json(json & j, const layerDefProperties & x);

    void from_json(const json & j, layerDef & x);
    void to_json(json & j, const layerDef & x);

    void from_json(const json & j, layerInstanceProperties & x);
    void to_json(json & j, const layerInstanceProperties & x);

    void from_json(const json & j, layerInstance & x);
    void to_json(json & j, const layerInstance & x);

    void from_json(const json & j, levelProperties & x);
    void to_json(json & j, const levelProperties & x);

    void from_json(const json & j, level & x);
    void to_json(json & j, const level & x);

    void from_json(const json & j, levelBgPosInfosProperties & x);
    void to_json(json & j, const levelBgPosInfosProperties & x);

    void from_json(const json & j, levelBgPosInfos & x);
    void to_json(json & j, const levelBgPosInfos & x);

    void from_json(const json & j, neighbourLevelProperties & x);
    void to_json(json & j, const neighbourLevelProperties & x);

    void from_json(const json & j, neighbourLevel & x);
    void to_json(json & j, const neighbourLevel & x);

    void from_json(const json & j, tableOfContentEntryProperties & x);
    void to_json(json & j, const tableOfContentEntryProperties & x);

    void from_json(const json & j, tableOfContentEntry & x);
    void to_json(json & j, const tableOfContentEntry & x);

    void from_json(const json & j, tileProperties & x);
    void to_json(json & j, const tileProperties & x);

    void from_json(const json & j, tile & x);
    void to_json(json & j, const tile & x);

    void from_json(const json & j, tileCustomMetadataProperties & x);
    void to_json(json & j, const tileCustomMetadataProperties & x);

    void from_json(const json & j, tileCustomMetadata & x);
    void to_json(json & j, const tileCustomMetadata & x);

    void from_json(const json & j, tilesetDefProperties & x);
    void to_json(json & j, const tilesetDefProperties & x);

    void from_json(const json & j, tilesetDef & x);
    void to_json(json & j, const tilesetDef & x);

    void from_json(const json & j, tilesetRectProperties & x);
    void to_json(json & j, const tilesetRectProperties & x);

    void from_json(const json & j, tilesetRect & x);
    void to_json(json & j, const tilesetRect & x);

    void from_json(const json & j, tocInstanceDataProperties & x);
    void to_json(json & j, const tocInstanceDataProperties & x);

    void from_json(const json & j, tocInstanceData & x);
    void to_json(json & j, const tocInstanceData & x);

    void from_json(const json & j, worldProperties & x);
    void to_json(json & j, const worldProperties & x);

    void from_json(const json & j, world & x);
    void to_json(json & j, const world & x);

    void from_json(const json & j, otherTypes & x);
    void to_json(json & j, const otherTypes & x);

    void from_json(const json & j, welcome & x);
    void to_json(json & j, const welcome & x);

    void from_json(const json & j, forcedRefsType & x);
    void to_json(json & j, const forcedRefsType & x);

    void from_json(const json & j, customCommandsType & x);
    void to_json(json & j, const customCommandsType & x);

    inline void from_json(const json & j, appBuildId& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const appBuildId & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, itemsValue& x) {
        x.setRef(j.at("$ref").get<std::string>());
    }

    inline void to_json(json & j, const itemsValue & x) {
        j = json::object();
        j["$ref"] = x.getRef();
    }

    inline void from_json(const json & j, customCommands& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setItems(j.at("items").get<itemsValue>());
        x.setType(j.at("type").get<std::vector<customCommandsType>>());
    }

    inline void to_json(json & j, const customCommands & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["items"] = x.getItems();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, defs& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setRef(j.at("$ref").get<std::string>());
    }

    inline void to_json(json & j, const defs & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["$ref"] = x.getRef();
    }

    inline void from_json(const json & j, flagsItems& x) {
        x.setItemsEnum(j.at("enum").get<std::vector<std::string>>());
    }

    inline void to_json(json & j, const flagsItems & x) {
        j = json::object();
        j["enum"] = x.getItemsEnum();
    }

    inline void from_json(const json & j, flags& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setItems(j.at("items").get<flagsItems>());
        x.setType(j.at("type").get<std::vector<customCommandsType>>());
    }

    inline void to_json(json & j, const flags & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["items"] = x.getItems();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, forcedRefs& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setProperties(j.at("properties").get<std::map<std::string, itemsValue>>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const forcedRefs & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, identifierStyle& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setIdentifierStyleEnum(j.at("enum").get<std::vector<std::string>>());
    }

    inline void to_json(json & j, const identifierStyle & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["enum"] = x.getIdentifierStyleEnum();
    }

    inline void from_json(const json & j, worldLayout& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setWorldLayoutEnum(j.at("enum").get<std::vector<std::optional<std::string>>>());
    }

    inline void to_json(json & j, const worldLayout & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["enum"] = x.getWorldLayoutEnum();
    }

    inline void from_json(const json & j, ldtkJsonRootProperties& x) {
        x.setBackupLimit(j.at("backupLimit").get<appBuildId>());
        x.setDefaultEntityWidth(j.at("defaultEntityWidth").get<appBuildId>());
        x.setBackupOnSave(j.at("backupOnSave").get<appBuildId>());
        x.setWorldGridWidth(j.at("worldGridWidth").get<appBuildId>());
        x.setIid(j.at("iid").get<appBuildId>());
        x.setDefaultLevelBgColor(j.at("defaultLevelBgColor").get<appBuildId>());
        x.setBgColor(j.at("bgColor").get<appBuildId>());
        x.setWorlds(j.at("worlds").get<customCommands>());
        x.setToc(j.at("toc").get<customCommands>());
        x.setNextUid(j.at("nextUid").get<appBuildId>());
        x.setImageExportMode(j.at("imageExportMode").get<identifierStyle>());
        x.setIdentifierStyle(j.at("identifierStyle").get<identifierStyle>());
        x.setDefaultPivotY(j.at("defaultPivotY").get<appBuildId>());
        x.setDummyWorldIid(j.at("dummyWorldIid").get<appBuildId>());
        x.setCustomCommands(j.at("customCommands").get<customCommands>());
        x.setWorldGridHeight(j.at("worldGridHeight").get<appBuildId>());
        x.setAppBuildId(j.at("appBuildId").get<appBuildId>());
        x.setDefaultGridSize(j.at("defaultGridSize").get<appBuildId>());
        x.setWorldLayout(j.at("worldLayout").get<worldLayout>());
        x.setFlags(j.at("flags").get<flags>());
        x.setLevelNamePattern(j.at("levelNamePattern").get<appBuildId>());
        x.setExportPng(j.at("exportPng").get<appBuildId>());
        x.setDefaultLevelWidth(j.at("defaultLevelWidth").get<appBuildId>());
        x.setPngFilePattern(j.at("pngFilePattern").get<appBuildId>());
        x.setForcedRefs(j.at("__FORCED_REFS").get<forcedRefs>());
        x.setExportTiled(j.at("exportTiled").get<appBuildId>());
        x.setDefs(j.at("defs").get<defs>());
        x.setLevels(j.at("levels").get<customCommands>());
        x.setJsonVersion(j.at("jsonVersion").get<appBuildId>());
        x.setDefaultEntityHeight(j.at("defaultEntityHeight").get<appBuildId>());
        x.setDefaultPivotX(j.at("defaultPivotX").get<appBuildId>());
        x.setDefaultLevelHeight(j.at("defaultLevelHeight").get<appBuildId>());
        x.setSimplifiedExport(j.at("simplifiedExport").get<appBuildId>());
        x.setExternalLevels(j.at("externalLevels").get<appBuildId>());
        x.setTutorialDesc(j.at("tutorialDesc").get<appBuildId>());
        x.setMinifyJson(j.at("minifyJson").get<appBuildId>());
        x.setExportLevelBg(j.at("exportLevelBg").get<appBuildId>());
        x.setBackupRelPath(j.at("backupRelPath").get<appBuildId>());
    }

    inline void to_json(json & j, const ldtkJsonRootProperties & x) {
        j = json::object();
        j["backupLimit"] = x.getBackupLimit();
        j["defaultEntityWidth"] = x.getDefaultEntityWidth();
        j["backupOnSave"] = x.getBackupOnSave();
        j["worldGridWidth"] = x.getWorldGridWidth();
        j["iid"] = x.getIid();
        j["defaultLevelBgColor"] = x.getDefaultLevelBgColor();
        j["bgColor"] = x.getBgColor();
        j["worlds"] = x.getWorlds();
        j["toc"] = x.getToc();
        j["nextUid"] = x.getNextUid();
        j["imageExportMode"] = x.getImageExportMode();
        j["identifierStyle"] = x.getIdentifierStyle();
        j["defaultPivotY"] = x.getDefaultPivotY();
        j["dummyWorldIid"] = x.getDummyWorldIid();
        j["customCommands"] = x.getCustomCommands();
        j["worldGridHeight"] = x.getWorldGridHeight();
        j["appBuildId"] = x.getAppBuildId();
        j["defaultGridSize"] = x.getDefaultGridSize();
        j["worldLayout"] = x.getWorldLayout();
        j["flags"] = x.getFlags();
        j["levelNamePattern"] = x.getLevelNamePattern();
        j["exportPng"] = x.getExportPng();
        j["defaultLevelWidth"] = x.getDefaultLevelWidth();
        j["pngFilePattern"] = x.getPngFilePattern();
        j["__FORCED_REFS"] = x.getForcedRefs();
        j["exportTiled"] = x.getExportTiled();
        j["defs"] = x.getDefs();
        j["levels"] = x.getLevels();
        j["jsonVersion"] = x.getJsonVersion();
        j["defaultEntityHeight"] = x.getDefaultEntityHeight();
        j["defaultPivotX"] = x.getDefaultPivotX();
        j["defaultLevelHeight"] = x.getDefaultLevelHeight();
        j["simplifiedExport"] = x.getSimplifiedExport();
        j["externalLevels"] = x.getExternalLevels();
        j["tutorialDesc"] = x.getTutorialDesc();
        j["minifyJson"] = x.getMinifyJson();
        j["exportLevelBg"] = x.getExportLevelBg();
        j["backupRelPath"] = x.getBackupRelPath();
    }

    inline void from_json(const json & j, ldtkJsonRoot& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setProperties(j.at("properties").get<ldtkJsonRootProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const ldtkJsonRoot & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, oneOf& x) {
        x.setType(get_stack_optional<std::vector<forcedRefsType>>(j, "type"));
        x.setRef(get_stack_optional<std::string>(j, "$ref"));
    }

    inline void to_json(json & j, const oneOf & x) {
        j = json::object();
        if (x.getType()) {
            j["type"] = x.getType();
        }
        if (x.getRef()) {
            j["$ref"] = x.getRef();
        }
    }

    inline void from_json(const json & j, icon& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setOneOf(j.at("oneOf").get<std::vector<oneOf>>());
    }

    inline void to_json(json & j, const icon & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["oneOf"] = x.getOneOf();
    }

    inline void from_json(const json & j, requiredBiomeValuesItems& x) {
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const requiredBiomeValuesItems & x) {
        j = json::object();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, requiredBiomeValues& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setItems(j.at("items").get<requiredBiomeValuesItems>());
        x.setType(j.at("type").get<std::vector<customCommandsType>>());
    }

    inline void to_json(json & j, const requiredBiomeValues & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["items"] = x.getItems();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, autoLayerRuleGroupProperties& x) {
        x.setName(j.at("name").get<appBuildId>());
        x.setCollapsed(j.at("collapsed").get<appBuildId>());
        x.setBiomeRequirementMode(j.at("biomeRequirementMode").get<appBuildId>());
        x.setColor(j.at("color").get<appBuildId>());
        x.setIsOptional(j.at("isOptional").get<appBuildId>());
        x.setIcon(j.at("icon").get<icon>());
        x.setUsesWizard(j.at("usesWizard").get<appBuildId>());
        x.setUid(j.at("uid").get<appBuildId>());
        x.setRequiredBiomeValues(j.at("requiredBiomeValues").get<requiredBiomeValues>());
        x.setActive(j.at("active").get<appBuildId>());
        x.setRules(j.at("rules").get<customCommands>());
    }

    inline void to_json(json & j, const autoLayerRuleGroupProperties & x) {
        j = json::object();
        j["name"] = x.getName();
        j["collapsed"] = x.getCollapsed();
        j["biomeRequirementMode"] = x.getBiomeRequirementMode();
        j["color"] = x.getColor();
        j["isOptional"] = x.getIsOptional();
        j["icon"] = x.getIcon();
        j["usesWizard"] = x.getUsesWizard();
        j["uid"] = x.getUid();
        j["requiredBiomeValues"] = x.getRequiredBiomeValues();
        j["active"] = x.getActive();
        j["rules"] = x.getRules();
    }

    inline void from_json(const json & j, autoLayerRuleGroup& x) {
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<autoLayerRuleGroupProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const autoLayerRuleGroup & x) {
        j = json::object();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, tileRectsIdsItems& x) {
        x.setItems(j.at("items").get<requiredBiomeValuesItems>());
        x.setType(j.at("type").get<std::vector<customCommandsType>>());
    }

    inline void to_json(json & j, const tileRectsIdsItems & x) {
        j = json::object();
        j["items"] = x.getItems();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, tileRectsIds& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setItems(j.at("items").get<tileRectsIdsItems>());
        x.setType(j.at("type").get<std::vector<customCommandsType>>());
    }

    inline void to_json(json & j, const tileRectsIds & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["items"] = x.getItems();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, autoRuleDefProperties& x) {
        x.setFlipX(j.at("flipX").get<appBuildId>());
        x.setPivotX(j.at("pivotX").get<appBuildId>());
        x.setPerlinActive(j.at("perlinActive").get<appBuildId>());
        x.setTileRectsIds(j.at("tileRectsIds").get<tileRectsIds>());
        x.setPerlinScale(j.at("perlinScale").get<appBuildId>());
        x.setOutOfBoundsValue(j.at("outOfBoundsValue").get<appBuildId>());
        x.setPattern(j.at("pattern").get<requiredBiomeValues>());
        x.setTileRandomXMin(j.at("tileRandomXMin").get<appBuildId>());
        x.setChecker(j.at("checker").get<identifierStyle>());
        x.setPerlinOctaves(j.at("perlinOctaves").get<appBuildId>());
        x.setTileIds(j.at("tileIds").get<requiredBiomeValues>());
        x.setAlpha(j.at("alpha").get<appBuildId>());
        x.setTileXOffset(j.at("tileXOffset").get<appBuildId>());
        x.setInvalidated(j.at("invalidated").get<appBuildId>());
        x.setXModulo(j.at("xModulo").get<appBuildId>());
        x.setSize(j.at("size").get<appBuildId>());
        x.setChance(j.at("chance").get<appBuildId>());
        x.setBreakOnMatch(j.at("breakOnMatch").get<appBuildId>());
        x.setTileYOffset(j.at("tileYOffset").get<appBuildId>());
        x.setUid(j.at("uid").get<appBuildId>());
        x.setPerlinSeed(j.at("perlinSeed").get<appBuildId>());
        x.setYOffset(j.at("yOffset").get<appBuildId>());
        x.setTileRandomYMax(j.at("tileRandomYMax").get<appBuildId>());
        x.setTileRandomYMin(j.at("tileRandomYMin").get<appBuildId>());
        x.setTileMode(j.at("tileMode").get<identifierStyle>());
        x.setFlipY(j.at("flipY").get<appBuildId>());
        x.setTileRandomXMax(j.at("tileRandomXMax").get<appBuildId>());
        x.setPivotY(j.at("pivotY").get<appBuildId>());
        x.setYModulo(j.at("yModulo").get<appBuildId>());
        x.setActive(j.at("active").get<appBuildId>());
        x.setXOffset(j.at("xOffset").get<appBuildId>());
    }

    inline void to_json(json & j, const autoRuleDefProperties & x) {
        j = json::object();
        j["flipX"] = x.getFlipX();
        j["pivotX"] = x.getPivotX();
        j["perlinActive"] = x.getPerlinActive();
        j["tileRectsIds"] = x.getTileRectsIds();
        j["perlinScale"] = x.getPerlinScale();
        j["outOfBoundsValue"] = x.getOutOfBoundsValue();
        j["pattern"] = x.getPattern();
        j["tileRandomXMin"] = x.getTileRandomXMin();
        j["checker"] = x.getChecker();
        j["perlinOctaves"] = x.getPerlinOctaves();
        j["tileIds"] = x.getTileIds();
        j["alpha"] = x.getAlpha();
        j["tileXOffset"] = x.getTileXOffset();
        j["invalidated"] = x.getInvalidated();
        j["xModulo"] = x.getXModulo();
        j["size"] = x.getSize();
        j["chance"] = x.getChance();
        j["breakOnMatch"] = x.getBreakOnMatch();
        j["tileYOffset"] = x.getTileYOffset();
        j["uid"] = x.getUid();
        j["perlinSeed"] = x.getPerlinSeed();
        j["yOffset"] = x.getYOffset();
        j["tileRandomYMax"] = x.getTileRandomYMax();
        j["tileRandomYMin"] = x.getTileRandomYMin();
        j["tileMode"] = x.getTileMode();
        j["flipY"] = x.getFlipY();
        j["tileRandomXMax"] = x.getTileRandomXMax();
        j["pivotY"] = x.getPivotY();
        j["yModulo"] = x.getYModulo();
        j["active"] = x.getActive();
        j["xOffset"] = x.getXOffset();
    }

    inline void from_json(const json & j, autoRuleDef& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<autoRuleDefProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const autoRuleDef & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, customCommandProperties& x) {
        x.setWhen(j.at("when").get<identifierStyle>());
        x.setCommand(j.at("command").get<appBuildId>());
    }

    inline void to_json(json & j, const customCommandProperties & x) {
        j = json::object();
        j["when"] = x.getWhen();
        j["command"] = x.getCommand();
    }

    inline void from_json(const json & j, customCommand& x) {
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<customCommandProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const customCommand & x) {
        j = json::object();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, definitionsProperties& x) {
        x.setTilesets(j.at("tilesets").get<customCommands>());
        x.setLayers(j.at("layers").get<customCommands>());
        x.setLevelFields(j.at("levelFields").get<customCommands>());
        x.setEnums(j.at("enums").get<customCommands>());
        x.setEntities(j.at("entities").get<customCommands>());
        x.setExternalEnums(j.at("externalEnums").get<customCommands>());
    }

    inline void to_json(json & j, const definitionsProperties & x) {
        j = json::object();
        j["tilesets"] = x.getTilesets();
        j["layers"] = x.getLayers();
        j["levelFields"] = x.getLevelFields();
        j["enums"] = x.getEnums();
        j["entities"] = x.getEntities();
        j["externalEnums"] = x.getExternalEnums();
    }

    inline void from_json(const json & j, definitions& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<definitionsProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const definitions & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, entityDefProperties& x) {
        x.setTileId(j.at("tileId").get<appBuildId>());
        x.setShowName(j.at("showName").get<appBuildId>());
        x.setTilesetId(j.at("tilesetId").get<appBuildId>());
        x.setMaxHeight(j.at("maxHeight").get<appBuildId>());
        x.setLimitScope(j.at("limitScope").get<identifierStyle>());
        x.setPivotX(j.at("pivotX").get<appBuildId>());
        x.setMaxCount(j.at("maxCount").get<appBuildId>());
        x.setAllowOutOfBounds(j.at("allowOutOfBounds").get<appBuildId>());
        x.setHollow(j.at("hollow").get<appBuildId>());
        x.setMinHeight(j.at("minHeight").get<appBuildId>());
        x.setKeepAspectRatio(j.at("keepAspectRatio").get<appBuildId>());
        x.setColor(j.at("color").get<appBuildId>());
        x.setMinWidth(j.at("minWidth").get<appBuildId>());
        x.setTileRect(j.at("tileRect").get<icon>());
        x.setDoc(j.at("doc").get<appBuildId>());
        x.setFieldDefs(j.at("fieldDefs").get<customCommands>());
        x.setTileRenderMode(j.at("tileRenderMode").get<identifierStyle>());
        x.setLimitBehavior(j.at("limitBehavior").get<identifierStyle>());
        x.setTileOpacity(j.at("tileOpacity").get<appBuildId>());
        x.setNineSliceBorders(j.at("nineSliceBorders").get<requiredBiomeValues>());
        x.setResizableX(j.at("resizableX").get<appBuildId>());
        x.setUiTileRect(j.at("uiTileRect").get<icon>());
        x.setUid(j.at("uid").get<appBuildId>());
        x.setLineOpacity(j.at("lineOpacity").get<appBuildId>());
        x.setMaxWidth(j.at("maxWidth").get<appBuildId>());
        x.setResizableY(j.at("resizableY").get<appBuildId>());
        x.setExportToToc(j.at("exportToToc").get<appBuildId>());
        x.setFillOpacity(j.at("fillOpacity").get<appBuildId>());
        x.setHeight(j.at("height").get<appBuildId>());
        x.setIdentifier(j.at("identifier").get<appBuildId>());
        x.setPivotY(j.at("pivotY").get<appBuildId>());
        x.setRenderMode(j.at("renderMode").get<identifierStyle>());
        x.setTags(j.at("tags").get<requiredBiomeValues>());
        x.setWidth(j.at("width").get<appBuildId>());
    }

    inline void to_json(json & j, const entityDefProperties & x) {
        j = json::object();
        j["tileId"] = x.getTileId();
        j["showName"] = x.getShowName();
        j["tilesetId"] = x.getTilesetId();
        j["maxHeight"] = x.getMaxHeight();
        j["limitScope"] = x.getLimitScope();
        j["pivotX"] = x.getPivotX();
        j["maxCount"] = x.getMaxCount();
        j["allowOutOfBounds"] = x.getAllowOutOfBounds();
        j["hollow"] = x.getHollow();
        j["minHeight"] = x.getMinHeight();
        j["keepAspectRatio"] = x.getKeepAspectRatio();
        j["color"] = x.getColor();
        j["minWidth"] = x.getMinWidth();
        j["tileRect"] = x.getTileRect();
        j["doc"] = x.getDoc();
        j["fieldDefs"] = x.getFieldDefs();
        j["tileRenderMode"] = x.getTileRenderMode();
        j["limitBehavior"] = x.getLimitBehavior();
        j["tileOpacity"] = x.getTileOpacity();
        j["nineSliceBorders"] = x.getNineSliceBorders();
        j["resizableX"] = x.getResizableX();
        j["uiTileRect"] = x.getUiTileRect();
        j["uid"] = x.getUid();
        j["lineOpacity"] = x.getLineOpacity();
        j["maxWidth"] = x.getMaxWidth();
        j["resizableY"] = x.getResizableY();
        j["exportToToc"] = x.getExportToToc();
        j["fillOpacity"] = x.getFillOpacity();
        j["height"] = x.getHeight();
        j["identifier"] = x.getIdentifier();
        j["pivotY"] = x.getPivotY();
        j["renderMode"] = x.getRenderMode();
        j["tags"] = x.getTags();
        j["width"] = x.getWidth();
    }

    inline void from_json(const json & j, entityDef& x) {
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<entityDefProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const entityDef & x) {
        j = json::object();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, entityInstanceProperties& x) {
        x.setIid(j.at("iid").get<appBuildId>());
        x.setDefUid(j.at("defUid").get<appBuildId>());
        x.setIdentifier(j.at("__identifier").get<appBuildId>());
        x.setTile(j.at("__tile").get<icon>());
        x.setPx(j.at("px").get<requiredBiomeValues>());
        x.setWorldX(j.at("__worldX").get<appBuildId>());
        x.setWorldY(j.at("__worldY").get<appBuildId>());
        x.setSmartColor(j.at("__smartColor").get<appBuildId>());
        x.setGrid(j.at("__grid").get<requiredBiomeValues>());
        x.setPivot(j.at("__pivot").get<requiredBiomeValues>());
        x.setFieldInstances(j.at("fieldInstances").get<customCommands>());
        x.setHeight(j.at("height").get<appBuildId>());
        x.setTags(j.at("__tags").get<requiredBiomeValues>());
        x.setWidth(j.at("width").get<appBuildId>());
    }

    inline void to_json(json & j, const entityInstanceProperties & x) {
        j = json::object();
        j["iid"] = x.getIid();
        j["defUid"] = x.getDefUid();
        j["__identifier"] = x.getIdentifier();
        j["__tile"] = x.getTile();
        j["px"] = x.getPx();
        j["__worldX"] = x.getWorldX();
        j["__worldY"] = x.getWorldY();
        j["__smartColor"] = x.getSmartColor();
        j["__grid"] = x.getGrid();
        j["__pivot"] = x.getPivot();
        j["fieldInstances"] = x.getFieldInstances();
        j["height"] = x.getHeight();
        j["__tags"] = x.getTags();
        j["width"] = x.getWidth();
    }

    inline void from_json(const json & j, entityInstance& x) {
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<entityInstanceProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const entityInstance & x) {
        j = json::object();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, entityReferenceInfosProperties& x) {
        x.setWorldIid(j.at("worldIid").get<appBuildId>());
        x.setEntityIid(j.at("entityIid").get<appBuildId>());
        x.setLayerIid(j.at("layerIid").get<appBuildId>());
        x.setLevelIid(j.at("levelIid").get<appBuildId>());
    }

    inline void to_json(json & j, const entityReferenceInfosProperties & x) {
        j = json::object();
        j["worldIid"] = x.getWorldIid();
        j["entityIid"] = x.getEntityIid();
        j["layerIid"] = x.getLayerIid();
        j["levelIid"] = x.getLevelIid();
    }

    inline void from_json(const json & j, entityReferenceInfos& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<entityReferenceInfosProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const entityReferenceInfos & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, enumDefProperties& x) {
        x.setExternalFileChecksum(j.at("externalFileChecksum").get<appBuildId>());
        x.setExternalRelPath(j.at("externalRelPath").get<appBuildId>());
        x.setUid(j.at("uid").get<appBuildId>());
        x.setValues(j.at("values").get<customCommands>());
        x.setIconTilesetUid(j.at("iconTilesetUid").get<appBuildId>());
        x.setIdentifier(j.at("identifier").get<appBuildId>());
        x.setTags(j.at("tags").get<requiredBiomeValues>());
    }

    inline void to_json(json & j, const enumDefProperties & x) {
        j = json::object();
        j["externalFileChecksum"] = x.getExternalFileChecksum();
        j["externalRelPath"] = x.getExternalRelPath();
        j["uid"] = x.getUid();
        j["values"] = x.getValues();
        j["iconTilesetUid"] = x.getIconTilesetUid();
        j["identifier"] = x.getIdentifier();
        j["tags"] = x.getTags();
    }

    inline void from_json(const json & j, enumDef& x) {
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<enumDefProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const enumDef & x) {
        j = json::object();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, enumDefValuesProperties& x) {
        x.setTileId(j.at("tileId").get<appBuildId>());
        x.setColor(j.at("color").get<appBuildId>());
        x.setTileRect(j.at("tileRect").get<icon>());
        x.setId(j.at("id").get<appBuildId>());
        x.setTileSrcRect(j.at("__tileSrcRect").get<requiredBiomeValues>());
    }

    inline void to_json(json & j, const enumDefValuesProperties & x) {
        j = json::object();
        j["tileId"] = x.getTileId();
        j["color"] = x.getColor();
        j["tileRect"] = x.getTileRect();
        j["id"] = x.getId();
        j["__tileSrcRect"] = x.getTileSrcRect();
    }

    inline void from_json(const json & j, enumDefValues& x) {
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<enumDefValuesProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const enumDefValues & x) {
        j = json::object();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, enumTagValueProperties& x) {
        x.setTileIds(j.at("tileIds").get<requiredBiomeValues>());
        x.setEnumValueId(j.at("enumValueId").get<appBuildId>());
    }

    inline void to_json(json & j, const enumTagValueProperties & x) {
        j = json::object();
        j["tileIds"] = x.getTileIds();
        j["enumValueId"] = x.getEnumValueId();
    }

    inline void from_json(const json & j, enumTagValue& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<enumTagValueProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const enumTagValue & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, defaultOverride& x) {
        x.setDescription(j.at("description").get<std::string>());
    }

    inline void to_json(json & j, const defaultOverride & x) {
        j = json::object();
        j["description"] = x.getDescription();
    }

    inline void from_json(const json & j, fieldDefProperties& x) {
        x.setAcceptFileTypes(j.at("acceptFileTypes").get<requiredBiomeValues>());
        x.setEditorDisplayScale(j.at("editorDisplayScale").get<appBuildId>());
        x.setSearchable(j.at("searchable").get<appBuildId>());
        x.setUseForSmartColor(j.at("useForSmartColor").get<appBuildId>());
        x.setEditorShowInWorld(j.at("editorShowInWorld").get<appBuildId>());
        x.setAllowedRefs(j.at("allowedRefs").get<identifierStyle>());
        x.setEditorAlwaysShow(j.at("editorAlwaysShow").get<appBuildId>());
        x.setArrayMinLength(j.at("arrayMinLength").get<appBuildId>());
        x.setEditorTextSuffix(j.at("editorTextSuffix").get<appBuildId>());
        x.setMin(j.at("min").get<appBuildId>());
        x.setType(j.at("__type").get<appBuildId>());
        x.setEditorDisplayMode(j.at("editorDisplayMode").get<identifierStyle>());
        x.setEditorDisplayColor(j.at("editorDisplayColor").get<appBuildId>());
        x.setCanBeNull(j.at("canBeNull").get<appBuildId>());
        x.setAutoChainRef(j.at("autoChainRef").get<appBuildId>());
        x.setDoc(j.at("doc").get<appBuildId>());
        x.setAllowedRefsEntityUid(j.at("allowedRefsEntityUid").get<appBuildId>());
        x.setTilesetUid(j.at("tilesetUid").get<appBuildId>());
        x.setAllowedRefTags(j.at("allowedRefTags").get<requiredBiomeValues>());
        x.setSymmetricalRef(j.at("symmetricalRef").get<appBuildId>());
        x.setUid(j.at("uid").get<appBuildId>());
        x.setEditorTextPrefix(j.at("editorTextPrefix").get<appBuildId>());
        x.setIsArray(j.at("isArray").get<appBuildId>());
        x.setExportToToc(j.at("exportToToc").get<appBuildId>());
        x.setEditorDisplayPos(j.at("editorDisplayPos").get<identifierStyle>());
        x.setTextLanguageMode(j.at("textLanguageMode").get<worldLayout>());
        x.setMax(j.at("max").get<appBuildId>());
        x.setAllowOutOfLevelRef(j.at("allowOutOfLevelRef").get<appBuildId>());
        x.setEditorCutLongValues(j.at("editorCutLongValues").get<appBuildId>());
        x.setDefaultOverride(j.at("defaultOverride").get<defaultOverride>());
        x.setEditorLinkStyle(j.at("editorLinkStyle").get<identifierStyle>());
        x.setRegex(j.at("regex").get<appBuildId>());
        x.setPropertiesType(j.at("type").get<appBuildId>());
        x.setIdentifier(j.at("identifier").get<appBuildId>());
        x.setArrayMaxLength(j.at("arrayMaxLength").get<appBuildId>());
    }

    inline void to_json(json & j, const fieldDefProperties & x) {
        j = json::object();
        j["acceptFileTypes"] = x.getAcceptFileTypes();
        j["editorDisplayScale"] = x.getEditorDisplayScale();
        j["searchable"] = x.getSearchable();
        j["useForSmartColor"] = x.getUseForSmartColor();
        j["editorShowInWorld"] = x.getEditorShowInWorld();
        j["allowedRefs"] = x.getAllowedRefs();
        j["editorAlwaysShow"] = x.getEditorAlwaysShow();
        j["arrayMinLength"] = x.getArrayMinLength();
        j["editorTextSuffix"] = x.getEditorTextSuffix();
        j["min"] = x.getMin();
        j["__type"] = x.getType();
        j["editorDisplayMode"] = x.getEditorDisplayMode();
        j["editorDisplayColor"] = x.getEditorDisplayColor();
        j["canBeNull"] = x.getCanBeNull();
        j["autoChainRef"] = x.getAutoChainRef();
        j["doc"] = x.getDoc();
        j["allowedRefsEntityUid"] = x.getAllowedRefsEntityUid();
        j["tilesetUid"] = x.getTilesetUid();
        j["allowedRefTags"] = x.getAllowedRefTags();
        j["symmetricalRef"] = x.getSymmetricalRef();
        j["uid"] = x.getUid();
        j["editorTextPrefix"] = x.getEditorTextPrefix();
        j["isArray"] = x.getIsArray();
        j["exportToToc"] = x.getExportToToc();
        j["editorDisplayPos"] = x.getEditorDisplayPos();
        j["textLanguageMode"] = x.getTextLanguageMode();
        j["max"] = x.getMax();
        j["allowOutOfLevelRef"] = x.getAllowOutOfLevelRef();
        j["editorCutLongValues"] = x.getEditorCutLongValues();
        j["defaultOverride"] = x.getDefaultOverride();
        j["editorLinkStyle"] = x.getEditorLinkStyle();
        j["regex"] = x.getRegex();
        j["type"] = x.getPropertiesType();
        j["identifier"] = x.getIdentifier();
        j["arrayMaxLength"] = x.getArrayMaxLength();
    }

    inline void from_json(const json & j, fieldDef& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<fieldDefProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const fieldDef & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, realEditorValuesItems& x) {
    }

    inline void to_json(json & j, const realEditorValuesItems & x) {
        j = json::object();
    }

    inline void from_json(const json & j, realEditorValues& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setItems(j.at("items").get<realEditorValuesItems>());
        x.setType(j.at("type").get<std::vector<customCommandsType>>());
    }

    inline void to_json(json & j, const realEditorValues & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["items"] = x.getItems();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, fieldInstanceProperties& x) {
        x.setType(j.at("__type").get<appBuildId>());
        x.setDefUid(j.at("defUid").get<appBuildId>());
        x.setIdentifier(j.at("__identifier").get<appBuildId>());
        x.setTile(j.at("__tile").get<icon>());
        x.setRealEditorValues(j.at("realEditorValues").get<realEditorValues>());
        x.setValue(j.at("__value").get<defaultOverride>());
    }

    inline void to_json(json & j, const fieldInstanceProperties & x) {
        j = json::object();
        j["__type"] = x.getType();
        j["defUid"] = x.getDefUid();
        j["__identifier"] = x.getIdentifier();
        j["__tile"] = x.getTile();
        j["realEditorValues"] = x.getRealEditorValues();
        j["__value"] = x.getValue();
    }

    inline void from_json(const json & j, fieldInstance& x) {
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<fieldInstanceProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const fieldInstance & x) {
        j = json::object();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, gridPointProperties& x) {
        x.setCy(j.at("cy").get<appBuildId>());
        x.setCx(j.at("cx").get<appBuildId>());
    }

    inline void to_json(json & j, const gridPointProperties & x) {
        j = json::object();
        j["cy"] = x.getCy();
        j["cx"] = x.getCx();
    }

    inline void from_json(const json & j, gridPoint& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<gridPointProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const gridPoint & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, intGridValueDefProperties& x) {
        x.setTile(j.at("tile").get<icon>());
        x.setColor(j.at("color").get<appBuildId>());
        x.setIdentifier(j.at("identifier").get<appBuildId>());
        x.setValue(j.at("value").get<appBuildId>());
        x.setGroupUid(j.at("groupUid").get<appBuildId>());
    }

    inline void to_json(json & j, const intGridValueDefProperties & x) {
        j = json::object();
        j["tile"] = x.getTile();
        j["color"] = x.getColor();
        j["identifier"] = x.getIdentifier();
        j["value"] = x.getValue();
        j["groupUid"] = x.getGroupUid();
    }

    inline void from_json(const json & j, intGridValueDef& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<intGridValueDefProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const intGridValueDef & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, intGridValueGroupDefProperties& x) {
        x.setColor(j.at("color").get<appBuildId>());
        x.setUid(j.at("uid").get<appBuildId>());
        x.setIdentifier(j.at("identifier").get<appBuildId>());
    }

    inline void to_json(json & j, const intGridValueGroupDefProperties & x) {
        j = json::object();
        j["color"] = x.getColor();
        j["uid"] = x.getUid();
        j["identifier"] = x.getIdentifier();
    }

    inline void from_json(const json & j, intGridValueGroupDef& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<intGridValueGroupDefProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const intGridValueGroupDef & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, intGridValueInstanceProperties& x) {
        x.setV(j.at("v").get<appBuildId>());
        x.setCoordId(j.at("coordId").get<appBuildId>());
    }

    inline void to_json(json & j, const intGridValueInstanceProperties & x) {
        j = json::object();
        j["v"] = x.getV();
        j["coordId"] = x.getCoordId();
    }

    inline void from_json(const json & j, intGridValueInstance& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<intGridValueInstanceProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const intGridValueInstance & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, layerDefProperties& x) {
        x.setPxOffsetX(j.at("pxOffsetX").get<appBuildId>());
        x.setTilePivotX(j.at("tilePivotX").get<appBuildId>());
        x.setUiFilterTags(j.at("uiFilterTags").get<requiredBiomeValues>());
        x.setDisplayOpacity(j.at("displayOpacity").get<appBuildId>());
        x.setParallaxFactorY(j.at("parallaxFactorY").get<appBuildId>());
        x.setHideInList(j.at("hideInList").get<appBuildId>());
        x.setType(j.at("__type").get<appBuildId>());
        x.setGuideGridHei(j.at("guideGridHei").get<appBuildId>());
        x.setUiColor(j.at("uiColor").get<appBuildId>());
        x.setDoc(j.at("doc").get<appBuildId>());
        x.setTilesetDefUid(j.at("tilesetDefUid").get<appBuildId>());
        x.setCanSelectWhenInactive(j.at("canSelectWhenInactive").get<appBuildId>());
        x.setUseAsyncRender(j.at("useAsyncRender").get<appBuildId>());
        x.setAutoSourceLayerDefUid(j.at("autoSourceLayerDefUid").get<appBuildId>());
        x.setAutoTilesetDefUid(j.at("autoTilesetDefUid").get<appBuildId>());
        x.setParallaxScaling(j.at("parallaxScaling").get<appBuildId>());
        x.setRenderInWorldView(j.at("renderInWorldView").get<appBuildId>());
        x.setIntGridValuesGroups(j.at("intGridValuesGroups").get<customCommands>());
        x.setInactiveOpacity(j.at("inactiveOpacity").get<appBuildId>());
        x.setUid(j.at("uid").get<appBuildId>());
        x.setExcludedTags(j.at("excludedTags").get<requiredBiomeValues>());
        x.setHideFieldsWhenInactive(j.at("hideFieldsWhenInactive").get<appBuildId>());
        x.setIntGridValues(j.at("intGridValues").get<customCommands>());
        x.setAutoRuleGroups(j.at("autoRuleGroups").get<customCommands>());
        x.setPropertiesType(j.at("type").get<identifierStyle>());
        x.setIdentifier(j.at("identifier").get<appBuildId>());
        x.setGuideGridWid(j.at("guideGridWid").get<appBuildId>());
        x.setRequiredTags(j.at("requiredTags").get<requiredBiomeValues>());
        x.setPxOffsetY(j.at("pxOffsetY").get<appBuildId>());
        x.setTilePivotY(j.at("tilePivotY").get<appBuildId>());
        x.setBiomeFieldUid(j.at("biomeFieldUid").get<appBuildId>());
        x.setGridSize(j.at("gridSize").get<appBuildId>());
        x.setParallaxFactorX(j.at("parallaxFactorX").get<appBuildId>());
        x.setAutoTilesKilledByOtherLayerUid(j.at("autoTilesKilledByOtherLayerUid").get<appBuildId>());
    }

    inline void to_json(json & j, const layerDefProperties & x) {
        j = json::object();
        j["pxOffsetX"] = x.getPxOffsetX();
        j["tilePivotX"] = x.getTilePivotX();
        j["uiFilterTags"] = x.getUiFilterTags();
        j["displayOpacity"] = x.getDisplayOpacity();
        j["parallaxFactorY"] = x.getParallaxFactorY();
        j["hideInList"] = x.getHideInList();
        j["__type"] = x.getType();
        j["guideGridHei"] = x.getGuideGridHei();
        j["uiColor"] = x.getUiColor();
        j["doc"] = x.getDoc();
        j["tilesetDefUid"] = x.getTilesetDefUid();
        j["canSelectWhenInactive"] = x.getCanSelectWhenInactive();
        j["useAsyncRender"] = x.getUseAsyncRender();
        j["autoSourceLayerDefUid"] = x.getAutoSourceLayerDefUid();
        j["autoTilesetDefUid"] = x.getAutoTilesetDefUid();
        j["parallaxScaling"] = x.getParallaxScaling();
        j["renderInWorldView"] = x.getRenderInWorldView();
        j["intGridValuesGroups"] = x.getIntGridValuesGroups();
        j["inactiveOpacity"] = x.getInactiveOpacity();
        j["uid"] = x.getUid();
        j["excludedTags"] = x.getExcludedTags();
        j["hideFieldsWhenInactive"] = x.getHideFieldsWhenInactive();
        j["intGridValues"] = x.getIntGridValues();
        j["autoRuleGroups"] = x.getAutoRuleGroups();
        j["type"] = x.getPropertiesType();
        j["identifier"] = x.getIdentifier();
        j["guideGridWid"] = x.getGuideGridWid();
        j["requiredTags"] = x.getRequiredTags();
        j["pxOffsetY"] = x.getPxOffsetY();
        j["tilePivotY"] = x.getTilePivotY();
        j["biomeFieldUid"] = x.getBiomeFieldUid();
        j["gridSize"] = x.getGridSize();
        j["parallaxFactorX"] = x.getParallaxFactorX();
        j["autoTilesKilledByOtherLayerUid"] = x.getAutoTilesKilledByOtherLayerUid();
    }

    inline void from_json(const json & j, layerDef& x) {
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<layerDefProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const layerDef & x) {
        j = json::object();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, layerInstanceProperties& x) {
        x.setCHei(j.at("__cHei").get<appBuildId>());
        x.setPxOffsetX(j.at("pxOffsetX").get<appBuildId>());
        x.setTilesetRelPath(j.at("__tilesetRelPath").get<appBuildId>());
        x.setIid(j.at("iid").get<appBuildId>());
        x.setLevelId(j.at("levelId").get<appBuildId>());
        x.setType(j.at("__type").get<appBuildId>());
        x.setAutoLayerTiles(j.at("autoLayerTiles").get<customCommands>());
        x.setOptionalRules(j.at("optionalRules").get<requiredBiomeValues>());
        x.setIdentifier(j.at("__identifier").get<appBuildId>());
        x.setGridSize(j.at("__gridSize").get<appBuildId>());
        x.setPxTotalOffsetY(j.at("__pxTotalOffsetY").get<appBuildId>());
        x.setIntGridCsv(j.at("intGridCsv").get<requiredBiomeValues>());
        x.setOverrideTilesetUid(j.at("overrideTilesetUid").get<appBuildId>());
        x.setVisible(j.at("visible").get<appBuildId>());
        x.setEntityInstances(j.at("entityInstances").get<customCommands>());
        x.setOpacity(j.at("__opacity").get<appBuildId>());
        x.setSeed(j.at("seed").get<appBuildId>());
        x.setLayerDefUid(j.at("layerDefUid").get<appBuildId>());
        x.setPxTotalOffsetX(j.at("__pxTotalOffsetX").get<appBuildId>());
        x.setCWid(j.at("__cWid").get<appBuildId>());
        x.setPxOffsetY(j.at("pxOffsetY").get<appBuildId>());
        x.setTilesetDefUid(j.at("__tilesetDefUid").get<appBuildId>());
        x.setGridTiles(j.at("gridTiles").get<customCommands>());
        x.setIntGrid(j.at("intGrid").get<customCommands>());
    }

    inline void to_json(json & j, const layerInstanceProperties & x) {
        j = json::object();
        j["__cHei"] = x.getCHei();
        j["pxOffsetX"] = x.getPxOffsetX();
        j["__tilesetRelPath"] = x.getTilesetRelPath();
        j["iid"] = x.getIid();
        j["levelId"] = x.getLevelId();
        j["__type"] = x.getType();
        j["autoLayerTiles"] = x.getAutoLayerTiles();
        j["optionalRules"] = x.getOptionalRules();
        j["__identifier"] = x.getIdentifier();
        j["__gridSize"] = x.getGridSize();
        j["__pxTotalOffsetY"] = x.getPxTotalOffsetY();
        j["intGridCsv"] = x.getIntGridCsv();
        j["overrideTilesetUid"] = x.getOverrideTilesetUid();
        j["visible"] = x.getVisible();
        j["entityInstances"] = x.getEntityInstances();
        j["__opacity"] = x.getOpacity();
        j["seed"] = x.getSeed();
        j["layerDefUid"] = x.getLayerDefUid();
        j["__pxTotalOffsetX"] = x.getPxTotalOffsetX();
        j["__cWid"] = x.getCWid();
        j["pxOffsetY"] = x.getPxOffsetY();
        j["__tilesetDefUid"] = x.getTilesetDefUid();
        j["gridTiles"] = x.getGridTiles();
        j["intGrid"] = x.getIntGrid();
    }

    inline void from_json(const json & j, layerInstance& x) {
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<layerInstanceProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const layerInstance & x) {
        j = json::object();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, levelProperties& x) {
        x.setNeighbours(j.at("__neighbours").get<customCommands>());
        x.setBgColor(j.at("__bgColor").get<appBuildId>());
        x.setWorldX(j.at("worldX").get<appBuildId>());
        x.setExternalRelPath(j.at("externalRelPath").get<appBuildId>());
        x.setUseAutoIdentifier(j.at("useAutoIdentifier").get<appBuildId>());
        x.setIid(j.at("iid").get<appBuildId>());
        x.setPropertiesBgColor(j.at("bgColor").get<appBuildId>());
        x.setPropertiesBgPos(j.at("bgPos").get<worldLayout>());
        x.setPxHei(j.at("pxHei").get<appBuildId>());
        x.setWorldY(j.at("worldY").get<appBuildId>());
        x.setBgPos(j.at("__bgPos").get<icon>());
        x.setUid(j.at("uid").get<appBuildId>());
        x.setSmartColor(j.at("__smartColor").get<appBuildId>());
        x.setFieldInstances(j.at("fieldInstances").get<customCommands>());
        x.setPxWid(j.at("pxWid").get<appBuildId>());
        x.setIdentifier(j.at("identifier").get<appBuildId>());
        x.setBgPivotY(j.at("bgPivotY").get<appBuildId>());
        x.setBgPivotX(j.at("bgPivotX").get<appBuildId>());
        x.setLayerInstances(j.at("layerInstances").get<customCommands>());
        x.setBgRelPath(j.at("bgRelPath").get<appBuildId>());
        x.setWorldDepth(j.at("worldDepth").get<appBuildId>());
    }

    inline void to_json(json & j, const levelProperties & x) {
        j = json::object();
        j["__neighbours"] = x.getNeighbours();
        j["__bgColor"] = x.getBgColor();
        j["worldX"] = x.getWorldX();
        j["externalRelPath"] = x.getExternalRelPath();
        j["useAutoIdentifier"] = x.getUseAutoIdentifier();
        j["iid"] = x.getIid();
        j["bgColor"] = x.getPropertiesBgColor();
        j["bgPos"] = x.getPropertiesBgPos();
        j["pxHei"] = x.getPxHei();
        j["worldY"] = x.getWorldY();
        j["__bgPos"] = x.getBgPos();
        j["uid"] = x.getUid();
        j["__smartColor"] = x.getSmartColor();
        j["fieldInstances"] = x.getFieldInstances();
        j["pxWid"] = x.getPxWid();
        j["identifier"] = x.getIdentifier();
        j["bgPivotY"] = x.getBgPivotY();
        j["bgPivotX"] = x.getBgPivotX();
        j["layerInstances"] = x.getLayerInstances();
        j["bgRelPath"] = x.getBgRelPath();
        j["worldDepth"] = x.getWorldDepth();
    }

    inline void from_json(const json & j, level& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<levelProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const level & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, levelBgPosInfosProperties& x) {
        x.setCropRect(j.at("cropRect").get<requiredBiomeValues>());
        x.setScale(j.at("scale").get<requiredBiomeValues>());
        x.setTopLeftPx(j.at("topLeftPx").get<requiredBiomeValues>());
    }

    inline void to_json(json & j, const levelBgPosInfosProperties & x) {
        j = json::object();
        j["cropRect"] = x.getCropRect();
        j["scale"] = x.getScale();
        j["topLeftPx"] = x.getTopLeftPx();
    }

    inline void from_json(const json & j, levelBgPosInfos& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<levelBgPosInfosProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const levelBgPosInfos & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, neighbourLevelProperties& x) {
        x.setLevelIid(j.at("levelIid").get<appBuildId>());
        x.setLevelUid(j.at("levelUid").get<appBuildId>());
        x.setDir(j.at("dir").get<appBuildId>());
    }

    inline void to_json(json & j, const neighbourLevelProperties & x) {
        j = json::object();
        j["levelIid"] = x.getLevelIid();
        j["levelUid"] = x.getLevelUid();
        j["dir"] = x.getDir();
    }

    inline void from_json(const json & j, neighbourLevel& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<neighbourLevelProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const neighbourLevel & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, tableOfContentEntryProperties& x) {
        x.setIdentifier(j.at("identifier").get<appBuildId>());
        x.setInstancesData(j.at("instancesData").get<customCommands>());
        x.setInstances(j.at("instances").get<customCommands>());
    }

    inline void to_json(json & j, const tableOfContentEntryProperties & x) {
        j = json::object();
        j["identifier"] = x.getIdentifier();
        j["instancesData"] = x.getInstancesData();
        j["instances"] = x.getInstances();
    }

    inline void from_json(const json & j, tableOfContentEntry& x) {
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<tableOfContentEntryProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const tableOfContentEntry & x) {
        j = json::object();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, tileProperties& x) {
        x.setT(j.at("t").get<appBuildId>());
        x.setD(j.at("d").get<requiredBiomeValues>());
        x.setPx(j.at("px").get<requiredBiomeValues>());
        x.setA(j.at("a").get<appBuildId>());
        x.setF(j.at("f").get<appBuildId>());
        x.setSrc(j.at("src").get<requiredBiomeValues>());
    }

    inline void to_json(json & j, const tileProperties & x) {
        j = json::object();
        j["t"] = x.getT();
        j["d"] = x.getD();
        j["px"] = x.getPx();
        j["a"] = x.getA();
        j["f"] = x.getF();
        j["src"] = x.getSrc();
    }

    inline void from_json(const json & j, tile& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<tileProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const tile & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, tileCustomMetadataProperties& x) {
        x.setTileId(j.at("tileId").get<appBuildId>());
        x.setData(j.at("data").get<appBuildId>());
    }

    inline void to_json(json & j, const tileCustomMetadataProperties & x) {
        j = json::object();
        j["tileId"] = x.getTileId();
        j["data"] = x.getData();
    }

    inline void from_json(const json & j, tileCustomMetadata& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<tileCustomMetadataProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const tileCustomMetadata & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, tilesetDefProperties& x) {
        x.setCachedPixelData(j.at("cachedPixelData").get<appBuildId>());
        x.setCHei(j.at("__cHei").get<appBuildId>());
        x.setPxHei(j.at("pxHei").get<appBuildId>());
        x.setCustomData(j.at("customData").get<customCommands>());
        x.setTagsSourceEnumUid(j.at("tagsSourceEnumUid").get<appBuildId>());
        x.setUid(j.at("uid").get<appBuildId>());
        x.setPadding(j.at("padding").get<appBuildId>());
        x.setEnumTags(j.at("enumTags").get<customCommands>());
        x.setPxWid(j.at("pxWid").get<appBuildId>());
        x.setCWid(j.at("__cWid").get<appBuildId>());
        x.setSpacing(j.at("spacing").get<appBuildId>());
        x.setIdentifier(j.at("identifier").get<appBuildId>());
        x.setSavedSelections(j.at("savedSelections").get<requiredBiomeValues>());
        x.setTags(j.at("tags").get<requiredBiomeValues>());
        x.setEmbedAtlas(j.at("embedAtlas").get<worldLayout>());
        x.setRelPath(j.at("relPath").get<appBuildId>());
        x.setTileGridSize(j.at("tileGridSize").get<appBuildId>());
    }

    inline void to_json(json & j, const tilesetDefProperties & x) {
        j = json::object();
        j["cachedPixelData"] = x.getCachedPixelData();
        j["__cHei"] = x.getCHei();
        j["pxHei"] = x.getPxHei();
        j["customData"] = x.getCustomData();
        j["tagsSourceEnumUid"] = x.getTagsSourceEnumUid();
        j["uid"] = x.getUid();
        j["padding"] = x.getPadding();
        j["enumTags"] = x.getEnumTags();
        j["pxWid"] = x.getPxWid();
        j["__cWid"] = x.getCWid();
        j["spacing"] = x.getSpacing();
        j["identifier"] = x.getIdentifier();
        j["savedSelections"] = x.getSavedSelections();
        j["tags"] = x.getTags();
        j["embedAtlas"] = x.getEmbedAtlas();
        j["relPath"] = x.getRelPath();
        j["tileGridSize"] = x.getTileGridSize();
    }

    inline void from_json(const json & j, tilesetDef& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<tilesetDefProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const tilesetDef & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, tilesetRectProperties& x) {
        x.setTilesetUid(j.at("tilesetUid").get<appBuildId>());
        x.setH(j.at("h").get<appBuildId>());
        x.setX(j.at("x").get<appBuildId>());
        x.setY(j.at("y").get<appBuildId>());
        x.setW(j.at("w").get<appBuildId>());
    }

    inline void to_json(json & j, const tilesetRectProperties & x) {
        j = json::object();
        j["tilesetUid"] = x.getTilesetUid();
        j["h"] = x.getH();
        j["x"] = x.getX();
        j["y"] = x.getY();
        j["w"] = x.getW();
    }

    inline void from_json(const json & j, tilesetRect& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<tilesetRectProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const tilesetRect & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, tocInstanceDataProperties& x) {
        x.setWorldX(j.at("worldX").get<appBuildId>());
        x.setWidPx(j.at("widPx").get<appBuildId>());
        x.setWorldY(j.at("worldY").get<appBuildId>());
        x.setHeiPx(j.at("heiPx").get<appBuildId>());
        x.setFields(j.at("fields").get<defaultOverride>());
        x.setIids(j.at("iids").get<defs>());
    }

    inline void to_json(json & j, const tocInstanceDataProperties & x) {
        j = json::object();
        j["worldX"] = x.getWorldX();
        j["widPx"] = x.getWidPx();
        j["worldY"] = x.getWorldY();
        j["heiPx"] = x.getHeiPx();
        j["fields"] = x.getFields();
        j["iids"] = x.getIids();
    }

    inline void from_json(const json & j, tocInstanceData& x) {
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<tocInstanceDataProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const tocInstanceData & x) {
        j = json::object();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, worldProperties& x) {
        x.setWorldGridWidth(j.at("worldGridWidth").get<appBuildId>());
        x.setIid(j.at("iid").get<appBuildId>());
        x.setWorldGridHeight(j.at("worldGridHeight").get<appBuildId>());
        x.setWorldLayout(j.at("worldLayout").get<worldLayout>());
        x.setDefaultLevelWidth(j.at("defaultLevelWidth").get<appBuildId>());
        x.setLevels(j.at("levels").get<customCommands>());
        x.setDefaultLevelHeight(j.at("defaultLevelHeight").get<appBuildId>());
        x.setIdentifier(j.at("identifier").get<appBuildId>());
    }

    inline void to_json(json & j, const worldProperties & x) {
        j = json::object();
        j["worldGridWidth"] = x.getWorldGridWidth();
        j["iid"] = x.getIid();
        j["worldGridHeight"] = x.getWorldGridHeight();
        j["worldLayout"] = x.getWorldLayout();
        j["defaultLevelWidth"] = x.getDefaultLevelWidth();
        j["levels"] = x.getLevels();
        j["defaultLevelHeight"] = x.getDefaultLevelHeight();
        j["identifier"] = x.getIdentifier();
    }

    inline void from_json(const json & j, world& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setRequired(j.at("required").get<std::vector<std::string>>());
        x.setAdditionalProperties(j.at("additionalProperties").get<bool>());
        x.setProperties(j.at("properties").get<worldProperties>());
        x.setType(j.at("type").get<std::vector<forcedRefsType>>());
    }

    inline void to_json(json & j, const world & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["required"] = x.getRequired();
        j["additionalProperties"] = x.getAdditionalProperties();
        j["properties"] = x.getProperties();
        j["type"] = x.getType();
    }

    inline void from_json(const json & j, otherTypes& x) {
        x.setTilesetRect(j.at("TilesetRect").get<tilesetRect>());
        x.setFieldInstance(j.at("FieldInstance").get<fieldInstance>());
        x.setEntityInstance(j.at("EntityInstance").get<entityInstance>());
        x.setDefinitions(j.at("Definitions").get<definitions>());
        x.setEnumTagValue(j.at("EnumTagValue").get<enumTagValue>());
        x.setAutoRuleDef(j.at("AutoRuleDef").get<autoRuleDef>());
        x.setFieldDef(j.at("FieldDef").get<fieldDef>());
        x.setCustomCommand(j.at("CustomCommand").get<customCommand>());
        x.setEntityDef(j.at("EntityDef").get<entityDef>());
        x.setAutoLayerRuleGroup(j.at("AutoLayerRuleGroup").get<autoLayerRuleGroup>());
        x.setIntGridValueGroupDef(j.at("IntGridValueGroupDef").get<intGridValueGroupDef>());
        x.setIntGridValueInstance(j.at("IntGridValueInstance").get<intGridValueInstance>());
        x.setTocInstanceData(j.at("TocInstanceData").get<tocInstanceData>());
        x.setNeighbourLevel(j.at("NeighbourLevel").get<neighbourLevel>());
        x.setLayerInstance(j.at("LayerInstance").get<layerInstance>());
        x.setWorld(j.at("World").get<world>());
        x.setEntityReferenceInfos(j.at("EntityReferenceInfos").get<entityReferenceInfos>());
        x.setTileCustomMetadata(j.at("TileCustomMetadata").get<tileCustomMetadata>());
        x.setTilesetDef(j.at("TilesetDef").get<tilesetDef>());
        x.setEnumDefValues(j.at("EnumDefValues").get<enumDefValues>());
        x.setTile(j.at("Tile").get<tile>());
        x.setLayerDef(j.at("LayerDef").get<layerDef>());
        x.setLevelBgPosInfos(j.at("LevelBgPosInfos").get<levelBgPosInfos>());
        x.setLevel(j.at("Level").get<level>());
        x.setTableOfContentEntry(j.at("TableOfContentEntry").get<tableOfContentEntry>());
        x.setEnumDef(j.at("EnumDef").get<enumDef>());
        x.setGridPoint(j.at("GridPoint").get<gridPoint>());
        x.setIntGridValueDef(j.at("IntGridValueDef").get<intGridValueDef>());
    }

    inline void to_json(json & j, const otherTypes & x) {
        j = json::object();
        j["TilesetRect"] = x.getTilesetRect();
        j["FieldInstance"] = x.getFieldInstance();
        j["EntityInstance"] = x.getEntityInstance();
        j["Definitions"] = x.getDefinitions();
        j["EnumTagValue"] = x.getEnumTagValue();
        j["AutoRuleDef"] = x.getAutoRuleDef();
        j["FieldDef"] = x.getFieldDef();
        j["CustomCommand"] = x.getCustomCommand();
        j["EntityDef"] = x.getEntityDef();
        j["AutoLayerRuleGroup"] = x.getAutoLayerRuleGroup();
        j["IntGridValueGroupDef"] = x.getIntGridValueGroupDef();
        j["IntGridValueInstance"] = x.getIntGridValueInstance();
        j["TocInstanceData"] = x.getTocInstanceData();
        j["NeighbourLevel"] = x.getNeighbourLevel();
        j["LayerInstance"] = x.getLayerInstance();
        j["World"] = x.getWorld();
        j["EntityReferenceInfos"] = x.getEntityReferenceInfos();
        j["TileCustomMetadata"] = x.getTileCustomMetadata();
        j["TilesetDef"] = x.getTilesetDef();
        j["EnumDefValues"] = x.getEnumDefValues();
        j["Tile"] = x.getTile();
        j["LayerDef"] = x.getLayerDef();
        j["LevelBgPosInfos"] = x.getLevelBgPosInfos();
        j["Level"] = x.getLevel();
        j["TableOfContentEntry"] = x.getTableOfContentEntry();
        j["EnumDef"] = x.getEnumDef();
        j["GridPoint"] = x.getGridPoint();
        j["IntGridValueDef"] = x.getIntGridValueDef();
    }

    inline void from_json(const json & j, welcome& x) {
        x.setDescription(j.at("description").get<std::string>());
        x.setTitle(j.at("title").get<std::string>());
        x.setSchema(j.at("$schema").get<std::string>());
        x.setRef(j.at("$ref").get<std::string>());
        x.setVersion(j.at("version").get<std::string>());
        x.setLdtkJsonRoot(j.at("LdtkJsonRoot").get<ldtkJsonRoot>());
        x.setOtherTypes(j.at("otherTypes").get<otherTypes>());
    }

    inline void to_json(json & j, const welcome & x) {
        j = json::object();
        j["description"] = x.getDescription();
        j["title"] = x.getTitle();
        j["$schema"] = x.getSchema();
        j["$ref"] = x.getRef();
        j["version"] = x.getVersion();
        j["LdtkJsonRoot"] = x.getLdtkJsonRoot();
        j["otherTypes"] = x.getOtherTypes();
    }

    inline void from_json(const json & j, forcedRefsType & x) {
        if (j == "boolean") x = forcedRefsType::boolean;
        else if (j == "integer") x = forcedRefsType::integer;
        else if (j == "null") x = forcedRefsType::null;
        else if (j == "number") x = forcedRefsType::number;
        else if (j == "object") x = forcedRefsType::object;
        else if (j == "string") x = forcedRefsType::string;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const forcedRefsType & x) {
        switch (x) {
            case forcedRefsType::boolean: j = "boolean"; break;
            case forcedRefsType::integer: j = "integer"; break;
            case forcedRefsType::null: j = "null"; break;
            case forcedRefsType::number: j = "number"; break;
            case forcedRefsType::object: j = "object"; break;
            case forcedRefsType::string: j = "string"; break;
            default: throw std::runtime_error("Unexpected value in enumeration \"forcedRefsType\": " + std::to_string(static_cast<int>(x)));
        }
    }

    inline void from_json(const json & j, customCommandsType & x) {
        if (j == "array") x = customCommandsType::array;
        else if (j == "null") x = customCommandsType::null;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const customCommandsType & x) {
        switch (x) {
            case customCommandsType::array: j = "array"; break;
            case customCommandsType::null: j = "null"; break;
            default: throw std::runtime_error("Unexpected value in enumeration \"customCommandsType\": " + std::to_string(static_cast<int>(x)));
        }
    }
}
