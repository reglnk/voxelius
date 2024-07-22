// SPDX-License-Identifier: Zlib
// Copyright (C) 2024, Voxelius Contributors
#include <client/event/language_set.hh>
#include <client/globals.hh>
#include <client/lang.hh>
#include <entt/signal/dispatcher.hpp>
#include <parson.h>
#include <shared/util/physfs.hh>
#include <shared/config.hh>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>
#include <unordered_map>

constexpr static char *DEFAULT_LANGUAGE = "en_US";

static LangManifest manifest = {};
static LangIterator current_lang = {};  
static std::unordered_map<std::string, std::string> lang_map = {};
static std::unordered_map<std::string, LangIterator> ietf_map = {};
static std::string config_language = DEFAULT_LANGUAGE;

static void send_event(LangIterator language)
{
    LanguageSetEvent event = {};
    event.language = language;
    globals::dispatcher.trigger(event);
}

void lang::init(void)
{
    Config::add(globals::client_config, "language", config_language);

    // Available languages are kept in a
    // special manifest file which consists
    // of keys in an IETF-ish format and values
    // as the according language's endonym
    const std::string manifest_path = "/lang/manifest.json";

    std::string source = {};

    if(!util::read_string(manifest_path, source)) {
        spdlog::critical("language: {}: {}", manifest_path, util::physfs_error());
        std::terminate();
    }

    JSON_Value *jsonv = json_parse_string(source.c_str());
    const JSON_Object *json = json_value_get_object(jsonv);
    const std::size_t count = json_object_get_count(json);

    if(!jsonv || !json || !count) {
        spdlog::critical("language: {}: parse error", manifest_path);
        json_value_free(jsonv);
        std::terminate();
    }

    for(std::size_t i = 0; i < count; ++i) {
        const char *ietf = json_object_get_name(json, i);
        const JSON_Value *value = json_object_get_value_at(json, i);
        const char *endonym = json_value_get_string(value);

        if(ietf && endonym) {
            LangInfo info = {};
            info.ietf = std::string(ietf);
            info.endonym = std::string(endonym);
            info.display = fmt::format("{} ({})", endonym, ietf);
            manifest.push_back(info);
        }
    }

    for(LangIterator it = manifest.cbegin(); it != manifest.cend(); ++it) {
        ietf_map.emplace(it->ietf, it);
    }

    json_value_free(jsonv);

    // This is temporary! This value will
    // be overriden in init_late after the
    // config system updates config_language
    current_lang = manifest.cend();
}

void lang::init_late(void)
{
    const auto conf_it = ietf_map.find(config_language);

    if(conf_it != ietf_map.cend()) {
        lang::set(conf_it->second);
        return;
    }

    const auto fallback_it = ietf_map.find(DEFAULT_LANGUAGE);

    if(fallback_it != ietf_map.cend()) {
        lang::set(fallback_it->second);
        return;
    }

    spdlog::critical("lang: we're doomed!");
    spdlog::critical("lang: {} doesn't exist!", DEFAULT_LANGUAGE);
    std::terminate();
}

void lang::set(LangIterator language)
{
    if(language != manifest.cend()) {
        const std::string path = fmt::format("/lang/lang.{}.json", language->ietf);

        std::string source = {};

        if(!util::read_string(path, source)) {
            spdlog::warn("lang: {}: {}", path, util::physfs_error());
            send_event(language);
            return;
        }

        JSON_Value *jsonv = json_parse_string(source.c_str());
        const JSON_Object *json = json_value_get_object(jsonv);
        const size_t count = json_object_get_count(json);

        if(!jsonv || !json || !count) {
            spdlog::warn("lang: {}: parse error");
            send_event(language);
            return;
        }

        lang_map.clear();

        for(size_t i = 0; i < count; ++i) {
            const char *key = json_object_get_name(json, i);
            const JSON_Value *value = json_object_get_value_at(json, i);
            const char *value_str = json_value_get_string(value);

            if(key && value_str) {
                lang_map.emplace(key, value_str);
                continue;
            }
        }

        json_value_free(jsonv);

        current_lang = language;
        config_language = language->ietf;
    }

    send_event(language);
}

LangIterator lang::find(const std::string &ietf)
{
    const auto it = ietf_map.find(ietf);
    if(it != ietf_map.cend())
        return it->second;
    return manifest.cend();
}

LangIterator lang::current(void)
{
    return current_lang;
}

LangIterator lang::cbegin(void)
{
    return manifest.cbegin();
}

LangIterator lang::cend(void)
{
    return manifest.cend();
}

const std::string &lang::resolve(const std::string &tag)
{
    const auto it = lang_map.find(tag);
    if(it != lang_map.cend())
        return it->second;
    return tag;
}
