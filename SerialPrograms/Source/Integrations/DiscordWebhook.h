/*  Discord Webhook
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_DiscordWebhook_H
#define PokemonAutomation_DiscordWebhook_H

#include <deque>
#include <condition_variable>
#include <thread>
#include <QNetworkReply>
#include "Common/Cpp/Time.h"
#include "CommonFramework/Logging/Logger.h"
#include "CommonFramework/Options/ScreenshotFormatOption.h"
#include "CommonFramework/Notifications/MessageAttachment.h"

class QEventLoop;

namespace PokemonAutomation{
    class JsonArray;
    class JsonObject;
namespace Integration{
namespace DiscordWebhook{


struct DiscordWebhookRequest{
    DiscordWebhookRequest() = default;
    DiscordWebhookRequest(QUrl p_url, QByteArray p_data, std::shared_ptr<PendingFileSend> file);
    DiscordWebhookRequest(QUrl p_url, std::shared_ptr<PendingFileSend> p_file);

    WallClock timestamp = current_time();
    QUrl url;

    QByteArray data;
    std::shared_ptr<PendingFileSend> file;
};

class DiscordWebhookSender : public QObject{
    Q_OBJECT

    static constexpr auto THROTTLE_DURATION = std::chrono::seconds(1);
    static constexpr size_t MAX_IN_WINDOW = 2;

private:
    DiscordWebhookSender();
    ~DiscordWebhookSender();


public:
    void send_json(Logger& logger, const QUrl& url, const JsonObject& obj, std::shared_ptr<PendingFileSend> file);
    void send_file(Logger& logger, const QUrl& url, std::shared_ptr<PendingFileSend> file);

    static DiscordWebhookSender& instance();


private:
    void cleanup_stuck_requests();
    void thread_loop();

    void process_reply(QNetworkReply* reply);
    void internal_send_json(QEventLoop& event_loop, const QUrl& url, const QByteArray& data);
    void internal_send_file(QEventLoop& event_loop, const QUrl& url, const std::string& filename);
    void internal_send_image_embed(QEventLoop& event_loop, const QUrl& url, const QByteArray& data, const std::string& filepath, const std::string& filename);

signals:
    void stop_event_loop();

private:
    TaggedLogger m_logger;
    bool m_stopping;
    std::mutex m_lock;
    std::condition_variable m_cv;
    std::deque<DiscordWebhookRequest> m_queue;
    std::deque<WallClock> m_sent;
    std::thread m_thread;
};




void send_message(
    Logger& logger,
    bool should_ping,
    const std::vector<std::string>& tags,
    const std::string& message,
    const JsonArray& embeds,
    std::shared_ptr<PendingFileSend> file
);



}
}
}
#endif
